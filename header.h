#include <stdio.h>
#include <signal.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include<sys/wait.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h> 
#define LISTENQ 1024 
#define MAXLINE 4096 
void err_sys(const char* x) 
{ 
    perror(x); 
    exit(1); 
}
int Socket(int family, int type, int protocol)
{
    int n;
    if( (n = socket( family, type, protocol)) < 0 )
    err_sys("socket error");
    return (n);
}
void Bind(int fd, const struct sockaddr *sa, socklen_t salen)
{
 if (bind(fd, sa, salen) < 0)
 err_sys("bind error");
}
void Listen(int fd, int backlog)
{
	char	*ptr;

		/*4can override 2nd argument with environment variable */
	if ( (ptr = getenv("LISTENQ")) != NULL)
		backlog = atoi(ptr);

	if (listen(fd, backlog) < 0)
		err_sys("listen error");
}

int Accept(int fd, struct sockaddr *sa, socklen_t *salenptr)
{
	int	n;
again:
	if ( (n = accept(fd, sa, salenptr)) < 0) {
#ifdef	EPROTO
		if (errno == EPROTO || errno == ECONNABORTED)
#else
		if (errno == ECONNABORTED)
#endif
			goto again;
		else
			err_sys("accept error");
	}
	return(n);
}
const char *Inet_pton(int family, const char *strptr, void *addrptr)
{
        int             n;

        if ( (n = inet_pton(family, strptr, addrptr)) < 0)
                err_sys("inet_pton error for address");      /* errno set */
        else if (n == 0)
		{
                printf("inet_pton error for address");     /* errno not set */
				exit(1);
		}

        /* nothing to return */
}
ssize_t						/* Write "n" bytes to a descriptor. */
writen(int fd, const void *vptr, size_t n)
{
	size_t		nleft;
	ssize_t		nwritten;
	const char	*ptr;

	ptr = vptr;
	nleft = n;
	while (nleft > 0) {
		if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;		/* and call write() again */
			else
				return(-1);			/* error */
		}

		nleft -= nwritten;
		ptr   += nwritten;
	}
	return(n);
}
/* end writen */

void Writen(int fd, void *ptr, size_t nbytes)
{
	if (writen(fd, ptr, nbytes) != nbytes)
		err_sys("writen error");
}

static int	read_cnt;
static char	*read_ptr;
static char	read_buf[MAXLINE];

static ssize_t
my_read(int fd, char *ptr)
{

	if (read_cnt <= 0) {
again:
		if ( (read_cnt = read(fd, read_buf, sizeof(read_buf))) < 0) {
			if (errno == EINTR)
				goto again;
			return(-1);
		} else if (read_cnt == 0)
			return(0);
		read_ptr = read_buf;
	}

	read_cnt--;
	*ptr = *read_ptr++;
	return(1);
}

ssize_t
readline(int fd, void *vptr, size_t maxlen)
{
	ssize_t	n, rc;
	char	c, *ptr;

	ptr = vptr;
	for (n = 1; n < maxlen; n++) {
		if ( (rc = my_read(fd, &c)) == 1) {
			*ptr++ = c;
			if (c == '\n')
				break;	/* newline is stored, like fgets() */
		} else if (rc == 0) {
			*ptr = 0;
			return(n - 1);	/* EOF, n - 1 bytes were read */
		} else
			return(-1);		/* error, errno set by read() */
	}

	*ptr = 0;	/* null terminate like fgets() */
	return(n);
}

ssize_t
readlinebuf(void **vptrptr)
{
	if (read_cnt)
		*vptrptr = read_ptr;
	return(read_cnt);
}
/* end readline */

ssize_t
Readline(int fd, void *ptr, size_t maxlen)
{
	ssize_t		n;

	if ( (n = readline(fd, ptr, maxlen)) < 0)
		err_sys("readline error");
	return(n);
}
void Connect(int fd, const struct sockaddr *sa, socklen_t salen)
{
	if (connect(fd, sa, salen) < 0)
		err_sys("connect error");
}
void Fputs(const char *ptr, FILE *stream)
{
	if (fputs(ptr, stream) == EOF)
		err_sys("fputs error");
}
void Inet_aton(const char *strptr, struct in_addr *addrptr) //wrapper
{
	if(inet_aton(strptr,  addrptr)==0)
	{
	 printf("inet_aton error");
	 exit(1);
	}

}

ssize_t Read(int fd, void *ptr, size_t nbytes)
{
	ssize_t		n;

	if ( (n = read(fd, ptr, nbytes)) == -1)
		err_sys("read error");
	return(n);
}
void Close(int fd)
{
	if (close(fd) == -1)
		err_sys("close error");
}
pid_t Fork(void)
{
	pid_t	pid;

	if ( (pid = fork()) == -1)
		err_sys("fork error");
	return(pid);
}
