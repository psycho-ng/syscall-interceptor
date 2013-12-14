#include "includes.h"

void handle_syscall_enter(tracee *ctx) {
	ptrace_getregs(ctx);
	// printf("[%d]> entering syscall #%d\n", ctx->pid, ctx->regs.orig_rax);
	int sc = ctx->regs.orig_rax;
	
	char *buf;
	switch(sc) {
		case __NR_write:
			buf = malloc(ctx->regs.rdx + 1);
			ptrace_peek(ctx->pid, (addr_t)ctx->regs.rsi, buf, (int)ctx->regs.rdx);
			buf[ strlen(buf) ] = '\0';
			write(1, buf, count);
			free(buf);
			break;
	}
	ctx->in_syscall = 0;	
};

void handle_syscall_exit(tracee *ctx) {
	ptrace_getregs(ctx);
	// printf("[%d]> ret = %d\n", ctx->pid, ctx->regs.rax);
	ctx->in_syscall = 1;
};

void handle_event(tracee *ctx, unsigned int e) {
};

void handle_syscall(tracee *ctx) {
	ctx->in_syscall ? handle_syscall_enter(ctx) : handle_syscall_exit(ctx);
};
