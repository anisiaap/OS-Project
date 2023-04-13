#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

void type_menu(struct stat file){

    //check POSIX flags
    if(S_ISREG(file.st_mode)){
        printf("REGULAR\n");
       printf("\n---MENU---\n");
        printf("\u2022 n: name\n\u2022 d: size\n\u2022 h: hard link count\n\u2022 m: time of last modification\n\u2022 a: access rights\n\u2022 l: create symbolic link\n");
        return;
    }

    if(S_ISDIR(file.st_mode)){
        printf("DIRECTORY\n");
        printf("\n---MENU---\n");
        printf("\u2022 n: name\n\u2022 d: size\n\u2022  a: access rights\n\u2022 c: nr of files with extension .c\n");
        return;
    }

    if(S_ISLNK((file.st_mode))){
        printf("SYMBOLIC LINK\n");
        printf("\n---MENU---\n");
        printf("\u2022 n: name\n\u2022 l: delete symbolic link\n\u2022 d: size of symbolic link\n\u2022 t: size of target file\n\u2022 a: access rights\n");
        return;
    }

}

void option_link(char* file_name,struct stat buf, int length, char* option_s){
    for(int i=1;i<length;i++){
            if(option_s[i]=='n'){
                printf("Name of symbolic link: %s\n", file_name);
            }

            if(option_s[i]=='l'){
                int retVal = unlink(file_name);

                if(retVal == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                printf("Symbolic link has been deleted");
                return;
            }

            if(option_s[i]=='d'){
                long size;
                size = buf.st_size;
                printf("Size of symbolic link: %ld bytes\n", size);
            }

            if(option_s[i]=='t'){
                struct stat buffer2;

                int retVal = stat(file_name, &buffer2);
                if(retVal == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                long size;
                size = buffer2.st_size;
                printf("Size of target file: %ld bytes\n", size);
            }

            if(option_s[i]=='a'){

            }
    }
}

void option_dir(char* file_name,struct stat buf, int length, char* option_s){
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
void option_reg(char* file_name,struct stat buf, int length, char* option_s){
    for(int i=1;i<length;i++){
            if(option_s[i]=='n'){
                printf("Name of file: %s\n", file_name);
            }

            if(option_s[i]=='d'){
                long size;
                size = buf.st_size;
                printf("Size of file: %ld bytes\n", size);
            }

            if(option_s[i]=='h'){
                int count;
                count = buf.st_nlink;
                printf("Hard link count:%d\n", count);
            }

            if(option_s[i]=='m'){
                struct timespec ts;
                timespec_get(&ts, buf.st_mtime);
                printf("Last modification time: %ld.%.9ld\n", ts.tv_sec, ts.tv_nsec);
            }

            if(option_s[i]=='a'){
                printf("Access rights:\n");
                printf("User:\n");
                // if(buf.st_mode & S_IRUSR){
                        
                //     }
            }

            if(option_s[i]=='l'){
                char link_name[1024];

                printf("Please give the link name: \nSTANDARD INPUT:");
                scanf("%s", link_name);

                int retVal = symlink(file_name, link_name);
                if(retVal == -1){
                    perror(strerror(errno));
                    exit(errno);
                }

                printf("STANDARD OUTPUT: Link has been created!\n");
            }
    }
}


void option(struct stat buf, char *filename){
    char option_s[10];
    int length;

    printf("\nPlease enter your options!\n\nSTANDARD INPUT:");
    if(scanf("%s", option_s) == 0){
        printf("Wrong input\n");
        exit(-1);
    }
    printf("\n");
    length = strlen(option_s);

    if(option_s[0]!='-'){
        printf("Wrong input\n");
        exit(-1);
    }

    if(S_ISREG(buf.st_mode)){

        //check option string for regular file
        for(int i=1;i<length;i++){
            if((option_s[i]=='n')||(option_s[i]=='d')||(option_s[i]=='h')||(option_s[i]=='m')||(option_s[i]=='a')||(option_s[i]=='l')){
               /**
                * do nothing
               */
            }
            else{
                printf("Wrong input\n");
                exit(-1);
            }
        }
        option_reg(filename, buf, length, option_s);
        return;
    }
        
    

    if(S_ISDIR(buf.st_mode)){
        //check option string for links file
        for(int i=1;i<length;i++){
            if((option_s[i]=='n')||(option_s[i]=='d')||(option_s[i]=='c')||(option_s[i]=='a')){
                 /**
                * do nothing
               */
            }
            else{
                printf("Wrong input\n");
                exit(-1);
            }
        }
        option_dir(filename, buf, length, option_s);
        return;
    }

    if(S_ISLNK(buf.st_mode)){
        //check option string for links file
        for(int i=1;i<length;i++){
            if((option_s[i]=='n')||(option_s[i]=='d')||(option_s[i]=='t')||(option_s[i]=='a')||(option_s[i]=='l')){
                 /**
                * do nothing
               */
            }
            else{
                printf("Wrong input\n");
                exit(-1);
            }
        }
        option_link(filename, buf, length, option_s);
        return;
    }
}

int main(int argc, char* argv[]){
    
    if(argc!=2){
        printf("not 2 argc\n");
        exit(-1);
    }
    //get and print name of file
    char filename[1024];
    strcpy(filename, argv[1]);
    printf("%s - ", filename);
    
    //use lstat on file and print file type
    struct stat buf;
    int check = lstat(filename, &buf);
    if(check == -1){
        perror(strerror(errno));
        exit(errno);
    }
    
    //print Menu and type for file type
    type_menu(buf);

    //input of the desired options
    option(buf, filename);

    return 0;
}
