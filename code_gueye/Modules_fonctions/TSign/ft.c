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
 
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ft.h"
#define pi 4*atan(1.0)
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr

void four1(double data[], unsigned long nn, int isign)
{
    unsigned long n,mmax,m,j,istep,i;
    double wtemp,wr,wpr,wpi,wi,theta;
    double tempr,tempi;
    n=nn << 1;
    j=1;
    for (i=1; i<n; i+=2)
    {
        if (j > i)
        {
            SWAP(data[j],data[i]);
            SWAP(data[j+1],data[i+1]);
        }
        m=nn;
        while (m >= 2 && j > m)
        {
            j -= m;
            m >>= 1;
        }
        j += m;
    }
    mmax=2;
    while (n > mmax)
    {
        istep=mmax << 1;
        theta=isign*(2*pi/mmax);
        wtemp=sin(0.5*theta);
        wpr = -2.0*wtemp*wtemp;
        wpi=sin(theta);
        wr=1.0;
        wi=0.0;
        for (m=1; m<mmax; m+=2)
        {
            for (i=m; i<=n; i+=istep)
            {
                j=i+mmax;
                tempr=wr*data[j]-wi*data[j+1];
                tempi=wr*data[j+1]+wi*data[j];
                data[j]=data[i]-tempr;
                data[j+1]=data[i+1]-tempi;
                data[i] += tempr;
                data[i+1] += tempi;
            }
            wr=(wtemp=wr)*wpr-wi*wpi+wr;
            wi=wi*wpr+wtemp*wpi+wi;
        }
        mmax=istep;
    }
}