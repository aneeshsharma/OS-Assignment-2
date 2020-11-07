# OS Assignment 2
This project demonstrates various methods of Inter-Process Communication (IPC), namely -
- Named Pipe Communication - Q1-1
- Unnamed Pipe Communication - Q1-2
- Shared Memory Communication - Q2

## Prerequisites
In order to run the programs you would need to install the following packages on your linux system -
- `gcc` - The GNU Compiler Collection
- `make` - The Make utility

## Named Pipe Communication - Q1-1
The directory Q1-1 contains files relevant to named pipe communication. In order to compile the code, change to the directory `Q1-1` and run `make` -
```
$ cd Q1-1
$ make
```

There are 3 seperate programs that will be compiled namely - `P1.o`, `P2.o` and `P3.o`.
- `P1.o (from P1.c)` - Reads temperature data from the user and sends to P2 through a named pipe
- `P2.o (from P2.c)` - Reads data from P1 and finds mean and standard deviation of the temperatures
- `P3.o (from P3.c)` - Reads data from P2 and determines category for each location and sends data back to P1, which prints revised temperatures.

Once the compilation is complete, run the three processes sperately using -
```
$ ./P1.o

$ ./P2.o

$ ./P3.o
```

After entering temperature data on P1, the data flow should occur and the output (revised temperatures) should be shown by P1.

## Unnamed Pipe Communication - Q1-2
The directory Q1-2 contains files relevant to unnamed pipe communication. In order to compile the code, change directory to `Q1-2` and run `make`-
```
$ cd Q1-2
$ make
```

There is 1 program that will be compiled to - `process.o` (from `process.c`). This program uses `fork` to create 3 seperate copies of itself. Each process (1 parent and 2 children) perform the roles of P1, P2 and P3 (as in Q1-1).

Once compilation is complete, run the process directly or using `make test` -
```
$ make test
```
OR
```
$ ./process.o
```
## Shared Memory Communicationi
The directory Q2 contains files relevant to shared memory communication. In order to compile the code, change to the directory `Q2` and run `make` -
```
$ cd Q2
$ make
```

There are 3 seperate programs that will be compiled namely - `P1.o`, `P2.o` and `P3.o`.
- `P1.o (from P1.c)` - Reads password from user and writes to shared memory
- `P2.o (from P2.c)` - Reads password from shared memory and counts number of alphanumerical and special characters in the password and stores it into the shared memory
- `P3.o (from P3.c)` - Reads character count from shared memory, calculates the password strength and writes the strength to shared memory. P1 then reads the strength from the shared memory and displays to the user.

Once the compilation is complete, run the three processes sperately using -
```
$ ./P1.o

$ ./P2.o

$ ./P3.o
```

After entering password on P1, the data flow should occur and the output (password strength) should be shown by P1.


