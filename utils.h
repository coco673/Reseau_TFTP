#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "./bib/SocketUDP.h"
#include <stdbool.h>
#include "tftp.h"

int send_packet(const tftp_packet *packet, SocketUDP* socket, char* adresse, int port);
int receive_with_timeout(SocketUDP *socket, char *adresse, int port, int sec, tftp_packet *response_packet, size_t *packet_length);
int sendDataWaitACK(SocketUDP *socket, tftp_packet_DATA *packetData, tftp_packet_ACK *packetAck, char *adresse, int port);
int sendACKWaitData(SocketUDP *socket, tftp_packet_ACK *packetAck, tftp_packet_DATA *packetData, char *adresse, int port);
int sendRRQWaitData(SocketUDP *socket, tftp_packet_RRQ *packetRrq, tftp_packet_DATA *packetData, char *adresse, int port);
int sendWRQWaitACK(SocketUDP *socket, tftp_packet_WRQ *packetWrq, tftp_packet_ACK *packetAck, char *adresse, int port);
int init_buff_RRQ(char *buff, tftp_packet_RRQ packet);
int init_buff_WRQ(char *buff, tftp_packet_WRQ packet);
int init_buff_DATA(char *buff, tftp_packet_DATA packet,bool *fini, int *tmp);
int init_buff_ACK(char *buff, tftp_packet_ACK packet);
int init_buff_ERR(char *buff, tftp_packet_ERROR packet);
