#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

/* Ici, on est oblig� d'utiliser la notation struct xxx,
car la structure s'auto-r�f�rence!*/
typedef struct node {
		char data ;
		struct node *link ;
		} Lnode ;

/* Trouver le dernier element de la liste */
Lnode* findLastElement(Lnode *first){
		Lnode *lastElement = first;
		while(lastElement->link != NULL)
			lastElement = lastElement->link;

		return lastElement;
}

/* Insertion en "t�te de liste" */
void insertionTete(Lnode **ph,char item){

		//Definition de l'objet
		Lnode *toInstert = NULL;
		toInstert = malloc(sizeof(Lnode));
		toInstert->data = item;

		//Pointer le link du nouvel objet vers l'élément suivant
		toInstert->link = (*ph);

		//Pointer la tete sur le nouvel element
		*ph = toInstert;
	}

/* Insertion en "queue de liste" */
void insertionQueue(Lnode **ph,char item)	{

		//Defnition du nouvel objet
		Lnode *toInstert = NULL;
		toInstert = malloc(sizeof(Lnode));
		toInstert->data = item;

		//Pointer le link sur NULL (Fin de liste)
		toInstert->link = NULL;

		//Faire pointer l'element précedent sur l'element à inserer
		Lnode *ptrLast = findLastElement(*ph);
		ptrLast->link = toInstert;

	}

/* Suppression en "t�te de liste" */
void suppressionTete(Lnode **ph){

		//Pointeur temporaire vers l'element a supprimer
		Lnode *toDelete = NULL;
		toDelete = *ph;

		//Rediriger la tete sur le nouveau premier
		*ph = toDelete->link;

		//Supprimer l'element
		free(toDelete);

	}

/* Suppression en "Queue" de liste" */
void suppressionQueue(Lnode **ph){

		//Pointeur vers le nouveau dernier element
		Lnode *newLastElement = *ph;
		while((newLastElement->link)->link != NULL)
			newLastElement = newLastElement->link;

		//Liberer le dernier element
		free(newLastElement->link);

		//Rediriger le lien du dernier element sur NULL (Fin de la liste)
		newLastElement->link = NULL;

	}

/* Proc�dure d'affichage de la liste. Ne doit pas �tre modifi�e!!! */
void listeAffiche(Lnode * ptr){
	if ( NULL == ptr )
		printf("Liste vide!") ;
	else
		printf("Contenu de la liste : ") ;
	while ( NULL != ptr ) 	{
		printf("%c ",ptr->data);
		ptr = ptr->link ;
		}
	printf("\n") ;
	}

/* Programme principal. Ne doit pas �tre modifi�!!! */
int main(void) {
	Lnode *tete = NULL ;

	listeAffiche(tete) ;
	insertionTete(&tete,'a');
	listeAffiche(tete) ;
	insertionTete(&tete,'c') ;
	listeAffiche(tete) ;
	insertionQueue(&tete,'t') ;
	listeAffiche(tete) ;
	insertionQueue(&tete,'s') ;
	listeAffiche(tete) ;
	suppressionTete(&tete) ;
	listeAffiche(tete) ;
	suppressionTete(&tete) ;
	listeAffiche(tete) ;
	suppressionQueue(&tete) ;
	listeAffiche(tete) ;
	suppressionTete(&tete) ;
	listeAffiche(tete) ;

   return EXIT_SUCCESS;
   }
