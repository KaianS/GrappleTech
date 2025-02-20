#ifndef __LWIPOPTS_H__
#define __LWIPOPTS_H__

// Habilitar suporte a sockets
#define NO_SYS 0
#define LWIP_SOCKET 1
#define LWIP_COMPAT_SOCKETS 1

// Configurações de memória
#define MEM_ALIGNMENT 4
#define MEM_SIZE 1600

// Configurações de TCP/IP
#define LWIP_TCP 1
#define TCP_TTL 255

// Configurações de buffer
#define PBUF_POOL_SIZE 10
#define PBUF_POOL_BUFSIZE 512

#endif /* __LWIPOPTS_H__ */
