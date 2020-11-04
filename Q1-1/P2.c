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

		char* pipe_name = "/tmp/temp-control-12";

		mkfifo(pipe_name, 0666);

		int temperatures[NO_OF_LOCATIONS];
						
		char read_buffer[1024];
		while(1) {
			fd = open(pipe_name, O_RDONLY);
			
			debug("New pipe opened\n");

			read(fd, read_buffer, 1024);

			int index = 0;
			for (int i = 0; i < NO_OF_LOCATIONS; i++)
				if (nextInt(read_buffer, &index, &temperatures[i]) < 0)
					printf("Error reading data");

			printf("-----------------\n");
			printf("Received temperature data\n");
			
			for (int i = 0; i < NO_OF_LOCATIONS; i++)
				printf("L%d - %d Celsius\n", (i + 1), temperatures[i]);

			// calculate avg and standard deviation
			float D = 0;
			float avg = 0;

			for (int i = 0; i < NO_OF_LOCATIONS; i++)
				avg += temperatures[i];
			avg /= NO_OF_LOCATIONS;

			for (int i = 0; i < NO_OF_LOCATIONS; i++)
				D += pow(temperatures[i] - avg, 2);
			D = sqrt(D / (NO_OF_LOCATIONS - 1));

			printf("Avg - %f\nStandard deviation - %f\n", avg, D);

			close(fd);

			debug("Pipe closed\n");
		}
}
