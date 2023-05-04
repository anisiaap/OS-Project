#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <dirent.h>
#include <libgen.h>
/**
 * global variables
 */
char option_string_ac[10];
int lengthOfOptionString_u;
/**
 * function used to print the menu for each type of file, acc to POSIX flags
 */
void type_menu(struct stat file)
{

    if (S_ISREG(file.st_mode))
    {
        printf("REGULAR\n");
        printf("\n---MENU---\n");
        printf("\u2022 n: name\n\u2022 d: size\n\u2022 h: hard link count\n\u2022 m: time of last modification\n\u2022 a: access rights\n\u2022 l: create symbolic link\n");
        return;
    }

    if (S_ISDIR(file.st_mode))
    {
        printf("DIRECTORY\n");
        printf("\n---MENU---\n");
        printf("\u2022 n: name\n\u2022 d: size\n\u2022  a: access rights\n\u2022 c: nr of files with extension .c\n");
        return;
    }

    if (S_ISLNK((file.st_mode)))
    {
        printf("SYMBOLIC LINK\n");
        printf("\n---MENU---\n");
        printf("\u2022 n: name\n\u2022 l: delete symbolic link\n\u2022 d: size of symbolic link\n\u2022 t: size of target file\n\u2022 a: access rights\n");
        return;
    }
}
void accesUser(struct stat buf)
{
    if ((buf.st_mode & S_IRUSR) != 0)
    {
        printf("Read - yes\n");
    }
    else
    {
        printf("Read - no\n");
    }
    if ((buf.st_mode & S_IWUSR) != 0)
    {
        printf("Write - yes\n");
    }
    else
    {
        printf("Write - no\n");
    }
    if ((buf.st_mode & S_IXUSR) != 0)
    {
        printf("Exec - yes\n");
    }
    else
    {
        printf("Exec - no\n");
    }
}
void accesGroup(struct stat buf)
{
    if ((buf.st_mode & S_IRGRP) != 0)
    {
        printf("Read - yes\n");
    }
    else
    {
        printf("Read - no\n");
    }
    if ((buf.st_mode & S_IWGRP) != 0)
    {
        printf("Write - yes\n");
    }
    else
    {
        printf("Write - no\n");
    }
    if ((buf.st_mode & S_IXGRP) != 0)
    {
        printf("Exec - yes\n");
    }
    else
    {
        printf("Exec - no\n");
    }
}
void accesOther(struct stat buf)
{
    if ((buf.st_mode & S_IROTH) != 0)
    {
        printf("Read - yes\n");
    }
    else
    {
        printf("Read - no\n");
    }
    if ((buf.st_mode & S_IWOTH) != 0)
    {
        printf("Write - yes\n");
    }
    else
    {
        printf("Write - no\n");
    }
    if ((buf.st_mode & S_IXOTH) != 0)
    {
        printf("Exec - yes\n");
    }
    else
    {
        printf("Exec - no\n");
    }
}
/**
 * function used to parse and do the actions for the string input of commands for links
 */
void option_link(char *link_path, struct stat buf, int length, char *option_s)
{
    pid_t pid;
    pid = fork();
    for (int i = 1; i < length; i++)
    {
        if (option_s[i] == 'n')
        {
            printf("Name of symbolic link: %s\n", basename(link_path));
        }

        if (option_s[i] == 'l')
        {
            int retVal = unlink(link_path);

            if (retVal == -1)
            {
                perror(strerror(errno));
                exit(errno);
            }

            printf("Symbolic link has been deleted");
            return;
        }

        if (option_s[i] == 'd')
        {
            long size;
            size = buf.st_size;
            printf("Size of symbolic link: %ld bytes\n", size);
        }

        if (option_s[i] == 't')
        {
            struct stat buffer2;

            int retVal = stat(link_path, &buffer2);
            if (retVal == -1)
            {
                perror(strerror(errno));
                exit(errno);
            }

            long size;
            size = buffer2.st_size;
            printf("Size of target file: %ld bytes\n", size);
        }

        if (option_s[i] == 'a')
        {
            printf("Access rights:\n");
            printf("\nUser:\n\n");
            accesUser(buf);
            printf("\nGroup:\n\n");
            accesGroup(buf);
            printf("\nOther:\n\n");
            accesOther(buf);
        }
    }
    exit(10);
}

/**
 * function used to parse and do the actions for the string input of commands for directories
 */

void option_dir(char *dir_path, struct stat buf, int length, char *option_s)
{
    pid_t pid;
    pid = fork();
    for (int i = 1; i < length; i++)
    {
        if (option_s[i] == 'n')
        {
            printf("Name of directory: %s\n", basename(dir_path));
        }
        if (option_s[i] == 'd')
        {
            long size;
            size = buf.st_size;
            printf("Size of directory: %ld bytes\n", size);
        }
        if (option_s[i] == 'c')
        {
            int file_count = 0;
            DIR *dirp;
            struct dirent *entry;

            dirp = opendir(dir_path); /* There should be error handling after this */
            while ((entry = readdir(dirp)) != NULL)
            {
                if (strstr(entry->d_name, ".c") != 0)
                { /* If the entry is a regular file */
                    file_count++;
                }
            }
            closedir(dirp);
            printf("The number of files with .c extension %c\n", file_count);
        }
        if (option_s[i] == 'a')
        {
            printf("Access rights:\n");
            printf("\nUser:\n\n");
            accesUser(buf);
            printf("\nGroup:\n\n");
            accesGroup(buf);
            printf("\nOther:\n\n");
            accesOther(buf);
        }
    }
    exit(9);
}

