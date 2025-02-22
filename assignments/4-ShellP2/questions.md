1. Can you think of why we use `fork/execvp` instead of just calling `execvp` directly? What value do you think the `fork` provides?

    > **Answer**:  fork creates a new child process, allowing the parent process to continue running while the child process executes the new program. Without fork, calling execvp would replace the current process and terminate the shell.

2. What happens if the fork() system call fails? How does your implementation handle this scenario?

    > **Answer**:  If fork fails, it returns -1. The implementation checks to see if fork returns -1 and if it does, prints an error message using perror("fork") and returns an error code to indicate that fork is where the failure happened.

3. How does execvp() find the command to execute? What system environment variable plays a role in this process?

    > **Answer**:  It searches for the command in the directories listed in the PATH environment variable. If the command is found, it executes the command, and otherwise it fails.

4. What is the purpose of calling wait() in the parent process after forking? What would happen if we didn’t call it?

    > **Answer**:  wait ensures the parent process waits for the child process to finish executing. Otherwise, the child process would become a 'zombie' and the parent wouldn't know when it completes.

5. In the referenced demo code we used WEXITSTATUS(). What information does this provide, and why is it important?

    > **Answer**:  It retrieves the exit status of the child process. This is important to determine whether the command was successful or had an error somewhere.

6. Describe how your implementation of build_cmd_buff() handles quoted arguments. Why is this necessary?

    > **Answer**:  It treats the entire quoted string as a single argument, which preserves the contents of the quoted argument for printing/parsing.

7. What changes did you make to your parsing logic compared to the previous assignment? Were there any unexpected challenges in refactoring your old code?

    > **Answer**:  Instead of the simpler command list, the parsing logic was refactored to use a single cmd_buff structure. The main challenge was handling quoted arguments and some edge cases like empty inputs.

8. For this quesiton, you need to do some research on Linux signals. You can use [this google search](https://www.google.com/search?q=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&oq=Linux+signals+overview+site%3Aman7.org+OR+site%3Alinux.die.net+OR+site%3Atldp.org&gs_lcrp=EgZjaHJvbWUyBggAEEUYOdIBBzc2MGowajeoAgCwAgA&sourceid=chrome&ie=UTF-8) to get started.

- What is the purpose of signals in a Linux system, and how do they differ from other forms of interprocess communication (IPC)?

    > **Answer**: Signals are used for inter-process communication (IPC) to notify processes of events, like termination requests. Unlike other IPC methods, signals are asynchronous and lightweight, which makes them easier to use.

- Find and describe three commonly used signals (e.g., SIGKILL, SIGTERM, SIGINT). What are their typical use cases?

    > **Answer**:  SIGKILL forces a process to terminate immediately, and cannot be caught or ignored. SIGTERM *requests* a process to terminate, asking nicely, but it can be caught or ignored. SIGINT is sent when the user presses Ctrl+C, which is typically used to *interrupt* an ongoing process. It can be caught or ignored.

- What happens when a process receives SIGSTOP? Can it be caught or ignored like SIGINT? Why or why not?

    > **Answer**:  SIGSTOP pauses a process. It cannot be caught or ignored because it is used for process control, not communication.
