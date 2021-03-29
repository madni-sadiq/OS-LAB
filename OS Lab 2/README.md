# A Linux Shell Using C Programming #
This C code can be compiled only in Linux machine simply by GCC compiler using **gcc my_shell.c -o shell"**\
where "shell" is the name of executable to be created. You may use any name convenient to you.\
\
This shell has two modes:
Markup : * Batch Mode
              * If a batch file of commands is provided as command-line input to the program, shell executes all the commands in the batch file one after the other.
              * Batch file input can be fed to the shell by typing **./shell commands.txt**, where "commands.txt" is the file containing commands.
              * Shell exits automatically after executing all the commands of input file.
         * Interactive Mode
              * Interactive mode of shell can be activated by typing **./shell**.
              * Almost all the basic linux commands are available in the shell.
              * History feature is also available which stores one previous command and can be used by type *!!* command inside the shell.
              * Input/Output redirection is also available but limited to one operation, either Reading or Writing a file, at a time.
