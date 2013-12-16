#if __i386__
#define GET_SYSORIG(x)	((x).orig_eax)
#define GET_SYSARG0(x)	((x).eax)
#define GET_SYSARG1(x)	((x).ebx)
#define GET_SYSARG2(x)	((x).ecx)
#define GET_SYSARG3(x)	((x).edx)
#define GET_SYSARG4(x)	((x).esi)
#define GET_SYSARG5(x)	((x).edi)
#define GET_SYSARG6(x)	((x).ebp)
#elif __x86_64__
#define GET_SYSORIG(x)	((x).orig_rax)
#define GET_SYSARG0(x)	((x).rax)
#define GET_SYSARG1(x)	((x).rdi)
#define GET_SYSARG2(x)	((x).rsi)
#define GET_SYSARG3(x)	((x).rdx)
#define GET_SYSARG4(x)	((x).r10)
#define GET_SYSARG5(x)	((x).r9)
#define GET_SYSARG6(x)	((x).r8)
#else
	#error	_("Please check syscall.h")
#endif

typedef struct ARG_sys_write {
	int fd;
	char *buf;
	size_t count;
} ARG_sys_write;

typedef struct ARG_sys_read {
	int fd;
	char *buf;
	size_t count;
} ARG_sys_read;
