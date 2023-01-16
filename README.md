# CShell
CShell is a command-line interprer that mimics a Linux/Unix command line made using C. This shell supports listing files and directories in the file system, history of executed commands, and environment variables. Upon calling an unsupported function, CShell will create a fork to a child process, where the command will be passed and executed by the normal Linux/Unix terminal. The built-in function include:
```
$var=World

print Hello $var

theme red

log

ls -i -R -l filename
```
1. A variable is signified by the '$' sign and whatever is on the right of the '=' sign is assigned to that variable. This variable can then be printed and will replace whatever name it had with the value it was assigned.
2. This program supports the themes red, blue and green
3. You can provide the flags -i -R -l (case sensitive) when using ls. They have to be provided prior to the file/directory.
    
    -i: Prints the index number of each file

    -R: Lists subdirectories by doing a recursive call

    -l: Uses a long listing format
4. ls takes in special paths similar to that of the Linux/Unix terminal. '~' us the user's home folder, '.' is the current directory, '*' is the wildcard, etc.


#

Upon running ./cshell, the shell will appear on the bash as follows:

```bash
./cshell
$cshell 
```

This shell also supports a scripting mode, where commands can be run in batch by providing a .txt file after running the program. The following testScripts.txt is provided in the repo.

```bash
./cshell testScripts.txt

foo 
/home/user/CShell
user
Sun Jan 15 16:46:12 2023
 $VAR=foo 0
Sun Jan 15 16:46:12 2023
 print 0
Sun Jan 15 16:46:12 2023
 pwd 0
Sun Jan 15 16:46:12 2023
 whoami 0
Sun Jan 15 16:46:12 2023
 theme 0
Bye!
```


## Disclaimer

A Linux/Unix environment is required to run this program successfully as it uses the Linux/Unix system calls to run certain functions. Windows users can run this program through [WSL](https://en.wikipedia.org/wiki/Windows_Subsystem_for_Linux), of which the download link can be found [here](https://ubuntu.com/wsl)