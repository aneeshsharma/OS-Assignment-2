CC=gcc

SRCS = P1.c P2.c P3.c

OBJS = $(SRCS:.c=.o)

all: $(OBJS)
	@echo Compiled successfully

.c.o:
	$(CC) $< -lm -o $@

clean: 
	rm *.o *.out
