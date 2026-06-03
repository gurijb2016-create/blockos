#include "linux.h"
#include "process.h"
#include "syscall.h"

/* Syscall dispatcher */
uint32_t syscall_handler(uint32_t syscall_num, uint32_t arg1, uint32_t arg2, uint32_t arg3, uint32_t arg4, uint32_t arg5) {
    process_t* current = process_get_current();
    
    switch (syscall_num) {
        case __NR_exit:
            syscall_exit((int)arg1);
            return 0;
            
        case __NR_fork:
            return syscall_fork();
            
        case __NR_read:
            return syscall_read((int)arg1, (void*)arg2, arg3);
            
        case __NR_write:
            return syscall_write((int)arg1, (const void*)arg2, arg3);
            
        case __NR_open:
            return syscall_open((const char*)arg1, (int)arg2, (int)arg3);
            
        case __NR_close:
            return syscall_close((int)arg1);
            
        case __NR_getpid:
            return syscall_getpid();
            
        case __NR_getuid:
            return syscall_getuid();
            
        case __NR_getgid:
            return syscall_getgid();
            
        case __NR_setuid:
            return syscall_setuid(arg1);
            
        case __NR_setgid:
            return syscall_setgid(arg1);
            
        case __NR_brk:
            return (uint32_t)syscall_brk((void*)arg1);
            
        case __NR_mmap:
            return (uint32_t)syscall_mmap((void*)arg1, arg2, (int)arg3, (int)arg4, (int)arg5, 0);
            
        case __NR_munmap:
            return syscall_munmap((void*)arg1, arg2);
            
        case __NR_execve:
            return syscall_execve((const char*)arg1, (const char* const*)arg2, (const char* const*)arg3);
            
        case __NR_waitpid:
            return syscall_waitpid(arg1, (int*)arg2, (int)arg3);
            
        case __NR_chdir:
            return syscall_chdir((const char*)arg1);
            
        case __NR_mkdir:
            return syscall_mkdir((const char*)arg1, (int)arg2);
            
        case __NR_rmdir:
            return syscall_rmdir((const char*)arg1);
            
        case __NR_unlink:
            return syscall_unlink((const char*)arg1);
            
        case __NR_chmod:
            return syscall_chmod((const char*)arg1, (int)arg2);
            
        case __NR_access:
            return syscall_access((const char*)arg1, (int)arg2);
            
        case __NR_uname:
            return syscall_uname((void*)arg1);
            
        case __NR_signal:
            return syscall_signal((int)arg1, (void*)arg2);
            
        case __NR_kill:
            return syscall_kill(arg1, (int)arg2);
            
        case __NR_pause:
            return syscall_pause();
            
        default:
            return -1;
    }
}

/* exit() - terminate process */
void syscall_exit(int code) {
    process_exit(code);
}

/* fork() - create child process */
int syscall_fork(void) {
    return process_fork();
}

/* read() - read from file descriptor */
int syscall_read(int fd, void* buffer, uint32_t count) {
    if (fd < 0 || fd >= 256) return -1;
    process_t* current = process_get_current();
    if (!current || !current->fd_table[fd]) return -1;
    
    /* TODO: Implement file reading */
    return 0;
}

/* write() - write to file descriptor */
int syscall_write(int fd, const void* buffer, uint32_t count) {
    if (fd < 0 || fd >= 256) return -1;
    process_t* current = process_get_current();
    if (!current || !current->fd_table[fd]) return -1;
    
    /* TODO: Implement file writing */
    return count;
}

/* open() - open file */
int syscall_open(const char* pathname, int flags, int mode) {
    process_t* current = process_get_current();
    if (!current) return -1;
    
    /* Find free file descriptor */
    int fd;
    for (fd = 0; fd < 256; fd++) {
        if (current->fd_table[fd] == NULL) {
            /* TODO: Open file and store in fd_table */
            return fd;
        }
    }
    return -1;
}

