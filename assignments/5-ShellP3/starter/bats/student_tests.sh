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

@test "Test built-in exit command" {
    run ./dsh <<EOF
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
}

@test "Test built-in cd command" {
    run ./dsh <<EOF
cd /tmp
pwd
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"/tmp"* ]]
}

@test "Test invalid command" {
    run ./dsh <<EOF
invalid_command
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"execvp: No such file or directory"* ]]
}

@test "Test single pipe" {
    run ./dsh <<EOF
ls | grep dshlib.c
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"dshlib.c"* ]]
}

@test "Test multiple pipes" {
    run ./dsh <<EOF
ls | grep dshlib.c | wc -l
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"1"* ]]
}

@test "Test empty input" {
    run ./dsh <<EOF

exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"warning: no commands provided"* ]]
}

@test "Test too many pipes" {
    run ./dsh <<EOF
ls | grep dshlib.c | cat | wc -l | sort | uniq | head -n 1 | wc -c | tr -d ' ' | xargs echo
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"error: piping limited to 8 commands"* ]]
}

@test "Test command with arguments" {
    run ./dsh <<EOF
echo "Hello, World!"
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello, World!"* ]]
}

@test "Test command with quotes" {
    run ./dsh <<EOF
echo "This is a 'test'"
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"This is a 'test'"* ]]
}

@test "Test command with redirection (optional)" {
    run ./dsh <<EOF
echo "Hello" > testfile.txt
cat testfile.txt
rm testfile.txt
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello"* ]]
}

@test "Test command with invalid arguments" {
    run ./dsh <<EOF
ls --invalid-option
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"ls: unrecognized option '--invalid-option'"* ]]
}

@test "Test command with excessive arguments" {
    run ./dsh <<EOF
$(printf 'arg%.0s ' {1..1000})
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"error: too many arguments"* ]]
}

@test "Test command with excessive pipes" {
    run ./dsh <<EOF
$(printf 'ls | %.0s' {1..100}) grep dshlib.c
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"error: piping limited to 8 commands"* ]]
}

@test "Test command with no output" {
    run ./dsh <<EOF
true
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" != *"error"* ]]
}

@test "Test command with large output" {
    run ./dsh <<EOF
cat /dev/urandom | head -c 1000000 | wc -c
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"1000000"* ]]
}