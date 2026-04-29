#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <linux/input.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

bool ctrl_pressed = false, shift_pressed = false, alt_pressed = false, meta_pressed = false;

void update_modifier(int code, int value) {
    bool is_active = (value == 1 || value == 2); 
    if (code == KEY_LEFTCTRL || code == KEY_RIGHTCTRL) ctrl_pressed = is_active;
    if (code == KEY_LEFTSHIFT || code == KEY_RIGHTSHIFT) shift_pressed = is_active;
    if (code == KEY_LEFTALT || code == KEY_RIGHTALT) alt_pressed = is_active;
    if (code == KEY_LEFTMETA || code == KEY_RIGHTMETA) meta_pressed = is_active;
}

// IPC Logic: Send a UDP packet to the Agent's socket
// Replace your old trigger_agent() function with this one:
void trigger_agent() {
    char *fifo_path = "/var/snap/linkedin-shortcut/common/trigger.fifo";
    
    // O_NONBLOCK prevents the root daemon from hanging if the agent isn't running
    int fd = open(fifo_path, O_WRONLY | O_NONBLOCK);
    if (fd >= 0) {
        write(fd, "1", 1);
        close(fd);
        printf("Trigger sent to FIFO.\n");
    } else {
        // Fail silently if the agent isn't listening yet
    }
}

int main(int argc, char *argv[]) {
    setbuf(stdout, NULL);
    char *device = NULL;
    int opt;

    while ((opt = getopt(argc, argv, "k:h")) != -1) {
        if (opt == 'k') device = optarg;
    }

    if (!device) return 1;

    int fd = open(device, O_RDONLY);
    if (fd < 0) return 1;

    struct input_event ev;
    printf("Hardware daemon listening on %s...\n", device);

    while (read(fd, &ev, sizeof(ev)) > 0) {
        if (ev.type == EV_KEY) {
            update_modifier(ev.code, ev.value);
            if (ev.code == KEY_L && ev.value == 1) {
                if (ctrl_pressed && shift_pressed && alt_pressed && meta_pressed) {
                    trigger_agent();
                }
            }
        }
    }
    close(fd);
    return 0;
}
