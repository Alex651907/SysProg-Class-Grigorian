1. Your shell forks multiple child processes when executing piped commands. How does your implementation ensure that all child processes complete before the shell continues accepting user input? What would happen if you forgot to call waitpid() on all child processes?

The shell uses waitpid() on each child process in the parent process in execute_pipeline. The parent process needs to wait for the child process to finish by using waitpid(). If we didn't do this, it could potentially lead to zombie processes that run indefinitely and are never used by the parent. It could also lead to resource leaks and other unpredictable errors.

2. The dup2() function is used to redirect input and output file descriptors. Explain why it is necessary to close unused pipe ends after calling dup2(). What could go wrong if you leave pipes open?

dup2() duplicates the pipe file descriptors to STDIN_FILENO and STDOUT_FILENO. You need to close them to avoid resource leaks or hanging pipelines, for example, if a process does not close its write end, the reader could end up waiting forever for EOF.

3. Your shell recognizes built-in commands (cd, exit, dragon). Unlike external commands, built-in commands do not require execvp(). Why is cd implemented as a built-in rather than an external command? What challenges would arise if cd were implemented as an external process?

cd needs to be a built-in command, because if it were an external command executed using execvp(), it would change the directory for the child process but not for the parent process, since each process has its own directory.

4. Currently, your shell supports a fixed number of piped commands (CMD_MAX). How would you modify your implementation to allow an arbitrary number of piped commands while still handling memory allocation efficiently? What trade-offs would you need to consider?

You could use malloc() to store the command list dynamically and allow any number of pipes commands. This makes the code generally a lot more complex, with error handling to consider over an unpredictable string of commands.