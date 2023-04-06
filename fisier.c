#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void print_type(struct stat file){

    //check POSIX flags
    if(S_ISREG(file.st_mode)){
        printf("REGULAR\n");
    }

    if(S_ISDIR(file.st_mode)){
        printf("DIRECTORY\n");
    }

    if(S_ISLNK((file.st_mode))){
        printf("SYMBOLIC LINK\n");
    }


}

void menu(struct stat file){
    printf("---- MENU ----\n");

    if(S_ISREG(file.st_mode)){
        printf("\u2022 n: name\n\u2022 d: size\n\u2022 h: hard link count\n\u2022 m: time of last modification\n\u2022 a: access rights\n\u2022 l: create symbolic link\n");
        return;
    }

    if(S_ISDIR(file.st_mode)){
        printf("\u2022 n: name\n\u2022 d: size\n\u2022  a: access rights\n\u2022 c: nr of files with extension .c\n");
        return;
    }

    if(S_ISLNK(file.st_mode)){
        printf("\u2022 n: name\n\u2022 l: delete symbolic link\n\u2022 d: size of symbolic link\n\u2022 t: size of target file\n\u2022 a: access rights\n");
        return;
    }
}
void option_link(char* file_name, int length, char* option_s){
    for(int i=1;i<length;i++){
            if(option_s[i]=='n'){
                printf("The name is %s\n", file_name);
            }

            if(option_s[i]=='l'){
                retVal = unlink(file_name);

                if(check == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                printf("Symbolic link has been deleted");
                return;
            }

            if(option_s[i]=='d'){
                long size;
                size = buf.st_size;
                printf("The size is %ld bytes\n", size);
            }

            if(option_s[i]=='t'){
                struct stat buffer2;

                retVal = stat(file_name, &buffer2);
                if(retVal == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                long size;
                size = buf2.st_size;
                printf("The size is %ld bytes\n", size);
            }

            if(option_s[i]=='a'){

            }
    }
}

void option_dir(char* file_name, int length, char* option_s){
    for(int i=1;i<length;i++){
        if(option_s[i]=='n'){
            printf("The name is %s\n", file_name);
        }
        if(option_s[i]=='d'){
            printf("The name is %s\n", file_name);
        }
        if(option_s[i]=='c'){
             printf("The name is %s\n", file_name);
        }
        if(option_s[i]=='a'){
             printf("The name is %s\n", file_name);
        }
    }
}
void option_reg(char* file_name, int length, char* option_s){
    for(i=1;i<length;i++){
            if(option_s[i]=='n'){
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

            if(option_s[i]=='l'){
                char link_name[1024];

                printf("Input name of link: ");
                scanf("%s", link_name);

                retVal = symlink(file_name, link_name);
                if(check == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                printf("Link has been created!\n");
            }
    }
}


void option(char* file_name){

    struct stat buffer;
    int retVal = 0;

    retVal = lstat(file_name, &buffer);
    if(retVal == -1){
        perror(strerror(errno));
        exit(errno);
    }

    printf("Please enter your options!\n");

    char option_s[10];
    int length;

    scanf("%s", option_s);
    
    length = strlen(option_s);

    if(option_s[0]!='-'){
        printf("Wrong input\n");
        exit(-1);
    }


    if(S_ISREG(buf.st_mode)){

        //check option string for regular file
        for(int i=1;i<length;i++){
            if((option_s[i]!='n')||(option_s[i]!='d')||(option_s[i]!='h')||(option_s[i]!='m')||(option_s[i]!='a')||(option_s[i]!='l')){
                printf("Wrong input\n");
                exit(-1);
            }
        }
        option_reg(file_name, length, option_s);
        return;
    }
        
    

    if(S_ISDIR(buf.st_mode)){
        //check option string for links file
        for(int i=1;i<length;i++){
            if((option_s[i]!='n')||(option_s[i]!='d')||(option_s[i]!='c')||(option_s[i]!='a')){
                printf("Wrong input\n");
                exit(-1);
            }
        }
        option_dir(file_name, length, option_s);
        return;
    }

    if(S_ISLNK(buf.st_mode)){
        //check option string for links file
        for(int i=1;i<length;i++){
            if((option_s[i]!='n')||(option_s[i]!='d')||(option_s[i]!='t')||(option_s[i]!='a')||(option_s[i]!='l')){
                printf("Wrong input\n");
                exit(-1);
            }
        }
        option_link(file_name, length, option_s);
        return;
    }
}

int main(int argc, char* argv[]){
    
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
    option(filename);

    return 0;
}
