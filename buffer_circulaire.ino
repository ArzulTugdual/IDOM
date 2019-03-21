#include <stdio.h>
#include <malloc.h>
#include <conio.h>
#include <string.h>

typedef struct
{
      char *debut;
      char *fin;
      char *producteur;
      char *consommateur;
} CIRC;

CIRC * open_circ(int taille);
int get_circ(CIRC *B);
int put_circ(CIRC *B, int c);

void setup()

{
  CIRC * Buffer;
  int  c;
  char choix; // Variable qui contiendra le choix de l'opération à effectuer

  // Ouverture d'un buffer circulaire
  Buffer = open_circ(20);

  do
  {
    printf("\t\t\t   buffer circulaire\n\n\n");
    printf("\t      1. Ajouter 1 caractère\n");
    printf("\t      2. Lire un caractère\n");
    printf("\t      3. Quitter\n\n\n");
    printf("  Faire votre choix svpl ? ");

    fflush(stdin);
    scanf("%c",&choix);
    switch(choix)
    {
      case '1' :
      // Ajouter 1 caractère
        printf("Entrer un caractère : ");
        fflush(stdin);
        scanf("%c", &c);
        if(put_circ(Buffer, c)== EOF)
          printf("Le buffer est plein!\n");

      break;

      case '2' :
      // Lire un caractère
         c = (char)get_circ(Buffer);
         if( c != EOF)
          printf("Le caractère lu est : %c\n",c);
         else
          printf("Le buffer est vide!\n");
      break;

      case '3' : printf("Au revoir");break;

      default  : printf("Vérifier votre choix svpl ! \n");
    }
  }
  while( choix != '3' ) ;
}
CIRC * open_circ(int taille)
{
  CIRC * B;
  B = (CIRC *)malloc(taille);
  if(B == NULL)
  {
    return(EOF);
  }
  else
  {
    B->debut = (char *)B+(sizeof(CIRC));
    B->consommateur = (char *)B+(sizeof(CIRC));
    B->producteur = (char *)B+(sizeof(CIRC));
    B->fin = (char *)B+taille;

  }
  return(B);
}
int get_circ(CIRC *B)
{
  if(B->consommateur == B->producteur)
    return(EOF);
  if(++(B->consommateur)==B->fin)
    B->consommateur = B->debut;
  return (*(B->consommateur));

}
int put_circ(CIRC *B, int c)
{
  char *p;
  p=B->producteur + 1;
  if (p == B->fin)
  {
    p = B->debut;
  }
  if (p == B->consommateur)
  {
    return(EOF);
  }

      
     p = c;


  B->producteur = p;
  return (c);
}
