#include "SocketUDP.h"

//Indique la routine à suivre pour le signal Alarm.
void gestionnaire();

SocketUDP *creerSocketUDP() {
  SocketUDP *s = (SocketUDP *) malloc(sizeof(SocketUDP *));
  if ((s->socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr,"Erreur pour la création de la socket\n");
    exit(EXIT_FAILURE);
  }
  return s;
}

SocketUDP *creerSocketUDPattachee(const char *address, int port) {
  SocketUDP *s = (SocketUDP*) malloc(sizeof(SocketUDP *));
  struct sockaddr_in sin;
  struct addrinfo hints;

  //Création de la socket.
  if ((s->socket = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    fprintf(stderr,"Erreur pour la création de la socket\n");
    exit(EXIT_FAILURE);
  }

  memset(&hints, 0, sizeof(struct addrinfo));
  //remplissage de la structure sockaddr_in.
  sin.sin_family = AF_INET;
  if (port == 0) {
    sin.sin_port = 0;
    s->local.port = 0;
  } else {
    sin.sin_port = htons(port);
    s->local.port = port;
  }
  printf("on recherche\n");
  //remplissage de la structure qui contiendra les
  //les indications de recherche.
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;
  
  struct addrinfo *res;
  int err;
  if((err = getaddrinfo(address, (char *)&port, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    exit(EXIT_FAILURE);
  }
  if(res == NULL) {
    fprintf(stderr,"Erreur lors de la construction de l'adresse\n");
    exit(EXIT_FAILURE);
  }

  for (struct addrinfo* rp = res; rp != NULL; rp = rp->ai_next) {
   struct sockaddr_in* ssin = (struct sockaddr_in*)rp->ai_addr;
    //On récupère l'ip.
  char ip[16];
  inet_ntop(AF_INET, (struct in_addr *)&ssin->sin_addr.s_addr, ip, sizeof(ip));
  inet_pton(AF_INET, ip, (struct in_addr *) &sin.sin_addr.s_addr);
  }

  //on affecte l'addresse à la socket.
  if (bind(s->socket, (struct sockaddr*) &sin, sizeof(struct sockaddr)) == -1) {
    fprintf(stderr,"Erreur dans le bind\n");
    exit(EXIT_FAILURE);
  }
  freeaddrinfo(res);
  return s;
}
char *getLocalName(SocketUDP *socket){
  char *buffer = (char *) malloc(sizeof(char *));
  strcpy(buffer, socket->local.name);
  return buffer;
}
char *getLocalIP(SocketUDP *socket){
  char *buffer = (char*) malloc(sizeof(char*));
  strcpy(buffer, socket->local.ip);
  return buffer;
}
int getLocalPort(SocketUDP *socket){
  return socket->local.port;
}
int writeToSocketUDP(SocketUDP *socket, const char *address, int port, const char *buffer, int length){
  struct addrinfo hints;
  struct sockaddr_in sin;
  
  memset(&hints, 0, sizeof(struct addrinfo));
  //remplissage de la structure sockaddr_in.
  sin.sin_family = AF_INET;
  if (port == 0) {
    sin.sin_port = 0;
    socket->local.port = 0;
  } else {
    sin.sin_port = htons(port);
    socket->local.port = port;
  }
  //remplissage de la structure qui contiendra les
  //les indications de recherche.
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_protocol = IPPROTO_UDP;
  
 struct addrinfo *res;
  int err;
  if((err = getaddrinfo(address, (char *)&port, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err));
    exit(EXIT_FAILURE);
  }
  if(res == NULL) {
    fprintf(stderr,"Erreur lors de la construction de l'adresse\n");
    exit(EXIT_FAILURE);
  }
for (struct addrinfo* rp = res; rp != NULL; rp = rp->ai_next) {
    struct sockaddr_in* ssin = (struct sockaddr_in*)rp->ai_addr;
    //On récupère l'ip.
  char ip[16];
  inet_ntop(AF_INET, (struct in_addr *) &ssin->sin_addr, ip, sizeof(ip));
  inet_pton(AF_INET, ip, (struct in_addr *)&sin.sin_addr.s_addr);
 }
  freeaddrinfo(res);

  //envoie des données.
  int data;
  if((data = sendto(socket->socket, buffer, length, 0, (struct sockaddr *) &sin, sizeof(struct sockaddr_in))) == -1) {
    fprintf(stderr,"sendto : Erreur pendant l'envoie des données\n");
    return -1;
  }
  return data;
}
int readFromSocketUDP(SocketUDP *socket, char *buffer, int length, const char *address, int *port, int timeout){
  struct sockaddr_in sin;
  int nbBloc;

  struct sigaction act;
  act.sa_handler = gestionnaire;
  if (sigfillset(&act.sa_mask) == -1) {
    perror("sigfillset");
    exit(EXIT_FAILURE);
  }
  act.sa_flags = 0;
  if (sigaction(SIGALRM, &act, NULL) == -1) {
    perror("sigaction");
    exit(EXIT_FAILURE);
  }
  alarm(timeout);
  socklen_t len = sizeof(struct sockaddr_in);
  nbBloc = recvfrom(socket->socket, &buffer, length, 0, (struct sockaddr *) &sin, &len);
  if (nbBloc == -1) {
    fprintf(stderr,"recvfrom : Erreur de Lecture\n");
    return -1;
  }
  *port = ntohs(sin.sin_port);
  inet_ntop(AF_INET, (struct in_addr *) &sin.sin_addr.s_addr, (char*)&address, sizeof(address));
  return nbBloc;
}
int closeSocketUDP(SocketUDP *socket){
  if (close(socket->socket) == -1) {
    fprintf(stderr,"close : Erreur de la fermeture de la socket\n");
    return -1;
  }
  free(socket);
  return 0;
}

void gestionnaire(){
  fprintf(stderr,"alarm : Timeout expiré\n");
  //return -1;
}
