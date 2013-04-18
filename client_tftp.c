#include "client_tftp.h"

int maint(int argc, char **argv){
	int rep = 0;
	tftp_packet *packet;
	SocketUDP *sock = CreerSocketUDP();
	printf("Que voulez-vous faire ?\n -0 Envoyer un fichier\n -1 Récupérer un fichier\n";
	fscanf(stdin,"%i",&rep);
	printf("Connexion au serveur ...\n");
	
	switch(rep) {
		case 0 :
		packet->type=01;
		init_packet_RRQ(packet->packet.tftp_packet_RRQ,argv[1],octet);
		tftp_packet_DATA *data;
		if (sendRRQWaitData(sock, packet->packet.tftp_packet_RRQ, packetData, "localhost",69) == -1){
			fprintf(stderr,"Erreur");
			exit(EXIT_FAILURE);
		}
		case 1 :
		default :
		
	}
	if (closeSocketUDP(sock) == -1){
		fprintf(stderr, "Une erreur est survenue lors de la fermeture de la socket\n");
		exit(EXIT_FAILURE);
	}
	printf("Connexion fermée...\n");
	return EXIT_SUCCESS;
} 
 
