#include "shell.h"
#include "../libc/libc.h"
#include "../kernel/types.h"
#include "../kernel/syscall.h"

static char current_dir[256] = "/";
static int shell_running = 1;
static char command_history[100][256];
static int history_index = 0;

void shell_init(void) {
    strcpy(current_dir, "/");
    shell_running = 1;
    history_index = 0;
}

void shell_run(void) {
    printf("blockOS 2.0 - Linux Compatible Shell\n");
    printf("Type 'help' for command list\n\n");
    
    while (shell_running) {
        shell_prompt();
        char* line = shell_read_line();
        
        if (!line || strlen(line) == 0) {
            continue;
        }
        
        /* Save to history */
        if (history_index < 100) {
            strcpy(command_history[history_index++], line);
        }
        
        shell_command_t* cmd = shell_parse_command(line);
        if (cmd) {
            shell_execute_command(cmd);
            shell_free_command(cmd);
        }
        
        free(line);
    }
}

void shell_prompt(void) {
    printf("blockos:%s$ ", current_dir);
}

char* shell_read_line(void) {
    char* line = (char*)malloc(1024);
    int i = 0;
    int c;
    
    while (i < 1023) {
        c = getchar();
        
        if (c == '\n' || c == '\r') {
            line[i] = 0;
            printf("\n");
            return line;
        } else if (c == 8 || c == 127) {
            if (i > 0) {
                i--;
                printf("\b \b");
            }
        } else if (c >= 32 && c < 127) {
            line[i++] = c;
            putchar(c);
        }
    }
    
    line[i] = 0;
    return line;
}

shell_command_t* shell_parse_command(const char* line) {
    if (!line || strlen(line) == 0) return NULL;
    
    shell_command_t* cmd = (shell_command_t*)malloc(sizeof(shell_command_t));
    cmd->argc = 0;
    cmd->in_fd = 0;
    cmd->out_fd = 1;
    
    char buffer[1024];
    strcpy(buffer, line);
    
    char* token = (char*)buffer;
    
    while (*token && cmd->argc < 256) {
        while (*token == ' ' || *token == '\t') token++;
        
        if (*token == 0) break;
        
        char* start = token;
        while (*token && *token != ' ' && *token != '\t') token++;
        
        int len = token - start;
        cmd->argv[cmd->argc] = (char*)malloc(len + 1);
        strncpy(cmd->argv[cmd->argc], start, len);
        cmd->argv[cmd->argc][len] = 0;
        cmd->argc++;
    }
    
    if (cmd->argc == 0) {
        free(cmd);
        return NULL;
    }
    
    cmd->name = cmd->argv[0];
    return cmd;
}

int shell_execute_command(shell_command_t* cmd) {
    if (!cmd || cmd->argc == 0) return -1;
    
    if (strcmp(cmd->name, "echo") == 0) {
        return shell_builtin_echo(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "exit") == 0) {
        return shell_builtin_exit(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "cd") == 0) {
        return shell_builtin_cd(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "pwd") == 0) {
        return shell_builtin_pwd(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "ls") == 0) {
        return shell_builtin_ls(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "mkdir") == 0) {
        return shell_builtin_mkdir(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "rmdir") == 0) {
        return shell_builtin_rmdir(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "rm") == 0) {
        return shell_builtin_rm(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "cp") == 0) {
        return shell_builtin_cp(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "mv") == 0) {
        return shell_builtin_mv(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "cat") == 0) {
        return shell_builtin_cat(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "grep") == 0) {
        return shell_builtin_grep(cmd->argc, cmd->argv);
    } else if (strcmp(cmd->name, "help") == 0) {
        return shell_builtin_help(cmd->argc, cmd->argv);
    }
    
    printf("blockos: command not found: %s\n", cmd->name);
    return -1;
}

void shell_free_command(shell_command_t* cmd) {
    if (!cmd) return;
    int i;
    for (i = 0; i < cmd->argc; i++) {
        free(cmd->argv[i]);
    }
    free(cmd);
}

