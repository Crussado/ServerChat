#ifndef __SLIST_H__
#define __SLIST_H__

#include <stddef.h>

typedef void (*FuncionVisitante) (int dato);

typedef struct _SNodo {
  int socket;
  char nickname[100];
  struct _SNodo *sig;
} SNodo;

typedef SNodo *SList;

/**
 * Devuelve una lista vacía.
 */
SList slist_Crear();
void slist_Imprimir(SList lista);
/**
 * Destruccion de la lista.
 */
void slist_Destruir(SList lista);

/**
 * Determina si la lista es vacía.
 */
int slist_Vacia(SList lista);

/**
 * Agrega un elemento al final de la lista.
 */
SList slist_Agregar_Final(SList lista, char* nickname, int socket);

/**
 * Agrega un elemento al inicio de la lista.
 */
void slist_Enviar_General(SList lista, char* mensaje);
int slist_Longitud(SList lista);
void slist_Cambiar_Dato(SNodo* posicionCliente, char* nickname);
SList slist_Eliminar(SList lista, char* nickname);
SNodo* slist_Ultimo_Nodo(SList lista);
void slist_Nickname(SNodo* posicionCliente, char * usuarioGral);
int slist_Socket(SNodo* posicionCliente);
int slist_Controlar(SList lista, char * buff);
int slist_Devolver_Socket(SList lista, char* nickname);

#endif /* __SLIST_H__ */
