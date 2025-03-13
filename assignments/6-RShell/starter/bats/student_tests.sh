#!/usr/bin/env bats

# File: student_tests.sh
# 
# Create your unit tests suit in this file

@test "Test built-in 'exit' command" {
    run ./dsh <<EOF
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"local mode"* ]]
}

@test "Test built-in 'cd' command" {
    run ./dsh <<EOF
cd /tmp
pwd
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"/tmp"* ]]
}

@test "Test pipeline with 'ls | grep dsh'" {
    run ./dsh <<EOF
ls | grep dsh
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"dsh"* ]]
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
echo "Hello, World!"
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello, World!"* ]]
}

@test "Test command with single quotes" {
    run ./dsh <<EOF
echo 'Hello, World!'
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello, World!"* ]]
}

@test "Test command with semicolon" {
    run ./dsh <<EOF
echo Hello; echo World
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello"* ]]
    [[ "$output" == *"World"* ]]
}

@test "Test command with &&" {
    run ./dsh <<EOF
echo Hello && echo World
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello"* ]]
    [[ "$output" == *"World"* ]]
}

@test "Test command with ||" {
    run ./dsh <<EOF
false || echo Hello
exit
EOF

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"Hello"* ]]
    [[ "$output" != *"World"* ]]
}

@test "Test input redirection" {
    echo "Hello, World!" > testfile.txt
    run ./dsh <<EOF
wc -l < testfile.txt
exit
EOF

    # Clean up
    rm testfile.txt

    # Assertions
    [ "$status" -eq 0 ]
    [[ "$output" == *"1"* ]]
}

@test "Test output redirection" {
    run ./dsh <<EOF
echo "Hello, World!" > output.txt
exit
EOF

    # Check the content of the file
    run cat output.txt

    # Clean up
    rm output.txt

    # Assertions
    [ "$status" -eq 0 ]
    [ "$output" = "Hello, World!" ]
}

@test "Test append output redirection" {
    echo "Line 1" > output.txt
    run ./dsh <<EOF
echo "Line 2" >> output.txt
exit
EOF

    # Check the content of the file
    run cat output.txt

    # Clean up
    rm output.txt

    # Assertions
    [ "$status" -eq 0 ]
    [ "$output" = $"Line 1\nLine 2" ]
}