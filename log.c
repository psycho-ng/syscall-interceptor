#include "includes.h"

int main(int argc,char **argv) {
	if( argc < 2 ) {
		fprintf(stderr, "Usage: %s [pid]\n", argv[0]);
		return 1;
	}
	tracee *ctx = malloc(sizeof(tracee));
	struct rusage *rup = NULL;
	sigset_t empty_mask;
	int status;

	/* init */
	memset(ctx, 0, sizeof(tracee));
	ctx->pid = ctx->master = atoi(argv[1]);
	ctx->in_syscall = 1;
	sigemptyset(&empty_mask);

	/* trace */
	ptrace_attach(ctx->pid);
	ptrace_setopts(ctx->pid);
	for(;;) {
		sigprocmask(SIG_SETMASK, &empty_mask, NULL);
		ptrace_syscall(ctx->pid);
		ctx->pid = wait4(-1, &status, __WALL, rup);

		if( errno == ECHILD ) continue;

		if( WIFSTOPPED(status) && WSTOPSIG(status) & 0x80 ) {
			handle_event(ctx, (status >> 16) & 0xffff);
			handle_syscall(ctx);
		} else if( WIFEXITED(status) && ctx->pid != ctx->master ) {
	 		ctx->pid = ctx->master;
		} else if( WIFEXITED(status) && ctx->pid == ctx->master ) {
			break;
		}
	}
	free(ctx);
	return 0;
}
