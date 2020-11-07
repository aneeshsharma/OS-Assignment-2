#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>

#define NO_OF_LOCATIONS 5

int nextInt(char* string, int* index, int* out) {
	int len = strlen(string);
	int num = 0;
	int neg = 1;
	while ((*index < len) && (string[*index])) {
		char c = string[*index];
		*index += 1;
		if (isdigit(c)) {
			num *= 10;
			num += (int) c - '0';
		} else if (c == '-') {
			neg = -1;
		} else if (c == ' ' || c == '\n' || c == '\r') {
			*out = neg * num;
			return 0;
		} else {
			return -1;
		}
	}
}

int main() {
		int fd;

		char* pipe_name_write = "/tmp/temp-control-12";
		char* pipe_name_read = "/tmp/temp-control-31";
		
		mkfifo(pipe_name_read, 0666);
		mkfifo(pipe_name_write, 0666);

		int temperatures[NO_OF_LOCATIONS];
		int categories[NO_OF_LOCATIONS];
						
		char write_buffer[1024], read_buffer[1024];
		
		/* read temperatures and sent to P2 */
		// open a named pipe to write data	
		fd = open(pipe_name_write, O_WRONLY);
			
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

		fd = open(pipe_name_read, O_RDONLY);
		read(fd, read_buffer, 1024);
		
		int index = 0;
		
		for (int i = 0; i < NO_OF_LOCATIONS; i++)
			if (nextInt(read_buffer, &index, &categories[i]) < 0)
				printf("Error reading data");

		printf("Received categories -\n");
		for (int i = 0; i < NO_OF_LOCATIONS; i++)
				printf("L%d - Cat%d\n", i + 1, categories[i]);

		for (int i = 0; i < NO_OF_LOCATIONS; i++) {
			switch(categories[i]) {
				case 1:
					temperatures[i] -= 3;
					break;
				case 2:
					temperatures[i] -= 1.5;
					break;
				case 3:
					temperatures[i] += 2;
					break;
				case 4:
					temperatures[i] += 2.5;
					break;
			}
		}

		printf("Revised temperatures -\n");
		for (int i = 0; i < NO_OF_LOCATIONS; i++)
			printf("L%d - %d Celsius\n", i + 1, temperatures[i]);
			close(fd);
}
