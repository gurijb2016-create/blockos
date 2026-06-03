#include "linux.h"
#include "process.h"
#include "syscall.h"

/* Global process table */
static process_t process_table[256];
static uint32_t process_count = 0;
static uint32_t next_pid = 1;
static process_t* current_process = NULL;

/* Process list management */
static process_t* process_ready_list = NULL;
static process_t* process_blocked_list = NULL;
static process_t* process_zombie_list = NULL;

/* Initialize process management */
void process_init(void) {
    process_count = 0;
    next_pid = 1;
    current_process = NULL;
    
    /* Clear process table */
    int i;
    for (i = 0; i < 256; i++) {
        process_table[i].pid = 0;
        process_table[i].state = PROCESS_STATE_DEAD;
    }
}

/* Create new process */
process_t* process_create(const char* name) {
    if (process_count >= 256) return NULL;
    
    process_t* proc = &process_table[process_count++];
    proc->pid = next_pid++;
    proc->ppid = current_process ? current_process->pid : 0;
    proc->uid = current_process ? current_process->uid : 0;
    proc->gid = current_process ? current_process->gid : 0;
    proc->state = PROCESS_STATE_NEW;
    proc->priority = 0;
    proc->flags = 0;
    proc->exit_code = 0;
    
    /* Memory layout */
    proc->code_start = 0x08048000;
    proc->code_end = 0x08048000;
    proc->data_start = 0x08048000;
    proc->data_end = 0x08048000;
    proc->heap_start = 0x08048000;
    proc->heap_end = 0x08048000;
    proc->stack_bottom = 0xBFFFFFFF;
    proc->stack_top = 0xBFFFFFFF;
    
    /* CPU context */
    proc->eip = 0;
    proc->esp = proc->stack_top;
    proc->eflags = 0x202;
    proc->cs = 0x08;
    proc->ds = 0x10;
    proc->es = 0x10;
    proc->fs = 0x10;
    proc->gs = 0x10;
    proc->ss = 0x10;
    
    /* File descriptors */
    int j;
    for (j = 0; j < 256; j++) {
        proc->fd_table[j] = NULL;
    }
    
    /* Scheduling */
    proc->time_slice = 20;
    proc->jiffies_used = 0;
    
    /* Relationships */
    proc->parent = current_process;
    proc->child = NULL;
    proc->sibling = NULL;
    
    /* Signals */
    proc->signal_pending = 0;
    proc->signal_mask = 0;
    
    /* Working directory */
    proc->working_dir = "/";
    
    /* Process name */
    int k = 0;
    while (name[k] && k < 31) {
        proc->name[k] = name[k];
        k++;
    }
    proc->name[k] = 0;
    
    return proc;
}

/* Get current process */
process_t* process_get_current(void) {
    return current_process;
}

/* Set current process */
void process_set_current(process_t* proc) {
    current_process = proc;
}

/* Set process state */
void process_set_state(process_t* proc, int state) {
    if (!proc) return;
    proc->state = state;
}

/* Get process by PID */
process_t* process_get_by_pid(uint32_t pid) {
    int i;
    for (i = 0; i < process_count; i++) {
        if (process_table[i].pid == pid) {
            return &process_table[i];
        }
    }
    return NULL;
}

/* Add process to ready list */
void process_add_to_ready(process_t* proc) {
    if (!proc) return;
    
    proc->state = PROCESS_STATE_READY;
    
    if (process_ready_list == NULL) {
        process_ready_list = proc;
        proc->sibling = NULL;
    } else {
        process_t* current = process_ready_list;
        while (current->sibling != NULL) {
            current = current->sibling;
        }
        current->sibling = proc;
        proc->sibling = NULL;
    }
}

/* Remove process from ready list */
void process_remove_from_ready(process_t* proc) {
    if (!proc || process_ready_list == NULL) return;
    
    if (process_ready_list == proc) {
        process_ready_list = proc->sibling;
    } else {
        process_t* current = process_ready_list;
        while (current && current->sibling != proc) {
            current = current->sibling;
        }
        if (current) {
            current->sibling = proc->sibling;
        }
    }
}

/* Fork process */
int process_fork(void) {
    if (!current_process) return -1;
    
    process_t* child = process_create(current_process->name);
    if (!child) return -1;
    
    /* Copy parent's memory and context */
    child->uid = current_process->uid;
    child->gid = current_process->gid;
    child->euid = current_process->euid;
    child->egid = current_process->egid;
    
    child->code_start = current_process->code_start;
    child->code_end = current_process->code_end;
    child->data_start = current_process->data_start;
    child->data_end = current_process->data_end;
    child->heap_start = current_process->heap_start;
    child->heap_end = current_process->heap_end;
    
    /* Copy CPU context */
    child->eax = current_process->eax;
    child->ebx = current_process->ebx;
    child->ecx = current_process->ecx;
    child->edx = current_process->edx;
    child->esi = current_process->esi;
    child->edi = current_process->edi;
    child->esp = current_process->esp;
    child->ebp = current_process->ebp;
    child->eip = current_process->eip;
    child->eflags = current_process->eflags;
    
    child->parent = current_process;
    
    process_add_to_ready(child);
    
    return child->pid;
}

/* Exit process */
void process_exit(int code) {
    if (!current_process) return;
    
    current_process->exit_code = code;
    current_process->state = PROCESS_STATE_ZOMBIE;
}

/* Wait for process */
void process_wait(uint32_t pid) {
    process_t* proc = process_get_by_pid(pid);
    if (!proc) return;
    
    while (proc->state != PROCESS_STATE_ZOMBIE && 
           proc->state != PROCESS_STATE_DEAD) {
        /* Wait */
    }
}

/* Destroy process */
void process_destroy(process_t* proc) {
    if (!proc) return;
    proc->state = PROCESS_STATE_DEAD;
}
