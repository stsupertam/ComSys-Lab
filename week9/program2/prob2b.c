#include <stdio.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#define WRFLAGS (O_WRONLY | O_CREAT | O_TRUNC)
#define MODE600 (S_IRUSR | S_IWUSR)
main()
{
    close(0); // close standard input
    if (open("4300.txt", O_RDONLY) == -1)
	printf("open input file failed\n");
    close(1); // close standard output
    if (open("tt", WRFLAGS, MODE600) == -1)
	printf("open output failed\n");
    execlp("wc", "wc", (char *) 0); // execute wc command
    printf("failed to execute ‘cat’\n");
}
