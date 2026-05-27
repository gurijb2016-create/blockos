#ifndef PROCESS_H
#define PROCESS_H

#include "types.h"

/* Process states */
#define PROCESS_STATE_NEW       0
#define PROCESS_STATE_READY     1
#define PROCESS_STATE_RUNNING   2
#define PROCESS_STATE_BLOCKED   3
#define PROCESS_STATE_STOPPED   4
#define PROCESS_STATE_ZOMBIE    5
#define PROCESS_STATE_DEAD      6

/* Process flags */
#define PF_TRACED    0x0001    /* Process is being traced */
#define PF_PTRACED   0x0002    /* Process is tracing a child */
#define PF_EXITING   0x0004    /* Process is exiting */

/* Process structure */
typedef struct process_struct {
    uint32_t pid;              /* Process ID */
    uint32_t ppid;             /* Parent process ID */
    uint32_t uid;              /* User ID */
    uint32_t gid;              /* Group ID */
    uint32_t euid;             /* Effective UID */
    uint32_t egid;             /* Effective GID */
    
    int state;                 /* Process state */
    int priority;              /* Priority level */
    uint32_t flags;            /* Process flags */
    
    uint32_t exit_code;        /* Exit status */
    
    /* Memory management */
    uint32_t code_start;       /* Code segment start */
    uint32_t code_end;         /* Code segment end */
    uint32_t data_start;       /* Data segment start */
    uint32_t data_end;         /* Data segment end */
    uint32_t heap_start;       /* Heap start */
    uint32_t heap_end;         /* Heap end */
    uint32_t stack_top;        /* Stack top */
    uint32_t stack_bottom;     /* Stack bottom */
    
    uint32_t page_dir;         /* Page directory physical address */
    
    /* CPU context */
    uint32_t eax, ebx, ecx, edx;
    uint32_t esi, edi, ebp, esp;
    uint32_t eip, eflags;
    uint32_t cs, ds, es, fs, gs, ss;
    
    /* File descriptors */
    void* fd_table[256];       /* File descriptor table */
    
    /* Scheduling */
    uint32_t time_slice;       /* Time slice for scheduling */
    uint32_t jiffies_used;     /* Jiffies used by process */
    
    /* Child processes */
    struct process_struct* parent;
    struct process_struct* child;
    struct process_struct* sibling;
    
    /* Signals */
    uint32_t signal_pending;   /* Pending signals */
    uint32_t signal_mask;      /* Signal mask */
    
    char* working_dir;         /* Current working directory */
    char name[32];             /* Process name */
    
} process_t;

/* Process management functions */
process_t* process_create(const char* name);
void process_destroy(process_t* proc);
process_t* process_get_current(void);
void process_set_current(process_t* proc);
void process_switch(process_t* new_proc);

/* Process lifecycle */
void process_set_state(process_t* proc, int state);
int process_fork(void);
void process_exit(int code);
void process_wait(uint32_t pid);

/* Process list management */
void process_add_to_ready(process_t* proc);
void process_remove_from_ready(process_t* proc);
process_t* process_get_by_pid(uint32_t pid);

#endif
