#include <stdio.h>
#include <string.h> /* For memset*/
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include <err.h>
#include "stft.h"
#include "libmfcc.h"
#include "fft.h"
#include "viderbuffer_supp_saut_ligne.h"
#define pi 4*atan(1.0)
#define MAX_FILE_NAME 100
#define BIZ_EPSILON 1E-21 /* Max error acceptable*/

void stft(char *tmp, unsigned int fs, int analyse){
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
	int window = 0;
	double SampleRate = 0.0f;/*freq à laquelle a été rééchantillonné le sigaal*/
	double fsample_k = 0.0f; /*les fréquences en abscisses dans la FFT*/
	double time_k = 0.0f; /*le temps correspondant à fsample_k*/
	float beta = 0.0f;
	float frameOffest=0.0f;
	
   /* Determine which MFCC coefficient to compute*/
/*	unsigned int coeff;*/
   /* Holds the value of the computed coefficient*/
/*	double mfcc_result;*/
	double magn;
	double emphFac = (double)0.97;
	double *spectrum = NULL;
	double **framedSample = NULL;
	double *resample = NULL;
	double *resample_window = NULL;
	double *w=NULL;
	double *re = NULL;
	double *im = NULL;
	char wind[30];
   /* Pointer to the sample data file*/
    char tmp0[100]="";
    /*char tmp1[100]="";
    char tmp2[100]="";*/
    char tmp3[100]="";
    char tmp4[100]="";
    char dir0[100]="";
    char dir3[100]="";
    char dir4[100]="";
    char c;             /* To store a character read from file */
	FILE *sampleFile = NULL;
	FILE *fich = NULL;
	/*FILE *fich1 = NULL;
	FILE *fich2 = NULL;*/
	FILE *fich3 = NULL;
	FILE *fich4 = NULL;
    FILE *fp = NULL;
 
    
    printf("\n********** ANALYSE DU SIGNAL PAR FENETRAGE : SHORT TIME FOURIER TRANSFORM (STFT) **********\n\n");
    if(analyse == 1){
      char dir[100]="";
      strcpy(dir,  "../../Fichiers_générés/TSign/Fichier_dat/");
      strcat(dir, tmp);
      fp = fopen(dir, "r");
      /* Check if file exists */
      if (fp == NULL)
      {
        printf("Could not open file %s\n", tmp);
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
 
    sampleN = count;
    spectrum = malloc((sampleN) * sizeof(double));
    if (spectrum == NULL)
      {
        exit(0);
      }
    /*On initialise preemphase*/
	for(i=0;i<sampleN;i++){
	 /* preemphase[i] = 0.0f;*/
	  spectrum[i] = 0.0f;
	}
    
	/*signout = malloc((sampleN) * sizeof(double));
    if (signout == NULL)
    {
        exit(0);
    }*/
	
	if(fs  < 16000)
	{
		frameLen = 256;
	}
	else 
	{/*16000*/
		frameLen = 512;
	}
    frameOffest= frameLen/(3*1.0f);
    pframeN = (count - frameLen) / frameOffest;
	
	
	
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
	
	printf("coi\n");
	/*prepreocessing, on extrait les données dans spectrum et on fait préemphaser le signal*/
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
    strcpy(tmp0,tmp);
    strcat(tmp0,"_stft_reech.dat");
    strcpy(dir0,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir0,tmp0);
  
	/*frameOffest is non overlapping samples*/
	/*double **framedSample=NULL;*/
	fich = fopen(dir0,"w");
	pframeN = (sampleN - frameLen) / frameOffest;
	printf("cou1 pframeN = %d\n",pframeN);
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
	fich = NULL;
	
	printf("pframeN*frameLen = %d\n",pframeN*frameLen);
	/*On stocke les données contenues dans le fichier reechantillon_stft.txt dans le tableau resample*/
	resample = malloc((pframeN*frameLen) * sizeof(double));
	resample_window = malloc((frameLen) * sizeof(double));
	
	if (resample == NULL)
    {
      exit(0);
    }
    if (resample_window == NULL)
    {
      exit(0);
    }
    
    fich = fopen(dir0,"r");
    for(i=0;i< (pframeN*frameLen);i++)
    {
        /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(fich,"%lf", &resample[i]);
        viderbuffer(fich);
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
     
     
     /*perform the window function requsted by the user*/
     n = frameLen;
     w=malloc( (n) * sizeof(double));
     
     /*On initialise w*/
     for(i=0;i<frameLen;i++){
       w[i] = 0.0f;
     }
     
     /*On choisit la fenêtre à appliquer*/
    /*for(i=0;i<(pframeN*frameLen);i++){
		 resample[i]= resample[i] * (float)(0.5 - 0.46*cos(2*pi*i/(frameLen -1)));
	}*/
    
    /*get the type of window to use on time domain*/   
    printf("***1.***  square window \n");   
    printf("***2.***  Bartlett window \n");   
    printf("***3.***  Hanning window \n");   
    printf("***4.***  Hamming window \n");   
    printf("***5.***  Blackman window \n");   
    printf("***6.***  Gaussian window \n");
    printf("***7.***  Welch window \n");
    printf("***8.***  Triangular window \n");
    printf("***9.***  Kaiser window \n");
    /*printf("please chose the style of window(1,2,3,4,5,6,7,8,9):");   
    scanf("%d",&window);   */
    
    /*printf("window = %d\n", window);*/
    for(l=1;l<=9;l++)
    {
      
      window = l;
      if(window==1)strcpy(wind,"SW");   /*dans le but d'écrire wind dans un fichier*/
      if(window==2)strcpy(wind,"BaW");   
      if(window==3)strcpy(wind,"HanW");   
      if(window==4)strcpy(wind,"HamW");   
      if(window==5)strcpy(wind,"BlW");   
      if(window==6)strcpy(wind,"GW");
      if(window==7)strcpy(wind,"WeW");
      if(window==8)strcpy(wind,"TW");
      if(window==9)strcpy(wind,"KW");
     
      switch(window)   
      {   
        case 1:   
            for(i=0;i<=n-1;i++)   
                w[i]=1.0f;   
            break;   
        case 2:   
            for(i=0;i<=(n-1)/2;i++)   
                w[i]=2.0f*i/(n-1);   
            for(i=(n-1)/2+1;i<=n-1;i++)   
                w[i]=2.0f-2.0f*i/(n-1);   
            break;   
        case 3:   
            for(i=0;i<=n-1;i++)   
                w[i]=(1.0f-cos(2.0f*pi*i/(n-1)))/2.0f;   
            break;   
        case 4:   
            for(i=0;i<=n-1;i++)   
                w[i]=1.0f*0.54-1.0f*0.46*cos(2*pi*i/(n-1));   
            break;   
        case 5:   
            for(i=0;i<=n-1;i++)   
                w[i]=1.0f*0.42-1.0f*0.5*cos(2*pi*i/(n-1))+1.0f*0.08*cos(4*pi*i/(n-1));   
            break;   
        case 6:   
            for(i=0;i<=n-1;i++)   
                w[i]=1.0f/exp(18*((n-1)/2.0f-i)*((n-1)/2.0f-i)/(n*n));   
            break;
        case 7:   
            for(i=0;i<=n-1;i++)   
                w[i]=1 - pow((i-1.0f*0.5*(n-1))/(1.0f*0.5*(n+1)),2) ;   
            break;
        case 8:   
            for(i=0;i<=n/2;i++)   
                w[i]=2.0f*i/n;   
            for(i=n/2+1;i<=n-1;i++)   
                w[i]=2.0f*(n-i)/n;   
            break;
        case 9:
            beta = 1.0f*3.384;
            af_window_kaiser(n, w, beta);
            break;
        default:   
            printf("error option!");   
            break;   
      }
      /*printf("nICE\n");*/
      /*On applique la fenêtre choisie*/
      strcpy(tmp4,tmp);
      strcat(tmp4,"_stft_reech_");
      strcat(tmp4,wind);
      strcat(tmp4,".dat");
      strcpy(dir4,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
      strcat(dir4, tmp4);
      fich4 = fopen(dir4,"w");
      for(i = 0, k =0; i < pframeN;i++)
	  {
		
		for(j = 0; j < frameLen;j++)
		{
		   resample_window[j]= resample[k++] * w[j];
		   fprintf(fich4,"%lf\n",resample_window[j]);
		}
		k = k + 1;
	
	  }
	  fclose(fich4);
	  fich4 = NULL;
	
	  /*On rouvre le fichier fich4 et on stocke les valeurs dans resample*/
	  fich4 = fopen(dir4,"r");
      for(i=0;i< (pframeN*frameLen);i++)
      {
        /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(fich4,"%lf", &resample[i]);
        viderbuffer(fich4);
      }  
     
     /*On doit faire la transformée de fourier dans chaque fenêtre*/
/*     fft(re,im,p,+1);*/
     /*strcpy(tmp1,tmp);
     strcat(tmp1,"_verif_rech_stft.txt");
     strcpy(tmp2,tmp);
     strcat(tmp2,"_stft_fenetre.txt");*/
      strcpy(tmp3,tmp);
      strcat(tmp3,"_stft_magn_");
      strcat(tmp3,wind);
      strcat(tmp3,".dat");
      strcpy(dir3,  "../../Fichiers_générés/TSign/Fichier_dat/");
      strcat(dir3,tmp3);
     
     /*fich1 = fopen(tmp1,"w");*//*pour vérifier si les échantillons ont été écrits*/
    /* fich2 = fopen(tmp2,"w");*/
      fich3 = fopen(dir3,"w");
     
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
		    /*fprintf(fich2,"%lf\t%lf\t%lf\t%lf\n",re[j],im[j],fsample_k,fsample_k);*/
		     magn = sqrt(re[j]*re[j] + im[j]*im[j])/(1.0f*frameLen);
		     fprintf(fich3,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",re[j],im[j],magn,10*log10(magn),fsample_k,fsample_k);/*/P_i(k) = (1/N)*|S_i(k)|**2*/
		   }
		   else{
		     time_k = 1.0f/(1.0f*fsample_k);
		    /*fprintf(fich2,"%lf\t%lf\t%lf\t%lf\n",re[j],im[j],fsample_k,time_k);*/
		     magn = sqrt(re[j]*re[j] + im[j]*im[j])/(1.0f*frameLen);
		     fprintf(fich3,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",re[j],im[j],magn,10*log10(magn),fsample_k,time_k);/*/P_i(k) = (1/N)*|S_i(k)|**2*/
		   }
		 }
		/*fprintf(fich,"\n\n");*/
	  }
	 /*printf("enfin\n");*/
	}
	
	fclose(sampleFile);
	sampleFile = NULL;
	fclose(fich);
	fich = NULL;
	fclose(fich4);
	fich4 = NULL;
	/*fclose(fich1);
	fich1 = NULL;
	fclose(fich2);
	fich2 = NULL;*/
	fclose(fich3);
	fich3 = NULL;
	/*free(spectrum);*/
	free(resample);
	resample = NULL;
}


double besselizero(double x)
{
  double temp;
  double sum   = 1.0;
  double u     = 1.0;
  double halfx = x/2.0;
  int      n     = 1;

  do {
    temp = halfx/(double)n;
    u *=temp * temp;
    sum += u;
    n++;
  } while (u >= BIZ_EPSILON * sum);
  return sum;
}  


/*
// Kaiser
//
// n window length
// w buffer for the window parameters
// b beta parameter of Kaiser window, Beta >= 1
//
// Beta trades the rejection of the low pass filter against the
// transition width from passband to stop band.  Larger Beta means a
// slower transition and greater stop band rejection.  See Rabiner and
// Gold (Theory and Application of DSP) under Kaiser windows for more
// about Beta.  The following table from Rabiner and Gold gives some
// feel for the effect of Beta:
//
// All ripples in dB, width of transition band = D*N where N = window
// length
//
// BETA    D       PB RIP   SB RIP
// 2.120   1.50  +-0.27      -30
// 3.384   2.23    0.0864    -40
// 4.538   2.93    0.0274    -50
// 5.658   3.62    0.00868   -60
// 6.764   4.32    0.00275   -70
// 7.865   5.0     0.000868  -80
// 8.960   5.7     0.000275  -90
// 10.056  6.4     0.000087  -100
*/
void af_window_kaiser(int n, double* w, double b)
{
  double tmp;
  double k1  = 1.0f/besselizero(b);
  int	   k2  = 1 - (n & 1);
  int      end = (n + 1) >> 1;
  int      i;

  /* Calculate window coefficients*/
  for (i=0 ; i<end ; i++){
    tmp = (double)(2*i + k2) / ((double)n - 1.0);
    w[end-(1&(!k2))+i] = w[end-1-i] = k1 * besselizero(b*sqrt(1.0 - tmp*tmp));
  }
  
}   
