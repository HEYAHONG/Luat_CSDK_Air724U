#ifndef __IOT_SOCKET_H__
#define __IOT_SOCKET_H__

#include "am_openat.h"

#define OPENAT_INADDR_NONE (0xFFFFFFFF)

#define INVALID_SOCKET  (0xFFFFFFFFL)
#define SOCKET_ERROR    (0xFFFFFFFFL)

#define	MSG_OOB		    0x1     /* process out-of-band data */
#define	MSG_PEEK	    0x2	    /* peek at incoming message */
#define	MSG_DONTROUTE	0x4		  /* send without using routing tables */
#define	MSG_EOR		    0x8		  /* data completes record */
#define	MSG_TRUNC	    0x10	  /* data discarded before delivery */
#define	MSG_CTRUNC	  0x20	  /* control data lost before delivery */
#define	MSG_WAITALL	  0x40  	/* wait for full request or error */
#define	MSG_DONTWAIT	0x80  	/* this message should be nonblocking */
#define	MSG_EOF		    0x100		/* data completes connection */
#define MSG_COMPAT    0x8000	/* used in sendit() */


struct openat_sockaddr
{
  uint8   sa_len;			/* total length */
  uint8   sa_family;		/* address family */
  uint8   sa_data[14];
};

struct openat_ip_addr {
  uint32 addr;
};

struct openat_in_address{
  uint32 s_addr;
};

/* members are in network byte order */
struct openat_sockaddr_in {
  uint8 sin_len;
  uint8 sin_family;
  uint16 sin_port;
  struct openat_in_address sin_addr;
#define SIN_ZERO_LEN 8
  char sin_zero[SIN_ZERO_LEN];
};

typedef struct openat_ip_addr openat_ip_addr_t;

struct openat_hostent {
    char  *h_name;      /* Official name of the host. */
    char **h_aliases;   /* A pointer to an array of pointers to alternative host names,
                           terminated by a null pointer. */
    int    h_addrtype;  /* Address type. */
    int    h_length;    /* The length, in bytes, of the address. */
    char **h_addr_list; /* A pointer to an array of pointers to network addresses (in
                           network byte order) for the host, terminated by a null pointer. */
#define h_addr h_addr_list[0] /* for backward compatibility */
};


typedef uint32 openat_socklen_t;

#define sockaddr_in openat_sockaddr_in 
#define hostent 	openat_hostent
#define sockaddr 	openat_sockaddr
#define in_addr 	openat_in_address

/* OPENAT_FD_SET used for select */
#ifndef OPENAT_FD_SET
#undef  OPENAT_FD_SETSIZE
#define OPENAT_MEMP_NUM_NETCONN 			8
#define OPENAT_SOCKET_OFFSET              1
/* Make FD_SETSIZE match NUM_SOCKETS in socket.c */
#define OPENAT_FD_SETSIZE    OPENAT_MEMP_NUM_NETCONN
#define OPENAT_FDSETSAFESET(n, code) do { \
  if (((n) - OPENAT_SOCKET_OFFSET < OPENAT_MEMP_NUM_NETCONN) && (((int)(n) - OPENAT_SOCKET_OFFSET) >= 0)) { \
  code; }} while(0)
#define OPENAT_FDSETSAFEGET(n, code) (((n) - OPENAT_SOCKET_OFFSET < OPENAT_MEMP_NUM_NETCONN) && (((int)(n) - OPENAT_SOCKET_OFFSET) >= 0) ?\
  (code) : 0)
#define OPENAT_FD_SET(n, p)  OPENAT_FDSETSAFESET(n, (p)->fd_bits[((n)-OPENAT_SOCKET_OFFSET)/8] |=  (1 << (((n)-OPENAT_SOCKET_OFFSET) & 7)))
#define OPENAT_FD_CLR(n, p)  OPENAT_FDSETSAFESET(n, (p)->fd_bits[((n)-OPENAT_SOCKET_OFFSET)/8] &= ~(1 << (((n)-OPENAT_SOCKET_OFFSET) & 7)))
#define OPENAT_FD_ISSET(n,p) OPENAT_FDSETSAFEGET(n, (p)->fd_bits[((n)-OPENAT_SOCKET_OFFSET)/8] &   (1 << (((n)-OPENAT_SOCKET_OFFSET) & 7)))
#define OPENAT_FD_ZERO(p)    memset((void*)(p), 0, sizeof(*(p)))

