# OS-Project

## Overview

This is a C program that utilizes system calls and library functions in Unix to perform various operations on regular files, directories, and symbolic links. The program takes multiple arguments representing paths to these file types and provides an interactive menu with options specific to each file type.

## Functionalities

- The program passes through all the arguments.
- For each file, depending on the file type, the name of the file and the type of the file will be displayed, and an interactive menu will be displayed with all the options available for that specific file type.
- The user inputs the desired options for that file from the keyboard, as a single string preceded by the '-' sign.
- After entering the options string, the information about that file will be printed to the standard output.
- If one of the letters of the string is not a valid option, an error message will be displayed, and the menu will appear again.
- The parent process creates one child process for each argument that will handle the options introduced by the user.
- If the argument is a regular file with the .c extension, the parent creates one more process (a second child process) that compiles the file and prints the number of errors and the number of warnings to the standard output.
- If the argument is a directory, the parent creates a second child process, which executes a command for creating a file with the .txt extension and a specific name (e.g. dirName_file.txt).
- All child processes run in parallel with each other.

## Options for Each File Type

### Regular file

- Name (-n)
- Size (-d)
- Hard link count (-h)
- Time of last modification (-m)
- Access rights (-a)
- Create symbolic link (-l, this will wait for user input for the name of the link)

The access rights will be displayed in the format:
User:
Read - yes
Write - yes
Exec - no

Group:
Read - yes
Write - no
Exec - no

Others:
Read - yes
Write - no
Exec - no

markdown
Copy code

### Symbolic link

- Name (-n)
- Delete symbolic link (-l)
- Size of symbolic link (-d)
- Size of target file (-t)
- Access rights (-a)

Note that if the `-l` option is given, the other following options will no longer be performed.

### Directory

- Name (-n)
- Size (-d)
- Access rights (-a)
- Total number of files with the .c extension (-c)

## Example Usage

$ ./os-project file1.txt file2.c directory -ndal


This will execute the OS-Project program on `file1.txt`, `file2.c`, and `directory`, displaying the name and type of each file and an interactive menu with options for each file type. The user inputs the desired options for each file from the keyboard as a single string preceded by the '-' sign. In this case, the options `-ndal` are used, which will display the name, size, access rights, and create a symbolic link for `file1.txt`, compile `file2.c`, and create a text file with a specific name for `directory`.