/* close() - close file descriptor */
int syscall_close(int fd) {
    if (fd < 0 || fd >= 256) return -1;
    process_t* current = process_get_current();
    if (!current || !current->fd_table[fd]) return -1;
    
    current->fd_table[fd] = NULL;
    return 0;
}

/* getpid() - get process ID */
uint32_t syscall_getpid(void) {
    process_t* current = process_get_current();
    return current ? current->pid : 0;
}

/* getuid() - get user ID */
uint32_t syscall_getuid(void) {
    process_t* current = process_get_current();
    return current ? current->uid : 0;
}

/* getgid() - get group ID */
uint32_t syscall_getgid(void) {
    process_t* current = process_get_current();
    return current ? current->gid : 0;
}

/* geteuid() - get effective user ID */
uint32_t syscall_geteuid(void) {
    process_t* current = process_get_current();
    return current ? current->euid : 0;
}

/* getegid() - get effective group ID */
uint32_t syscall_getegid(void) {
    process_t* current = process_get_current();
    return current ? current->egid : 0;
}

/* setuid() - set user ID */
int syscall_setuid(uint32_t uid) {
    process_t* current = process_get_current();
    if (!current) return -1;
    current->uid = uid;
    return 0;
}

/* setgid() - set group ID */
int syscall_setgid(uint32_t gid) {
    process_t* current = process_get_current();
    if (!current) return -1;
    current->gid = gid;
    return 0;
}

/* brk() - change data segment size */
void* syscall_brk(void* addr) {
    process_t* current = process_get_current();
    if (!current) return NULL;
    
    if (addr) {
        current->heap_end = (uint32_t)addr;
    }
    return (void*)current->heap_end;
}

/* mmap() - map memory */
void* syscall_mmap(void* addr, uint32_t length, int prot, int flags, int fd, uint32_t offset) {
    /* TODO: Implement memory mapping */
    return NULL;
}

/* munmap() - unmap memory */
int syscall_munmap(void* addr, uint32_t length) {
    /* TODO: Implement memory unmapping */
    return 0;
}

/* execve() - execute program */
int syscall_execve(const char* filename, const char* const* argv, const char* const* envp) {
    /* TODO: Implement program execution */
    return -1;
}

/* waitpid() - wait for process */
int syscall_waitpid(uint32_t pid, int* status, int options) {
    process_wait(pid);
    process_t* proc = process_get_by_pid(pid);
    if (proc && status) {
        *status = proc->exit_code;
    }
    return pid;
}

/* chdir() - change directory */
int syscall_chdir(const char* path) {
    process_t* current = process_get_current();
    if (!current) return -1;
    current->working_dir = (char*)path;
    return 0;
}

/* mkdir() - create directory */
int syscall_mkdir(const char* path, int mode) {
    /* TODO: Implement directory creation */
    return 0;
}

/* rmdir() - remove directory */
int syscall_rmdir(const char* path) {
    /* TODO: Implement directory removal */
    return 0;
}

/* unlink() - remove file */
int syscall_unlink(const char* path) {
    /* TODO: Implement file removal */
    return 0;
}

/* chmod() - change file permissions */
int syscall_chmod(const char* path, int mode) {
    /* TODO: Implement file permission change */
    return 0;
}

/* access() - check file access */
int syscall_access(const char* path, int mode) {
    /* TODO: Implement access check */
    return 0;
}

/* uname() - get system information */
int syscall_uname(void* buf) {
    /* TODO: Implement uname */
    return 0;
}

/* signal() - set signal handler */
int syscall_signal(int signum, void* handler) {
    /* TODO: Implement signal handler */
    return 0;
}

/* kill() - send signal to process */
int syscall_kill(uint32_t pid, int sig) {
    process_t* proc = process_get_by_pid(pid);
    if (!proc) return -1;
    proc->signal_pending |= (1 << sig);
    return 0;
}

/* pause() - wait for signal */
int syscall_pause(void) {
    /* TODO: Implement pause */
    return -1;
}
