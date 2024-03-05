#ifndef __INTERPRETE_H__
#define __INTERPRETE_H__
#include <stddef.h>
#include "../srv_stream.h"

enum estados {SALIR, SUSURRAR, CAMBIAR_NICKNAME, GLOBAL};

int corroborar_Msj(char* buff, char* aux);

#endif /* __INTERPRETE_H__ */
