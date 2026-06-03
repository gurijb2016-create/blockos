#include "shell.h"
#include "../libc/libc.h"
#include "../kernel/types.h"

static char current_dir[256] = "/";
static int shell_running = 1;

void shell_init(void) {
    strcpy(current_dir, "/");
    shell_running = 1;
}

void shell_run(void) {
    while (shell_running) {
        shell_prompt();
        char* line = shell_read_line();
        
        if (!line) continue;
        
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
    int i = 0;
    
    while (*token && cmd->argc < 256) {
        while (*token == ' ') token++;
        
        if (*token == 0) break;
        
        char* start = token;
        while (*token && *token != ' ') token++;
        
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
    shell_running = 0;
    return 0;
}

int shell_builtin_cd(int argc, char* argv[]) {
    if (argc < 2) {
        strcpy(current_dir, "/");
    } else {
        strcpy(current_dir, argv[1]);
    }
    return 0;
}

int shell_builtin_pwd(int argc, char* argv[]) {
    printf("%s\n", current_dir);
    return 0;
}

int shell_builtin_ls(int argc, char* argv[]) {
    printf("blockos:ls - listing directory\n");
    return 0;
}

int shell_builtin_mkdir(int argc, char* argv[]) {
    if (argc < 2) {
        printf("mkdir: missing directory name\n");
        return -1;
    }
    mkdir(argv[1], 0755);
    return 0;
}

int shell_builtin_rmdir(int argc, char* argv[]) {
    if (argc < 2) {
        printf("rmdir: missing directory name\n");
        return -1;
    }
    rmdir(argv[1]);
    return 0;
}

int shell_builtin_rm(int argc, char* argv[]) {
    if (argc < 2) {
        printf("rm: missing file name\n");
        return -1;
    }
    unlink(argv[1]);
    return 0;
}

int shell_builtin_cp(int argc, char* argv[]) {
    if (argc < 3) {
        printf("cp: missing arguments\n");
        return -1;
    }
    printf("blockos:cp %s to %s\n", argv[1], argv[2]);
    return 0;
}

int shell_builtin_mv(int argc, char* argv[]) {
    if (argc < 3) {
        printf("mv: missing arguments\n");
        return -1;
    }
    printf("blockos:mv %s to %s\n", argv[1], argv[2]);
    return 0;
}

int shell_builtin_cat(int argc, char* argv[]) {
    if (argc < 2) {
        printf("cat: missing file name\n");
        return -1;
    }
    printf("blockos:cat %s\n", argv[1]);
    return 0;
}

int shell_builtin_grep(int argc, char* argv[]) {
    if (argc < 3) {
        printf("grep: missing arguments\n");
        return -1;
    }
    printf("blockos:grep %s in %s\n", argv[1], argv[2]);
    return 0;
}

int shell_builtin_help(int argc, char* argv[]) {
    printf("blockOS 2.0 - Linux Compatible Shell\n\n");
    printf("Built-in commands:\n");
    printf("  echo   - Display text\n");
    printf("  cd     - Change directory\n");
    printf("  pwd    - Print working directory\n");
    printf("  ls     - List files\n");
    printf("  mkdir  - Create directory\n");
    printf("  rmdir  - Remove directory\n");
    printf("  rm     - Remove file\n");
    printf("  cp     - Copy file\n");
    printf("  mv     - Move file\n");
    printf("  cat    - Display file\n");
    printf("  grep   - Search text\n");
    printf("  exit   - Exit shell\n");
    printf("  help   - Show this help\n");
    return 0;
}
