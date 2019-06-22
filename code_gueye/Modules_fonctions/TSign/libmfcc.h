/*
 * libmfcc.h - Header for libMFCC
 * Copyright (c) 2010 Jeremy Sawruk
 *
 * This code is released under the MIT License. 
 * For conditions of distribution and use, see the license in LICENSE
 */

#pragma once

#define PI 3.14159265358979323846264338327
#define _MFCC_H_
#define TRIANGLE_Num (20)/*nombre de filtre mel à appliquer*/
#define DCT_Num (12)/*nombre de coeff à calculer*/
#define MFCC_DIMENSION (39) /*(12+1)*3*/
#define N 512
#define WAVE_SAMPLE_RATE 44100 /*TODO*/

/*Pré-emphaser le signal*/
void preemphasize(double *sample, int sampleN);

/*préprocessing*/
void preprocessing(double *sample, int sampleN, double *out);

/*freq to mel and mel to freq*/
double freqtoMel(double f);
double meltoFreq(double m);

/*
Trouvez la différence de m [beg, end) et placez-la après
*/
void deltaCoeff(double *m, int beg, int end);

/*Log de base 2*/
int Log2( int n );
