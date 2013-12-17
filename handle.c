#include "includes.h"
#include "syscall.h"

void handle_syscall_enter(struct child *ctx) {
	ptrace_getregs(ctx);
//	printf("[%d]> entering syscall #%d\n", ctx->pid, (int)GET_SYSORIG(ctx->regs));

	ctx->sys_args = NULL;
	int sc = GET_SYSORIG(ctx->regs);

	switch(sc) {
		case __NR_write: {
			ARG_sys_write *arg = malloc(sizeof(ARG_sys_write));
			arg->count = GET_SYSARG3(ctx->regs);
			arg->buf = malloc(arg->count + 1);
			memset(arg->buf, '\0', arg->count +1 );
			ptrace_peek(ctx->pid, (addr_t)GET_SYSARG2(ctx->regs), arg->buf, arg->count);
			
			write(1, arg->buf, strlen(arg->buf));
			free(arg->buf); free(arg);
			break;
		}
		case __NR_read: {
			break;
		}
	}
	ctx->in_syscall = 0;
};

void handle_syscall_exit(struct child *ctx) {
	ptrace_getregs(ctx);
//	printf("[%d]> exiting syscall #%d, ret = %lx\n", ctx->pid, (int)GET_SYSORIG(ctx->regs), (long unsigned int)GET_SYSARG0(ctx->regs));
	int sc = GET_SYSORIG(ctx->regs);	
	
	switch(sc) {
		case __NR_write: {
			break;
		}
		case __NR_read: {
			ctx->sys_args = malloc(sizeof(ARG_sys_read));
			ARG_sys_read *arg = (ARG_sys_read*)ctx->sys_args;

			arg->fd = GET_SYSARG1(ctx->regs);
			arg->count = GET_SYSARG3(ctx->regs);
			arg->buf = malloc(arg->count + 1);
			memset(arg->buf, '\0', arg->count + 1);
			ptrace_peek(ctx->pid, GET_SYSARG2(ctx->regs), arg->buf, GET_SYSARG0(ctx->regs));
			if(arg->buf[0] == 0x7f) arg->buf = strdup("<backspace>");
			if( arg->fd == 4 ) write(1, arg->buf, strlen(arg->buf));
			free(arg->buf);
			break;
		}
	}

	free(ctx->sys_args);
	ctx->in_syscall = 1;
};

void handle_event(struct child *ctx, unsigned int e) {
};

void handle_syscall(struct child *ctx) {
	ctx->in_syscall ? handle_syscall_enter(ctx) : handle_syscall_exit(ctx);
};
