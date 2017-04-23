//Sample Program: Disk Usage

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/file.h>
#include <sys/dir.h>
#include <sys/stat.h>

long processFile();
long processDirectory();
main(argc, argv) int argc;

char *argv[];
{
    long count;
    count = processFile(argv[1]);
    printf("Total number of non-directory files is %ld \n", count);
    return (/* EXIT_SUCCESS */ 0);
}

long processFile(name) char *name;
{
    struct stat statBuf; /* To hold the return data from stat() */
    mode_t mode;
    int result;

    result = stat(name, &statBuf); /* Stat the specified file */
    if (result == -1)
        return (0); /* Error */

    mode = statBuf.st_mode; /* Look at the file’s mode */
    if (S_ISDIR(mode))
    {
        return (processDirectory(name)); /* Directory */
    }
    else
    {
        return (1); /* A nondirectory file was processed */
    }
}

long processDirectory(dirName) char *dirName;
{
    int fd, children, i, childPid, status;
    long count, totalCount;
    char fileName[100];
    struct dirent *dirEntry;

    DIR *dir = opendir(dirName); /* Open directory for reading */

    children = 0; /* Initialize child process count */
    while (1)     /* Scan directory */
    {
        dirEntry = readdir(dir);
        if (dirEntry == NULL)
            break; /* End of directory */
        if (strcmp(dirEntry->d_name, ".") != 0 &&
            strcmp(dirEntry->d_name, "..") != 0)
        {
            if (fork() == 0) /* Creates a child to process dir.  entry */
            {
                sprintf(fileName, "%s/%s", dirName, dirEntry->d_name);
                count = processFile(fileName);
                exit(count);
            }
            else
            {
                close(fd[1]); // close up output side of the pipe
                int returnStatus;
                waitpid(pid, &returnStatus, 0);                 // wait for little child to terminate
                read(fd[0], &child_count, sizeof(child_count)); // get counter value from child
                counter = counter + child_count;
            }
            ++children; /* Increment count of child processes */
        }

        lseek(fd, dirEntry->d_off, SEEK_SET); /* Jump to next dir. entry */
    }

    close(fd);                      /* Close directory */
    totalCount = 0;                 /* initialize file count */
    for (i = 1; i <= children; i++) /* wait for children to terminate */
    {
        childPid = wait(&status);    /* accept child’s termination code */
        totalCount += (status >> 8); /* update file count */
    }
    return (totalCount); /* return number of files in directory */
}
