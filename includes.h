#include <stdio.h>
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
	pid_t pid;
	unsigned int event;
	char in_syscall;
	struct user_regs_struct regs;
} tracee;

void ptrace_attach(pid_t pid);
void ptrace_detach(pid_t pid);
void ptrace_syscall(pid_t pid);
void ptrace_setopts(pid_t pid);
void ptrace_getregs(tracee *ctx);
void ptrace_peek(pid_t pid, addr_t addr, void *ptr, int len);
addr_t ptrace_peekuser(pid_t pid, int reg_offset);
void handle_syscall_enter(tracee *ctx);
void handle_syscall_exit(tracee *ctx);
void handle_syscall(tracee *ctx);
void handle_event(tracee *ctx, unsigned int event);
