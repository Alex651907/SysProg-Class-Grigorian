// CONCEPT: how many syscalls does this simple app make?
// make && ./0-whatis
// echo $?
// strace ./0-whatis > 0-whatis-trace.txt 2>&1
int main()
{
    return 101;
}

/*
strace ./0-whatis
execve("./0-whatis", ["./0-whatis"], 0x7ffd64f1a560 ) = 0
brk(NULL)                               = 0x564490583000
mmap(NULL, 8192, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f550eb9b000
access("/etc/ld.so.preload", R_OK)      = -1 ENOENT (No such file or directory)
openat(AT_FDCWD, "/etc/ld.so.cache", O_RDONLY|O_CLOEXEC) = 3
fstat(3, {st_mode=S_IFREG|0644, st_size=15101, ...}) = 0
mmap(NULL, 15101, PROT_READ, MAP_PRIVATE, 3, 0) = 0x7f550eb97000
close(3)                                = 0
openat(AT_FDCWD, "/lib/x86_64-linux-gnu/libc.so.6", O_RDONLY|O_CLOEXEC) = 3
read(3, "\177ELF\2\1\1\3\0\0\0\0\0\0\0\0\3\0>\0\1\0\0\0\200\245\2\0\0\0\0\0"..., 832) = 832
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
fstat(3, {st_mode=S_IFREG|0755, st_size=2182752, ...}) = 0
pread64(3, "\6\0\0\0\4\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0@\0\0\0\0\0\0\0"..., 784, 64) = 784
mmap(NULL, 2227672, PROT_READ, MAP_PRIVATE|MAP_DENYWRITE, 3, 0) = 0x7f550e800000
mmap(0x7f550e828000, 1662976, PROT_READ|PROT_EXEC, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x28000) = 0x7f550e828000
mmap(0x7f550e9be000, 323584, PROT_READ, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x1be000) = 0x7f550e9be000
mmap(0x7f550ea0d000, 24576, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_DENYWRITE, 3, 0x20c000) = 0x7f550ea0d000
mmap(0x7f550ea13000, 52696, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_FIXED|MAP_ANONYMOUS, -1, 0) = 0x7f550ea13000
close(3)                                = 0
mmap(NULL, 12288, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0) = 0x7f550eb94000
arch_prctl(ARCH_SET_FS, 0x7f550eb94740) = 0
set_tid_address(0x7f550eb94a10)         = 39687
set_robust_list(0x7f550eb94a20, 24)     = 0
rseq(0x7f550eb95060, 0x20, 0, 0x53053053) = 0
mprotect(0x7f550ea0d000, 16384, PROT_READ) = 0
mprotect(0x5644693c0000, 4096, PROT_READ) = 0
mprotect(0x7f550ebdb000, 8192, PROT_READ) = 0
prlimit64(0, RLIMIT_STACK, NULL, {rlim_cur=8192*1024, rlim_max=RLIM64_INFINITY}) = 0
munmap(0x7f550eb97000, 15101)           = 0
exit_group(101)                         = ?
+++ exited with 101 +++
*/