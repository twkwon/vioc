/**
 * @file     vioc_app.c
 * @author   Team VOUT
 * @date     11 Oct 2016
 * @version  0.1
 * @brief
 *
*/
#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

#include <pmap.h>
#include <mmap.h>


/** @brief Function List
 */
void sighandler(int);
void exit_function(int);
char *string_malloc(char *);
extern int test_main(char *, char *);

int end_of_app = 0;


/** @brief Main function
 *  @return returns 0 if successful
 */
int main(int argc, char **argv)
{
	char *file_name;
	char *pmap_name;

	if (argc != 3) {
		printf("\nUasge:\n");
		printf("%s [file name] [pmap name]\n", argv[0]);
		printf("- file name: data file about test cases\n");
		printf("- pmap name: pmap name for video buffers\n");
		exit(-1);
	}

	file_name = string_malloc(argv[1]);
	pmap_name = string_malloc(argv[2]);

	signal(SIGINT, sighandler);

	test_main(file_name, pmap_name);

	do {
		pause();
	} while (end_of_app == 0);

	free(file_name);
	free(pmap_name);

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
		//exit_function(0);
		end_of_app = 1;
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

char *string_malloc(char *sz)
{
    char *ret = NULL;
    size_t len = 0;

    if (sz) {
        len = strlen(sz);
        ret = (char *)malloc((len * sizeof(char)) + sizeof(char));
        if (ret) {
            strcpy(ret, sz);
        }
    }
    return ret;
}