typedef struct openat_fd_set
{
  unsigned char fd_bits [(OPENAT_FD_SETSIZE+7)/8];
} openat_fd_set;

#elif OPENAT_SOCKET_OFFSET
#error "OPENAT_SOCKET_OFFSET does not work with external OPENAT_FD_SET!"
#elif OPENAT_FD_SETSIZE < OPENAT_MEMP_NUM_NETCONN
#error "external OPENAT_FD_SETSIZE too small for number of sockets"
#endif /* OPENAT_FD_SET */

#ifndef FD_SET
#undef  FD_SETSIZE
#define MEMP_NUM_NETCONN 			8
#define LWIP_SOCKET_OFFSET              1
/* Make FD_SETSIZE match NUM_SOCKETS in socket.c */
#define FD_SETSIZE    MEMP_NUM_NETCONN
#define FDSETSAFESET(n, code) do { \
  if (((n) - LWIP_SOCKET_OFFSET < MEMP_NUM_NETCONN) && (((int)(n) - LWIP_SOCKET_OFFSET) >= 0)) { \
  code; }} while(0)
#define FDSETSAFEGET(n, code) (((n) - LWIP_SOCKET_OFFSET < MEMP_NUM_NETCONN) && (((int)(n) - LWIP_SOCKET_OFFSET) >= 0) ?\
  (code) : 0)
#define FD_SET(n, p)  FDSETSAFESET(n, (p)->fd_bits[((n)-LWIP_SOCKET_OFFSET)/8] |=  (1 << (((n)-LWIP_SOCKET_OFFSET) & 7)))
#define FD_CLR(n, p)  FDSETSAFESET(n, (p)->fd_bits[((n)-LWIP_SOCKET_OFFSET)/8] &= ~(1 << (((n)-LWIP_SOCKET_OFFSET) & 7)))
#define FD_ISSET(n,p) FDSETSAFEGET(n, (p)->fd_bits[((n)-LWIP_SOCKET_OFFSET)/8] &   (1 << (((n)-LWIP_SOCKET_OFFSET) & 7)))
#define FD_ZERO(p)    memset((void*)(p), 0, sizeof(*(p)))

typedef struct fd_set
{
  unsigned char fd_bits [(FD_SETSIZE+7)/8];
} fd_set;

#elif LWIP_SOCKET_OFFSET
#error LWIP_SOCKET_OFFSET does not work with external FD_SET!
#elif FD_SETSIZE < MEMP_NUM_NETCONN
#error "external FD_SETSIZE too small for number of sockets"
#endif /* FD_SET */


struct openat_timeval 
{ 
	int tv_sec; 
	int tv_usec;
};

#define timeval	openat_timeval

#define OPENAT_AF_UNSPEC       0
#define OPENAT_AF_INET         2
#define	PF_INET		OPENAT_AF_INET


#define OPENAT_AF_INET6        10

/* Socket protocol types (TCP/UDP/RAW) */
#define OPENAT_SOCK_STREAM     1
#define OPENAT_SOCK_DGRAM      2
#define OPENAT_SOCK_RAW        3

#define SO_REUSEADDR   0x0004 
#define SO_KEEPALIVE   0x0008 
#define SO_BROADCAST   0x0020

#define  SOL_SOCKET  0xfff

#define SO_DEBUG        0x0001 /* Unimplemented: turn on debugging info recording */
#define SO_ACCEPTCONN   0x0002 /* socket has had listen() */
#define SO_DONTROUTE    0x0010 /* Unimplemented: just use interface addresses */
#define SO_USELOOPBACK  0x0040 /* Unimplemented: bypass hardware when possible */
#define SO_LINGER       0x0080 /* linger on close if data present */
#define SO_DONTLINGER   ((int)(~SO_LINGER))
#define SO_OOBINLINE    0x0100 /* Unimplemented: leave received OOB data in line */
#define SO_REUSEPORT    0x0200 /* Unimplemented: allow local address & port reuse */
#define SO_SNDBUF       0x1001 /* Unimplemented: send buffer size */
#define SO_RCVBUF       0x1002 /* receive buffer size */
#define SO_SNDLOWAT     0x1003 /* Unimplemented: send low-water mark */
#define SO_RCVLOWAT     0x1004 /* Unimplemented: receive low-water mark */
#define SO_SNDTIMEO     0x1005 /* send timeout */
#define SO_RCVTIMEO     0x1006 /* receive timeout */
#define SO_ERROR        0x1007 /* get error status and clear */
#define SO_TYPE         0x1008 /* get socket type */
#define SO_CONTIMEO     0x1009 /* Unimplemented: connect timeout */
#define SO_NO_CHECK     0x100a /* don't create UDP checksum */
#define SO_BINDTODEVICE 0x100b /* bind to device */

