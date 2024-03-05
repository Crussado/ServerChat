#ifndef __SRV_STREAM_H__
#define __SRV_STREAM_H__
#include <stddef.h>

#define BLOG 25
#define CANT_USUARIOS 3

#define COMANDO_MEN "/msg"
#define COMANDO_SALIDA "/exit"
#define COMANDO_NICK "/nickname"
#define MAXMSG 1024

void* atender(void* arg);

void handler();

#endif /* __SRV_STREAM_H__ */
