#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <ctype.h>
#include <string.h>
#include "shared_data.h"

typedef struct PasswordData PasswordData;

int main() {
	key_t key = ftok("password_data", 72);

	int mid = shmget(key, sizeof(PasswordData), 0666|IPC_CREAT);

	PasswordData* data = (PasswordData*) shmat(mid, NULL, 0);

	while (data->status != 2);

	printf("Received count -\n");
	printf("Alphanumeric - %d\n", data->alphanum);
	printf("Special - %d\n", data->special);

	if (data->special > data->alphanum) {
		printf("Strong\n");
		data->strength = 2;
	} else {
		printf("Weak\n");
		data->strength = 1;
	}

	data->status = 3;

	shmdt(data);
	return 0;
}
