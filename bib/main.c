#include "SocketUDP.h"
#include <stdlib.h>
#include <stdio.h>

#define ADDR 'localhost'
int main (int argc, char ** argv) {

  SocketUDP *socket = creerSocketUDP("localhost",50000);
  while(1);
  return 0;
}
