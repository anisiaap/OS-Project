#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>
#include <libgen.h>
#include <dirent.h>
#include <sys/wait.h>
#include <fcntl.h>
/**
 * global variables
 */
char option_string_ac[10];
int lengthOfOptionString_u;
pid_t pid, w;
pid_t pid2, w2;
int wstatus, wstatus2;
int errors = 0, warnings = 0, score = 0;
int pfd[2];
/**
 * wait for all children for the parent process
 */
void wait_kids()
{
    printf("this is the parent process\n");

    for (int i = 1; i <= 2; i++)
    {
        w = wait(&wstatus);
        if (WIFEXITED(wstatus))
        {
            printf(" process terminated ok with pid %d, exited, status = %d\n", w, WEXITSTATUS(wstatus));
        }
        else
        {
            printf(" process terminated not ok with pid %d, exited, status = %d\n", w, WEXITSTATUS(wstatus));
        }
    }
}
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
    // char option
    for (int i = 1; i < length; i++)
    {
        printf("\n optiune : %c\n", option_s[i]);
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
     * create 1st child process to handle the op for args
     */
    pid = fork();
    if (pid < 0)
    {
        perror(strerror(errno));
        exit(errno);
    }
    // if we are in the first child process
    if (pid == 0)
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
        exit(EXIT_SUCCESS);
    }
    //?? else if (pid > 0)
    // {
    //     wait_kids();
    // }
}

void calc_score(char *filepath)
{
    // int pfd[2];
    FILE *stream;
    char buff[1024] = "";

    close(pfd[1]);
    int byte_read = read(pfd[0], buff, 1024);
    if (byte_read <= 0)
    {
        printf("error no bytes read from script");
        exit(-1);
    }

    printf("am citit %s \n", buff);

    char *token = strtok(buff, " ");
    char *first_word = token;
    token = strtok(NULL, " ");
    char *second_word = token;
    errors = atoi(first_word);
    warnings = atoi(second_word);

    if (errors == 0 && warnings == 0)
    {
        score = 10;
    }

    if (errors >= 1)
    {
        score = 1;
    }

    if (errors == 0 && warnings > 10)
    {
        score = 2;
    }

    if (errors == 0 && warnings <= 10)
    {
        score = 2 + 8 * (10 - warnings) / 10;
    }

    int fd;

    fd = open("grades.txt", O_RDWR | O_CREAT, S_IRWXU);
    if (fd == -1)
    {
        perror(strerror(errno));
        exit(errno);
    }

    char filename[1024];
    strcpy(filename, basename(filepath));

    char score_string[3];
    if (score < 10)
    {
        score_string[0] = score + '0';
    }
    else
    {
        score_string[0] = score / 10 + '0';
        score_string[1] = score % 10 + '0';
    }

    char output[1050];
    strcpy(output, filename);
    strcat(output, ":");
    strcat(output, score_string);
    strcat(output, "\n\0");

    int check;
    check = write(fd, output, strlen(output));
    if (check == -1)
    {
        perror(strerror(errno));
        exit(errno);
    }
    close(fd);
    close(pfd[0]);
}
void create_file(char *filepath, struct stat buf)
{
    if (pid2 == 0)
    {
        char new_filename[1050], new_filepath[1050];

        strcpy(new_filename, basename(filepath));
        strcat(new_filename, "_file.txt");

        strcpy(new_filepath, filepath);
        strcat(new_filepath, "/");
        strcat(new_filepath, new_filename);

        int check;
        check = execlp("touch", "touch", new_filepath, NULL);
        if (check == -1)
        {
            perror(strerror(errno));
            exit(errno);
        }
        exit(EXIT_SUCCESS);
    }
}
void change_permissions(char *filepath, struct stat buf)
{
    if (pid2 == 0)
    {
        int check;

        check = execlp("chmod", "chmod", "u+rwx,g+rw-x,o-rwx", filepath, NULL);
        if (check == -1)
        {
            perror(strerror(errno));
            exit(errno);
        }

        exit(EXIT_SUCCESS);
    }
    /*
    else if(pid2>0){
        wait_for_children();
    }*/
}
void check_extension(char *filepath, struct stat buf)
{
    // int pfd[2];
    int check;
    char filename[1024];
    strcpy(filename, basename(filepath));

    if (filename[strlen(filename) - 1] == 'c' && filename[strlen(filename) - 2] == '.') // c ext
    {

        close(pfd[0]);
        dup2(pfd[1], 1);
        check = execlp("bash", "bash", "script_count.sh", filepath, NULL);
        if (check == -1)
        {
            perror(strerror(errno));
            exit(errno);
        }
        exit(EXIT_SUCCESS);
    }

    else // not a c ext
    {

        printf("The number of lines in this file is:\n");
        check = execlp("wc", "wc", "-l", filepath, NULL);

        if (check == -1)
        {
            perror(strerror(errno));
            exit(errno);
        }
        exit(EXIT_SUCCESS);
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
        readOptions();
        option(buf, filepath); //-here will be created 1st new child processes
        /**
         * check we will wait for children in parent process
         */

        /**
         * create sec child process
         */
        if (pipe(pfd) < 0)
        {
            perror(strerror(errno));
            exit(errno);
        }
        pid2 = fork();
        if (pid2 < 0)
        {
            perror(strerror(errno));
            exit(errno);
        }
        if (pid2 == 0)
        {
            if (S_ISREG(buf.st_mode))
            {
                check_extension(filepath, buf);
            }

            else if (S_ISDIR(buf.st_mode))
            {
                create_file(filepath, buf);
            }

            else if (S_ISLNK(buf.st_mode))
            {
                change_permissions(filepath, buf);
            }
            exit(EXIT_SUCCESS);
        }

        wait_kids();
        calc_score(filepath);
    }

    return 0;
}
