#include <stdio.h>
#include <string.h> /* For memset*/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "supp_chaine_caractere.h"



/*Ce module supprime une chîne de caractères contenu dans une chaine de caractères*/
void supp_chaine_caractere (char *s, char chr)/* supprime x dans la chaine */
{

  {
   int i, j = 0;
   for ( i = 0; s[i] != '\0'; i++ ) /* 'i' moves through all of original 's' */
   {
      if ( s[i] != chr )
      {
         s[j++] = s[i]; /* 'j' only moves after we write a non-'chr' */
      }
   }
   s[j] = '\0'; /* re-null-terminate */
}
}
