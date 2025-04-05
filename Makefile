CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

OBJS = main.o utils.o metrics.o

all: app

app: $(OBJS)
	$(CC) $(CFLAGS) -o app $(OBJS)

main.o: main.c utils.h metrics.h
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c utils.h
	$(CC) $(CFLAGS) -c utils.c

metrics.o: metrics.c utils.h metrics.h
	$(CC) $(CFLAGS) -c metrics.c

clean:
	rm -f $(OBJS) app
