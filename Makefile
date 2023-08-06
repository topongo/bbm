CLIBS = $(shell pkg-config --libs libnotify)
CFLAGS = -Wall -Wextra $(shell pkg-config --cflags libnotify)
CC = gcc

all: bbm_c

run: bbm_c
	./bbm

bbm_c: debug_c readsys_c notifications_c sleep_c
	$(CC) $(CFLAGS) $(CLIBS) -o bbm notifications.o debug.o readsys.o mock.o sleep.o bbm.c 

readsys_c: debug_c
	$(CC) $(CFLAGS) -c readsys.c

notifications_c: debug_c
	$(CC) $(CFLAGS) -c notifications.c

debug_c:
	$(CC) $(CFLAGS) -c debug.c

mock_c:
	$(CC) $(CFLAGS) -c mock.c

sleep_c:
	$(CC) $(CFLAGS) -c sleep.c

clean:
	rm -f *.o bbm

debug: CFLAGS += -DBBM_DEBUG
debug: bbm_c
	./bbm

mock: CFLAGS += -DBBM_MOCK
mock: mock_c bbm_c
	./bbm

debug_and_mock: CFLAGS += -DBBM_DEBUG -DBBM_MOCK
debug_and_mock: bbm_c
	./bbm
