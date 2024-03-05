#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "interprete.h"
#include "../srv_stream.h"

int corroborar_Msj(char* buff, char* aux) {
  if(strcmp(COMANDO_SALIDA, buff) == 0) // Salir del chat.
    return SALIR;

  if(sscanf(buff, "/msg %s %[^\n]", aux, buff) == 2) // Susurrar a alguien.
    return SUSURRAR;

  if(sscanf(buff, "/nickname %[^\n]", buff) == 1) // Cambio de nombre.
    return CAMBIAR_NICKNAME;

  return GLOBAL; // Hablar para todos los usuarios.
}
