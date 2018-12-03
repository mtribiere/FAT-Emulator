#define BLOCSIZE 20
#define BLOCNUM 6
#define NAMELEN 256

typedef struct objet
{
char nom[NAMELEN];
unsigned int taille;
unsigned short auteur;
unsigned short index;
struct objet *next;
}Objet;

struct objet *obj  ;
char volume[BLOCSIZE * BLOCNUM ];
unsigned short FAT[BLOCNUM];
unsigned short freeblocks;


/**
\brief Cette fonction permet :
D'initialiser le tableau FAT en déclarant tous les blocs libres.
D'initialiser la variable freeblocks à BLOCNUM.
D'initialiser la variable obj
*/
/*
  initialiser toutes le valeurs des FAT à 0xFFFF
  initialiser freeblocks egal au nombre de block disponible
  declarer obj à NULL (memoire vide)

*/
void initialise_fat();


/**
\brief Cette fonction permet de rechercher un objet par son nom dans la liste chaînée décrivant les objets
\param nom nom de l'objet à rechercher
\return pointeur vers l'objet trouvé ou NULL sinon.
*/
/*
  Verifier pour tout les objets si le nom ne correspond pas
  Si il ne correspod pas avec le nom prendre le lien vers le suivant
*/
struct objet *rechercher_objet(char *nom);

/**
\brief Cette fonction permet de créer un objet en vérifiant qu'aucun objet n'a le même nom dans la liste (pas triée par nom)
si possible, de réserver des blocs dans le tableau FAT et de copier les données (data) dans ces blocs.
mettre à jour la variable freeblocks
\param nom nom de l'objet
\param auteur proprietaire de l'objet
\param taille la taille de l'objet
\param data les données à copier
*/
/*
  Verifier que aucun autre fichier n'a le meme nom
    copier les data dans le bon nombre de block et mettre a jour le FAT
    Mettre à jour la varible freeblocks
    ajouter le fichier avec ses infos dans la liste obj
  retourner un pointer vers les metadonées (NULL si erreur)

*/
struct objet *creer_objet(char *nom, unsigned short auteur,unsigned int taille, char *data);

/**
\brief  Cette fonction permet de supprimer un objet trouvé par son nom, de libérer les blocs dans le tableau FAT, et de mettre à jour la variable freeblocks
\param nom
\return -1 si erreur, 0 sinon.
*/
/*
  Chercher l'objet avec ce nom et son precedent (CAS PARTICULIER : suppression en tete)
  Si trouver le supprimer et retourner la valeur adequat
  Mettre a jour tout les block de FAT
  Mettre a jour la variable freeblocks
*/
int supprimer_objet(char *nom);


/**
\brief Cette fonction permet :
De supprimer l'ensemble des objets
De liberer l'ensemble des blocs dans le tableau FAT
De modifier la variable freeblocks
*/
/*
  Tant que obj n'est pas NULL (tant que on a des fichiers)
    Supprimer l'objet en tete avec son nom et la fonction supprimer_objet
*/
void supprimer_tout();

/** POUR LES PLUS RAPIDES ..................** BONUS ** BONUS ** BONUS **
\brief Cette fonction permet :
De lire le contenu d'un objet et de le copier dans une structure de données allouée dynamiquement
Attention à la taille !!!!!!!!!!!!!!
\param nom nom de l'objet
\return -1 si erreur, 0 sinon.
*/

int lire_objet(struct objet *o,char **data);

/* Fonction qui affiche un block (DEBUG)*/
void printBlock(int id);
