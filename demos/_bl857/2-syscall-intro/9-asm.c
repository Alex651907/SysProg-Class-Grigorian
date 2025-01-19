#include <unistd.h>

int main()
{
    const char *message = "Hello, world!\n";

    asm volatile(
        "mov $1, %%rax\n"  // Syscall number for write
        "mov $1, %%rdi\n"  // File descriptor (stdout)
        "mov %0, %%rsi\n"  // Pointer to message
        "mov $14, %%rdx\n" // Length of message
        "syscall\n"        // Make the syscall
        :
        : "r"(message)                 // Input constraint: %0 refers to 'message'
        : "rax", "rdi", "rsi", "rdx"); // Clobbered registers

    return 0;
}
