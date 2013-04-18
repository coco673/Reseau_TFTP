#include "utils.h"

/**
 *Envoie le paquet @param:packet à travers @param:socket
 * à l'adresse IP @param:adresse et de port @param:port.
 * @return 0 | -1 en cas d'erreur.
 */
int send_packet(const tftp_packet *packet, SocketUDP *socket, char *adresse, int port){
  char *buffer = (char*) malloc(sizeof(char*));
  int16_t type;
  int len = 0;
  bool fini = false;
int tmp = 0;

    type = getType(packet);
  while (!fini) {
    switch(type) {
      //paquets de type RRQ
    case 1:
      len = init_buff_RRQ(buffer, packet->packet.tftp_packet_RRQ);
      fini = true;
      break;
      //paquets de type WRQ
    case 2: 
len = init_buff_WRQ(buffer, packet->packet.tftp_packet_WRQ);
      fini = true;
      break;
      //paquets de type DATA
    case 3 :
      len = init_buff_DATA(buffer, packet->packet.tftp_packet_DATA, &fini, &tmp);
      break;
      //paquets de type ACK
    case 4 :
      len = init_buff_ACK(buffer, packet->packet.tftp_packet_ACK);
      fini = true;
      break;
      //paquets de type ERROR
    case 5:
      len = init_buff_ERR(buffer, packet->packet.tftp_packet_ERROR);
      fini = true;
      break;

    default : 
      fprintf(stderr,"Type de paquet inconnu\n");
      exit(EXIT_FAILURE);
      fini = true;
      break;
    }
    if(writeToSocketUDP(socket, adresse, port, buffer, len) == -1) {
      perror("WriteToSocketUDP");
      return -1;
    }
  }
  return 0;
}

/**
 *Attend sur @param:socket de recevoir un paquet TFTP de l'adresse @param:adresse 
 *et de port @param:port avec un timeout @param:sec. Si un message d'une autre 
 *autre source, un message d'erreur est envoyé à l'expediteur et l'écoute recommence.
 *@return: 0 si le timeout à expiré | -1 en cas d'erreur.
 */
int receive_with_timeout(SocketUDP *socket, char *adresse, int port, int sec, tftp_packet *response_packet, size_t *packet_length){
  char *buffer = (char*) malloc (sizeof(char *));
  char *adresseRecu = (char*) malloc (sizeof(char *));
  int portRecu = 0;
  int dataSize = 0;

  while(!strcmp(adresse,adresseRecu) && port != portRecu){
    dataSize = readFromSocketUDP(socket, buffer, sizeof(buffer), adresseRecu, &portRecu, sec);
    if(!strcmp(adresse,adresseRecu) && port != portRecu){
      tftp_packet_ERROR *err;
      init_packet_ERROR(err,05);
      if(send_packet((tftp_packet *)&err,socket,adresseRecu,portRecu) == -1){
	fprintf(stderr,"Une erreur est survenue pendant l'envoie\n");
      }
    }
  }
  switch(dataSize) {
  case -1 :
    fprintf(stderr, "readFromSocketUDP: Erreur de lecture\n");
    return -1;
    break;
  case 0 :
    fprintf(stderr, "readFromSocketUDP: timeout expiré\n");
    break;
  default :
    return dataSize;
  }
return dataSize;
}

/**
 * Envoie d'un paquet data et attente de l'accusé de recepetion ack en retour.
 **/
int sendDataWaitACK(SocketUDP *socket, tftp_packet_DATA *packetData, tftp_packet_ACK *packetAck, char *adresse, int port){
  int i = 0;
  bool recu = false;
  char *adresseRecu = (char *)malloc(sizeof(char *));
  int portRecu = 0;

  while(i < 10 && !recu) {
    send_packet((tftp_packet *)&packetData, socket,adresse, port);
size_t packetLength = sizeof(packetAck);
    receive_with_timeout(socket, adresseRecu, portRecu, 10, (tftp_packet *)&packetAck, &packetLength);
    if(getType((tftp_packet *)&packetAck) == 04) {
      recu = true;
    } else if(getType((tftp_packet *)&packetAck) != 04) {
      tftp_packet_ERROR *err;
      init_packet_ERROR(err, 04);
      send_packet((tftp_packet *)&err, socket, adresseRecu, portRecu);
      return -1;
    }
    i++;
  }
  if(i == 10 && !recu) {
    return -1;
  }
  return 0;
}

/**
 * Envoie d'un accusé de reception ack et attente d'un paquet data.
 */
int sendACKWaitData(SocketUDP *socket, tftp_packet_ACK *packetAck, tftp_packet_DATA *packetData, char *adresse, int port){
  int i = 0;
  bool recu = false;
  char *adresseRecu = (char *)malloc(sizeof(char *));
  int portRecu = 0;
  char *buffer = (char *)malloc(sizeof(char*));

  while(i < 10 && !recu) {
    send_packet((tftp_packet *)&packetAck, socket,adresse, port);
    readFromSocketUDP(socket, buffer, 516, adresseRecu, &portRecu, 10);
    if(getType((tftp_packet *)&packetData) == 03) {
      recu = true;
    } else if(getType((tftp_packet *)&packetData) != 03) {
      tftp_packet_ERROR *err;
      init_packet_ERROR(err, 04);
      send_packet((tftp_packet *)&err, socket, adresseRecu, portRecu);
      return -1;
    }
    i++;
  }
  if(i == 10 && !recu) {
    return -1;
  }
  return 0;
}

