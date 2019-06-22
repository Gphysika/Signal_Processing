#include <stdio.h>
#include <string.h> /* For memset*/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "viderbuffer_supp_saut_ligne.h"

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
  /* La fonction suivante permet de remplacer '\n' par '\0' dans une chaine de caractere  */
  /* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void supp_saut_ligne ( char *chaine )
{
   char *c = NULL ;
   if( (c = strchr(chaine,'\n')) != NULL )
   {
      *c = '\0' ;
   }
   return ;
}

/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* La fonction suivante permet de vider le tampon associe a un flot                     */
/*    -> l'adresse du flot est donnee en argument par l'utilisateur                     */
/*    -> la fonction ne renvoie rien. Aucune erreur n'est prise en compte               */
/* ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
void viderbuffer ( FILE *stream )
{
   char cc = 0 ;
   while (cc != '\n' && cc != EOF)
   {
      cc = fgetc(stream) ;
   }
   return ;
}
