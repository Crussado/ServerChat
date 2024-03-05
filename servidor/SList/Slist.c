#include <stdlib.h>
#include <stdio.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "Slist.h"


SList slist_Crear() {
  return NULL;
}

void eliminar_Nodo(SNodo * nodo) {
  free(nodo);
}

void slist_Destruir(SList lista) {
  SNodo *nodoAEliminar;
  while (lista != NULL) {
    nodoAEliminar = lista;
    lista = lista->sig;
    close(nodoAEliminar->socket);
    eliminar_Nodo(nodoAEliminar);
  }
}

int slist_Longitud(SList lista){
  int cont=0;
  for(; lista!=NULL; lista=lista->sig, cont++);
  return cont;
}

int slist_Vacia(SList lista) {
  return lista == NULL;
}

int slist_Controlar (SList lista, char* nombre) {
  for(;lista != NULL && (strcmp(lista->nickname, nombre) != 0); lista = lista->sig);

  return !(lista == NULL);
}

SList slist_Eliminar(SList lista, char* nickname){
  if(slist_Vacia(lista))
    return lista;
  SList aux;
  if(strcmp(nickname, lista->nickname) == 0) {
    close(lista->socket);
    aux = lista->sig;
    free(lista);
    return aux;
  }

  SList nodo = lista;
  int elimino = 0;
  for(; !elimino && nodo->sig != NULL; nodo = nodo->sig) {
    if(strcmp(nickname, nodo->sig->nickname) == 0) {
      close(nodo->sig->socket);
      aux = nodo->sig->sig;
      free(nodo->sig);
      nodo->sig = aux;
      elimino = 1;
    }
  }
  return lista;
}

void slist_Nickname(SNodo* posicionCliente, char * usuarioGral) {
  strcpy(usuarioGral, posicionCliente->nickname);
}

int slist_Socket(SNodo* posicionCliente) {
  return posicionCliente->socket;
}

int slist_Devolver_Socket(SList lista, char* nickname) {
  if(slist_Vacia(lista))
    return -1;
  int encontro = 0;
  for(; !slist_Vacia(lista) && !encontro; lista = lista->sig) {
    if(strcmp(lista->nickname, nickname) == 0)
      return lista->socket;
  }
  if(encontro)
    return lista->socket;
  return -1;
}

SNodo* slist_Ultimo_Nodo(SList lista) {
  if(slist_Vacia(lista))
    return lista;

  SList nodo = lista;
  for (; nodo->sig != NULL; nodo = nodo->sig);
  return nodo;
}

SList slist_Agregar_Final(SList lista, char* nickname, int socket) {
  SNodo *nuevoNodo = malloc(sizeof(SNodo));
  nuevoNodo->socket = socket;
  strcpy(nuevoNodo->nickname, nickname);
  nuevoNodo->sig = NULL;

  if (slist_Vacia(lista))
    return nuevoNodo;

  SList nodo = lista;

  for (; nodo->sig != NULL; nodo = nodo->sig);

  nodo->sig = nuevoNodo;

  return lista;
}

void slist_Cambiar_Dato(SNodo* posicionCliente, char* nickname) {
  strcpy(posicionCliente->nickname, nickname);
}

void slist_Enviar_General(SList lista, char* mensaje) {
  ssize_t nbytes;  
  for(; !slist_Vacia(lista); lista = lista->sig) {
    nbytes = send(lista->socket, mensaje, 1024, 0);
    if(nbytes < 0 )
      perror("Falló el envío del mensaje.");
  }
}

void slist_Imprimir(SList lista) {
  for(; lista != NULL; lista = lista->sig)
    printf("%s %d\n", lista->nickname, lista->socket);
  printf("\n");
}