#include <sys/types.h>
#include <sys/stat.h>
#include <sys/times.h>
#include <sys/time.h>
#include <errno.h>
#include <unistd.h>
#include <os/unistd.h>
#include <fcntl.h>
#include <utime.h>
#include <dirent.h>
#include <signal.h>
#undef errno
extern int errno;

/* process */
void _exit(int err_code)
{
	usr_sys_call1(__NR_exit,err_code);
}

int _execve(char *path, char *argv[], char *envp[])
{
	return usr_sys_call3(__NR_execve, path, argv, envp);
}

int _fork()
{
	return usr_sys_call0(__NR_fork);
}

int _getpid()
{
	return usr_sys_call0(__NR_getpid);
}

int _wait(int *status)
{
	return usr_sys_call3(__NR_waitpid, -1, status, 0);
}

int _kill(int pid, int sig)
{
	errno = EINVAL;
	return -1;
}

int getppid()
{
	return usr_sys_call0(__NR_getppid);
}

int sigprocmask(int how, const sigset_t *set, sigset_t *oldset)
{
	errno = -EINVAL;
	return -1;
}

int sigsuspend(const sigset_t *mask)
{
	errno = -EFAULT;
	return -1;
}

int sigaction(int signum, const struct sigaction *act, struct sigaction *oldact)
{
	errno = EINVAL;
	return -1;
}

int wait3(int *status, int options, struct rusage *rusage)
{
	return usr_sys_call3(__NR_waitpid, -1, status, options);
}

uid_t getuid()
{
	return 0;
}

uid_t geteuid()
{
	return 0;
}

gid_t getgid()
{
	return 0;
}

gid_t getegid()
{
	return 0;
}

int getgroups(int size, gid_t list[])
{
	return 0;
}

/* file */
int _fstat(int fd, struct stat *st)
{
	st->st_mode = S_IFREG;
	/* return usr_sys_call2(__NR_fstat, fd, st); */
	return 0;
}

int _stat(const char *path, struct stat *st)
{
	st->st_mode = S_IFREG;
	return 0;
}

int lstat(const char *path, struct stat *st)
{
	return _stat(path, st);
}

int access(const char *path, int mode)
{
	return 0;
}

mode_t umask(mode_t mask)
{
	return mask;
}

int utime(const char *file, const struct utimbuf *times)
{
	return 0;
}

int chmod(const char *path, mode_t mode)
{
	return 0;
}

int chown(const char *path, uid_t owner, gid_t grp)
{
	return 0;
}

long pathconf(const char *path, int name)
{
	return -1;
}

char *getwd(char *buf)
{
	buf[0] = '/';
	buf[1] = 0;
	return buf;
}

char *getcwd(char *buf, size_t size)
{
	return getwd(buf);
}

int chdir(const char *path)
{
	errno = EACCES;
	return -1;
}

int _isatty(int fd)
{
	return 1;
}

int _link(char *old, char *new)
{
	errno = EMLINK;
	return -1;
}

int _unlink(char *name)
{
	errno = ENOENT;
	return -1;
}

int _lseek(int fd, int offset, int whence)
{
	return usr_sys_call3(__NR_lseek, fd, offset, whence);
}

int _open(char *name, int flags, int mode)
{
	int fd;
	if(flags & _FCREAT)
		usr_sys_call2(__NR_mknod, name, 0);
	fd = usr_sys_call2(__NR_open, name, flags);
	if(fd > 0 && (flags & _FAPPEND))
		usr_sys_call3(__NR_lseek, fd, 0, 2);
	if(fd < 0)
		errno = ENOENT;
	return fd;
}

int _read(int fd, void *buf, int count)
{
	return usr_sys_call3(__NR_read, fd, buf, count);
}

int _write(int fd, void *buf, int count)
{
	int write, n;
	write = 0;
	while(write < count)
	{
		n = usr_sys_call3(__NR_write, fd, buf+write, count-write);
		if(n == -1)
			return -1;
		write += n;
	}
	return write;
}

int getdents(int fd, struct dirent *dirp, int count)
{
	int ret;
	if(count < sizeof(struct dirent))
	{
		errno = EINVAL;
		return -1;
	}
	ret = usr_sys_call3(__NR_readdir, fd, dirp, 1);
	if(ret == -1)
		return -1;
	return ret*sizeof(struct dirent);
}

int mkdir(const char *path, mode_t mode)
{
	//return usr_sys_call2(__NR_mknod, path, 1);
	return 0;
}

int rmdir(const char *path)
{
	//errno = EACCES;
	return 0;
}


int _close(int fd)
{
	return usr_sys_call1(__NR_close, fd);
}

int dup2(int oldfd, int newfd)
{
	return usr_sys_call2(__NR_dup2, oldfd, newfd);
}

int pipe(int pipefd[])
{
	int fd1 = open("/dev/pipe/0", 0);
	int fd2 = -1;
	fd2 = dup2(fd1, -1);
	if(fd2 == -1)
	{
		close(fd1);
		return -1;
	}
	pipefd[0] = fd1;
	pipefd[1] = fd2;
	return 0;
}

/* memory */
void *_sbrk(int inc)
{
	unsigned long ret;
	ret = usr_sys_call1(__NR_sbrk, inc);
	if(ret == -1)
		errno = ENOMEM;
	else
		errno = 0;
	return (void *)ret;
}

/* time */
clock_t _times(struct tms *buf)
{
	errno = EACCES;
	return -1;
}

int _gettimeofday(struct timeval *tv, struct timezone *tz)
{
	tv->tv_sec = usr_sys_call0(__NR_gettime);
	tv->tv_usec = 0;
	return 0;
}