int shell_builtin_echo(int argc, char* argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
        printf("%s", argv[i]);
        if (i < argc - 1) printf(" ");
    }
    printf("\n");
    return 0;
}

int shell_builtin_exit(int argc, char* argv[]) {
    printf("blockos: logout\n");
    shell_running = 0;
    return 0;
}

int shell_builtin_cd(int argc, char* argv[]) {
    if (argc < 2) {
        strcpy(current_dir, "/");
    } else {
        if (argv[1][0] == '/') {
            strcpy(current_dir, argv[1]);
        } else {
            strcat(current_dir, "/");
            strcat(current_dir, argv[1]);
        }
    }
    return 0;
}

int shell_builtin_pwd(int argc, char* argv[]) {
    printf("%s\n", current_dir);
    return 0;
}

int shell_builtin_ls(int argc, char* argv[]) {
    printf("blockos: ls %s\n", argc > 1 ? argv[1] : current_dir);
    return 0;
}

int shell_builtin_mkdir(int argc, char* argv[]) {
    if (argc < 2) {
        printf("mkdir: missing operand\n");
        return -1;
    }
    printf("blockos: mkdir %s\n", argv[1]);
    return syscall_mkdir(argv[1], 0755);
}

int shell_builtin_rmdir(int argc, char* argv[]) {
    if (argc < 2) {
        printf("rmdir: missing operand\n");
        return -1;
    }
    printf("blockos: rmdir %s\n", argv[1]);
    return syscall_rmdir(argv[1]);
}

int shell_builtin_rm(int argc, char* argv[]) {
    if (argc < 2) {
        printf("rm: missing operand\n");
        return -1;
    }
    printf("blockos: rm %s\n", argv[1]);
    return syscall_unlink(argv[1]);
}

int shell_builtin_cp(int argc, char* argv[]) {
    if (argc < 3) {
        printf("cp: missing operand\n");
        return -1;
    }
    printf("blockos: cp %s to %s\n", argv[1], argv[2]);
    return 0;
}

int shell_builtin_mv(int argc, char* argv[]) {
    if (argc < 3) {
        printf("mv: missing operand\n");
        return -1;
    }
    printf("blockos: mv %s to %s\n", argv[1], argv[2]);
    return 0;
}

int shell_builtin_cat(int argc, char* argv[]) {
    if (argc < 2) {
        printf("cat: missing operand\n");
        return -1;
    }
    printf("blockos: cat %s\n", argv[1]);
    return 0;
}

int shell_builtin_grep(int argc, char* argv[]) {
    if (argc < 3) {
        printf("grep: missing operand\n");
        return -1;
    }
    printf("blockos: grep %s in %s\n", argv[1], argv[2]);
    return 0;
}

int shell_builtin_help(int argc, char* argv[]) {
    printf("\n=== blockOS 2.0 - Linux Compatible Shell ===\n\n");
    printf("Built-in commands:\n");
    printf("  echo   <text>          - Display text\n");
    printf("  cd     [directory]     - Change directory\n");
    printf("  pwd                    - Print working directory\n");
    printf("  ls     [path]          - List files\n");
    printf("  mkdir  <directory>     - Create directory\n");
    printf("  rmdir  <directory>     - Remove directory\n");
    printf("  rm     <file>          - Remove file\n");
    printf("  cp     <src> <dst>     - Copy file\n");
    printf("  mv     <src> <dst>     - Move file\n");
    printf("  cat    <file>          - Display file contents\n");
    printf("  grep   <text> <file>   - Search in file\n");
    printf("  help                   - Show this help\n");
    printf("  exit                   - Exit shell\n\n");
    printf("Examples:\n");
    printf("  echo Hello World\n");
    printf("  cd /home\n");
    printf("  mkdir mydir\n");
    printf("  cat file.txt\n\n");
    return 0;
}
