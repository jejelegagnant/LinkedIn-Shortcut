#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <string.h>

// Track the state of our four crucial modifiers
bool ctrl_pressed = false;
bool shift_pressed = false;
bool alt_pressed = false;
bool meta_pressed = false;

// Update modifier states
void update_modifier(int code, int value) {
    bool is_active = (value == 1 || value == 2); 
    
    if (code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL) ctrl_pressed = is_active;
    if (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT) shift_pressed = is_active;
    if (code == KEY_LEFTALT || code == KEY_RIGHTALT) alt_pressed = is_active;
    if (code == KEY_LEFTMETA || code == KEY_RIGHTMETA) meta_pressed = is_active;
}

int main(int argc, char *argv[]) {
    // Force standard output to be unbuffered so systemd sees our logs immediately
    setbuf(stdout, NULL);

    char *device = NULL;
    char *browser = "firefox"; 
    char *target_user = "root";
    int opt;

    // Parse command-line arguments (-k keyboard, -b browser, -u user)
    while ((opt = getopt(argc, argv, "k:b:u:h")) != -1) {
        switch (opt) {
            case 'k': device = optarg; break;
            case 'b': browser = optarg; break;
            case 'u': target_user = optarg; break;
            case 'h':
            default:
                printf("Usage: %s -k <device> [-b <browser>] [-u <user>]\n", argv[0]);
                return 1;
        }
    }

    if (device == NULL) {
        fprintf(stderr, "Error: Keyboard device (-k) is required.\n");
        return 1;
    }

    int fd = open(device, O_RDONLY);
    if (fd < 0) {
        perror("Error: Cannot open device");
        return 1;
    }

    struct input_event ev;
    printf("Listening on %s. Launching %s as user %s...\n", device, browser, target_user);

    while (1) {
        ssize_t n = read(fd, &ev, sizeof(ev));
        if (n == (ssize_t)-1) {
            perror("Read error");
            break;
        } else if (n != sizeof(ev)) {
            continue;
        }

        if (ev.type == EV_KEY) {
            update_modifier(ev.code, ev.value);

            if (ev.code == KEY_L && ev.value == 1) {
                if (ctrl_pressed && shift_pressed && alt_pressed && meta_pressed) {
                    printf("Shortcut activated! Launching LinkedIn...\n");
                    
                    char cmd[1024];
                    // Dynamically look up the user's UID and launch the browser on their display
                    snprintf(cmd, sizeof(cmd), 
                        "sudo -u %s env XDG_RUNTIME_DIR=/run/user/$(id -u %s) DISPLAY=:0 WAYLAND_DISPLAY=wayland-0 %s \"https://www.linkedin.com\" > /dev/null 2>&1 &", 
                        target_user, target_user, browser);
                    
                    system(cmd);
                }
            }
        }
    }
    
    close(fd);
    return 0;
}
