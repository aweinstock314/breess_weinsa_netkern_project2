#include <stdlib.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ptrace.h>
#include <sys/reg.h>
#include <sys/wait.h>
#include <sys/types.h>

int do_child(int argc, char **argv) {
	char **args = (char **) calloc(argc+1, sizeof(char *));
	memcpy(args, argv, argc * sizeof(char *));
	args[argc] = NULL;
	ptrace(PTRACE_TRACEME);
	kill(getpid(), SIGSTOP);
	return execvp(args[0], args);
}

int wait_for_syscall(pid_t child) {
	int status;
	while (1) {
		ptrace(PTRACE_SYSCALL, child, 0, 0);
		waitpid(child, &status, 0);
		if (WIFSTOPPED(status) & WSTOPSIG(status) & 0x80) return 0;
		if (WIFEXITED(status)) return 1;
	}
}

int do_trace(pid_t child) {
	int status, syscall, retval;
	waitpid(child, &status, 0);
	ptrace(PTRACE_SETOPTIONS, child, 0, PTRACE_O_TRACESYSGOOD);
	while (1) {
		if (wait_for_syscall(child) != 0) break;
		syscall = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * ORIG_RAX);
		fprintf(stderr, "syscall(%d) = ", syscall);
		if (wait_for_syscall(child) != 0) break;
		retval = ptrace(PTRACE_PEEKUSER, child, sizeof(long) * RAX);
		fprintf(stderr, "%d\n", retval);
	}
	return 0;
}

int main(int argc, char **argv) {
	if (argc < 2) {
		fprintf(stderr, "Usage: %s PROG ARGS\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	pid_t child = fork();
	if (child == 0) return do_child(argc-1, argv+1);
	else return do_trace(child);
}
