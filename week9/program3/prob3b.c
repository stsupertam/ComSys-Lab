#include <stdio.h>
#include <sys/file.h>
#include <sys/dir.h>
#include <sys/stat.h>



long processFile(name) char *name;
{
    struct stat statBuf; /* To hold the return data from stat() */
    mode_t mode;
    int result;

    result = stat(name, &statBuf); /* Stat the specified file */
    if (result == -1)
        return (0); /* Error */

    mode = statBuf.st_mode; /* Look at the file’s mode */
    if (S_ISDIR(mode))      /* Directory */
        return (processDirectory(name));
    else
        return (1); /* A nondirectory file was processed */
}
long processDirectory(dirName) char *dirName;
{
    int fd, children, i, charsRead, childPid, status;
    long count, totalCount;
    char fileName[100];
    struct dirent dirEntry;

    fd = open(dirName, O_RDONLY); /* Open directory for reading */
    children = 0;                 /* Initialize child process count */
    while (1)                     /* Scan directory */
    {
        charsRead = getdents(fd, &dirEntry, sizeof(struct dirent));
        if (charsRead == 0)
            break; /* End of directory */
        if (strcmp(dirEntry.d_name, ".") != 0 &&
            strcmp(dirEntry.d_name, "..") != 0)
        {
            if (fork() == 0) /* Creates a child to process dir.  entry */
            {
                sprintf(fileName, "%s/%s", dirName, dirEntry.d_name);
                count = processFile(fileName);
                exit(count);
            }
            else
                ++children; /* Increment count of child processes */
        }
        lseek(fd, dirEntry.d_off, SEEK_SET); /* Jump to next dir. entry */
    }
    close(fd);                      /* Close directory */
    totalCount = 0;                 /* Initialize file count */
    for (i = 1; i <= children; i++) /* Wait for children to terminate */
    {
        childPid = wait(&status);    /* Accept child’s termination code */
        totalCount += (status >> 8); /* Update file count */
    }
    return (totalCount); /* Return number of files in directory */
}

int main(int argc, char **argv)
{
    long count;
    count = processFile(argv[1]);
    printf("Total number of non-directory files is %ld \n", count);
    return (/* EXIT_SUCCESS */ 0);
}