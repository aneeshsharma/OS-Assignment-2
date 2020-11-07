#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/wait.h>
#include <ctype.h>
#include <math.h>

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

void process_1(int* write_pipe, int* read_pipe) {
	close(write_pipe[0]);
	close(read_pipe[1]);

	int temperatures[NO_OF_LOCATIONS];

	printf("Enter temperatures for the locations -\n");
	for (int i = 0; i < NO_OF_LOCATIONS; i++) {
		printf("L%d - ", i + 1);
		scanf("%d", &temperatures[i]);
	}
	
	char write_buffer[1024];
	char* buffer = write_buffer;
	for (int i = 0; i < NO_OF_LOCATIONS; i++) {
		buffer += sprintf(buffer, "%d ", temperatures[i]);
	}

	if (write(write_pipe[1], write_buffer, strlen(write_buffer) + 1) == -1) {
		printf("Error writing\n");
	}
	close(write_pipe[1]);

	char read_buffer[1024];

	read(read_pipe[0], read_buffer, 1024);

	int index = 0;
	int categories[NO_OF_LOCATIONS];
	for (int i = 0; i < NO_OF_LOCATIONS; i++) {
		if (nextInt(read_buffer, &index, &categories[i]) == -1)
			printf("Error translating data\n");
	}

	printf("Received categories -\n");
	for (int i = 0; i < NO_OF_LOCATIONS; i++) {
		printf("L%d - Cat%d\n", i + 1, categories[i]);
	}

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
	for (int i = 0; i < NO_OF_LOCATIONS; i++) {
		printf("L%d - %d Celsius\n", i + 1, temperatures[i]);
	}
	close(read_pipe[0]);
}
void process_2(int* write_pipe, int* read_pipe) {
	char read_buffer[1024];
	close(write_pipe[0]);
	close(read_pipe[1]);

	int temperatures[NO_OF_LOCATIONS];
	
	read(read_pipe[0], read_buffer, 1024);

	int index = 0;
	for (int i = 0; i < NO_OF_LOCATIONS; i++) {
		if (nextInt(read_buffer, &index, &temperatures[i]) == -1) {
			printf("Error translating data\n");
		}
	}

	float avg = 0;
	float D = 0;

	for (int i = 0; i < NO_OF_LOCATIONS; i++)
		avg += temperatures[i];
	avg /= NO_OF_LOCATIONS;

	for (int i = 0; i < NO_OF_LOCATIONS; i++)
		D += pow(temperatures[i] - avg, 2);
	D = sqrt(D / (NO_OF_LOCATIONS - 1));

	//printf("Avg - %f\nStandard deviation - %f\n", avg, D);

	close(read_pipe[0]);

	char write_buffer[1024];
	
	char* buffer = write_buffer;
	buffer += sprintf(buffer, "%f %f D", avg, D); 
	for (int i = 0; i < NO_OF_LOCATIONS; i++) {
		buffer += sprintf(buffer, "%d ", temperatures[i]);
	}

	if (write(write_pipe[1], write_buffer, strlen(write_buffer) + 1) == -1) {
		printf("Error writing data\n");
	}

	close(write_pipe[1]);
}

void process_3(int write_pipe[2], int read_pipe[2]) {
	close(write_pipe[0]);
	close(read_pipe[1]);

	char read_buffer[1024];

	read(read_pipe[0], read_buffer, 1024);

	float avg, D;
	int temperatures[NO_OF_LOCATIONS];
	int categories[NO_OF_LOCATIONS];

	sscanf(read_buffer, "%f %f", &avg, &D);

	int index = 0;
	while (read_buffer[index] != 'D')
		index++;
	index++;

	for (int i = 0; i < NO_OF_LOCATIONS; i++) {
		if (nextInt(read_buffer, &index, &temperatures[i]) == -1)
			printf("Error translating data\n");
	}

	//printf("Avg = %f\nStandard deviation - %f\n", avg, D);

	//printf("Determined caregories -\n");
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
		//printf("L%d - Cat%d\n", i + 1, categories[i]);
	}	
	
	close(read_pipe[1]);
	
	char write_buffer[1024];
	char* buffer = write_buffer;
	for (int i = 0; i < NO_OF_LOCATIONS; i++) {
		buffer += sprintf(buffer, "%d ", categories[i]);
	}

	write(write_pipe[1], write_buffer, strlen(write_buffer) + 1);

	close(write_pipe[1]);
}

int main() {
	int fd1[2];
	int fd2[2];
	int fd3[2];

	if (pipe(fd1) == -1) {
		printf("Error creating pipe\n");
		return 1;
	}
	if (pipe(fd2) == -1) {
		printf("Error creating pipe\n");
		return 1;
	}
	if (pipe(fd3) == -1) {
		printf("Error creating pipe\n");
		return 1;
	}

	pid_t p = fork();

	if (p > 0) {
		process_1(fd1, fd3);
	} else {
		pid_t p2 = fork();
		if (p2 > 0) {
			process_2(fd2, fd1);
		} else {
			process_3(fd3, fd2);
		}
	}

	return 0;
}
