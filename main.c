#include <stdio.h>
#include <stdlib.h>
#include "fat.h"

int main(){

  char data[] = "Welcome my son to the machine";


  initialise_fat();
  creer_objet("fichier1",0,50,data);
  creer_objet("fichier2",0,50,data);
  creer_objet("fichier3",1,50,data);

  supprimer_tout();

  creer_objet("fichier4",2,50,"New data");
  creer_objet("fichier5",2,50,"New data");

  printf("FAT : %d ; %d ; %d ; %d ; %d ; %d\n\n",FAT[0],FAT[1],FAT[2],FAT[3],FAT[4],FAT[5]);
  printBlock(0);
  printBlock(1);
  printBlock(2);
  printBlock(3);
  printBlock(4);
  printBlock(5);

  printf("\nFree Block : %d\n",freeblocks);

  return EXIT_SUCCESS;
}
