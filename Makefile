CLIBS = $(shell pkg-config --libs libnotify)
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags libnotify)
CC = gcc

all: bbm_c

run: bbm_c
	./bbm

bbm_c: debug_c readsys_c notifications_c mock_c
	$(CC) $(CFLAGS) $(CLIBS) -o bbm notifications.o debug.o readsys.o mock.o bbm.c 

readsys_c: debug_c
	$(CC) $(CFLAGS) -c readsys.c

notifications_c: debug_c
	$(CC) $(CFLAGS) -c notifications.c

debug_c:
	$(CC) $(CFLAGS) -c debug.c

mock_c:
	$(CC) $(CFLAGS) -c mock.c

clean:
	rm -f *.o bbm

debug: CFLAGS += -DDEBUG
debug: bbm_c
	./bbm

mock: CFLAGS += -DMOCK
mock: bbm_c
	./bbm

debug_and_mock: CFLAGS += -DDEBUG -DMOCK
debug_and_mock: bbm_c
	./bbm
