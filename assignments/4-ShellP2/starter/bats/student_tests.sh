#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Example: check ls runs without errors" {
    run ./dsh <<EOF                
ls
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

run_shell() {
    run ./dsh <<EOF
$1
exit
EOF
}

# Test built-in commands
@test "Test exit command" {
    run_shell "exit"
    [ "$status" -eq 0 ]
}

@test "Test cd command with no arguments" {
    run_shell "cd"
    [ "$status" -eq 0 ]
    [ "$output" = "dsh2> " ]  # Expect no output (cd does nothing)
}

@test "Test cd command with valid directory" {
    mkdir -p /tmp/test_dir
    run_shell "cd /tmp/test_dir"
    [ "$status" -eq 0 ]
    [ "$output" = "dsh2> " ]  # Expect no output (cd succeeds)
    rmdir /tmp/test_dir
}

@test "Test cd command with invalid directory" {
    run_shell "cd /nonexistent_dir"
    [ "$status" -eq 0 ]  # Shell should not crash
    [[ "$output" == *"cd: /nonexistent_dir: No such file or directory"* ]]
}

@test "Test cd command with too many arguments" {
    run_shell "cd dir1 dir2"
    [ "$status" -eq 0 ]  # Shell should not crash
    [[ "$output" == *"cd: too many arguments"* ]]
}

# Test external commands
@test "Test external command (ls)" {
    run_shell "ls"
    [ "$status" -eq 0 ]
    [[ "$output" == *"dsh2>"* ]]  # Ensure the shell prompt is present
}

@test "Test external command with arguments (echo)" {
    run_shell 'echo "hello, world"'
    [ "$status" -eq 0 ]
    [[ "$output" == *"hello, world"* ]]
}

@test "Test external command with quoted arguments" {
    run_shell 'echo "   hello,    world   "'
    [ "$status" -eq 0 ]
    [[ "$output" == *"   hello,    world   "* ]]
}

@test "Test external command with invalid command" {
    run_shell "nonexistent_command"
    [ "$status" -eq 0 ]  # Shell should not crash
    [[ "$output" == *"execvp: nonexistent_command: No such file or directory"* ]]
}

# Test edge cases
@test "Test empty input" {
    run_shell ""
    [ "$status" -eq 0 ]
    [ "$output" = "dsh2> " ]  # Expect no output (empty input)
}

@test "Test input with only spaces" {
    run_shell "     "
    [ "$status" -eq 0 ]
    [ "$output" = "dsh2> " ]  # Expect no output (input with only spaces)
}

@test "Test input with leading/trailing spaces" {
    run_shell "   ls   "
    [ "$status" -eq 0 ]
    [[ "$output" == *"dsh2>"* ]]  # Ensure the shell prompt is present
}

@test "Test input with multiple spaces between arguments" {
    run_shell "echo    hello    world"
    [ "$status" -eq 0 ]
    [[ "$output" == *"hello world"* ]]
}

@test "Test input with quoted string and spaces" {
    run_shell 'echo "   hello,    world   "'
    [ "$status" -eq 0 ]
    [[ "$output" == *"   hello,    world   "* ]]  # Preserve spaces inside quotes
}

# Test error handling
@test "Test command with too many arguments" {
    run_shell "echo arg1 arg2 arg3 arg4 arg5 arg6 arg7 arg8 arg9 arg10"
    [ "$status" -eq 0 ]
    [[ "$output" == *"error: too many arguments"* ]]
}

@test "Test command with invalid arguments" {
    run_shell "cd /invalid/path"
    [ "$status" -eq 0 ]
    [[ "$output" == *"cd: /invalid/path: No such file or directory"* ]]
}