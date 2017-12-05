#include <stdio.h>

int signal_handler(void)
{
	sigset_t mask;

	sigemptyset(&mask);
	sigaddset(&mask, SIGTERM);
	sigaddset(&mask, SIGINT);
	pthread_sigmask(SIG_UNBLOCK, &mask, 0);
}

int main(int argc, char **argv)
{
	int ret;

	signal_handler();

	return ret;
}
