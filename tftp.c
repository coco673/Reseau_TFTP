#include "tftp.h"

char *getErrMsg(int16_t err){
	char *buff = (char*) malloc(sizeof(char*));
	strcpy(buff,erreurs[err]);
  return buff;
}
int16_t getType(const tftp_packet *packet){
  return packet->type;
}
int getSize(tftp_packet *packet){
  return sizeof(packet);
}
void init_packet_RRQ(tftp_packet_RRQ *packet, char *fileName, char *mode){
  packet->fileName = (char *) malloc (sizeof(char *));
  memcpy(packet->fileName,fileName, strlen(fileName));
  packet->mode = (char *) malloc (sizeof(char *));
  memcpy(packet->mode, mode, strlen(mode));
}
void init_packet_WRQ(tftp_packet_WRQ *packet, char *fileName, char *mode){
  packet->fileName = (char *) malloc (sizeof(char *));
  memcpy(packet->fileName,fileName, strlen(fileName));
  packet->mode = (char *) malloc (sizeof(char *));
  memcpy(packet->mode, mode, strlen(mode));  
}
void init_packet_DATA(tftp_packet_DATA *packet, int16_t blockNb, char *data){
  packet->blockNb = blockNb;
  packet->data = (char *) malloc (sizeof(char *));
  memcpy(packet->data, data, strlen(data));
}
void init_packet_ACK(tftp_packet_ACK *packet, int16_t blockNb){
  packet->blockNb = blockNb;
}
void init_packet_ERROR(tftp_packet_ERROR *packet, int16_t errCode){
  packet->errorCode = errCode;
}