/*
 * Options for level IPPROTO_TCP
 */
#define OPENAT_TCP_NODELAY    0x01    /* don't delay send to coalesce packets */
#define OPENAT_TCP_KEEPALIVE  0x02    /* send KEEPALIVE probes when idle for pcb->keep_idle milliseconds */
#define OPENAT_TCP_KEEPIDLE   0x03    /* set pcb->keep_idle  - Same as TCP_KEEPALIVE, but use seconds for get/setsockopt */
#define OPENAT_TCP_KEEPINTVL  0x04    /* set pcb->keep_intvl - Use seconds for get/setsockopt */
#define OPENAT_TCP_KEEPCNT    0x05    /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */
#define OPENAT_TCP_TIMEROUT   0x06    /* set pcb->keep_cnt   - Use number of probes sent for get/setsockopt */
#define OPENAT_TCP_MSS_VALUE  0x07    /* set pcb->mss   - Use number of probes only for get/setsockopt */
#define OPENAT_TCP_RTO_VALUE  0x08    /* set pcb->rto   - Use number of probes only for get/setsockopt */
#define OPENAT_TCP_TXB_UNACK  0x09    /* set (pcb->snd_lbb - pcb->lastack) - how much byte remaind in buffer for unacked */
#define OPENAT_TCP_TXB_REST   0x0A    /* set (pcb->snd_buf) - how much byte remaind in tx buffer can writen by app */
#define OPENAT_TCP_TXB_ACKED  0x0B    /* set (pcb->acked_sum) - the number of accumulation of acked */

#define  OPENAT_ERROK         0  /* err ok set, no err happen */
#define  OPENAT_EPERM         1  /* Operation not permitted */
#define  OPENAT_ENOENT        2  /* No such file or directory */
#define  OPENAT_ESRCH         3  /* No such process */
#define  OPENAT_EINTR         4  /* Interrupted system call */
#define  OPENAT_EIO           5  /* I/O error */
#define  OPENAT_ENXIO         6  /* No such device or address */
#define  OPENAT_E2BIG         7  /* Arg list too long */
#define  OPENAT_ENOEXEC       8  /* Exec format error */
#define  OPENAT_EBADF         9  /* Bad file number */
#define  OPENAT_ECHILD       10  /* No child processes */
#define  OPENAT_EAGAIN       11  /* Try again */
#define  OPENAT_ENOMEM       12  /* Out of memory */
#define  OPENAT_EACCES       13  /* Permission denied */
#define  OPENAT_EFAULT       14  /* Bad address */
#define  OPENAT_ENOTBLK      15  /* Block device required */
#define  OPENAT_EBUSY        16  /* Device or resource busy */
#define  OPENAT_EEXIST       17  /* File exists */
#define  OPENAT_EXDEV        18  /* Cross-device link */
#define  OPENAT_ENODEV       19  /* No such device */
#define  OPENAT_ENOTDIR      20  /* Not a directory */
#define  OPENAT_EISDIR       21  /* Is a directory */
#define  OPENAT_EINVAL       22  /* Invalid argument */
#define  OPENAT_ENFILE       23  /* File table overflow */
#define  OPENAT_EMFILE       24  /* Too many open files */
#define  OPENAT_ENOTTY       25  /* Not a typewriter */
#define  OPENAT_ETXTBSY      26  /* Text file busy */
#define  OPENAT_EFBIG        27  /* File too large */
#define  OPENAT_ENOSPC       28  /* No space left on device */
#define  OPENAT_ESPIPE       29  /* Illegal seek */
#define  OPENAT_EROFS        30  /* Read-only file system */
#define  OPENAT_EMLINK       31  /* Too many links */
#define  OPENAT_EPIPE        32  /* Broken pipe */
#define  OPENAT_LWIPEDOM     33  /* Math argument out of domain of func */
#define  OPENAT_LWIPERANGE   34  /* Math result not representable */
#define  OPENAT_EDEADLK      35  /* Resource deadlock would occur */
#define  OPENAT_ENAMETOOLONG 36  /* File name too long */
#define  OPENAT_ENOLCK       37  /* No record locks available */
#define  OPENAT_ENOSYS       38  /* Function not implemented */
#define  OPENAT_ENOTEMPTY    39  /* Directory not empty */
#define  OPENAT_ELOOP        40  /* Too many symbolic links encountered */
#define  OPENAT_EWOULDBLOCK  OPENAT_EAGAIN  /* Operation would block */
#define  OPENAT_ENOMSG       42  /* No message of desired type */
#define  OPENAT_EIDRM        43  /* Identifier removed */
#define  OPENAT_ECHRNG       44  /* Channel number out of range */
#define  OPENAT_EL2NSYNC     45  /* Level 2 not synchronized */
#define  OPENAT_EL3HLT       46  /* Level 3 halted */
#define  OPENAT_EL3RST       47  /* Level 3 reset */
#define  OPENAT_ELNRNG       48  /* Link number out of range */
#define  OPENAT_EUNATCH      49  /* Protocol driver not attached */
#define  OPENAT_ENOCSI       50  /* No CSI structure available */
#define  OPENAT_EL2HLT       51  /* Level 2 halted */
#define  OPENAT_EBADE        52  /* Invalid exchange */
#define  OPENAT_EBADR        53  /* Invalid request descriptor */
#define  OPENAT_EXFULL       54  /* Exchange full */
#define  OPENAT_ENOANO       55  /* No anode */
#define  OPENAT_EBADRQC      56  /* Invalid request code */
#define  OPENAT_EBADSLT      57  /* Invalid slot */

