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
		fd = open(pipe_name, O_WRONLY);
				
		char write_buffer[1024];
		while(1) {
			for (int i = 0; i < NO_OF_LOCATIONS; i++) {
				scanf("%d", &temperatures[i]);
			}
			char* buffer = write_buffer;
			for (int i = 0; i < NO_OF_LOCATIONS; i++)
				buffer += (int) sprintf(buffer, "%d ", temperatures[i]);
			sprintf(buffer, "E\n");
			write(fd, write_buffer, strlen(write_buffer) + 1);
		}
}
