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

	while (data->status != 1);

	printf("Password received - %s\n", data->password);

	int alphanum = 0, special = 0;
	
	int len = strlen(data->password);
	for (int i = 0; i < len; i++) {
		if (isalnum(data->password[i]))
			alphanum++;
		else
			special++;
	}

	data->alphanum = alphanum;
	data->special = special;

	data->status = 2;

	shmdt(data);
	return 0;
}
