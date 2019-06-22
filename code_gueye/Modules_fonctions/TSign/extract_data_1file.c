#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "extract_data_1file.h"
#include "wave.h"
#include "viderbuffer_supp_saut_ligne.h"
#define NCAR 100
#define nbfile 2

struct HEADER header;


void extract_data_1file(double *data_uniq, signed long taille_uniq, int nfile, int nbech_uniq, char *fichieraudio_uniq, char **file, int p){
 
    
    int i=0;
    /*int k=0;*/
    unsigned int fs;
    int puissance=0; /*variable qui serta incrémentée qui correspond a l indice de la puissance de 2 pour la taille du tableau*/
    /*int n=0;*/
    float tempstotal=0;
    char tp[NCAR]="";
    char dir_E[100]="";
    char dir_uniq[100]="";
    /*char tmp_uniq[NCAR]="";*/
    /*char tm_uniq[NCAR]="";*/
    /*char tmp1_uniq[NCAR]="";*/
    /*char tmp2_uniq[NCAR]="";*/
    char tmp3_uniq[NCAR]="";
    /*FILE *fic=NULL;*/
    /*FILE *dat=NULL;
    FILE *dat1=NULL;*/
    FILE *wav=NULL;
    FILE *fich_uniq=NULL; /*fichier info*/

    
    
    printf("On lit le fichier unique\n");
  
    /*On ouvre le fichier unique*/
    /*strcpy(tm_uniq,"dataFILE_uniq");
    fic = fopen(tm_uniq,"w");
   	if (fic != NULL)
    {
      printf("le fichier %s est ouvert\n",tm_uniq);  	
    }
    else
    {
      printf("Erreur à l'ouverture du fichier %s\n",tm_uniq);
    }*/
  
  /*-----ouverture du wave-------*/
    supp_saut_ligne (fichieraudio_uniq) ;
    printf("fich_unique = %s\n",fichieraudio_uniq);
      
    for(i = 0;i<nbfile;i++){
      file[i] = fichieraudio_uniq;
    }
    
    /*struct HEADER header;*/
    wave(fichieraudio_uniq,file,nfile);
    /*strcpy(tmp_uniq, "dataFILE_unique");*/
    fs = header.sample_rate;
    /*printf("Amadousix\n");*/

    /*-------------------------determiner la taille des tableaux---------------------*/
    
    /*nbech=(header.bytes_in_data/header.bytes_by_capture);*/
     nbech_uniq=(8 * header.data_size) / (header.channels * header.bits_per_sample);/*TODO A revoir*/
      puissance = 0; /*On initialise puissance*/
      /*fprintf (fic,"le fichier audio %s contient %d echantillons\n",fichieraudio_uniq,nbech_uniq);*/
      printf ("le fichier audio %s contient %d echantillons\n",fichieraudio_uniq,nbech_uniq);
      while (nbech_uniq>taille_uniq)
      {
        taille_uniq=taille_uniq*2;
        puissance=puissance+1;
      }
      p = puissance;
      printf ("nombre de points traites : 2^%d=%ld\n",p,taille_uniq);/*j'ajoute par fourier*/
    /*tempstotal=(1./header.frequency)*taille;*/
      tempstotal=(1./header.sample_rate)*taille_uniq;
      printf ("temps total : %f s\n",tempstotal);
      taille_uniq=taille_uniq*2;
      printf("taille = %ld\n",taille_uniq);
    /*------------------fin de determiner la taille des tableaux---------------------*/      
      /*float *data2=NULL;*/ /*tableau de l'onde temporelle*/
      data_uniq=malloc( (taille_uniq) * sizeof(double));
    /*printf("cou\n");*/
      if (data_uniq == NULL)
      {
        exit(0);
      }
    
      /*---------------------initialisation des tableaux dynamiques--------------------*/
      for(i=0; i<taille_uniq; i++)
      {
        data_uniq[i]=0;
      }
      
      /*fichier data des echantillons*/
      strcpy(tmp3_uniq,  "E_");
      strcat(tmp3_uniq, fichieraudio_uniq);
      strcat(tmp3_uniq,  "_data.dat");
      strcpy(dir_uniq,  "../../Fichiers_générés/TSign/Fichier_dat/");
      strcat(dir_uniq, tmp3_uniq);
      wav = fopen(dir_uniq, "r");
      
      fgets(tp,46,wav);
      viderbuffer(wav);
      
      for(i=0;i<nbech_uniq;i++)
      {
        /*lecture des echantillons et enregistrement dans le tableau*/
        /*data[2*i]=value;*/
        fscanf(wav,"%lf", &data_uniq[2*i]);
        /*fprintf(fic,"%lf\n",data_uniq[2*i]);*/
        viderbuffer(wav);
        /*i++;*/
      }
    
      printf("\nnombre d'echantillons lus : %d\n",i);
      printf("nombre de valeurs sauvegardees %d\n",i);
      printf("i = %d et nbech otenus lorsqu'on determinait taille = %d\n",i,nbech_uniq);
    
    /*  k=0;
      for (k=0; k<i; k++)
      {*/
        /*fprintf(dat1,"%lf\n", data_uniq[2*k]);*/
/*permet de sauvegarder le tableau contenant que les données dans le fichier onde1.dat pour vérification manuelle des données*/
      /*}*/
      /*for (i=0; i<taille_uniq; i++)
      {
        fprintf(dat,"%lf\n", data_uniq[i]);*/
/*permet de sauvegarder le tableau dans le fichier onde.dat pour vérification manuelle des données*/
     /* }*/
      
      /*On ouvre le fichier fich_uniq où on stocke les information taille_uniq, nbech_unq et tmp2_uniq*/
      
      strcpy(dir_E,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
      strcat(dir_E, "E_Info.dat");
      fich_uniq = fopen(dir_E,"w");
      fprintf(fich_uniq,"%ld\n%d\n%s\n%d\n%d",taille_uniq, nbech_uniq, tmp3_uniq, fs, p);
      
      fclose(wav);
      wav = NULL;
      /*fclose(dat);
      fclose(dat1);*/
      /*fclose(fic);
      fic = NULL;*/
      fclose(fich_uniq);
      fich_uniq = NULL;
     
  }/*fin de else*/
