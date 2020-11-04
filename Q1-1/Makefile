CC=gcc

SRCS = P1.c P2.c

OBJS = $(SRCS:.c=.o)

all: $(OBJS)
	@echo Compiled successfully

.c.o:
	$(CC) -c $< -o $@

clean: 
	rm *.o *.out
