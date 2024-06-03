CC = gcc
CFLAGS = -Wall -Werror -Wextra
OBJECTS = *.o

all: app

app: app.o
	$(CC) $(CFLAGS) $(OBJECTS) -o app
	./app

app.o:
	$(CC) $(CFLAGS) -c task.c

clean:
	rm -rf $(OBJECTS) app