/**
 *Envoie d'une requete de lecture et attente d'un paquet data.
 */
int sendRRQWaitData(SocketUDP *socket, tftp_packet_RRQ *packetRrq, tftp_packet_DATA *packetData, char *adresse, int port){
  int i = 0;
  bool recu = false;
  char *adresseRecu = (char *)malloc(sizeof(char *));
  int portRecu = 0;

  while(i < 10 && !recu) {
    send_packet((tftp_packet *)&packetRrq, socket,adresse, port);
size_t packetLength = sizeof(packetData);
    receive_with_timeout(socket, adresseRecu, portRecu, 10, (tftp_packet *)&packetData, &packetLength);
    if(getType((tftp_packet *)&packetData) == 03) {
      recu = true;
    } else if(getType((tftp_packet *)&packetData) != 03) {
      tftp_packet_ERROR *err;
      init_packet_ERROR(err, 04);
      send_packet((tftp_packet *)&err, socket, adresseRecu, portRecu);
      return -1;
    }
    i++;
  }
  if(i == 10 && !recu) {
    return -1;
  }
  return 0;
}

/**
 *Envoie d'une requete d'écriture et attente de l'accord du serveur pour débuter l'envoie.
 */
int sendWRQWaitACK(SocketUDP *socket, tftp_packet_WRQ *packetWrq, tftp_packet_ACK *packetAck, char *adresse, int port){
  int i = 0;
  bool recu = false;
  char *adresseRecu = (char *)malloc(sizeof(char *));
  int portRecu = 0;

  while(i < 10 && !recu) {
    send_packet((tftp_packet *)&packetWrq, socket,adresse, port);
size_t packetLength = sizeof(packetAck);
    receive_with_timeout(socket, adresseRecu, portRecu, 10, (tftp_packet *)&packetAck, &packetLength);
    if(getType((tftp_packet *)&packetAck) == 04) {
      recu = true;
    } else if(getType((tftp_packet *)&packetAck) != 04) {
      tftp_packet_ERROR *err;
      init_packet_ERROR(err, 04);
      send_packet((tftp_packet *)&err, socket, adresseRecu, portRecu);
      return -1;
    }
    i++;
  }
  if(i == 10 && !recu) {
    return -1;
  }
  return 0;
}

int init_buff_RRQ(char *buffer, tftp_packet_RRQ packet) {
int len = 0;
memcpy(buffer, (char*)&packet.opCode, 2);
      len += 2;
      memcpy(buffer+len, packet.fileName, strlen(packet.fileName));
      len += strlen(packet.fileName);
      memcpy(buffer+len,'\0', 1);
      len += 1;
      memcpy(buffer+len, packet.mode, strlen(packet.mode));
      len += strlen(packet.mode);
      memcpy(buffer+len,'\0', 1);
len+=1;
return len;
}

int init_buff_WRQ(char *buffer, tftp_packet_WRQ packet) {
int len = 0;
      memcpy(buffer, (char*)&packet.opCode, 2);
      len += 2;
      memcpy(buffer+len, packet.fileName,strlen(packet.fileName)); 
      len += strlen(packet.fileName);
      memcpy(buffer+len,'\0', 1);
	len += 1;
      memcpy(buffer+len, packet.mode, strlen(packet.mode));
      len +=  strlen(packet.mode);
      memcpy(buffer+len,'\0', 1);
      len += 1;
return len;
}

int init_buff_DATA(char *buffer, tftp_packet_DATA packet, bool *fini, int *tmp) {
int len = 0;
memcpy(buffer, (char*)&packet.opCode, 2);
      len += 2;
      memcpy(buffer+len, (char*)&packet.blockNb, 2);
      len += 2;
size_t length = strlen(packet.data) - (size_t)tmp;
      if (length > 512) {
	  memcpy(buffer+len, packet.data+(*tmp),512);
	len += 512;
	tmp += 512;
      } else { 
	memcpy(buffer+len, packet.data+(*tmp), length);
	len += 512;
	*fini = true;
	}
return len;
}
int init_buff_ACK(char *buffer, tftp_packet_ACK packet) {
int len = 0;
memcpy(buffer, (char*)&packet.opCode, 2);
      len += 2;
      memcpy(buffer+len, (char*)&packet.blockNb, 2);
      len += 2;
return len;
}

int init_buff_ERR(char *buffer, tftp_packet_ERROR packet) {
int len = 0;
memcpy(buffer,(char*)&packet.opCode, 2);
      len += 2;
      memcpy(buffer+len,(char*)&packet.errorCode, 2);
      len += 2;
      memcpy(buffer+len, packet.errMsg, strlen(packet.errMsg));
      len += strlen(packet.errMsg);
      memcpy(buffer+len,'\0', 1);
      len += 1;
return len;
}