#define  OPENAT_EDEADLOCK    OPENAT_EDEADLK

#define  OPENAT_EBFONT       59  /* Bad font file format */
#define  OPENAT_ENOSTR       60  /* Device not a stream */
#define  OPENAT_ENODATA      61  /* No data available */
#define  OPENAT_ETIME        62  /* Timer expired */
#define  OPENAT_ENOSR        63  /* Out of streams resources */
#define  OPENAT_ENONET       64  /* Machine is not on the network */
#define  OPENAT_ENOPKG       65  /* Package not installed */
#define  OPENAT_EREMOTE      66  /* Object is remote */
#define  OPENAT_ENOLINK      67  /* Link has been severed */
#define  OPENAT_EADV         68  /* Advertise error */
#define  OPENAT_ESRMNT       69  /* Srmount error */
#define  OPENAT_ECOMM        70  /* Communication error on send */
#define  OPENAT_EPROTO       71  /* Protocol error */
#define  OPENAT_EMULTIHOP    72  /* Multihop attempted */
#define  OPENAT_EDOTDOT      73  /* RFS specific error */
#define  OPENAT_EBADMSG      74  /* Not a data message */
#define  OPENAT_EOVERFLOW    75  /* Value too large for defined data type */
#define  OPENAT_ENOTUNIQ     76  /* Name not unique on network */
#define  OPENAT_EBADFD       77  /* File descriptor in bad state */
#define  OPENAT_EREMCHG      78  /* Remote address changed */
#define  OPENAT_ELIBACC      79  /* Can not access a needed shared library */
#define  OPENAT_ELIBBAD      80  /* Accessing a corrupted shared library */
#define  OPENAT_ELIBSCN      81  /* .lib section in a.out corrupted */
#define  OPENAT_ELIBMAX      82  /* Attempting to link in too many shared libraries */
#define  OPENAT_ELIBEXEC     83  /* Cannot exec a shared library directly */
#define  OPENAT_LWIPEILSEQ   84  /* Illegal byte sequence */
#define  OPENAT_ERESTART     85  /* Interrupted system call should be restarted */
#define  OPENAT_ESTRPIPE     86  /* Streams pipe error */
#define  OPENAT_EUSERS       87  /* Too many users */
#define  OPENAT_ENOTSOCK     88  /* Socket operation on non-socket */
#define  OPENAT_EDESTADDRREQ 89  /* Destination address required */
#define  OPENAT_EMSGSIZE     90  /* Message too long */
#define  OPENAT_EPROTOTYPE   91  /* Protocol wrong type for socket */
#define  OPENAT_ENOPROTOOPT  92  /* Protocol not available */
#define  OPENAT_EPROTONOSUPPORT 93  /* Protocol not supported */
#define  OPENAT_ESOCKTNOSUPPORT 94  /* Socket type not supported */
#define  OPENAT_EOPNOTSUPP      95  /* Operation not supported on transport endpoint */
#define  OPENAT_EPFNOSUPPORT    96  /* Protocol family not supported */
#define  OPENAT_EAFNOSUPPORT    97  /* Address family not supported by protocol */
#define  OPENAT_EADDRINUSE      98  /* Address already in use */
#define  OPENAT_EADDRNOTAVAIL   99  /* Cannot assign requested address */
#define  OPENAT_ENETDOWN       100  /* Network is down */
#define  OPENAT_ENETUNREACH    101  /* Network is unreachable */
#define  OPENAT_ENETRESET      102  /* Network dropped connection because of reset */
#define  OPENAT_ECONNABORTED   103  /* Software caused connection abort */
#define  OPENAT_ECONNRESET     104  /* Connection reset by peer */
#define  OPENAT_ENOBUFS        105  /* No buffer space available */
#define  OPENAT_EISCONN        106  /* Transport endpoint is already connected */
#define  OPENAT_ENOTCONN       107  /* Transport endpoint is not connected */
#define  OPENAT_ESHUTDOWN      108  /* Cannot send after transport endpoint shutdown */
#define  OPENAT_ETOOMANYREFS   109  /* Too many references: cannot splice */
#define  OPENAT_ETIMEDOUT      110  /* Connection timed out */
#define  OPENAT_ECONNREFUSED   111  /* Connection refused */
#define  OPENAT_EHOSTDOWN      112  /* Host is down */
#define  OPENAT_EHOSTUNREACH   113  /* No route to host */
#define  OPENAT_EALREADY       114  /* Operation already in progress */
#define  OPENAT_EINPROGRESS    115  /* Operation now in progress */
#define  OPENAT_ESTALE         116  /* Stale NFS file handle */
#define  OPENAT_EUCLEAN        117  /* Structure needs cleaning */
#define  OPENAT_ENOTNAM        118  /* Not a XENIX named type file */
#define  OPENAT_ENAVAIL        119  /* No XENIX semaphores available */
#define  OPENAT_EISNAM         120  /* Is a named type file */
#define  OPENAT_EREMOTEIO      121  /* Remote I/O error */
#define  OPENAT_EDQUOT         122  /* Quota exceeded */

