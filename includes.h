#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/syscall.h>
#include <sys/reg.h>
#include <linux/limits.h>

#define syscall_state(x)	((x)->in_syscall)

typedef unsigned long addr_t;
typedef unsigned char uint8_t;

typedef struct {
	pid_t master;
	struct child *child;
} tracee;

struct child {
	pid_t pid;
	unsigned int event;
	char in_syscall;
	char follow_child;
	addr_t *sys_args;
	struct user_regs_struct regs;
	struct child *prev;
	struct child *next;
};

void ptrace_attach(pid_t pid);
void ptrace_detach(pid_t pid);
void ptrace_syscall(pid_t pid);
void ptrace_setopts(pid_t pid);
void ptrace_getregs(struct child *ctx);
void ptrace_peek(pid_t pid, addr_t addr, void *ptr, int len);
addr_t ptrace_peekuser(pid_t pid, int reg_offset);
void handle_syscall_enter(struct child *ctx);
void handle_syscall_exit(struct child *ctx);
void handle_syscall(struct child *ctx);
void handle_event(struct child *ctx, unsigned int event);
struct child *new_child(struct child *current, pid_t pid);
struct child *first_child(struct child *current);
struct child *last_child(struct child *current);
struct child *find_child(struct child *current, pid_t pid);
void c_dup(struct child *c);
