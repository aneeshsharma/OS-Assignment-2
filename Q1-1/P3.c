#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#define NO_OF_LOCATIONS 5

int nextInt(char* string, int* index, int* out) {
	int len = strlen(string);
	int num = 0;
	while ((*index < len) && (string[*index])) {
		char c = string[*index];
		*index += 1;
		if (isdigit(c)) {
			num *= 10;
			num += (int) c - '0';
		} else if (c == ' ' || c == '\n' || c == '\r') {
			*out = num;
			return 0;
		} else {
			return -1;
		}
	}
}

int debug(char* log) {
	printf("\033[1;31mDebug: \033[0m");
	return printf(log);
}

int main() {
		int fd;

		char* pipe_name_read = "/tmp/temp-control-23";
		char* pipe_name_write = "/tmp/temp-control-31";

		mkfifo(pipe_name_read, 0666);
		mkfifo(pipe_name_write, 0666);

		int temperatures[NO_OF_LOCATIONS];
						
		char read_buffer[1024], write_buffer[1024];
		while(1) {
			fd = open(pipe_name_read, O_RDONLY);
			
			debug("New pipe opened\n");

			read(fd, read_buffer, 1024);
			int index = 0;
			float avg, D;

			sscanf(read_buffer, "%f %f", &avg, &D);
			
			printf("-----------------\n");
			printf("Received temperature data\n");

			printf("Avg - %f\nDeviation - %f\n", avg, D);
			
			while(read_buffer[index] != 'D')
					index++;
			index++;

			for (int i = 0; i < NO_OF_LOCATIONS; i++)
				if (nextInt(read_buffer, &index, &temperatures[i]) < 0)
					printf("Error reading data at %d\n", index);

			for (int i = 0; i < NO_OF_LOCATIONS; i++)
				printf("L%d - %d Celsius\n", (i + 1), temperatures[i]);

			close(fd);

			int categories[NO_OF_LOCATIONS];

			for (int i = 0; i < NO_OF_LOCATIONS; i++) {
				if (temperatures[i] < avg - D) {
					categories[i] = 4;
				} else if (temperatures[i] < avg) {
					categories[i] = 3;
				} else if (temperatures[i] > avg + D) {
					categories[i] = 1;
				} else if (temperatures[i] > avg) {
					categories[i] = 2;
				} else {
					categories[i] = 0;
				}
				printf("L%d - Cat%d\n", i + 1, categories[i]);
			}

			fd = open(pipe_name_write, O_WRONLY);

			char* buffer = write_buffer;
			for (int i = 0; i < NO_OF_LOCATIONS; i++) {
				buffer += sprintf(buffer, "%d ", categories[i]);
			}

			write(fd, write_buffer, strlen(write_buffer) + 1);

			close(fd);
		}
}
