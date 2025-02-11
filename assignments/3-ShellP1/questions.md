1. In this assignment I suggested you use `fgets()` to get user input in the main while loop. Why is `fgets()` a good choice for this application?

    > **Answer**:  fgets() is ideal because it reads input line by line, which is
                   good for a shell program, since it processes commands one at a time.
                   Since it reads until a newline or EOF, this will ensure it gets
                   the full command that the user wished to enter.

2. You needed to use `malloc()` to allocte memory for `cmd_buff` in `dsh_cli.c`. Can you explain why you needed to do that, instead of allocating a fixed-size array?

    > **Answer**:  While we could use a fixed-size array since we are given the
                   max length of a command, it is more efficient to dynamically
                   allocate the memory based on the length of the command.


3. In `dshlib.c`, the function `build_cmd_list(`)` must trim leading and trailing spaces from each command before storing it. Why is this necessary? If we didn't trim spaces, what kind of issues might arise when executing commands in our shell?

    > **Answer**:  There are plenty of issues that could arise from not trimming
                   leading and trailing spaces. The main thing is that it will
                   probably run into an error parsing the commands, for example
                   "  exit " might not be interpreted as "exit".

4. For this question you need to do some research on STDIN, STDOUT, and STDERR in Linux. We've learned this week that shells are "robust brokers of input and output". Google _"linux shell stdin stdout stderr explained"_ to get started.

- One topic you should have found information on is "redirection". Please provide at least 3 redirection examples that we should implement in our custom shell, and explain what challenges we might have implementing them.

    > **Answer**: Output redirection (>)
                  Example: ls > output.txt
                  This is a useful redirection to implement as it allows the user to
                  redirect the output to a file. The challenges with this would be
                  making sure the program can handle file creation and writing accurately.

                  Input redirection (<)
                  Example: sort < input.txt
                  This is a useful redirection because it allows the user to
                  directly pass in a file as input, e.g. < input.txt. Similarly to
                  output redirection, we would need to implement file handling
                  functionality, specifically reading and parsing a file.

                  Appending output (>>)
                  Example: echo "new line" >> output.txt
                  This is a useful redirection because it allows the user to append
                  new information to the end of an existing output file. This can
                  be useful for running multiple tests and storing certain information.
                  The challenge with this would be similar to output redirection,
                  but with appending lines to the end of the file instead.

- You should have also learned about "pipes". Redirection and piping both involve controlling input and output in the shell, but they serve different purposes. Explain the key differences between redirection and piping.

    > **Answer**:  Redirection directs input/output to/from a file, whereas piping
                   is for sending the return from one command to another command.
                   For example, cmd1 | cmd2 will send the output of cmd1 as input
                   for cmd2, but redirection looks more like sort < input.txt.

- STDERR is often used for error messages, while STDOUT is for regular output. Why is it important to keep these separate in a shell?

    > **Answer**:  STDERR is only used for errors, so it's useful as a uniform
                   way to display errors instead of just output, which might make
                   an error less clear. Errors can also be redirected independently
                   of regular output, giving the programmer more control over what
                   happens when there's an error.

- How should our custom shell handle errors from commands that fail? Consider cases where a command outputs both STDOUT and STDERR. Should we provide a way to merge them, and if so, how?

    > **Answer**:  The shell should capture them separately by default, but could
                   be redirected if we supported a syntax like cmd 2>&1, which would redirect STDERR into STDOUT, merging them into one.