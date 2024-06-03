CC = gcc
CFLAGS = -Wall -Werror -Wextra
OBJECTS = *.o

all: start

start: app
	./app

app: app.o
	$(CC) $(CFLAGS) $(OBJECTS) -o app -g

app.o:
	$(CC) $(CFLAGS) -c task.c

clean:
	rm -rf $(OBJECTS) app

valgrind: app
	valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./app