#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

char *erreurs[] = {"non defini",
	"fichier non trouve",
	"violation d'acces",
	"disque plein ou depacement",
	"operation illegale",
	"TID inconnu",
	"le fichier existe deja",
	"utilisateur inconnu"}; 

typedef struct {
  int16_t opCode;
  char *fileName;
  char *mode;
}tftp_packet_RRQ;

typedef struct {
  int16_t opCode;
  char *fileName;
  char *mode;
}tftp_packet_WRQ;

typedef struct {
  int16_t opCode;
  int16_t blockNb;
  char *data;
}tftp_packet_DATA;

typedef struct {
  int16_t opCode;
  int16_t blockNb;
}tftp_packet_ACK; 

typedef struct {
  int16_t opCode;
  int16_t errorCode;
  char *errMsg;
}tftp_packet_ERROR;

typedef struct{
  union {
    tftp_packet_RRQ tftp_packet_RRQ;
    tftp_packet_WRQ tftp_packet_WRQ;
    tftp_packet_DATA tftp_packet_DATA;
    tftp_packet_ACK tftp_packet_ACK;
    tftp_packet_ERROR tftp_packet_ERROR;
  }packet;
  int16_t type;
}tftp_packet;

char *getErrMsg(int16_t errCode);
int16_t getType(const tftp_packet *packet);
int getSize(tftp_packet *packet);
void init_packet_RRQ(tftp_packet_RRQ *packet, char *fileName, char *mode);
void init_packet_WRQ(tftp_packet_WRQ *packet, char *fileName, char *mode);
void init_packet_DATA(tftp_packet_DATA *packet, int16_t blockNb, char *data);
void init_packet_ACK(tftp_packet_ACK *packet, int16_t blockNb);
void init_packet_ERROR(tftp_packet_ERROR *packet, int16_t errCode);

