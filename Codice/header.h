#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<netdb.h>
#include<errno.h>
#include<time.h>
#include<limits.h>
#include<arpa/inet.h>
#include<sys/socket.h>
#include<sys/select.h>

int FullWrite(int fd, void *buf, int count)
{
    int n_written, n_left = count;

    while(n_left > 0)
    {
        if((n_written = write(fd, buf, n_left)) < 0)
        {
            if(errno == EINTR)
                continue;
            else
                exit(n_written);
        }

        n_left -= n_written;
        buf += n_written;
    }

    return n_left;
}
int FullRead(int fd, void *buf, int count)
{
    int n_read, n_left = count;

    while(n_left > 0)
    {
        if((n_read = read(fd, buf, n_left)) < 0)
        {
            if(errno == EINTR)
                continue;
            else
                exit(n_read);
        }
        else if(n_read == 0)
            break;
        
        n_left -= n_read;
        buf += n_read;
    }

    buf = 0;
    return n_left;
}

int Socket(int domain, int type, int protocol)
{
    int sockfd;
    if((sockfd = socket(domain, type, protocol)) < 0)
    {
        perror("!SOCKET!");
        exit(-1);
    }

    return sockfd;
}

//CLIENT
struct hostent* Gethostbyname(char *argv)
{
    struct hostent *simbolicAddress;
    if((simbolicAddress = gethostbyname(argv)) == NULL)
    {
        perror("!GET_HOST_BY_NAME!");
        exit(-1);
    }

    return simbolicAddress;
}

void Connect(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
    if(connect(sockfd, addr, addrlen) < 0)
    {
        perror("!CONNECT!");
        exit(-1);
    }
}

//SERVER
void Setsockopt(int sockfd, int level, int optname, void* val, socklen_t size_of)
{
    if(setsockopt(sockfd, level, optname, val, size_of) < 0)
    {
        perror("!SET_SOCK_OPT!");
        exit(-1);
    }
}

void Bind(int sockfd, const struct sockaddr* addr, socklen_t addrlen)
{
    if(bind(sockfd, addr, addrlen) < 0)
    {
        perror("!BIND!");
        exit(-1);
    }
}

void Listen(int sockfd, int n)
{
    if(listen(sockfd, n) < 0)
    {
        perror("!LISTEN!");
        exit(-1);
    }
}

int Accept(int sockfd, struct sockaddr* addr, socklen_t* addrlen)
{
    int new_fd;
    if((new_fd = accept(sockfd, addr, addrlen)) < 0)
    {
        perror("!ACCEPT!");
        exit(-1);
    }

    return new_fd;
}

int Select(int max_fd, fd_set *readfds, fd_set *writefds, fd_set *exceptfds, struct timeval *timeout)
{
    int n;
    if((n = select(max_fd, readfds, writefds, exceptfds, timeout)) < 0)
    {
        perror("!SELECT!");
        exit(-1);
    }

    return n;
}