/**
 * function used to parse and do the actions for the string input of commands for regular files
 */

void option_reg(char *filepath, struct stat buf, int length, char *option_s)
{
    pid_t pid;
    pid = fork();
    if (pid == 0)
    {
        for (int i = 1; i < length; i++)
        {
            if (option_s[i] == 'n')
            {
                printf("Name of file: %s\n", basename(filepath));
            }

            if (option_s[i] == 'd')
            {
                long size;
                size = buf.st_size;
                printf("Size of file: %ld bytes\n", size);
            }

            if (option_s[i] == 'h')
            {
                int count;
                count = buf.st_nlink;
                printf("Hard link count:%d\n", count);
            }

            if (option_s[i] == 'm')
            {
                struct timespec ts;
                timespec_get(&ts, buf.st_mtime);
                printf("Last modification time: %ld.%.9ld\n", ts.tv_sec, ts.tv_nsec);
            }

            if (option_s[i] == 'a')
            {
                printf("Access rights:\n");
                printf("\nUser:\n\n");
                accesUser(buf);
                printf("\nGroup:\n\n");
                accesGroup(buf);
                printf("\nOther:\n\n");
                accesOther(buf);
            }

            if (option_s[i] == 'l')
            {
                char link_name[1024];

                printf("Please give the link name: \nSTANDARD INPUT:");
                scanf("%s", link_name);

                int retVal = symlink(filepath, link_name);
                if (retVal == -1)
                {
                    perror(strerror(errno));
                    exit(errno);
                }
                printf("STANDARD OUTPUT: Link has been created!\n");
            }
        }
        exit(8);
    }
}
void readOptions()
{
    printf("\nPlease enter your options!\n\nSTANDARD INPUT:");
    if (scanf("%s", option_string_ac) == 0)
    {
        printf("Wrong input, not a valid option\n");
        exit(-1);
    }
    printf("\n");
    lengthOfOptionString_u = strlen(option_string_ac);

    if (option_string_ac[0] != '-')
    {
        printf("Wrong input, it should start with '-'\n");
        exit(-1);
    }
}
void checkOptionStringReg()
{
    for (int i = 1; i < lengthOfOptionString_u; i++)
    {
        if ((option_string_ac[i] != 'n') && (option_string_ac[i] != 'd') && (option_string_ac[i] != 'h') && (option_string_ac[i] != 'm') && (option_string_ac[i] != 'a') && (option_string_ac[i] != 'l'))
        {
            printf("Wrong input, not a valid option\n");
            exit(-1);
        }
    }
}
void checkOptionStringDir()
{
    for (int i = 1; i < lengthOfOptionString_u; i++)
    {
        if ((option_string_ac[i] != 'n') && (option_string_ac[i] != 'd') && (option_string_ac[i] != 'c') && (option_string_ac[i] != 'a'))
        {

            printf("Wrong input, not a valid option\n");
            exit(-1);
        }
    }
}
void checkOptionStringLink()
{
    for (int i = 1; i < lengthOfOptionString_u; i++)
    {
        if ((option_string_ac[i] != 'n') && (option_string_ac[i] != 'd') && (option_string_ac[i] != 't') && (option_string_ac[i] != 'a') && (option_string_ac[i] != 'l'))
        {
            printf("Wrong input, not a valid option\n");
            exit(-1);
        }
    }
}
/**
 * function used to check the list of commands
 * and
 * do the actions of each commands accordingly with the type of
 * file
 */
void option(struct stat buf, char *filepath)
{
    /**
     * check option string for regular files
     */
    if (S_ISREG(buf.st_mode))
    {
        checkOptionStringReg();
        option_reg(filepath, buf, lengthOfOptionString_u, option_string_ac);
        return;
    }
    /**
     * check option string for directories
     */
    if (S_ISDIR(buf.st_mode))
    {
        checkOptionStringDir();
        option_dir(filepath, buf, lengthOfOptionString_u, option_string_ac);
        return;
    }
    /**
     * check option string for links file
     */
    if (S_ISLNK(buf.st_mode))
    {
        checkOptionStringLink();
        option_link(filepath, buf, lengthOfOptionString_u, option_string_ac);
        return;
    }
}
/**
 * create a parent process, then for each child process and handle the options from the
 * menu accordingly
 */
int main(int argc, char *argv[])
{
    /**
     * error handling for wrong input of arguments
     */
    pid_t pid, w;
    int wstatus;
    if (argc < 2)
    {
        printf("not enough argcs\n");
        exit(-1);
    }

    for (int i = 1; i < argc; i++)
    {

        char filepath[1024];
        strcpy(filepath, argv[i]);
        printf("%s - ", basename(filepath));

        // use lstat on file and print file type
        struct stat buf;
        int check = lstat(filepath, &buf);

        if (check == -1)
        {
            perror(strerror(errno));
            exit(errno);
        }
        type_menu(buf);
        option(buf, filepath);

        if (pid > 0)
        {
            printf("this is the parent process\n");
            w = wait(&wstatus);
            if (WIFEXITED(wstatus))
            {
                printf("process with pid %d, exited, status = %d\n", w, WEXITSTATUS(wstatus));
            }
        }
    }

    return 0;
}
