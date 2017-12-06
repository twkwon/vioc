/**
 * @file     vioc_app.c
 * @author   Team VOUT
 * @date     11 Oct 2016
 * @version  0.1
 * @brief
 *
*/
#include  <stdio.h>
#include  <signal.h>
#include  <stdlib.h>

#include <pmap.h>
#include <mmap.h>
#include <dummy.h>


/** @brief Function List
 */
void sighandler(int);
void exit_function(int);


/** @brief Main function
 *  @return returns 0 if successful
 */
int main(void)
{
	signal(SIGINT, sighandler);

	test_pmap();

	test_mmap();

	while(1)
		pause();

	return 0;
}

/** @brief Signal handler
 *  @return void
 */
void sighandler(int sig)
{
	char c;

	signal(sig, SIG_IGN);
	printf("Do you want to quit? [y/n] ");
	c = getchar();
	if (c == 'y' || c == 'Y') {
		exit_function(0);
	} else {
		signal(SIGINT, sighandler);
	}
	getchar();
}

/** @brief Exit function. It has to release resources
 *  @return void
 */
void exit_function(int status)
{
	//TODO: exit routine
	
	exit(status);
}
