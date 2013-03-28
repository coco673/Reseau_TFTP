#include <sys/socket.h>

typedef struct {
  char *name;
  char *ip;
  int port;
}id;

typedef struct {
  int socket;
  id local;
}SocketUDP

 /**
  * Crée une SocketUDP
  * @return: socketUDP
  */
SocketUDP *creerSocketUDP();
/**
 * Crée une SocketUDP attachée sur l'adresse locale
 * (Nom | IP)et le port local donné.
 * @param:address: adresse locale.
 * @param:port: port local.
 * @return: socketUDP
 */
SocketUDP *creerSocketUDPattachee(const char *address, int port);
/**
 * Retourne le nom local de la SocketUDP
 * @return: SocketUDP.
 */
char *getLocalName(SocketUDP *socket);
/**
 * Retourne l'IP local de la SocketUDP
 * @return: name.
 */
char *getLocalIP(socketUDP *socket);
/**
 * Retourne le port local de la SocketUDP
 * @return: port
 */
int getLocalPort(SocketUDP *socket);
/**
 * Ecrit sur @param:socket vers @param:address (NOM | IP)et @param:port,
 * un bloc d'octets @param:buffer de taile @param:length 
 * @return: données lues | -1 en cas d'erreur.
 */
int writeToSocketUDP(SocketUDP *socket, const char *address, int port, const char *buffer, int length);
/**
 * Lit sur @param:socket les données envoyées par une machine 
 * d'adresse @param:address et @param:port. La fonction est bloquante
 * pendant @param:timeout secondes. 
 * Elle lit et place dans @param:buffer un bloc d'octets de 
 * taille <= @param:length.
 * @return: données lues | -1 en cas d'erreur.
 */
int readFromSocketUDP(SocketUDP *socket, char *buffer, int length, const char *address, int *port, int timeout);
/**
 * Libère la socket.
 */
int closeSocketUDP(SocketUDP *socket); 