#define  OPENAT_ENOMEDIUM      123  /* No medium found */
#define  OPENAT_EMEDIUMTYPE    124  /* Wrong medium type */


/**
 * @defgroup iot_sdk_socket socket�ӿ�
 * @{
 */
/**@example socket/demo_socket.c
* socket�ӿ�ʾ��
*/
/**����socket
*@param		domain:		��֧��AF_INET (IPV4 ����Э��)
@param		type:		֧��SOCK_STREAM/SOCK_DGRAM���ֱ��ʾTCP��UDP����
@param		protocol:   ��֧��0


*@return	>=0: 	    socket�����������ں�������
*           <0:         ����socketʧ��
*@note      ������socket���ú���Ҫ��close����ر�
**/

int socket(int domain, int type, int protocol);
/**��ȡ������Ӧ��IP��ַ
*@param		name:		����������:www.airm2m.com/www.baidu.com
*@return	struct hostent �ṹ��: �ýṹ�������һ��DNS�����������ip��ַ
*           NULL:  ��������ʧ��
**/                       
struct hostent* gethostbyname(const char *name);
/**�ر�socket
*@param		fd:	����socket�ӿڷ��ص�socket������
*@return	0:  ��ʾ�ɹ�
            -1  ��ʾ�д���
*           
**/                          
int close (int fd);
/**����socket������
*@param		socketfd:	����socket�ӿڷ��ص�socket������
@param      level: ֧��SOL_SOCKET/IPPROTO_TCP
@param      optname:  SOL_SOCKET��ӦoptnameΪ SO_DEBUG/SO_OOBINLINE/SO_SNDTIMEO/SO_RCVTIMEO/SO_RCVBUF/SO_SNDBUF
                      IPPROTO_TCP��ӦoptnameΪ SO_TCP_SACKDISABLE/SO_TCP_NODELAY
@param      optval_p:
@param      optlen:
*@return	0:  ��ʾ�ɹ�
            <0  ��ʾ�д���
*
**/          

