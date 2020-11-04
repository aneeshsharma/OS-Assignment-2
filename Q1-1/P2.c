#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

#define NO_OF_LOCATIONS 5

int main() {
		int fd;

		char* pipe_name = "/tmp/temp-control-12";

		mkfifo(pipe_name, 0666);

		int temperatures[NO_OF_LOCATIONS];
		fd = open(pipe_name, O_RDONLY);
				
		char read_buffer[1024];
		while(1) {
			read(fd, read_buffer, 1024);

			char* buffer = read_buffer;
			for (int i = 0; i < NO_OF_LOCATIONS; i++) {
				int r = sscanf(buffer, "%d", &temperatures[i]);
				if (r < 0) {
					printf("Error reading data\n");
				} else {
					buffer += r;
				}
			}

			for (int i = 0; i < NO_OF_LOCATIONS; i++) {
				printf("L%d - %d Celsius\n", (i + 1), temperatures[i]);
			}
		}
}
