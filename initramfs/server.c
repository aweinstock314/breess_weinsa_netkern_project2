#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define ERROR(S) (perror(S), exit(EXIT_FAILURE))

int SOCK = -1;

void reap(int sig) { waitpid(-1, NULL, WNOHANG); }
void cleanup(int sig) { close(SOCK); exit(EXIT_SUCCESS); }

int main(int argc, char **argv) {
	if (argc < 2) { fprintf(stderr, "Usage: %s PORT\n", argv[0]); return EXIT_FAILURE; }

	signal(SIGCHLD, reap);
	signal(SIGINT, cleanup);

	int port = atoi(argv[1]);

	if ((SOCK = socket(AF_INET, SOCK_STREAM, 0)) == -1) ERROR("Failed to open socket");

	setsockopt(SOCK, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int));

	struct sockaddr_in server = { .sin_family=PF_INET, .sin_addr.s_addr=INADDR_ANY, .sin_port=htons(port) };
	if (bind(SOCK, (struct sockaddr *) &server, sizeof(server)) == -1) ERROR("Failed to bind socket");
	if (listen(SOCK, 5) == -1) ERROR("Failed to listen on socket");

	while (1) {
		struct sockaddr_in client;
		socklen_t client_size = sizeof(client);
		int fd = accept(SOCK, (struct sockaddr *) &client, &client_size);
		if (fd == -1) ERROR("Failed to accept");

		pid_t pid = fork();
		if (pid == 0) {
			SOCK = fd;
			char buf[1024];
			snprintf(buf, 1024, "[%d] PID %d", port, getpid());
			syscall(332, fd, buf, strlen(buf), MSG_NOSIGNAL, 5);
			shutdown(fd, SHUT_RDWR);
			close(fd);
			exit(EXIT_SUCCESS);
		}
	}

	return EXIT_SUCCESS;
}