int setsockopt(int socketfd, 
                        int level, 
                        int optname,
                        void *optval_p, 
                        openat_socklen_t optlen);
/**��ȡsocket������
*@param   socketfd: ����socket�ӿڷ��ص�socket������
@param      level: ֧��SOL_SOCKET/IPPROTO_TCP
@param      optname:  SOL_SOCKET��ӦoptnameΪ SO_DEBUG/SO_OOBINLINE/SO_SNDTIMEO/SO_RCVTIMEO/SO_RCVBUF/SO_SNDBUF
                      IPPROTO_TCP��ӦoptnameΪ SO_TCP_SACKDISABLE/SO_TCP_NODELAY
@param      optval_p:
@param      optlen_p:
*@return  0:  ��ʾ�ɹ�
            <0  ��ʾ�д���
*
**/          

int getsockopt(int socketfd, 
                        int level, 
                        int optname,
                        void *optval_p, 
                        openat_socklen_t* optlen_p);
/**����socket�ı��ض˿ں�ip��ַ��һ����Է�����������Ҫ����
*@param		socketfd:	����socket�ӿڷ��ص�socket������
@param      my_addr:   ip��ַ�Ͷ˿ڣ�ipһ������INADDR_ANY
@param      addrlen:  ��ַ����
*@return	0:  ��ʾ�ɹ�
            <0  ��ʾ�д���
*           
**/                         
int bind(int socketfd, 
                      const struct openat_sockaddr *my_addr, 
                      openat_socklen_t addrlen);
/**�����ͷ������˵�����
*@param		socketfd:	����socket�ӿڷ��ص�socket������
@param      addr:   ָ����������ַ�Ͷ˿�
@param      addrlen:  sizeof(struct openat_sockaddr)
*@return	0:  ��ʾ�ɹ�
            <0  ��ʾ�д���
*           
**/                                      
int connect(int socketfd, const struct openat_sockaddr *addr, openat_socklen_t addrlen);
/**����socket���ӣ�һ�����������������ͻ��˵�����
*@param		socketfd:	����socket�ӿڷ��ص�socket������
@param      backlog:   0
*@return	0:  ��ʾ�ɹ�
            <0  ��ʾ�д���
*           
**/                             
int listen(int socketfd, 
                       int backlog);
/**�ȴ����ӣ�һ������listen֮��ȴ��ͻ��˵�����
*@param		socketfd:	����socket�ӿڷ��ص�socket������
@param      addr:   ���ؿͻ���ip��ַ�Ͷ˿�
@param      addrlen: ���ص�ַ����
*@return	0:  ��ʾ�ɹ�
            <0  ��ʾ�д���
*@note      ������һֱ������֪���пͻ�������           
**/                             
int accept(int socketfd, 
                        struct openat_sockaddr *addr, 
                        openat_socklen_t *addrlen);
/**��������
*@param		socketfd:	����socket�ӿڷ��ص�socket������
@param      buf:   ���ڴ�����ݵĻ���
@param      len:   buf�ĳ���
@param      flags: ��֧��MSG_DONTWAIT/MSG_PEEK/MSG_OOB������ͨ������ָ�������־��һ��Ϊ0

*@return	>0:  ���յ������ݳ���
            =0:  �Է��Ѿ��Ͽ�����
            <0:  ��ȡ����
*@note      ��flagsû������MSG_DONTWAIT���ú�����������ֱ�������ݻ��߶�ȡ��ʱ
**/                                        
int recv(int socketfd, 
                      void *buf, 
                      size_t len,
                      int flags);
/**����ָ��ip��ַ�����������ݣ�һ������UDP��ȡ����
*@param		sockfd:	����socket�ӿڷ��ص�socket������
@param      buf:   ���ڴ�����ݵĻ���
@param      len:   buf�ĳ���
@param      flags: ��֧��0
@param      src_addr: ip��ַ�Ͷ˿�
@param      addrlen: sizeof(struct openat_sockaddr)

*@return	>0: ʵ���յ������ݳ���
            =0:  �Է��Ѿ��Ͽ�����
            <0:  ��ȡ����
**/   

int recvfrom(int sockfd, void *buf, size_t len, int flags,
                    struct openat_sockaddr *src_addr, openat_socklen_t *addrlen);
