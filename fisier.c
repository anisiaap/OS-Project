#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void print_type(struct stat buf){

    //check POSIX flags
    if(S_ISREG(buf.st_mode)){
        printf("REGULAR\n");
    }

    if(S_ISDIR(buf.st_mode)){
        printf("DIRECTORY\n");
    }

    if(S_ISLNK((buf.st_mode))){
        printf("SYMBOLIC LINK\n");
    }

    /*if(S_ISCHAR((buf.st_mode))){
        strcpy(type, "CHARACTER DEVICE");
        return type;
    }

    if(S_ISBLK((buf.st_mode))){
        strcpy(type, "BLOCK DEVICE");
        return type;
    }

    if(S_ISFIFO((buf.st_mode))){
        strcpy(type, "FIFO");
        return type;
    }*/
}

void menu(struct stat buf){
    printf("---- MENU ----\n");

    if(S_ISREG(buf.st_mode)){
        printf("\u2022 n: name\n\u2022 d: size\n\u2022 h: hard link count\n\u2022 m: time of last modification\n\u2022 a: access rights\n\u2022 l: create symbolic link\n");
        return;
    }

    if(S_ISDIR(buf.st_mode)){
        printf("Will be added\n");
        return;
    }

    if(S_ISLNK(buf.st_mode)){
        printf("\u2022 n: name\n\u2022 l: delete symbolic link\n\u2022 d: size of symbolic link\n\u2022 t: size of target file\n\u2022 a: access rights\n");
        return;
    }
}

void input_options(char* file_name){

    struct stat buf;
    int check;

    check = lstat(file_name, &buf);
    if(check == -1){
        perror(strerror(errno));
        exit(errno);
    }

    printf("Please enter your options!\n");

    char s[10];
    int i, l;
    scanf("%s", s);

    if(s[0]!='-'){
        printf("Wrong input\n");
        exit(-1);
    }

    l = strlen(s);

    if(S_ISREG(buf.st_mode)){
        for(i=0;i<l;i++){
            if(s[i]=='n'){
                printf("The name is %s\n", file_name);
            }

            if(s[i]=='d'){
                long size;
                size = buf.st_size;
                printf("The size is %ld bytes\n", size);
            }

            if(s[i]=='h'){
                int count;
                count = buf.st_nlink;
                printf("The number of hard links is %d\n", count);
            }

            if(s[i]=='m'){
                struct timespec ts;
                timespec_get(&ts, buf.st_mtime);
                printf("The last modification time is %ld.%.9ld\n", ts.tv_sec, ts.tv_nsec);
            }

            if(s[i]=='a'){
                printf("User:\n"){
                    if(buf.st_mode & S_IRUSR){
                        
                    }
                }
            }

            if(s[i]=='l'){
                char link_name[1024];

                printf("Input name of link: ");
                scanf("%s", link_name);

                check = symlink(file_name, link_name);
                if(check == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                printf("Link has been created!\n");
            }
        }
        return;
    }
        
    

    if(S_ISDIR(buf.st_mode)){
        printf("Not yet\n");
        return;
    }

    if(S_ISLNK(buf.st_mode)){
        for(i=0;i<l;i++){
            if(s[i]=='n'){
                printf("The name is %s\n", file_name);
            }

            if(s[i]=='l'){
                check = unlink(file_name);

                if(check == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                printf("Symbolic link has been deleted");
                return;
            }

            if(s[i]=='d'){
                long size;
                size = buf.st_size;
                printf("The size is %ld bytes\n", size);
            }

            if(s[i]=='t'){
                struct stat buf2;

                check = stat(file_name, &buf2);
                if(check == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                long size;
                size = buf2.st_size;
                printf("The size is %ld bytes\n", size);
            }

            if(s[i]=='a'){

            }
        }
        return;
    }
}

int main(int argc, char* argv[]){
    
    //verify if the argument number is wrong
    /*if(argc != 3){
        printf("Wrong number of arguments");
        exit(-1);
    }*/

    //get and print name of file
    char filename[1024];
    strcpy(filename, argv[1]);
    printf("%s - ", filename);
    
    //use lstat on file and print file type
    int check;
    struct stat buf;
    check = lstat(filename, &buf);
    if(check == -1){
        perror(strerror(errno));
        exit(errno);
    }

    print_type(buf);

    //print Menu for file type
    menu(buf);

    //input of the desired options
    input_options(filename);

    return 0;
}
