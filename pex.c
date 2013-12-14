#include "includes.h"

void ptrace_attach(pid_t pid) {
	if( ptrace(PTRACE_ATTACH, pid, NULL, NULL) == -1 ) {
		perror("ptrace_attach()");
		exit(1);
	}
};

void ptrace_detach(pid_t pid) {
	if( ptrace(PTRACE_DETACH, pid, NULL, NULL) == -1 ) {
		perror("ptrace_detach()");
		exit(1);
	}
};

void ptrace_setopts(pid_t pid) {
	if( ptrace(PTRACE_SETOPTIONS, pid, NULL, PTRACE_O_TRACEEXEC|PTRACE_O_TRACESYSGOOD|PTRACE_O_TRACECLONE|PTRACE_O_TRACEFORK|PTRACE_O_TRACEVFORK) == -1 ) {
		perror("ptrace_setopts()");
		exit(1);
	}
};

void ptrace_peek(pid_t pid, addr_t addr, void *ptr, int len) {
	long word;
	int count = 0;

	while(len > 0) {
		if( (word = ptrace(PTRACE_PEEKDATA, pid, addr+count, NULL)) == -1 ) {
			perror("ptrace_peek()");
			exit(1);
		}
		memcpy((uint8_t*)(ptr + count), &word, (len < (int)sizeof(long)) ? len : (int)sizeof(long));
		len -= sizeof(long);
		count += sizeof(long);
	}
};

void ptrace_getregs(tracee *ctx) {
	if( (ptrace(PTRACE_GETREGS, ctx->pid, NULL, &ctx->regs)) == -1 ) {
		perror("ptrace_getregs()");
		exit(1);
	}
};

addr_t ptrace_peekuser(pid_t pid, int reg_offset) {
	return ptrace(PTRACE_PEEKUSER, pid, sizeof(long) * reg_offset, NULL);
};

void ptrace_syscall(pid_t pid) {
	if( ptrace(PTRACE_SYSCALL, pid, NULL, NULL) == -1 ) {
		/*
		perror("ptrace_syscall()");
		exit(1);
		*/
	}
};
