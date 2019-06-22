/*
 * example.c - example of using libMFCC
 * Written 2010 - J. Sawruk
 */

#include <stdio.h>
#include <string.h> /* For memset*/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <err.h>
#include "mfcc.h"
#include "libmfcc.h"
#include "fft.h"
#include "viderbuffer_supp_saut_ligne.h"
#define pi 4*atan(1.0)
#define MAX_FILE_NAME 100



void mfcc(char *tmp, unsigned int fs, int analyse)
{
	/* Read in sample data from sample.dat
	// sample.dat contains an 8192-point spectrum from a sine wave at 440Hz (A) in double precision
	// Spectrum was computed using FFTW (http://www.fftw.org/)
	// Data was not windowed (rectangular)*/

	/* Holds the spectrum data to be analyzed*/
	/*double spectrum[734265];*/
	int sampleN = 0;
	/*int frameLen = 256;*/
	/*double preemphase[734265] ;*/
	/*double *signout = NULL;*/
	/* Index counter - used to keep track of which data point is being read in*/
	int i = 0;
	int j= 0;
	int k=0;
	int n=0;
	int l = 0;
	int count = 0;
	int pframeN = 0;
	int frameLen = 0;/*nombre de point dans chaque segment*/
	/*int fs  fréquence d'échantillonnage du signal brute*/
	double SampleRate = 0.0f;/*freq à laquelle a été
	rééchantillonné le sigaal*/
	double fsample_k = 0.0f; /*les fréquences en abscisses dans la FFT*/
	double time_k = 0.0f; /*le temps correspondant à fsample_k*/
	float frameOffest= 0.0f;
	
   /* Determine which MFCC coefficient to compute*/
/*	unsigned int coeff;*/
   /* Holds the value of the computed coefficient*/
/*	double mfcc_result;*/
	double magn;
	double emphFac = (double)0.97;
/*	char tmp[100] ="";*/
	double *spectrum = NULL;
	double **framedSample = NULL;
	double **mfcc = NULL;
	double *resample = NULL;
	double *resample_hamming = NULL;
	double *re = NULL;
	double *im = NULL;
	double **F = NULL;
	/* Pointer to the sample data file*/
    char tmp0[100]="";
    /*char tmp1[100]="";
    char tmp2[100]="";*/
    char tmp3[100]="";
    char tmp4[100]="";
    char tmp5[100]="";
    char tmp6[100]="";
    char dir0[100]="";
    char dir3[100]="";
    char dir4[100]="";
    char dir5[100]="";
    char dir6[100]="";
    char c;  /* To store a character read from file */
   /* Pointer to the sample data file*/
	FILE *sampleFile = NULL;
	FILE *fich = NULL;
	/*FILE *fich1 = NULL;*/
	/*FILE *fich2 = NULL;*/
	FILE *fich3 = NULL;
	FILE *fich4 = NULL;
	FILE *fich5 = NULL;
	FILE *fich6= NULL;
	FILE *fichier= NULL;
   

	/* Open the sample spectrum data*/
	FILE *fp = NULL;
    
    
 
    /* Get file name from user. The file should be */
    /* either in current folder or complete path should be provided */
    printf("\n********** ANALYSE DU SIGNAL PAR FENETRAGE : MEL Coefficient (MFCC) **********\n\n");
    
    if(analyse == 1){
      char dir[100]="";
      strcpy(dir,  "../../Fichiers_générés/TSign/Fichier_dat/");
      strcat(dir, tmp);
      fp = fopen(dir, "r");
 
      /* Check if file exists */
      if (fp == NULL)
      {
        printf("Could not open file %s\n", tmp);
       /* return 0;*/
      }
      
      /* Extract characters from file and store in character c*/
      for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') /* Increment count if this character is newline */
            count = count + 1;
 
      /* Close the file */
      printf("The file %s has %d lines\n ", tmp, count);
    
      fclose(fp);
      fp = NULL;
    
	  sampleFile = fopen(dir,"r");
    }
    else{
      char dir1[100]="";
      strcpy(dir1,  "../../Données/TSign_AStat/Data_vibration/48k Drive End Bearing Fault Data/");
      strcat(dir1, tmp);
      fp = fopen(dir1, "r");
 
      /* Check if file exists */
      if (fp == NULL)
      {
        printf("Could not open file %s\n", tmp);
       /* return 0;*/
      }
      
      /* Extract characters from file and store in character c*/
      for (c = getc(fp); c != EOF; c = getc(fp))
        if (c == '\n') /* Increment count if this character is newline */
            count = count + 1;
 
      /* Close the file */
      printf("The file %s has %d lines\n ", tmp, count);
    
      fclose(fp);
      fp = NULL;
    
	  sampleFile = fopen(dir1,"r");
    }
 
	
	/*On alloue le tableau preemphase*/
	/*preemphase = malloc((sampleN) * sizeof(double));
    if (preemphase == NULL)
    {
        exit(0);
    }*/
    
    sampleN = count;
    if(fs  < 16000)
	{
		frameLen = 256;
	}
	else 
	{/*16000*/
		frameLen = 512;
	}
    frameOffest= frameLen/(3*1.0f);
    
    spectrum = malloc((sampleN) * sizeof(double));
    if (spectrum == NULL)
    {
        exit(0);
    }
    /*On initialise preemphase*/
	for(i=0;i<sampleN;i++){
	  spectrum[i] = 0.0f;
	}
    
	/*signout = malloc((sampleN) * sizeof(double));
    if (signout == NULL)
    {
        exit(0);
    }*/
	
	
	
	/* Initialize the spectrum*/
	/*memset(&spectrum, 0, sizeof(spectrum));*/
	/*memset(&preemphase, 0, sizeof(preemphase));*/
	/*memset(&signout, 0, sizeof(signout));*/
	
	/*On détermine le nombre de ligne dans le fichier de données*/
	
	/* Read in the contents of the sample file*/
	for(i=0;i<sampleN;i++)
    {
        /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(sampleFile,"%lf", &spectrum[i]);
        viderbuffer(sampleFile);
    }
    
	/* Close the sample file*/
	/*fclose(sampleFile);*/
	
	printf("i=%d\n",i);
	
	/*printf("coi\n");*/
	/*prepreocessing, on extrait les données dans spectrum et on le met
	dans un autre tableau puis on fait préemphaser le signal*/
  /*  preprocessing(spectrum,sampleN, preemphase);*/
	
	
   
   
   for (i = sampleN - 1; i > 0; --i) 
   {   
        spectrum[i] = 1.0f*(spectrum[i] - emphFac * spectrum[i-1]);
		/* Sr(k) = e(k)(1 - S^-1) 一 H(z) = 1-0.9375z^-1
		//H(s)  = 1- 0.9375e^-s 
		//H(jw) = 1- 0.9375e^-jw */
    }   
    spectrum[0] = (double)(1.0 - emphFac) * spectrum[0];
	
   /*framing*/
  
	/*frameOffest is non overlapping samples*/
	/*double **framedSample=NULL;*/
	strcpy(tmp0,tmp);
	strcat(tmp0,"_mfcc_reechantillon.dat");
	strcpy(dir0,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir0, tmp0);
	fich = fopen(dir0,"w");
	pframeN = (sampleN - frameLen) / frameOffest;
	printf("pframeN = %d\n",pframeN);
    framedSample = (double**)calloc(pframeN,sizeof(double*));
    
	for(i = 0, k =0; i < pframeN;i++)
	{
		framedSample[i] = (double*)calloc(frameLen,sizeof(double));
		for(j = 0; j < frameLen;j++)
		{

			framedSample[i][j] = spectrum[k++];
			fprintf(fich,"%lf\n",framedSample[i][j]);
		}
		k = k - frameLen + frameOffest; /*quand la boucle for(j = 0; j < frameLen;j++)  aura terminé, k= framelen*/
		/*fprintf(fich,"\n\n");*/
	}
	
	fclose(fich);
	
	printf("pframeN*frameLen = %d\n",pframeN*frameLen);
	/*On stocke les données contenues dans le fichier reechantillon.txt*/
	resample = malloc((pframeN*frameLen) * sizeof(double));
	resample_hamming = malloc((frameLen) * sizeof(double));
	
	if (resample == NULL)
    {
      exit(0);
    }
    
    if (resample_hamming == NULL)
    {
      exit(0);
    }
    
    fich = fopen(dir0,"r");
    strcpy(tmp5,tmp);
	strcat(tmp5,"_mfcc_reechantillon_hamming.dat");
	strcpy(dir5,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir5, tmp5);
    fich6 = fopen(dir5,"w");
    
    for(i=0;i< (pframeN*frameLen);i++)
    {
        /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(fich,"%lf", &resample[i]);
        viderbuffer(fich);
    }
    
    /*On applique la fenêtre de hamming dans chaque fenêtre*/
    for(i = 0, k =0; i < pframeN;i++)
	{
		
		for(j = 0; j < frameLen;j++)
		{
		   resample_hamming[j]= resample[k++] * (float)(0.5 - 0.46*cos(2*pi*j/(frameLen -1)));
		   fprintf(fich6,"%lf\n",resample_hamming[j]);
		}
		k = k + 1;
	
	}
	fclose(fich6);
	
	/*On rouvre le fichier fich6 et on stocke les valeurs dans resample*/
	fich6 = fopen(dir5,"r");
    for(i=0;i< (pframeN*frameLen);i++)
    {
        /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(fich6,"%lf", &resample[i]);
        viderbuffer(fich6);
    }
    
    /*On applique la FFt dans chaqu fenêtre*/
     /*tableau de la partie réelle de tf*//*TODO*/
    re=malloc( (frameLen) * sizeof(double));
    if (re == NULL)
    {
        exit(0);
    }
    
    /*double im=NULL;*/ /*tableau de la imaginaire de tf*//*TODO*/
    im=malloc( (frameLen) * sizeof(double));
    if (im == NULL)
    {
        exit(0);
    }
    
    /*---------------------initialisation des tableaux dynamiques--------------------*/
    for(i=0; i<frameLen; i++)
    {
        re[i]=0;
        im[i]=0;
    }
    
    /*---------------------On remplit les parties réels et
    imaginaires--------------------*/
  /*  for(i=0; i<(frameLen); i++)
    {
        re[i]=resample[i];
    }*/
    /*On appelle la fonction fft*/
     int p;
     p = Log2(frameLen );
     printf("p = %d\n",p);
     
     /*On doit faire la transformée de fourier dans chaque fenêtre*/
     strcpy(tmp3,tmp);
     strcat(tmp3,"_mfcc_fft_magnitude.dat");
     strcpy(tmp4,tmp);
     strcat(tmp4,"_mfcc_melfiltrebank.dat");
     strcpy(tmp5,tmp);
	 strcat(tmp5,"_mfcc.dat");
     /*fich1 = fopen(tmp1,"w");*/
    /* fich2 = fopen(tmp2,"w");*/
     
     strcpy(dir3,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
     strcat(dir3, tmp3);
     fich3 = fopen(dir3,"w");
     strcpy(dir4,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
     strcat(dir4, tmp4);
     fich4 = fopen(dir4,"w");
     strcpy(dir5,  "../../Fichiers_générés/TSign/Fichier_dat/");
     strcat(dir5, tmp5);
     fich5 = fopen(dir5,"w");
     
     SampleRate = fs/(1.0f*frameLen);
     for(i = 0, k =0; i < pframeN;i++)
	 {
		
		for(j = 0; j < frameLen;j++)
		{
			re[j] = resample[k++];
			/*fprintf(fich1,"%lf\n",re[j]);*/
		}
		k = k + 1;/*quand la boucle for(j = 0; j < frameLen;j++)  aura terminé, k= framelen-1*/
		fft(re,im,p,+1);
		
		for(j = 0; j < frameLen;j++)
		{
		  fsample_k = j*SampleRate/(frameLen);
		  /*time_k = 1.0f/(1.0f*fsample_k);*/
		  if(fsample_k == 0){
		    /*fprintf(fich2,"%lf %lf %lf\n",re[j],im[j],fsample_k);*/
		    magn = sqrt(re[j]*re[j] + im[j]*im[j])/(1.0f*frameLen);
		    fprintf(fich3,"%lf\t%lf\t%lf\t%lf\t%lf\n",re[j],im[j],magn,fsample_k,fsample_k);/*/P_i(k) = (1/N)*|S_i(k)|**2*//*TODO magn en db*/
		  }
		  else{
		    time_k = 1.0f/(1.0f*fsample_k);
		    magn = sqrt(re[j]*re[j] + im[j]*im[j])/(1.0f*frameLen);
		    fprintf(fich3,"%lf\t%lf\t%lf\t%lf\t%lf\n",re[j],im[j],magn,fsample_k,time_k);/*/P_i(k) = (1/N)*|S_i(k)|**2*//*TODO magn en db*/
		  }
		}
		/*fprintf(fich,"\n\n");*/
	}
    fclose(fich3);
     
     /*On calcule la magnitude dans chaque fenêtre*/
/*     for(i=0; i<(pframeN*frameLen); i++)
     {
       sqrt(re[i]*re[i] + im[i]*im[i])/(1.0f*frameLen);*//*P_i(k) = (1/N)*|S_i(k)|**2*/
    /* }*/
    
    /*On applique le filtre mel
     F: spectre de puissance nécessitant un filtrage dans chaque fenêtre

            n: longueur de la fenêtre
            SampleRate: taux d'échantillonnage ou fréquence à laquelle a été
            rééchantillonné le signal
            k: nombre de filtres
    
    */
    
    /*On stocke les données contenues dans le fichier reechantillon.txt*/
    
    fich3 = fopen(dir3,"r");
    F = (double**)calloc(pframeN,sizeof(double*));
    for(i=0;i< pframeN;i++)
    {
      F[i] = (double*)calloc(frameLen,sizeof(double));
      for(j = 0; j<frameLen;j++){
        /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(fich3,"%lf\t%lf\t%lf", &re[j], &im[j], &F[i][j]);
        viderbuffer(fich3);
      }
    }
     
    /*Le fichier pour stocker les fréquences mel*/
    strcpy(tmp6,tmp);
	strcat(tmp6,"_mfcc_magn-melfreq.dat");
	strcpy(dir6,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir6, tmp6);
	
	fichier = fopen(dir6,"w");
    /*On initialise n comme étént la taile de chque fenêtre*/
    n = frameLen;
    /*On initialise k comme étant le nombre de filtreà appliquer*/
    k = TRIANGLE_Num;
    double **d,
		  detaSamplefreq = (double)SampleRate / n ,
		  detaMel = SampleRate / 2  / (k + 1),
		  oMel    = 0,
		  cMel    = detaMel,
		  hMel    = detaMel * 2,
		  fMel = 0,f = 0;
	int midIndex;
	i = 0;
	d = (double**)calloc(pframeN,sizeof(double*)); /*ce tableau doit avoir pour taille le nbre de filtre*(nbre de segements ou nbre de fenêtre)*/
for(l=0;l< pframeN;l++){
  d[l] = (double*)calloc(k,sizeof(double));
	for(j = 0;j < k && i  < n;++j)
	{
		d[l][j] = 0.0;
		/*printf("i_in = %d\n",i);*/
		while(fMel  <= hMel && i  < n)
		{
			if(fMel  <  cMel)
			{
				d[l][j] += F[l][i] *(fMel - oMel)/detaMel;
				midIndex = i;
			}
			else
			{
				d[l][j] += F[l][i] * (hMel - fMel) /detaMel;
			}
			++i;
			f += detaSamplefreq;
			fMel = (double)freqtoMel(f);
			fprintf(fichier,"%lf %lf\n",d[l][j], fMel);
		}
		/*printf("ifin = %d\n",i);*/
		i = midIndex + 1 ;
		f = detaSamplefreq * (i-1);
		fMel = (double)freqtoMel(f);
		oMel += detaMel;
		cMel += detaMel;
		hMel += detaMel;
	}
	/*nombre de filtre mel à appliquer*/
	for(j = 0;j < TRIANGLE_Num;++j)
	{
		if(d[l][j])
			d[l][j] = (double)log10(d[l][j]) * 10;
		fprintf(fich4,"%lf\n",d[l][j]);
	}
    
}

    /*On applique le discret cosinus trans frame*/
    mfcc = (double**)calloc(pframeN,sizeof(double*));
    /*On initialise la tableau mfcc*/
    
for(l=0;l< pframeN;l++){
  mfcc[l] = (double*)calloc(DCT_Num+1,sizeof(double)); 
    for(j = 0;j < DCT_Num; ++j)
	{
		/*int k;*/
		mfcc[l][j] = 0;
		for(k =0;k < TRIANGLE_Num; ++k)
		{
			mfcc[l][j] += (double)cos(j * (k - 0.5) * pi / TRIANGLE_Num) * d[l][k];
		}
		
	}
}	

    

	/****Ajouer l'énergie logarithmique pour avoir DCT_Num + 1 coefficients******/
	/*mfcc[DCT_Num] = logEnergy;*/
for(l=0;l< pframeN;l++){
  mfcc[l][DCT_Num] = d[l][DCT_Num];
}

/* On stocke les mffcs dans un fichier *mfcc.dat*/
for(l=0;l< pframeN;l++){
  for(j=0;j<DCT_Num+1;j++){
		fprintf(fich5,"%lf\t%d\n",mfcc[l][j],l);
  }
}	

    fclose(fich5);
    /*mfccfinal*/
    double *mfccfinal;
    mfccfinal = malloc((pframeN*(DCT_Num+1)) * sizeof(double));
    /*On ouvre le fichier mfcc.txt et on staocke les onnées dans mfccfinal*/
    fich5 = fopen(dir5,"r");
    for(i=0;i<pframeN*(DCT_Num+1);i++){
      fscanf(fich5,"%lf",&mfccfinal[i]);
      viderbuffer(fich5);
    }


	deltaCoeff(mfccfinal,0, DCT_Num + 1);
	deltaCoeff(mfccfinal,DCT_Num + 1,2 *( DCT_Num + 1));
     
	/* Compute the first 13 coefficients*/
/*	for(coeff = 0; coeff < 13; coeff++)
	{
		mfcc_result = GetCoefficient(spectrum, 44100, 48, 128, coeff);
		printf("%i %f\n", coeff, mfcc_result);
	}*/
	/*getchar();*/
	fclose(fich);
	fich = NULL;
	fclose(fich6);
	fich6 = NULL;
	/*fclose(fich1);*/
	/*fich1 = NULL;
	fclose(fich2);
	fich2 = NULL;*/
	fclose(fich3);
	fich3 = NULL;
	fclose(fich4);
	fich4 = NULL;
	fclose(fich5);
	fich5 = NULL;
	fclose(fichier);
	fichier = NULL;
	fclose(sampleFile);
	sampleFile = NULL;
	/*free(spectrum);*/
	free(resample);
	resample = NULL;
	free(resample_hamming);
	resample_hamming = NULL;
	free(F);
	F = NULL;
	free(d);
	d = NULL;
	free(mfcc);
	mfcc = NULL;
	/*return 0;*/
}
