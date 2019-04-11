#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MY_SIGNAL		40
#define FILENAME		"/dev/buttons"

int button_state;

void signal_handle(int n, siginfo_t *info, void* data)
{
	printf("jump to sig_handle\n");

	button_state = info->si_int;
	printf("button_state is %d\n", button_state);
}

int main()
{
	struct sigaction sig;
	int fd;

	printf("process id is %d\n", getpid());
	sig.sa_sigaction = signal_handle;
	sig.sa_flags = SA_SIGINFO;

	sigaction(MY_SIGNAL, &sig, NULL);

	fd = open(FILENAME, O_RDONLY);
	if (fd < 0) {
		perror("open: Failed\n");
		return fd;
	}

	while (1) {
	}	

	return 0;
}
