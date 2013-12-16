#include "includes.h"

int main(int argc,char **argv) {
	if( argc < 2 ) {
		fprintf(stderr, "Usage: %s [pid]\n", argv[0]);
		return 1;
	}
	tracee *ctx = malloc(sizeof(tracee));
	struct child *c;
	c = ctx->child = malloc(sizeof(struct child));
	struct rusage *rup = NULL;
	sigset_t empty_mask;
	int status;
	pid_t w_pid;

	/* init */
	c->prev = c->next = NULL;
	c->pid = ctx->master = atoi(argv[1]);
	c->in_syscall = 1;
	sigemptyset(&empty_mask);

	/* trace */
	ptrace_attach(c->pid);
	wait4(c->pid, &status, __WALL, rup);
	ptrace_setopts(c->pid);
	for(;;) {
		sigprocmask(SIG_SETMASK, &empty_mask, NULL);
		ptrace_syscall(c->pid);
		w_pid = wait4(-1, &status, __WALL, rup);
//		printf("wait4(-1, ...) = %d\n", w_pid);

		if( find_child(c, w_pid) == NULL ) /* new clone() */
			c = new_child(c, w_pid);
		else
			c = find_child(c, w_pid);

		if( errno == ECHILD ) continue;

		if( WIFSTOPPED(status) && WSTOPSIG(status) & 0x80 ) {
			handle_event(c, (status >> 16) & 0xffff);
			handle_syscall(c);
		} else if( WIFEXITED(status) && c->pid != ctx->master ) {
	 		c->pid = ctx->master;
		} else if( WIFEXITED(status) && c->pid == ctx->master ) {
			break;
		}
	}
	free(ctx);
	return 0;
}
