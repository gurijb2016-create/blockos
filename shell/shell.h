#ifndef SHELL_H
#define SHELL_H

#include "types.h"

/* Shell command structure */
typedef struct {
    char* name;
    int argc;
    char* argv[256];
    int in_fd;
    int out_fd;
    int pid;
} shell_command_t;

/* Shell built-in commands */
int shell_builtin_echo(int argc, char* argv[]);
int shell_builtin_exit(int argc, char* argv[]);
int shell_builtin_cd(int argc, char* argv[]);
int shell_builtin_pwd(int argc, char* argv[]);
int shell_builtin_ls(int argc, char* argv[]);
int shell_builtin_mkdir(int argc, char* argv[]);
int shell_builtin_rmdir(int argc, char* argv[]);
int shell_builtin_rm(int argc, char* argv[]);
int shell_builtin_cp(int argc, char* argv[]);
int shell_builtin_mv(int argc, char* argv[]);
int shell_builtin_cat(int argc, char* argv[]);
int shell_builtin_grep(int argc, char* argv[]);
int shell_builtin_help(int argc, char* argv[]);

/* Shell functions */
void shell_init(void);
void shell_run(void);
void shell_prompt(void);
char* shell_read_line(void);
shell_command_t* shell_parse_command(const char* line);
int shell_execute_command(shell_command_t* cmd);
void shell_free_command(shell_command_t* cmd);

#endif
