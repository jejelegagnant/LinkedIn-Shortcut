#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main() {
    setbuf(stdout, NULL);
    char *fifo_path = "/var/snap/linkedin-shortcut/common/trigger.fifo";
    
    printf("User Agent active. Waiting for hardware daemon triggers via FIFO...\n");

    while (1) {
        // This blocks until the root daemon opens the pipe to write
        int fd = open(fifo_path, O_RDONLY);
        if (fd < 0) {
            perror("Failed to open FIFO. Did you run setup?");
            sleep(2);
            continue;
        }

        char buf;
        // Read until the pipe is closed by the daemon
        while (read(fd, &buf, 1) > 0) {
            if (buf == '1') {
                printf("Hardware trigger received! Executing xdg-open...\n");
                system("xdg-open \"https://www.linkedin.com\" > /dev/null 2>&1 &");
            }
        }
        close(fd);
    }
    return 0;
}
