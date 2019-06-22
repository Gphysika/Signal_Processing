/*
 * libmfcc.c - Code implementation for libMFCC
 * Copyright (c) 2010 Jeremy Sawruk
 *
 * This code is released under the MIT License. 
 * For conditions of distribution and use, see the license in LICENSE
 */

#include <stdio.h>
#include <string.h> /* For memset*/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <err.h>
#include <math.h>
#include "libmfcc.h"

/* 
 * Computes the specified (mth) MFCC
 *
 * spectralData - array of doubles containing the results of FFT computation. This data is already assumed to be purely real
 * samplingRate - the rate that the original time-series data was sampled at (i.e 44100)
 * NumFilters - the number of filters to use in the computation. Recommended value = 48
 * binSize - the size of the spectralData array, usually a power of 2
 * m - The mth MFCC coefficient to compute
 *
 */
 
 /*préemphaser le signal*/
 void preemphasize(double *sample, int sampleN)   
{     
   int i;   
   float emphFac = (float)0.97;
   
   
   for (i = sampleN - 1; i > 0; --i) 
   {   
        sample[i] = 1.0f*(sample[i] - emphFac * sample[i-1]);
		/* Sr(k) = e(k)(1 - S^-1) 一 H(z) = 1-0.9375z^-1
		//H(s)  = 1- 0.9375e^-s 
		//H(jw) = 1- 0.9375e^-jw */
    }   
    sample[0] = (float)(1.0 - emphFac) * sample[0];
    
    
}
 
 /*préprocessign of signal*/
 void preprocessing(double *sample, int sampleN, double *out)   
{   
    int i;
    FILE *fichier= NULL;
    
    fichier=fopen("preemphase.txt","w");
    printf("sampleN = %d\n",sampleN);
    for(i=0;i<sampleN;i++)    
        out[i]=sample[i];   
    /*if (m_dither) Dither(out, sampleN);  */ 
    preemphasize(out, sampleN);
    
    for(i=0;i<sampleN;i++){
       fprintf(fichier,"%lf\n",out[i]);
    }
       
    fclose(fichier);
	fichier = NULL;
      
   /* return 1;   */
}   
 
 double freqtoMel(double f)
{
	return 2596.0 * log10(1 + f / 700.0);
}
double meltoFreq(double m)
{
	return  700.0 * (pow(10, m / 2596.0) - 1);
}

/*
 * Trouvez la différence de m [beg, end) et placez-la après
 */
void deltaCoeff(double *m, int beg, int end)
{
	int i,j;
	j = end;
	m[j++] = 0.0;
	m[j++] = 0.0;
	for (i = beg + 2; i < end - 2; ++i,++j)
	{
		m[j] = 2.0 * (m[i + 2] - m[i - 2]) + m[i + 1] - m[i - 1];
		m[j] /= sqrt(2.0 * (1.0 + 2.0* 2.0));
	}
	m[j++] = 0.0;
	m[j++] = 0.0;
}

/****filte mel
F: spectre de puissance nécessitant un filtrage dans chaque fenêtre

            n: longueur

SampleRate: taux d'échantillonnage

k: nombre de filtres

*************/
double* MelScaleTriangleFilters(double *F,int n,int SampleRate,int k)
{
	double *d,
		  detaSamplefreq = (double)SampleRate / n ,
		  detaMel = SampleRate / 2  / (k + 1),
		  oMel    = 0,
		  cMel    = detaMel,
		  hMel    = detaMel * 2,
		  fMel = 0,f = 0;
	int i =0,j,midIndex;
	d = (double*)calloc(k,sizeof(double));
	for(j = 0;j < k && i  < n;++j)
	{
		d[j] = 0.0;
		while(fMel  <= hMel && i  < n)
		{
			if(fMel  <  cMel)
			{
				d[j] += F[i] *(fMel - oMel)/detaMel;
				midIndex = i;
			}
			else
			{
				d[j] += F[i] * (hMel - fMel) /detaMel;
			}
			++i;
			f += detaSamplefreq;
			fMel = (double)freqtoMel(f);
		}
		i = midIndex + 1 ;
		f = detaSamplefreq * (i-1);
		fMel = (double)freqtoMel(f);
		oMel += detaMel;
		cMel += detaMel;
		hMel += detaMel;
	}
	for(j = 0;j < TRIANGLE_Num;++j)
	{
		if(d[j])
			d[j] = (double)log10(d[j]) * 10;  
	}
	return d;

}


int Log2( int n ){ 
    /* log(n)/log(2) is log2. */ 
    return log( 1.0f*n ) / log( 1.0f*2 );
 }
