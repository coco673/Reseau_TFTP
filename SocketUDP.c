#include"SocketUDP.h"

SocketUDP *creerSocketUDP() {
  SocketUDP *socket;
  int sock;
  socket = (SocketUDP *) malloc(sizeof(SocektUDP));
  if ((sock = socket(AF_INET,SOCK_DGRAM,0)) == -1) {
    perror("socket");
    exit(EXIT_FAILURE);
  }
  socket->socket = sock;
  return socket;
}

SocketUDP *creerSocketUDPattachee(const char *address, int port){
   SocketUDP* s = (SocketUDP*) malloc(sizeof(SocketUDP));
  struct sockaddr_in sin;
  struct in_addr in;
  struct hostent* host;
  
  if ((s->desc = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("Erreur pour la création de la socket\n");
    return NULL;
  }
  
  bzero(&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  if (port == 0) {
    sin.sin_port = 0;
    s->port = 0;
  } else {
    sin.sin_port = htons(port);
    s->port = port;
  }
  
  if (address == NULL) {    
    sin.sin_addr.s_addr = htonl(INADDR_ANY);
  } else {
    switch (TypeAddress(address)) {
      case -1 : return NULL;
      case 0 : { 
        strcpy(s->name, address);
        if ((host = (struct hostent*) gethostbyname (s->name)) == NULL) {
          perror("Erreur dans le gethostbyname\n");
          return NULL;
        }  
        strcpy(s->IP, (char*) inet_ntoa(*((struct in_addr *) host->h_addr_list[0]))); 
        inet_aton(s->IP, &in);
        sin.sin_addr = in; 
        break;
      }
      case 1 : { 
        inet_aton(address, &in);
        sin.sin_addr = in; 
	    if ((host = gethostbyaddr(&in, 4, AF_INET)) == NULL) {
	      perror("Erreur dans le gethostbyaddr\n");
	      return NULL;
	    }  
	    strcpy(s->name, host->h_name);
	    strcpy(s->IP, address); 
      }
    } 
  }
  
  if (bind(s->desc, (struct sockaddr*) &sin, sizeof(sin)) == -1) {
    perror("Erreur dans le bind\n");
    return NULL;
  }
  
  return s;
}
char *getLocalName(SocketUDP *socket){
	return socket->name;
}
char *getLocalIP(socketUDP *socket){
	return socket->IP;	
}
int getLocalPort(SocketUDP *socket){
	return socket->port;
}
int writeToSocketUDP(SocketUDP *socket, const char *address, int port, const char *buffer, int length){
	struct hostent* host;
  struct sockaddr_in sin;
  struct in_addr in;
  
  bzero(&sin, sizeof(sin));
  sin.sin_family = AF_INET;
  sin.sin_port = htons(port);
  switch (TypeAddress(address)) {
    case -1 :
        perror("Erreur d'adresse\n");
        return -1;
    case 0 : 
        if ((host = (struct hostent*) gethostbyname (address)) == NULL) {
          perror("Erreur dans le gethostbyname\n");
          return -1;
        }  
        sin.sin_addr = (*((struct in_addr*) host->h_addr)); 
        break;   
    case 1 :
        if ((inet_aton(address, &in)) < 0) {
          perror("Erreur dans le inet_aton\n");
          return -1;
        }  
        sin.sin_addr = in;     
  }
  
  return sendto(socket->desc, buffer, length, 0, (struct sockaddr *) &sin, sizeof(struct sockaddr_in));
}
int readFromSocketUDP(SocketUDP *socket, char *buffer, int length, const char *address, int *port, int timeout){
	 struct sockaddr_in sin;
	  int sockLength = sizeof(struct sockaddr_in);
	  int nbBloc;
	  alarm(timeout);
	  nbBloc = recvfrom(socket->desc, buffer, length, 0, (struct sockaddr *) &sin, &sockLength);
	  *port = ntohs(sin.sin_port);
	  strcpy(address, inet_ntoa(sin.sin_addr));
	  return nbBloc;
}
int closeSocketUDP(SocketUDP *socket){
int j;
  
  if ((j = close(socket->desc)) == -1) {
    perror("Erreur dans le Close\n");
    return -1;
  }
  free(socket);
  return 0;
}
