#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h> /* PThread */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <semaphore.h>
#include <signal.h> /* Signals */
#include <netinet/in.h>
#include "./SList/Slist.h"
#include "./interprete/interprete.h"
#include "srv_stream.h"
#include <time.h>

sem_t cantAtendidos;
int sock_srv;
SList lista;
pthread_mutex_t candado;

void handler(){
  slist_Destruir(lista);
  close(sock_srv);
  sem_destroy(&cantAtendidos);
  pthread_mutex_destroy(&candado);
  
  printf("\nServer off\n");
  exit(EXIT_SUCCESS);
}

void* atender(void* arg) {
  ssize_t nbytes;
  char buff[MAXMSG], aux[MAXMSG], mensaje[MAXMSG];
  int contador, esta, socketOtraPer;
  SNodo* posicionCliente = (SNodo *) arg;
  char usuarioGral[100], usuarioSus[100];

  printf("Conexion establecida\n");

  int sock_cli = slist_Socket(posicionCliente);
  slist_Nickname(posicionCliente, usuarioGral);


  strcpy(&usuarioSus[1], usuarioGral);
  usuarioSus[0] = '[';
  strcat(usuarioSus, "]");

  do {
    nbytes = recv(sock_cli, buff, MAXMSG, 0);
    if(nbytes < 0 )
      perror("Falló recepcion del mensaje");
    else {
      printf("ACK\n");

      contador = corroborar_Msj(buff, aux);

      switch (contador) {
      case SALIR:
        nbytes = send(sock_cli, buff, 100, 0);
        if(nbytes < 0 )
          perror("Falló el envío del mensaje.");

        pthread_mutex_lock(&candado);
        lista = slist_Eliminar(lista, usuarioGral);
        pthread_mutex_unlock(&candado);
        break;

      case CAMBIAR_NICKNAME:
        pthread_mutex_lock(&candado);
        esta = slist_Controlar(lista, buff);
        
        if(esta) {
          pthread_mutex_unlock(&candado);
          nbytes = send(sock_cli, "Servidor: el nombre ya existe.", 100, 0);
        }
        else {
          slist_Cambiar_Dato(posicionCliente, buff);
          pthread_mutex_unlock(&candado);
          strcpy(usuarioGral, buff);
          strcpy(&usuarioSus[1], usuarioGral);
          strcat(usuarioSus, "]");
          nbytes = send(sock_cli, "Servidor: cambio hecho.", 100, 0);
        }
        if(nbytes < 0 )
          perror("Falló el envío del mensaje.");
        break;

      case SUSURRAR:
        strcpy(mensaje, usuarioSus);
        strcat(mensaje, ": ");
        strcat(mensaje, buff);

        pthread_mutex_lock(&candado);
        socketOtraPer = slist_Devolver_Socket(lista, aux);
        pthread_mutex_unlock(&candado);

        if(socketOtraPer == -1) 
          nbytes = send(sock_cli, "Servidor: dicho usuario no existe.", 100, 0);
        else 
          nbytes = send(socketOtraPer, mensaje, 100, 0);
        
        if(nbytes < 0 )
          perror("Falló el envío del mensaje.");
        break;

      case GLOBAL:
        strcpy(mensaje, usuarioGral);
        strcat(mensaje, ": ");
        strcat(mensaje, buff);
        pthread_mutex_lock(&candado);
        slist_Enviar_General(lista, mensaje);
        pthread_mutex_unlock(&candado);
      default:
        break;
      }
    }
  }
  while(strcmp(COMANDO_SALIDA, buff) != 0);

  printf("Conexion terminada\n");
  //sem ++
  sem_post(&cantAtendidos);
  return 0;
}

void generar_Nombre(char* usuario) {
  int numero = rand () % (1 + 50);
  char buff[100];
  strcpy(usuario, "Usuario");
  sprintf(buff, "%d", numero);
  strcat(usuario, buff);
  usuario[strlen(usuario)] = 0;
}

int main(int argc, char* argv[]){
  if(argc != 2) {
    perror("Error en los datos iniciales");
    exit(EXIT_FAILURE);
  }

  int sock_cli, esta;
  // sockaddr_in similar to sockaddr
  struct sockaddr_in servidor, clientedir;
  socklen_t clientelen = sizeof(clientedir);
  char usuario[10];
  pthread_t clientes;
  pthread_attr_t attr;
  SNodo *posicionCliente;

  /* Creación de Socket Servidor */
  sock_srv = socket(AF_INET, SOCK_STREAM, 0);
  if(sock_srv < 0){
    perror("Falló la creación del socket");
    exit(EXIT_FAILURE);
  }

  /* Asignamos la dirección del servidor */
  servidor.sin_family = AF_INET;
  servidor.sin_addr.s_addr = INADDR_ANY; 
  servidor.sin_port = htons(atoi(argv[1]));

  // El socket sock_srv estara escuchando en cualquier interfaz, gracias a INADDR_ANY
  if (bind(sock_srv, (struct sockaddr *) &servidor, sizeof(servidor))) {
    perror("Error en el bind");
    close(sock_srv);
    exit(EXIT_FAILURE);
  }

  // inicializamos atributos para poder modificarlos.
  pthread_attr_init(&attr);
  /* Hilos que no van a ser *joinables* */
  pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);

  /* El servidor se pone a la espera de conexiones */
  // BLOG es el tamanio maximo de cola de espera.
  if(listen(sock_srv, BLOG) < 0){
    perror("Falló el listen");
    close(sock_srv);
    exit(EXIT_FAILURE);
  }

  lista = slist_Crear();
  // inicializar el semaforo en cant_usuarios indicando con 0 que sera compartido por hilos.
  sem_init(&cantAtendidos, 0, CANT_USUARIOS);
  pthread_mutex_init(&candado, NULL);
  printf("Listen\n");
  // Al captar sigint(ctrl+c) ejecutar handler
  signal(SIGINT, handler);

  srand (time(NULL));

  while(1) {
    /* Apareció una conexión :D! */
    //semaforo --
    sem_wait(&cantAtendidos);
    sock_cli = accept(sock_srv, (struct sockaddr *) &clientedir, &clientelen);

    if(sock_cli < 0){
      perror("Falló el 'accept'");
    }

    else {
      // Agrego persona a la lista de clientes conectados.
      pthread_mutex_lock(&candado);
      do {
        generar_Nombre(usuario);
        esta = slist_Controlar(lista, usuario);
      }
      while(esta);

      lista = slist_Agregar_Final(lista, usuario, sock_cli);
      posicionCliente = slist_Ultimo_Nodo(lista);
      pthread_mutex_unlock(&candado);
      // Se crea un hilo para establecer la comunicacion.
      pthread_create(&clientes, &attr, atender, (void *) posicionCliente);
    }
  }

  return(EXIT_SUCCESS);
}
