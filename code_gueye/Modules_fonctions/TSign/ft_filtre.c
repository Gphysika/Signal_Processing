#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "ft_filtre.h"
#include "ft.h"
#include "wave.h"
#define pi 4*atan(1.0)

struct HEADER header;
void ft_filtre(double *data0, signed long taille, int nbech, char *tmp){

int i = 0;
double time_k = 0.0f; /*le temps correspondant à fsample_k*/
double fsample_k = 0.0f; /*les fréquences en abscisses dans la FFT*/
short value=0;
double *filtre=NULL;
float filtre_freq, filtre_large;
char fichieraudio_out[100];
/*FILE *ptr;*/
char tmp2[100]="";
char tmp3[100]="";
char dir2[100]=""; /*Indique le chemin du répertoire où il faut mettre le fichier*/
char dir3[100]="";
char dir_out[100]="";

FILE *dat2=NULL;
FILE *dat3=NULL;
FILE *waveout=NULL;

    printf("voici la taille du tableau filtre = %ld\n", taille);
    printf("voici le nombre d'échantillon = %d\n", nbech);
    /*---------------------ft-----------------------------------------*/
    
    four1(data0,taille/2,1); /*J'ai modifié data en data1*/
    /*---------------------fin de ft------------------------------------------------*/
    strcpy(tmp2,tmp);
    strcat(tmp2,"_ft_filtre_magnitude-avant-filtre.dat");
    strcpy(dir2,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir2, tmp2);
    dat2 = fopen(dir2,"w");/*fichier.dat des fft*/
    
 for(i=0; i<taille/2; i++)
    {
      fsample_k = (2.*i*header.sample_rate/taille);
      if(fsample_k == 0){
        /*fprintf (dat2,"%f %f\n",10*log10((data[2*i]/taille)*(data[2*i]/taille)),(2.*i*header.frequency/taille));*/
        fprintf (dat2,"%f\t%f\t%f\n",10*log10((data0[2*i]/taille)*(data0[2*i]/taille)),fsample_k,fsample_k);
        
      }
      else{
        time_k = 1.0f/(2.*i*header.sample_rate/taille);
        fprintf (dat2,"%f\t%f\t%f\n",10*log10((data0[2*i]/taille)*(data0[2*i]/taille)),fsample_k,time_k);
      }
    }

    /*---------------------filtre--------------------------------------*/
    printf("frequence du filtre (Hz) ?");
    scanf("%f",&filtre_freq);
    printf("largeur du filtre ?");
    scanf("%f",&filtre_large);
    /*filtre_freq=(int)(filtre_freq*taille/header.frequency);*/
    filtre_freq=(int)(filtre_freq*taille/header.sample_rate);
    /*filtre_large=(int)(filtre_large*taille/header.frequency);*/
    filtre_large=(int)(filtre_large*taille/header.sample_rate);

    printf("filtre_freq=%f,filtre_large=%f\n",filtre_freq,filtre_large);
    
    /*double *filtre=NULL;*/ /*tableau calculé par la transformée de fourrier*/
      filtre=malloc((taille) * sizeof(double));
      printf("coucu");
      if (filtre == NULL)
      {
        exit(0);
      }
      
      for(i=0; i<taille; i++)
      {
        filtre[i]=0;
      }

    for(i=0; i<taille; i++)
    {
        if(i>(filtre_freq-(filtre_large/2))&&i<(filtre_freq+(filtre_large/2)))
        {
            filtre[i]=0.5*(1-cos(2*pi*(i-(filtre_freq-(filtre_large/2)))/(filtre_large-1)));
        }
        else if (i>(taille-1-filtre_freq-(filtre_large/2))&&i<(taille-1-filtre_freq+(filtre_large/2)))
        {
            filtre[i]=0.5*(1-cos(2*pi*(i-(taille-1-filtre_freq-(filtre_large/2)))/(filtre_large-1)));
        }
        else
        {
            filtre[i]=0;
        }
    }

    for(i=0; i<taille/2; i++)
    {
        data0[2*i]=filtre[2*i]*data0[2*i];
        data0[2*i+1]=filtre[2*i]*data0[2*i+1];
    }
    
    strcpy(tmp3,tmp);
    strcat(tmp3,"_ft_filtre_magnitude-après-filtre.dat");
    strcpy(dir3,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir3, tmp3);
    dat3=fopen(dir3,"w");/*fichier.dat */
    
    
    for(i=0; i<taille; i++)
    {
        /*fprintf (dat3,"%f %f\n",data[i],(1.*i*header.frequency/taille));*/
        fprintf (dat3,"%lf\t%lf\n",data0[i],(1.*i*header.sample_rate/taille));
    }
    /*--------------fin de filtre----------------------------------------------------*/

    /*---------------------ft inverse-----------------------------------------------*/
    four1(data0,taille/2,-1);
    /*--------------fin de fft inverse-----------------------------------------------*/


  

    /*---------------------creation fichier wave-------------------------------------*/
    /*printf ("entrer le nom du fichier audio filtre pour l enregistrement :");
    scanf("%s", fichieraudio_out);*/
    strcpy(fichieraudio_out,tmp);
    strcat(fichieraudio_out,"_tf_audio-filtre.wav");
    printf ("nom du fichier : %s\n", fichieraudio_out);
    strcpy(dir_out,  "../../Fichiers_générés/TSign/Audio_MT/");
    strcat(dir_out, fichieraudio_out);
    waveout = fopen(dir_out,"wb"); /*ouverture du fichier wave*/
    if (waveout != NULL)
    {
       	printf("le fichier waveout est ouvert\n");  	
    }
    else
    {
        printf("Erreur à l'ouverture du fichier waveout \n");
    }
    
    fwrite(&header,sizeof(header),1,waveout);
    for(i=0; i<nbech; i++)
    {
        value=/*fabs*/((data0[2*i]*2)/taille); /*on peut noter la normalisation /taille de la tf inverse*/
        fwrite(&value,(header.bits_per_sample)/8,1,waveout);
    }
    /*---------------------fin de creation fichier wave------------------------------*/
    
    free(filtre);
    filtre = NULL;
    fclose(waveout);
    fclose(dat2);
    fclose(dat3);
}
