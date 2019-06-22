/*PROGRAM diagnostic by sound/vibration analysis
  NAME:
    ft

  AUTHOR:
    AMADOU GUEYE (AG), Année pré-doctorale, amadou.gueye@univ-psl.fr
 
  PURPOSE:
 
 
  CALLING SEQUENCE: CALL coordonnees
   
  DESCRIPTION:
 
  ARGUMENTS:
 
 
 
  RETURN VALUE:
 
    0 if everything worked fine
  
 OUTPUT:
 
 
  MODIFICATION HISTORY:
    
  

  Compilation: 
    gcc -c -ansi -Wall -Wextra fft.c -lm
    gcc fft.o fft.o -o
    

  Execution:
    ./ft
*/

void four1(double data[], unsigned long nn, int isign);
