CFLAGS = -Wall -Werror -static
all: client server
test: all
	./server 12345 &
	sleep 1
	./client 12345 log.txt
clean:
	rm -f client server
