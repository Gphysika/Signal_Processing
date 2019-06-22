#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "fft.h"
#include "call_fft.h"
#define EPSILON 1E-31

void call_fft(double *re, double *im, double *data, signed long taille, int p, char *tmp, unsigned int freq){
  
  int i=0;
  int k=0;
  double magn = 0.0f;
  double fsample_k = 0.0f; /*les fréquences en abscisses dans la FFT*/
  double time_k = 0.0f; /*le temps correspondant à fsample_k*/
  char tmp1[100]="";
  char tmp2[100]="";
  char tmp3[100]="";
  char dir1[100]="";
  char dir2[100]="";
  char dir3[100]="";
  FILE *fichier=NULL;
  FILE *fichier1=NULL;
  FILE *fichier2=NULL;
  
    /*--------------deuxième fft-----------------------------------------------*/
    /*double re=NULL; tableau de la partie réelle de fft*//*TODO*/
    printf("voici le tableau des re ei im, taille=%ld\n",taille);
    re=malloc( (taille/2) * sizeof(double));
    if (re == NULL)
    {
        exit(0);
    }
    
    /*double im=NULL;*/ /*tableau de la imaginaire de tf*//*TODO*/
    im=malloc( (taille/2) * sizeof(double));
    if (im == NULL)
    {
        exit(0);
    }
    
    /*---------------------initialisation des tableaux dynamiques--------------------*/
    for(i=0; i<taille/2; i++)
    {
        re[i]=0;
        im[i]=0;
    }
    
    /*---------------------On remplit les parties réels et imaginaires--------------------*/
    strcpy(tmp1,tmp);
    /*strcat(tmp1,"_recup.dat");*/
    printf("Amadou voici %s\n", tmp1);
    strcat(tmp1,"_onde-time-freq.dat");
    strcpy(dir1,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir1, tmp1);
    fichier1 = fopen(dir1,"w");
    for (k=0; k<taille/2; k++)
    {
        re[k] = data[2*k];
        im[k] = data[2*k+1];
        fsample_k = 1.0f*i*freq/(1.0f*taille);
        if(fsample_k == 0){
          fprintf(fichier1,"%lf\t%lf%lf\t%lf\n",re[k],im[k],fsample_k,fsample_k);
        }
        else{
          time_k = 1.0f/(1.0f*fsample_k);
          fprintf(fichier1,"%lf\t%lf%lf\t%lf\n",re[k],im[k],fsample_k,time_k);
        }
    }
    
    /*On ouvre le fichier FFT.dat*/
    strcpy(tmp2,tmp);
    strcpy(tmp3,tmp);
    
    strcat(tmp3,"_FFT.dat");
    strcpy(dir3,  "../../Fichiers_générés/TSign/Fichier_dat/");
    strcat(dir3, tmp3);
    fichier = fopen(dir3,"w");
    
    if (fichier != NULL)
    {
       	printf("le fichier _FFT.dat est ouvert\n");  	
    }
    else
    {
        printf("Erreur à l'ouverture du fichier _FFT.dat \n");
    }
    /*Fin de l'ouverture du fichier FFT.dat*/
    
    /*on appelle le module fft*/
    fft(re,im,p,+1);
    printf("Transformation:\n");
    for(i=0; i<taille/2; i++){
      fsample_k = 2.*i*freq/(1.0f*taille);
      if(fsample_k == 0){
	    magn = sqrt(re[i]*re[i] + im[i]*im[i])/(1.0f*taille);
        fprintf(fichier,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",re[i],im[i],magn,10*log10(magn),fsample_k,fsample_k);
      }
      else{
        time_k = 1.0f/(1.0f*fsample_k);
	    magn = sqrt(re[i]*re[i] + im[i]*im[i])/(1.0f*taille);
        fprintf(fichier,"%lf\t%lf\t%lf\t%lf\t%lf\t%lf\n",re[i],im[i],magn,10*log10(magn),fsample_k,time_k);
      }
    }
    
    /*FFT inverse*/
    strcat(tmp2,"_FFT-inverse-onde-time-freq.dat");
    strcpy(dir2,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir2, tmp2);
    fichier2 = fopen(dir2,"w");
    fft(re,im,p,-1); 
    printf("FFT inverse:\n");
    for(i=0; i<taille/2; i++){
      fsample_k = 2.*i*freq/(1.0f*taille);
      if(fsample_k == 0){
        /*printf(" %lf %lf i\n",re[i],im[i]);*/
        fprintf(fichier2,"%lf\t%lf\t%lf\t%lf\n",re[i],im[i],fsample_k,fsample_k);/*on enregistre dans FFT.dat*/
      }
      else{
        time_k = 1.0f/(1.0f*fsample_k);
        /*printf(" %lf %lf i\n",re[i],im[i]);*/
        fprintf(fichier2,"%lf\t%lf\t%lf\t%lf\n",re[i],im[i],fsample_k,time_k);/*on enregistre dans FFT.dat*/
      }
    }
    /*system("PAUSE");*/
    /*--------------fin du deuxième fft--------------------------------------*/
    
    
    /*Fermeture des fichiers*/
    
    fclose(fichier);
    fichier = NULL;
    fclose(fichier1);
    fichier1 = NULL;
    fclose(fichier2);
    fichier2 = NULL;
}
