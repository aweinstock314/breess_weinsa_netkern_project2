#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

#define ERROR(S) (perror(S), exit(EXIT_FAILURE))

int main(int argc, char **argv) {
	if (argc < 3) { fprintf(stderr, "Usage: %s PORT LOG\n", argv[0]); return EXIT_FAILURE; }

	FILE *log = fopen(argv[2], "w");
	if (!log) fprintf(stderr, "Failed to open log file\n");

	struct addrinfo *ai;
	if (getaddrinfo("127.0.0.1", argv[1], NULL, &ai)) {
		fprintf(stderr, "Failed getaddrinfo\n");
		return EXIT_FAILURE;
	}

	int fd = socket(ai->ai_family, ai->ai_socktype, ai->ai_protocol);
	if (fd == -1) ERROR("Failed to open socket");

	if (connect(fd, ai->ai_addr, ai->ai_addrlen)) ERROR("Failed to connect");

	char buf[1024];
	while (recv(fd, buf, 1024, 0) > 0) { fprintf(log, "%s\n", buf); fflush(log); }
	fclose(log);
	close(fd);

	return EXIT_SUCCESS;
}
