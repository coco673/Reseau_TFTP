#include <sys/types.h>

errCode[] = ["Non defini","Fichier non trouve", "Violation d'acces","Disque plein ou depassement","Operation TFTP Illegale","TID Inconnu","Le fichier existe deja","Utilisateur inconnu"];

typedef struct {
  int16_t opCode = 1;
  char *fileName;
  char *mode;
} tftp_packet_RRQ;

typedef struct {
  int16_t opCode = 2;
  char *fileName;
  char *mode;
} tftp_packet_WRQ;

typedef struct {
  int16_t opCode = 3;
  int16_t blockNb;
  char *data;
} tftp_packet_DATA;

typedef struct {
  int16_t opCode = 4;
  int16_t blockNb;
} tftp_packet_ACK; 

typedef struct {
  int16_t opCode = 5;
  int16_t errorCode;
  char *errMsg;
} tftp_packet_ERROR;

typedef union {
  tftp_packet_RRQ rrq;
  tftp_packet_WRQ wrq;
  tftp_packet_DATA data;
  tftp_packet_ACK ack;
  tftp_packet_ERROR error;
}tftp_packet;

char *getErrMsg(int16_t errCode);
int16_t getType(tft_packet *packet);
int16_t getSize(tftp_packet *packet);
void init_packet_RRQ(tftp_packet_RRQ *packet, char *fileName, char *mode);
void init_packet_WRQ(tftp_packet_WRQ *packet, char *fileName, char *mode);
void init_packet_DATA(tftp_packet_DATA *packet, int16_t blockNb, char *data);
void init_packet_ACK(tftp_packet_ACK *packet, int16_t blockNb);
void init_packet_ERROR(tftp_packet_ERROR *packet, int16_t errCode);
