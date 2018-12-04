#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "fat.h"

void initialise_fat(){

  //Declarer tout les blocks libres dans FAT
  for(int i = 0;i<BLOCNUM;i++){
    FAT[i] = 0xFFFF;
  }

  //Declarer le nombre de blocks libres au max
  freeblocks = BLOCNUM;

  //Declarer qu'il n'y a aucun fichier dans la liste chainée
  obj = NULL;
}

int findFreeBlock(){

  int blockFound = 0;
  int idBlockFound = 0;

  while(idBlockFound<BLOCNUM && blockFound == 0){
    if(FAT[idBlockFound] == 0xFFFF){
      blockFound = 1;
    }else{
        idBlockFound++;
    }
  }

  return idBlockFound;

}

struct objet *creer_objet(char *nom, unsigned short auteur,unsigned int taille, char *data){

  //Le nouvel element
  Objet *newElement = NULL;

  //Verifier si aucun fichier ne porte pas le meme nom
  int foundSameName = 0;
  Objet *ptr = obj;
  while(foundSameName == 0 && ptr != NULL){
    if(strcmp(ptr->nom,nom) == 0) foundSameName = 1;
    ptr = ptr->next;
  }

  //Si pas de doulons proceder a la creation
  if(foundSameName == 0){

    newElement = malloc(sizeof(Objet));

    //Definir le debut du fichier
    int blockIndex = findFreeBlock();
    newElement->index = blockIndex;
    FAT[blockIndex] = 0xFFFE;

    //Copier data dans les block
    int currentDataBlockIndex = 0;
    for(int dataIndex = 0;data[dataIndex] != '\0' ;dataIndex++){

      //Si on arrive à la fin d'un block
      if(currentDataBlockIndex == BLOCSIZE){
          //Pour ne pas reecrire sur le meme block
          FAT[blockIndex] = 0xFFFE;

          //Chercher un nouveau block et mettre a jour FAT
          int nextBlockIndex = findFreeBlock();
          FAT[blockIndex] = nextBlockIndex;
          blockIndex = nextBlockIndex;

          //Signaler qu'un block a été pris
          freeblocks--;

          //Remettre au debut du block
          currentDataBlockIndex = 0;
      }

      //Ecrire dans le block
      //printf("Block : %d <- %c \n",blockIndex,data[dataIndex]);
      volume[(blockIndex*BLOCSIZE)+currentDataBlockIndex] = data[dataIndex];

      currentDataBlockIndex++;
    }

    //printf("\n");

    //Declarer le block pris (FIN)
    FAT[blockIndex] = 0xFFFE;
    freeblocks--;

    //Creer les metadonées en tete
    strcpy(newElement->nom,nom);
    newElement->auteur = auteur;
    newElement->taille = taille;
    newElement->next = obj;

    obj = newElement;
  }

  return newElement;

}

struct objet *rechercher_objet(char *nom){

  //Prendre le premier objet
  Objet *ptr = obj;
  int foundName = 0;
  while(ptr != NULL && foundName == 0){
    //Si le nom correspond
    if(strcmp(ptr->nom,nom) == 0){
      foundName = 1;
    }else{
        //Sinon prendre le lien pour le suivant
        ptr = ptr->next;
    }
  }

  //Si pas de correspondance
  if(foundName == 0)
    ptr = NULL;

  return ptr;

}

int supprimer_objet(char *nom){

  //Pointeur vers l'objet avant celui a supprimer
  Objet *beforeDeleted = obj;
  Objet *toDelete = obj;
  int foundObject = -1;
  int jumpCount = 0;

  //Trouver l'element a supprimer et le precedent
  while(foundObject == -1 && toDelete != NULL){
    if(strcmp(toDelete->nom,nom) == 0){
      foundObject = 0;
    }else{
      jumpCount++;
      beforeDeleted = toDelete;
      toDelete = toDelete->next;
    }
  }

  //Si on a trouver l'objet le supprimer
  if(foundObject == 0){
    //Rediriger le next vers le nouveau suivant (CAS PARTICULIER : suppression tete)
    if(jumpCount == 0){
      obj = toDelete->next;
    }else{
      beforeDeleted->next = toDelete->next;
    }
    //Liberer tout les emplacements FAT
    int toFreeIndex = toDelete->index;
    while(FAT[toFreeIndex] != 0xFFFE){

      int nextIndex = FAT[toFreeIndex];
      FAT[toFreeIndex] = 0xFFFF;
      toFreeIndex = nextIndex;

      freeblocks++;
    }

    //Liberere le dernier block
    FAT[toFreeIndex] = 0xFFFF;
    freeblocks++;

    //Liberer l'objet à supprimer
    free(toDelete);
  }

  return foundObject;
}

void supprimer_tout(){

  //Tant que il reste des fichiers
  while(obj != NULL){
    //Supprimer le premier
    supprimer_objet(obj->nom);
  }
}

int lire_objet(struct objet *o,char **data){
  int flag = 0;
  //Verifier que il existe des fichier
  if(obj == NULL || o == NULL)
    flag = -1;

  //Compter le nombre de block
  if(flag != -1){
    int blockTake = 1;
    int currentFatIndex = o->index;

    while(FAT[currentFatIndex] != 0xFFFE){
      currentFatIndex = FAT[currentFatIndex];
      blockTake++;
    }

    //Creer le buffer pour recevoir
    char *retrived = malloc(sizeof(char)*(BLOCSIZE*blockTake));

    //Recuperer les donnees
    int currentDataFatIndex = 0;
    int currentDataIndex = 0;
    currentFatIndex = o->index;
    char currentChar = ' ';

    while(currentChar != '\0'){
      currentChar = volume[BLOCSIZE*currentFatIndex + currentDataFatIndex];
      retrived[currentDataIndex] = currentChar;
      currentDataIndex++;
      currentDataFatIndex++;

      //Si on atteint la fin d'un block
      if(currentDataFatIndex == BLOCSIZE){
        currentFatIndex = FAT[currentFatIndex];
        currentDataFatIndex = 0;
      }
    }

    //Retourner la chaine
    *data = retrived;
  }

  return flag;
}

/* DEBUG */
void printBlock(int id){
  printf("Block %d : ",id);
  for(int i = 0 ;i<BLOCSIZE;i++){
    printf("%c",volume[(id*BLOCSIZE)+i]);
  }
  printf("\n");
}
