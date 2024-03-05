#ifndef __CLI_STREAM_H__
#define __CLI_STREAM_H__
#include <stddef.h>

void error(int *socket, struct addrinfo *resultado, char *printError );
void* enviar(void* arg);
void* recibir(void* arg);


#endif /* __CLI_STREAM_H__ */