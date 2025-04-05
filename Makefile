CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -pedantic

OBJS = main.o utils.o metrics.o

all: app

app: $(OBJS)
	$(CC) $(CFLAGS) -o app $(OBJS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

utils.o: utils.c
	$(CC) $(CFLAGS) -c utils.c

metrics.o: metrics.c
	$(CC) $(CFLAGS) -c metrics.c

clean:
	rm -f $(OBJS) app
