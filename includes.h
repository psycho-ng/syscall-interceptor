#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/mman.h>
#include <sys/ptrace.h>
#include <sys/wait.h>
#include <sys/user.h>
#include <sys/stat.h>
#include <sys/syscall.h>
#include <sys/reg.h>
#include <fcntl.h>
#include <linux/limits.h>

#define __NR_MAX			(512) // actually it's about 300
#define MAX_MAPPINGS	(256)
#define syscall_state(x)	( (x)->in_syscall ) 

typedef unsigned long addr_t;
typedef unsigned char uint8_t;

typedef struct map {
	addr_t start;
	addr_t end;
	size_t len;
	int perm;
	uint8_t *data;	
} map;

typedef struct {
	pid_t master;
	pid_t pid;
	unsigned int event;
	char in_syscall;
	addr_t sys_addr[__NR_MAX];
	map *map[MAX_MAPPINGS];
	struct user_regs_struct regs;
} tracee;

enum {
	MEM_R = 1,
	MEM_W = 2,
	MEM_X = 4,
	MEM_P = 8 
};

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