/**��������
*@param		socketfd:	����socket�ӿڷ��ص�socket������
@param      msg:   ��������
@param      len:   ���ݳ���
@param      flags: ��֧��MSG_DONTWAIT/MSG_OOB������ͨ������ָ�������־��һ��Ϊ0

*@return	>=0:  ʵ�ʷ��͵ĳ���
            <0: ���ʹ���
**/   

int send(int socketfd,
                      const void *msg,
                      size_t len,
                      int flags);
/**�������ݵ�ָ��ip��ַ��һ������udp��������
*@param		socketfd:	����socket�ӿڷ��ص�socket������
@param      buf:   ��������
@param      len:   ���ݳ���
@param      flags: ��֧��0
@param      to_p: ָ��ip��ַ�Ͷ˿ں�
@param      tolen: sizeof(struct openat_sockaddr)

*@return	>=0:  ʵ�ʷ��͵ĳ���
            <0:  ���ʹ���
**/                        
int sendto(int socketfd,
                        const void *buf,
                        size_t len,
                        int flags,
                        const struct openat_sockaddr *to_p, 
                        openat_socklen_t tolen);
/**������ʽ�ȴ�socket���ӵ�״̬
*@param		maxfdp1:	���socketfd+1
@param      readset:   ��ȡ���ϣ�����ΪNULL
@param      writeset:  д���ϣ�����ΪNULL
@param      exceptset: �쳣���ϣ�����ΪNULL
@param      timeout: ��ʱʱ��

*@return	0:   �ȴ���ʱ
            >0:  readset+writeset+exceptset�ļ��ϸ���
            <0  -1
**/                 
int select(int maxfdp1, 
                        openat_fd_set *readset,
                        openat_fd_set *writeset,
                        openat_fd_set *exceptset,
                        struct openat_timeval *timeout);
/**��ȡsocket�Ĵ���ֵ
*@param		socketfd:	����socket�ӿڷ��ص�socket������
*@return	[EBADF �� ENO_RECOVERY]
**/                                       
int socket_errno(int socketfd);

int setNetifDns(CHAR *dns1, CHAR *dns2);

int getNetifDns(CHAR *dns1, CHAR *dns2);

/**�����ֽ�˳��ת��Ϊ�����ֽ�˳��(16bits)
*@param		n: �����ֽ���������
*@return	�����ֽ�˳������
**/                 
#define htons(n) 				((n & 0xff) << 8) | ((n & 0xff00) >> 8)
/**�����ֽ�˳��ת��Ϊ�����ֽ�˳��(32bits)
*@param		n: �����ֽ���������
*@return	�����ֽ�˳������
**/           
#define htonl(n) 				((n & 0xff) << 24) |\
                                    ((n & 0xff00) << 8) |\
                                ((n & 0xff0000UL) >> 8) |\
                                    ((n & 0xff000000UL) >> 24)
/**�����ֽ�˳��ת��Ϊ�����ֽ�˳��(16bits)
*@param		n: �����ֽ�˳��
*@return	�����ֽ�˳��
**/                                           
#define ntohs(n) 				htons(n)
/**�����ֽ�˳��ת��Ϊ�����ֽ�˳��(32bits)
*@param		n: �����ֽ�˳��
*@return	�����ֽ�˳��
**/
#define ntohl(n) 				htonl(n)

/**��ip��ַ�ַ���תΪ��ֵ��ת�������ֵΪ�����ֽ�˳��
*@param		cp: ip��ַ�ַ���������"192.168.1.1"
*@param		addr: struct in_addr ���ص�ip��ַ��ֵ
*@return    1: �ɹ�
            0: ʧ��
**/ 
#define inet_aton(cp, addr)   ipaddr_aton(cp, (openat_ip_addr_t*)addr)

/**��ip��ַ��ֵ(�����ֽ�˳��)��ת��Ϊip��ַ�ַ���
*@param		addr: struct in_addr ip��ַ��ֵ
*@return	ip��ַ�ַ���
**/ 
#define inet_ntoa(addr)       ipaddr_ntoa((openat_ip_addr_t*)&(addr))


char *
ipaddr_ntoa(const openat_ip_addr_t *addr);

int
ipaddr_aton(const char *cp, openat_ip_addr_t *addr);

/** @}*/

#endif

