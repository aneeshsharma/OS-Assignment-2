#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "shared_data.h"
#include <string.h>

typedef struct PasswordData PasswordData;

int main() {
	key_t key = ftok("password_data", 72);

	int mid = shmget(key, sizeof(PasswordData), 0666|IPC_CREAT);

	PasswordData* data = (PasswordData*) shmat(mid, 0, 0);

	data->status = 0;

	printf("Enter password: ");
	scanf("%s", data->password);

	data->status = 1;

	while(data->status != 3);

	if (data->strength == 2) {
		printf("Password is STRONG\n");
	} else if (data->strength == 1) {
		printf("Password is WEAK\n");
	} else {
		printf("Invalid data received\n");
	}

	shmdt(data);

	shmctl(mid, IPC_RMID, NULL);

	return 0;
}
