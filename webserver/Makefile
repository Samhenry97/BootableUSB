# Remove the "-DSHOW_LOG_ERROR" to eliminate the
# verbose error logging messages generated by EZNet
CFLAGS=-g -std=gnu11 -Wall -Werror -DSHOW_LOG_ERROR

all: webserver 

webserver: webserver.c errors.h eznet.o utils.o threadpool.o
	$(CC) $(CFLAGS) -owebserver webserver.c eznet.o utils.o threadpool.o -lpthread

eznet.o: eznet.c eznet.h
	$(CC) -c $(CFLAGS) eznet.c

utils.o: utils.c utils.h
	$(CC) -c $(CFLAGS) utils.c -lpthread

threadpool.o: threadpool.c threadpool.h
	$(CC) -c $(CFLAGS) threadpool.c -lpthread

clean:
	rm webserver *.o

