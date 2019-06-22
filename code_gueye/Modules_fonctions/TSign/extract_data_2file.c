#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <math.h>
#include "extract_data_2file.h"
#include "wave.h"
#include "viderbuffer_supp_saut_ligne.h"
#define NCAR 100
#define nbfile 2

struct HEADER header;
/*boucle if pour selon le nombre de fichier entré*/
void extract_data_2file(double *data0, double *data00, signed long taille1, signed long taille2, int nfile, int nbech1, int nbech2, char **fichieraudio, char **file, int p1, int p2){

  int i=0;
  /*int k=0;*/
  int l=0;
  unsigned int fs1;
  unsigned int fs2;
  /*int p=0;*/
  /*int m=0;*/
  int puissance=0; /*variable qui serta incrémentée qui correspond a l indice de la puissance de 2 pour la taille du tableau*/
  char *filenam=NULL;
  char tp[NCAR]="";
  /*char tmp[NCAR]="";
  char tm[NCAR]="";
  char tmp1_file1[NCAR]="";*/
  /*char tmp2_file1[NCAR]="";*/
  char tmp3_file1[NCAR]="";
  /*char tmp1_file2[NCAR]="";*/
  /*char tmp2_file2[NCAR]="";*/
  char tmp3_file2[NCAR]="";
  char dir1[100]="";
  char dir2[100]="";
  char dir3[100]="";
  float tempstotal=0;
  /*FILE *fic=NULL;
  FILE *dat=NULL;
  FILE *dat1=NULL;*/
  FILE *wav=NULL;
  FILE *fich_1_2=NULL;/*contient les informations*/


    printf("On lit les deux fichiers entrés\n");
    for(l=0; l<nfile; l++){
    /*---------------------ouverture du wave----------------------------------------*/
      supp_saut_ligne (fichieraudio[l]) ;

      filenam=fichieraudio[l];
    
      printf("fich = %s\n",fichieraudio[l]);
    
    /*struct HEADER header;*/
      wave(filenam,file,nfile);
      /*strcpy(tmp, "dataFILE_1");*/
    /*On stocke les fréquences d'échantillonnage des fichiers*/
      if(l==0){
        fs1 = header.sample_rate;
      }
      else{
        fs2 = header.sample_rate;
      }
    /*struct wavfile header;commented 10/04/2018*/ /*creation du header*/
      /*printf("coucou\n");*/
      printf("%s\n",header.riff);
      printf("%d\n",header.overall_size);
      printf("%s\n",header.wave);
      printf("%s\n",header.fmt_chunk_marker);
      printf("%d\n",header.length_of_fmt);
      printf("%d\n",header.format_type);
      printf("%d\n",header.channels);
      printf("%d\n",header.sample_rate);
      printf("%d\n",header.byterate);
      printf("%d\n",header.block_align);
      printf("%d\n",header.bits_per_sample);
      printf("%s\n",header.data_chunk_header);
      printf("%d\n",header.data_size);
    
    
    /*---------------------fin de ouverture du wave---------------------------------*/



    /*---------------------lecture de l entete et enregistrement dans header--------*/
    
    /*----------------fin de lecture de l entete et enregistrement dans header-------*/
    
    /*-----------------Ouverture du fichier test.txt----*/
      if(l==0){
        /*strcpy(tmp, "dataFILE_1");*/ 
   	    /*fic = fopen(tmp,"w");
   	    if (fic != NULL)
        {
       	  printf("le fichier %s est ouvert\n",tmp);  	
        }
        else
        {
          printf("Erreur à l'ouverture du fichier %s\n",tmp);
        }*/
	   /*-----------------fin d'ouverture du fichier test.txt----*/
	

       /*---------------------determiner la taille des tableaux---------------------*/
    
       /*nbech=(header.bytes_in_data/header.bytes_by_capture);*/
        nbech1=(8 * header.data_size) / (header.channels * header.bits_per_sample);/*TODO A revoir*/
        /*fprintf (fic,"le fichier audio %s contient %d echantillons\n",fichieraudio[l],nbech1);*/
        printf ("le fichier audio %s contient %d echantillons\n",fichieraudio[l],nbech1);
        while (nbech1>taille1)
        {
          taille1=taille1*2;
          puissance=puissance+1;
        
        }
        p1 = puissance;
        printf ("nombre de points traites : 2^%d=%ld\n",p1,taille1);/*j'ajoute par fourier*/
        /*tempstotal=(1./header.frequency)*taille;*/
        tempstotal=(1./header.sample_rate)*taille1;
        printf ("temps total : %f s\n",tempstotal);
        taille1=taille1*2;
        printf("taille = %ld\n",taille1);
       /*------------------fin de determiner la taille des tableaux---------------------*/

    
       /*---------------------creation des tableaux dynamiques--------------------------*/
    
       /*if(l==1){*/
       /*float *data=NULL;*/ /*tableau de l'onde temporelle*/
        data0=malloc( (taille1) * sizeof(double));
       
        if (data0 == NULL)
        {
          exit(0);
        }
        /*printf("cou\n");*/
    /*double *filtre=NULL;*/ /*tableau calculé par la transformée de fourrier*/
/*      filtre=malloc((taille) * sizeof(double));
      if (filtre == NULL)
      {
        exit(0);
      }*/
    
    
    /*---------------------initialisation des tableaux dynamiques--------------------*/
        for(i=0; i<taille1; i++)
        {
          data0[i]=0;
        /*filtre[i]=0;*/
        }
       /* printf("cou\n");
        strcpy(tmp1_file1, "ondeFILE_1");*/
        /*strcpy(tmp2_file1, "onde1FILE_1");*/
        /*strcpy(tmp3_file1, "checkdataFILE_1");*/
        strcpy(tmp3_file1,  "R_");
	    strcat(tmp3_file1, filenam);
		strcat(tmp3_file1,  "_data.dat");
		strcpy(dir1,  "../../Fichiers_générés/TSign/Fichier_dat/");
		strcat(dir1, tmp3_file1);
		wav = fopen(dir1, "r");
        /*printf("cou\n");
        dat=fopen(tmp1_file1,"w");*/ /*fichier data des echantillons*/
        /*printf("cou\n");*/
        /*dat1=fopen(tmp2_file1,"w");*/ /*fichier data contenant uniquement que les echantillons*/
        /*printf("cou\n");*/
        fgets(tp,46,wav);
        viderbuffer(wav);
        printf("cou\n");
        for(i=0;i<nbech1;i++)
        {
         /*lecture des echantillons et enregistrement dans le tableau*/
          fscanf(wav,"%lf", &data0[2*i]);
          /*fprintf(fic,"%lf\n",data0[2*i]);*/
          viderbuffer(wav);
        }
    
        printf("\nnombre d'echantillons lus : %d\n",i);
        printf("nombre de valeurs sauvegardees %d\n",i);
        printf("i = %d et nbech otenus lorsqu'on determinait taille1 = %d\n",i,nbech1);
    
        /*k=0;
        for (k=0; k<i; k++)
        {
          fprintf(dat1,"%lf\n", data0[2*k]);*/
/*permet de sauvegarder le tableau contenant que les données dans le fichier onde1.dat pour vérification manuelle des données et pour être utilisé dans la tf, fft..*/
       /* }
        for (i=0; i<taille1; i++)
        {
          fprintf(dat,"%lf\n", data0[i]);*/
/*permet de sauvegarder le tableau dans le fichier onde.dat pour vérification manuelle des données*/
       /* }*/
    
        fclose(wav);
        wav = NULL;
        /*fclose(dat);
        dat = NULL;
        fclose(dat1);
        dat1 = NULL;
        fclose(fic);
        fic = NULL;*/
     }
    /*On ouvre le fichier2*/
     else{  
       /*strcpy(tm,"dataFILE_2");
       fic = fopen(tm,"w");
   	   if (fic != NULL)
       {
       	 printf("le fichier %s est ouvert\n",tm);  	
       }
       else
       {
         printf("Erreur à l'ouverture du fichier %s\n",tm);
       }*/
	

    /*-------------------------determiner la taille des tableaux---------------------*/
    
    /*nbech=(header.bytes_in_data/header.bytes_by_capture);*/
       nbech2=(8 * header.data_size) / (header.channels * header.bits_per_sample);/*TODO A revoir*/
       puissance = 0; /*On initialise puissance*/
       /*fprintf (fic,"le fichier audio %s contient %d echantillons\n",fichieraudio[l],nbech2);*/
       printf ("le fichier audio %s contient %d echantillons\n",fichieraudio[l],nbech2);
       while (nbech2>taille2)
       {
         taille2=taille2*2;
         puissance=puissance+1;
       }
       p2= puissance;
       printf ("nombre de points traites : 2^%d=%ld\n",p2,taille2);/*j'ajoute par fourier*/
    /*tempstotal=(1./header.frequency)*taille;*/
       tempstotal=(1./header.sample_rate)*taille2;
       printf ("temps total : %f s\n",tempstotal);
       taille2=taille2*2;
       printf("taille = %ld\n",taille2);
    /*------------------fin de determiner la taille des tableaux---------------------*/
    
    /*float *data2=NULL;*/ /*tableau de l'onde temporelle*/
       data00=malloc( (taille2) * sizeof(double));
    /*printf("cou\n");*/
       if (data00 == NULL)
       {
         exit(0);
       }
    
    /*double *filtre2=NULL;*/ /*tableau calculé par la transformée de fourrier*/
      /*filtre2=malloc((taille2) * sizeof(double));
      if (filtre2 == NULL)
      {
        exit(0);
      }*/
    
    
    /*---------------------initialisation des tableaux dynamiques--------------------*/
       for(i=0; i<taille2; i++)
       {
         data00[i]=0;
         
       }
    
       /*strcpy(tmp1_file2, "ondeFILE_2");
       strcpy(tmp2_file2, "onde1FILE_2");*/
       /*strcpy(tmp3_file2, "checkdataFILE_2");*/
       strcpy(tmp3_file2,  "O_");
       strcat(tmp3_file2, filenam);
       strcat(tmp3_file2,  "_data.dat");
       /*dat=fopen(tmp1_file2,"w"); *//*fichier data des echantillons*/
       /*dat1=fopen(tmp2_file2,"w"); *//*fichier data contenant uniquement que les echantillons*/
       strcpy(dir2,  "../../Fichiers_générés/TSign/Fichier_dat/");
       strcat(dir2, tmp3_file2);
       wav = fopen(dir2, "r");
    /*while( fread(&value,(header.bits_per_sample)/8,2,wav) )*//* 1 changed to 2*/
    /*while( fread(&value,header.byterate*8,2,wav) )*//* 1 changed to 2*/
       fgets(tp,46,wav);
       viderbuffer(wav);
       for(i=0;i<nbech2;i++)
       {
        /*lecture des echantillons et enregistrement dans le tableau*/
        /*data[2*i]=value;*/
         fscanf(wav,"%lf", &data00[2*i]);
         /*fprintf(fic,"%lf\n",data00[2*i]);*/
         viderbuffer(wav);
        /*i++;*/
       }
    
       printf("\nnombre d'echantillons lus : %d\n",i);
       printf("nombre de valeurs sauvegardees %d\n",i);
       printf("i = %d et nbech otenus lorsqu'on determinait taille = %d\n",i,nbech2);
    
       /*k=0;
       for (k=0; k<i; k++)
       {
         fprintf(dat1,"%lf\n", data00[2*k]);*/
/*permet de sauvegarder le tableau contenant que les données dans le fichier onde1.dat pour vérification manuelle des données*/
       /*}
       for (i=0; i<taille2; i++)
       {
         fprintf(dat,"%lf\n", data00[i]);*/
/*permet de sauvegarder le tableau dans le fichier onde.dat pour vérification manuelle des données*/
      /* }*/
    
       fclose(wav);
       wav = NULL;
       /*fclose(dat);
       dat=NULL;
       fclose(dat1);
       dat1 = NULL;
       fclose(fic);
       fic = NULL;*/
     }/*end else dans for l=*/
    
    }/*end for l=*/
  
    
    /*On ouvre le fichier fich_uniq où on stocke les informations taille1, taille2, nbech1, nbech2,
    tmp2_file1, tmp2_file2*/
    strcpy(dir3,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir3, "R-O_Info.dat");
    fich_1_2 = fopen(dir3,"w");
    fprintf(fich_1_2,"%ld\n%ld\n%d\n%d\n%s\n%s\n%d\n%d\n%d\n%d",taille1, taille2, nbech1, nbech2, tmp3_file1, tmp3_file2,fs1,fs2,p1,p2);
    
    fclose(fich_1_2);
    fich_1_2 = NULL;
  
}/*fin void*/

