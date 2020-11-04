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
						
		char write_buffer[1024];
		while(1) {
			/* read temperatures and sent to P2 */
			// open a named pipe to write data	
			fd = open(pipe_name, O_WRONLY);
			
			printf("Please input temperatures for the %d locations -\n", NO_OF_LOCATIONS);
			for (int i = 0; i < NO_OF_LOCATIONS; i++) {
				printf("L%d - ", i + 1);
				scanf("%d%*c", &temperatures[i]);
			}
			
			// write temperatures to a string seperated by spaces
			char* buffer = write_buffer;
			for (int i = 0; i < NO_OF_LOCATIONS; i++)
				buffer += (int) sprintf(buffer, "%d ", temperatures[i]);
			
			// write the generated string to the pipe
			write(fd, write_buffer, strlen(write_buffer) + 1);
			close(fd);
		}
}
