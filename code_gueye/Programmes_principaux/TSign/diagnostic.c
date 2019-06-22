/*PROGRAM diagnostic by sound/vibration analysis
  NAME:
    diagnostic

  AUTHOR:
    AMADOU GUEYE (AG), Année pré-doctorale, amadou.gueye@univ-psl.fr
 
  PURPOSE:
 
 
  CALLING SEQUENCE: wave, call_fft, fft, mfcc, libmfcc, stft, ft, ft_filtre, extract_data_1file, extract_data_2file, viderbuffer_supp_saut_ligne
   
  DESCRIPTION:
 
  ARGUMENTS:
 
 
 
  RETURN VALUE:
 
    0 if everything worked fine
  
 OUTPUT:
 
 
  MODIFICATION HISTORY:
    change into fprintf(dat,"%lf\n", data[2*i]);
    int k=0;
    change for (i=0; i<taille; i++) line 378 : for (i=0; i<taille; i++){fprintf(dat,"%lf\n", data
    [i])} is chnged to for (k=0; k<i; k++){fprintf(dat,"%lf\n", data[2*k])};
     changed theta=isign*(6.28318530717959/mmax) by intrduce 2*pi;
    Commented void four1() (28/03/2018)

  Compilation: 
    gcc -c -ansi -Wall -Wextra diagnostic.c -lm
    gcc diagnostic.o diagnostic.o -o
    

  Execution:
    ./diagnostic


On distingue 2 parties dans un fichier de format .wav : l’entête et la partie DATA. L’entête mesure 56 octets et contient les informations sur le format utilisé pour encoder le son (nombre d’octets par point échantillonné, nombre de canaux, fréquence d’échantillonnage, longueur du fichier…).
La partie DATA commence à l’octet 57 et contient toutes les valeurs des points échantillonnés. Comment ouvrir un fichier WAVE dans un programme [C] ?
Il nous faut créer une structure (WAVFILE dans notre programme) rigoureusement identique à celle de l’entête d’un fichier WAVE.
Charger le fichier dans le programme avec la commande « fopen ».
Dans le programme notre fichier audio est de format .wav. On remarque aussi l’argument « rb » (read binary) dans la commande « fopen ». Le « b » est ajouté pour permettre au compilateur [C] de Windows de faire la distinction entre un fichier « TXT » et un fichier de données brutes. Lecture de l’entête et initialisation du Header par la commande « fread ».
« fread » est une fonction permettant la lecture de données brutes (binaires) d’un fichier dans un programme [C]. Il y a 4 paramètres à indiquer :
-La variable où les données seront enregistrées (tableau, structure ou variable simple)
-Taille des données à lire (en nombre d’octets)
-Nombre de fois que doit être effectuée la lecture
-Fichier à lire
Dans notre cas le programme va lire une fois le fichier deWAV. Sa lecture se fera sur la taille du HEADER (correspondant à celle de l’entête) et les données lues seront enregistrées dans le HEADER. 
Le HEADER ainsi rempli contient toutes les caractéristiques de notre fichier de format .wav, notamment la fréquence d’échantillonnage (header.frequency), le nombre de canaux (header.channels), le nombre d’octets par échantillon (header.bytes_by_capture), le nombre d’octets dans la partie DATA (header.bytes_in_data).
Les fichiers .wav utilisés seront des mono (header.channels = 1).
On peut alors déterminer le nombre d’échantillons contenus dans la partie DATA (=header.bytes_in_data / header.bytes_by_capture).
Pour la suite de notre programme, « taille » correspondra à la puissance de 2 supérieure au nombre total d’échantillons du fichier .wav lu.
Création d’un tableau dynamique qui contiendra les échantillons.
On utilisera un tableau de type malloc, car la taille du tableau ne pourra être déterminée qu’après exécution du programme.
Ce tableau est une matrice à 2 dimensions de 2 colonnes et « taille » lignes.
Il s’agit d’un tableau de nombres complexes, car nous verrons par la suite que l’algorithme de TF utilisé dans nos autres programmes d'analyse nécessite un tableau de cette forme. Enfin nous allons remplir ce tableau de nouveau avec la fonction « fread ».
Enfin ce tableau sera enregistré dans un fichier data
*/

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include <assert.h>
#include "../../Modules_fonctions/TSign/wave.h"
#include "../../Modules_fonctions/TSign/ft.h"
#include "../../Modules_fonctions/TSign/fft.h"
#include "../../Modules_fonctions/TSign/call_fft.h"
#include "../../Modules_fonctions/TSign/libmfcc.h"
#include "../../Modules_fonctions/TSign/mfcc.h"
#include "../../Modules_fonctions/TSign/stft.h"
#include "../../Modules_fonctions/TSign/ft_filtre.h"
#include "../../Modules_fonctions/TSign/extract_data_1file.h"
#include "../../Modules_fonctions/TSign/extract_data_2file.h"
#include "../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.h"
#include "../../Modules_fonctions/TSign/supp_chaine_caractere.h"
#define SWAP(a,b) tempr=(a);(a)=(b);(b)=tempr
#define pi 4*atan(1.0)
#define NCAR 100
#define nbfile 2


/*struct wavfile*/ /*définit la structure de l entete d un wave*/
/*{
    char        id[4];  */        /*doit contenir "RIFF"*/
 /*   int         totallength;*/        /*taille totale du fichier moins 8 bytes*/
 /*   char        wavefmt[8]; */    /*doit etre "WAVEfmt "*/
  /*  int         format; */            /*16 pour le format PCM*/
  /*  short       pcm;     */         /*1 for PCM format*/
   /* short       channels;  */       /*nombre de channels*/
 /*   int         frequency;   */       /*frequence d echantillonage*/
  /*  int         bytes_per_second; */  /*nombre de bytes par secondes*/
 /*   short       bytes_by_capture; */  /*nombre de bytes par echantillon*/
  /*  short       bits_per_sample;*/  /*nombre de bits par echantillon*/
  /*  char        data[4];  */      /*doit contenir "data"*/
  /*  int         bytes_in_data; */     /*nombre de bytes de la partie data*/
/*};*/



/*On appelle la structure HEADER*/
struct HEADER header;

/*Début du programme principal*/
int main(/*int argc, char *argv[]*/)
{


    int i=0;
    int nfile= 0;
    int type_analyse = 0;
    char **fichieraudio = NULL;
    char fichieraudio_uniq[NCAR];
    char analyse[NCAR];
    char **file;
   
    /*-------------------------------------Analyse sonore------------------------------------------*/
    
    /****************************** selection des fichiers audio **********************/
    /**** allocation des tableaux  de caractères file et fichieraudio qui vont contenir le nom des fichiers audio ****/
    file = calloc(2, sizeof(char));
    if(file == NULL)
    {
        fprintf(stderr, "Erreur");
        exit(EXIT_FAILURE);
    }
    
    for(i = 0; i < 2; i++)
    {
        file[i] = calloc(1, sizeof(char));
 
        if(file == NULL)
        {
            fprintf(stderr, "Erreur");
            exit(EXIT_FAILURE);
        }
    }
    
    fichieraudio = calloc(2, sizeof(char));
    if(fichieraudio == NULL)
    {
        fprintf(stderr, "Erreur");
        exit(EXIT_FAILURE);
    }
    
    for(i = 0; i < 2; i++)
    {
        fichieraudio[i] = calloc(1, sizeof(char));
 
        if(fichieraudio == NULL)
        {
            fprintf(stderr, "Erreur");
            exit(EXIT_FAILURE);
        }
    }
    
    /**** Choisir le type d'analyse sonore ou vibratoire ****/
    printf("Choisissez le type d'analyse à effectuer : \n"); 
    printf("***1.***  Analyse sonore \n");   
    printf("***2.***  Analyse vibratoire \n");
    scanf("%d",&type_analyse);
    
    /**** On copie le "type d'analyse" dans la variable "analyse" afin de visualiser à l'écran ****/
    if(type_analyse==1)strcpy(analyse,"Analyse sonore");
    if(type_analyse==2)strcpy(analyse,"Analyse vibratoire");
    
    /**** ----Condition if(type_analyse == 1) permet d'orienter de choisir une analyse sonore ----****/
  if(type_analyse == 1){   
    printf("Vous avez choisi de faire une %s\n",analyse);
    
    /**** On demande à l'utilisateur d'entre le nombre fichier.wav à lire ****/
    printf ("entrer le nombre de fichier audio à analyser :\n");
    scanf("%d", &nfile);
    
    /**** On fait une condition pour demander à l'utilisateur d'entrer le(s) nom(s) du (des) fichie r(s) ****/
    if(nfile == 2){
    
      unsigned int fs1;
      unsigned int fs2;
      int p1=0;                       /*pour la transformée de Fourier le fichier1 doit avoir 2^p1 points, où p1 est un entier à déterminer*/
      int p2=0;                       /*pour la transformée de Fourier le fichier2 doit avoir 2^p2 points, où p2 est un entier à déterminer*/
      int nbech1=0;                   /*nombre de points lus dans le fichier1*/
      int nbech2=0;                   /*nombre de points lus dans le fichier1*/
      signed long taille1=1;          /*variable qui correspondra par la suite à la longueur du tableau (puissance de 2)*/
      signed long taille2=1;          /*variable qui correspondra par la suite à la longueur du tableau (puissance de 2)*/
      char tmp2_file1[NCAR];
      char tmp2_file2[NCAR];
      char dir_1_2[NCAR];
      double *data0=NULL;             /*tableau de l'onde temporelle contenue dans le fichier1.wav*/
      double *data00=NULL;            /*tableau de l'onde temporelle contenue dans le fichier2.wav*/
      double *re=NULL;                /*partie réelle du tableau pour faire la tf*/
      double *im=NULL;                /*partie du tableau pour faire image de la tf*/
      FILE *fich_1_2=NULL;            /*Contient les info taille1, taille2, nbech1, nbech2, tmp2_file1, tmp2_file2*/
      FILE *dat2=NULL;                /*pour ouvrir le fichier tmp2_file1*/
      FILE *dat3=NULL;                /*pour ouvrir le fichier tmp2_file2*/
      
      
      
      /**** On donne le nom des fichiers audio à analyser ****/
      printf ("entrer le nom du fichier audio n°1 a analyser :\n");
      scanf("%s", fichieraudio[0]);
      printf ("nom du fichier 1 : %s\n", fichieraudio[0]);
      file[0]=fichieraudio[0];
      printf("file[0] = %s\n",file[0]);
  
    
      printf ("entrer le nom du fichier audio n°2 a analyser :\n");
      scanf("%s", fichieraudio[1]);
      printf ("nom du fichier 2 : %s\n", fichieraudio[1]);
      file[1]=fichieraudio[1];
      printf("file[1] = %s\n",file[1]);
      
      /**** On appelle le module extarct_data_2file ****/
      extract_data_2file(data0, data00, taille1, taille2, nfile, nbech1, nbech2, fichieraudio, file, p1, p2);
      
      /**** On récuppère taille1, taille2, nbech1, nbech2, tmp2_file1, tmp2_file2 ****/
      strcpy(dir_1_2,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
      strcat(dir_1_2, "R-O_Info.dat");
      fich_1_2 = fopen(dir_1_2,"r");
      fscanf(fich_1_2,"%ld\n%ld\n%d\n%d\n%s\n%s\n%d\n%d\n%d\n%d",&taille1, &taille2, &nbech1, &nbech2, tmp2_file1,tmp2_file2,&fs1,&fs2,&p1,&p2);
      
      /**** Allocation des tableaux des ondes temporelles ****/
      data0=malloc( (taille1) * sizeof(double));
      data00=malloc( (taille2) * sizeof(double));
      
      if (data0 == NULL)
      {
        exit(0);
      }
      if (data00 == NULL)
      {
        exit(0);
      }
    
      /**** initialisation des tableaux dynamiques ****/
      for(i=0; i<taille1; i++)
      {
        data0[i]=0;
      }
      for(i=0; i<taille2; i++)
      {
        data00[i]=0;
      }
      
      /**** On stocke les données des fichiers tmp2_file1 et tmp2_file2 respectivement dans data0 et data00 ****/
      strcpy(dir_1_2,  "../../Fichiers_générés/TSign/Fichier_dat/");
      strcat(dir_1_2, tmp2_file1);
      dat2 = fopen(dir_1_2,"r");
      strcpy(dir_1_2,  "../../Fichiers_générés/TSign/Fichier_dat/");
      strcat(dir_1_2, tmp2_file2);
      dat3 = fopen(dir_1_2,"r");
      
      for(i=0;i<nbech1;i++)
      {
        /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(dat2,"%lf", &data0[2*i]);
        viderbuffer(dat2);
      }
      for(i=0;i<nbech2;i++)
      {
        /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(dat3,"%lf", &data00[2*i]);
        viderbuffer(dat3);
      }
      
      /*fft sans filtre*/
      call_fft(re, im, data0, taille1, p1, tmp2_file1, fs1);
      call_fft(re, im, data00, taille2, p2, tmp2_file2, fs2);
      /*fft sans filtre*/
      
      /*fft avec filtre de welsch*/
      printf("taille1 = %ld\n",taille1);
      ft_filtre(data0,taille1,nbech1,tmp2_file1);
      ft_filtre(data00,taille2,nbech2,tmp2_file2);
      /*fin fft avec filtre de Welsch*/
     
     /*****
      MFCC : prend en entrée le nom d'un fichier de données et écrit les
      coefficients cemptraux dans un fichier mfcc.txt
      *****/
      mfcc(tmp2_file1,fs1,type_analyse);
      mfcc(tmp2_file2,fs2,type_analyse);
     /*--------------------fin mfcc---------------------*/
    
    
     /*****
      stft : prend en entrée le nom d'un fichier de données et la fréq d'échantillonnage correspondante
      puis écrit le résultat dans un fichier stft.txt
      *****/
      stft(tmp2_file1,fs1,type_analyse);
      stft(tmp2_file2,fs2,type_analyse);
      
      
      free(data0);
      data0 = NULL;
      free(data00);
      data00 = NULL;
      free(re);
      re = NULL;
      free(im);
      im = NULL;
      fclose(dat2);
      dat2 = NULL;
      fclose(dat3);
      dat3 = NULL;
      fclose(fich_1_2);
      fich_1_2 = NULL;
      
      
    }
    else{
      
      unsigned int fs;
      int p_uniq=0;                  /*pour la transformée de Fourier de 2^p points du fichier unique*/
      int nbech_uniq=0;              /*nombre d echantillons extraits du fichier audio*/
      signed long taille_uniq=1;
      char tmp2_uniq[NCAR];
      double *data_uniq=NULL;        /*fichier_uniq.wav*/
      char dir_uniq[NCAR]="";        /*chemin du répertoire où il chercher les fichiers*/
      double *re=NULL;               /*partie réelle de la tf*/
      double *im=NULL;               /*partie image de la tf*/
      FILE *fich_uniq=NULL;          /*Contient les info taille_uniq, nbech_uniq et tmp2_uniq*/
      FILE *dat1=NULL;               /*pour ouvrir le fichier tmp2_uniq*/
      
      
      
      /**** On donne le nom du fichier audio à analyser ****/
      printf("entrer le nom du fichier unique a analyser:\n");
      scanf("%s", fichieraudio_uniq);
      extract_data_1file(data_uniq, taille_uniq, nfile, nbech_uniq, fichieraudio_uniq, file, p_uniq);
      printf("Coucou Amadou\n");
      
      /**** On récuppère taille_uniq, nbech_uniq et tmp2_uniq ****/
      strcpy(dir_uniq,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
      strcat(dir_uniq, "E_Info.dat");
      fich_uniq = fopen(dir_uniq,"r");
      fscanf(fich_uniq,"%ld\n%d\n%s\n%d%d",&taille_uniq, &nbech_uniq, tmp2_uniq,&fs,&p_uniq);
      
      /**** Allocation du tableau de l'onde temporelle ****/
      data_uniq=malloc( (taille_uniq) * sizeof(double));
     
      if (data_uniq == NULL)
      {
        exit(0);
      }
    
      /**** initialisation des tableaux dynamiques ****/
      for(i=0; i<taille_uniq; i++)
      {
        data_uniq[i]=0;
      }
      /*On stocke les données du fichier tmp2_uniq dans data_uniq*/
      strcpy(dir_uniq,  "../../Fichiers_générés/TSign/Fichier_dat/");
      strcat(dir_uniq, tmp2_uniq);
      dat1 = fopen(dir_uniq,"r");
      
      for(i=0;i<nbech_uniq;i++)
      {
        /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(dat1,"%lf", &data_uniq[2*i]);
        viderbuffer(dat1);
      }
      
     /**** fft sans filtre ****/
      call_fft(re, im, data_uniq, taille_uniq, p_uniq,tmp2_uniq,fs);
      
     /*fft avec filtre*/
      printf("taille_uniq = %ld\n",taille_uniq);
      ft_filtre(data_uniq,taille_uniq,nbech_uniq,tmp2_uniq);
     /*fin fft avec filtre*/
     
     /*****
      MFCC : prend en entrée le nom d'un fichier de données et écrit les
      coefficients cemptraux dans un fichier mfcc.txt
      *****/
      mfcc(tmp2_uniq,fs,type_analyse);
     /*--------------------fin mfcc---------------------*/
    
    
     /*****
      stft : prend en entrée le nom d'un fichier de données et écrit les
      le résultat de sortie dans un fichier stft.txt
      *****/
      stft(tmp2_uniq,fs,type_analyse);
      
      free(data_uniq);
      data_uniq = NULL;
      fclose(fich_uniq);
      fich_uniq = NULL;
      fclose(dat1);
      dat1 = NULL;
      free(re);
      re = NULL;
      free(im);
      im = NULL;
      
    }
  }
  
  /*-------------------------------------Analyse vibratoire------------------------------------------*/
  else{
    
    int i = 0;
    unsigned int freq = 0;      /*fréquence d'échantillonnage des fichiers de données*/
    int choix_freq = 0;
    int nlign1 = 0;             /*nombre de ligne dans le fichier1*/
    int nlign2 = 0;
    int puissance1 = 0;         /*pour la transformée de fourier qui doit être de taille = 2^puissance1*/
    int puissance2 = 0;         /*pour la transformée de fourier qui doit être de taille = 2^puissance1*/
    signed long dim1 = 1;
    signed long dim2 = 1;
    double *tab1 = NULL;
    double *tab2 = NULL;
    double *tab3 = NULL;
    double *re = NULL;
    double *im = NULL;
    char filename1[NCAR] = "";  /*contient le nom du fichier*/
    char filename2[NCAR] = "";
    char filename3[NCAR] = "";
    char type_freq[NCAR] = "";
    char dir1[NCAR] = "";
    char c;                     /*Pour stocker un a caractère lu dans un fichier*/
    FILE *fichier1 = NULL;
    FILE *fichier2 = NULL;
    /*FILE *fichier3 = NULL;*/
    
    
    printf("Vous avez choisi de faire une %s\n",analyse);
    printf("Choisissez la fréquence d'échantillonnage des données contenues dans le fichier : \n");
    printf("***1.***  48Khz \n");   
    printf("***2.***  12Khz \n");
    scanf("%d",&choix_freq);
    
    if(choix_freq==1){
      strcpy(type_freq,"Vous avez choisi la fréquence 48000 Hz");/*dans le but de stocker la fréquence choisie dans la variable type_freq*/
      freq = 48000;
    }
    if(choix_freq==2){
      strcpy(type_freq,"Vous avez choisi la fréquence 12000 Hz");
      freq = 12000;
    }
    printf("%s\n",type_freq);
    
    printf ("entrer le nom du fichier n°1 à analyser :\n");
    scanf("%s", filename1);
    printf ("nom du fichier 1 : %s\n", filename1);
    printf ("entrer le nom du fichier n°2 à analyser :\n");
    scanf("%s", filename2);
    printf ("nom du fichier 2 : %s\n", filename2);
    /*printf ("entrer le nom 'diff' du fichier n°3, différence entre les 2 fichiers :\n");
    scanf("%s", filename3);*/
    strcpy(filename3,  "R-O_");
    strcat(filename3,  filename1);
    strcat(filename3,  "-");
    strcat(filename3,  filename2);
    /**** On compte le nombre de ligne dans chaque fichier ****/
    strcpy(dir1,  "../../Données/TSign_AStat/Data_vibration/48k Drive End Bearing Fault Data/");
    strcat(dir1, filename1);
    fichier1 = fopen(dir1, "r");
    
    /**** On vérifie si le fichier existe ****/
    if (fichier1 == NULL)
    {
       printf("Could not open file %s\n", filename1);
    }
 
    /**** On extrait des carctères dans le fichier et on le stocke dans c*/
    for (c = getc(fichier1); c != EOF; c = getc(fichier1))
        if (c == '\n') /* Increment count if this character is newline */
            nlign1 = nlign1 + 1;
    /* Close the file */
    printf("The file %s has %d lines\n ", filename1, nlign1);
    
    fclose(fichier1);
    fichier1 = NULL;
    
    /*On compte le nombre de ligne dans chaque fichier*/
    strcpy(dir1,  "../../Données/TSign_AStat/Data_vibration/48k Drive End Bearing Fault Data/");
    strcat(dir1, filename2);
    fichier2 = fopen(dir1, "r");
    /* Check if file exists */
    if (fichier2 == NULL)
    {
       printf("Could not open file %s\n", filename2);
    }
 
    /* Extract characters from file and store in character c*/
    for (c = getc(fichier2); c != EOF; c = getc(fichier2))
        if (c == '\n') /* Increment count if this character is newline */
            nlign2 = nlign2 + 1;
    /* Close the file */
    printf("The file %s has %d lines\n ", filename2, nlign2);
    
    fclose(fichier2);
    fichier2 = NULL;
    
    /*On détermine la taille du tableau à allouer pour la transformée de Fourier*/
     while (nlign1>dim1)
     {
        dim1=dim1*2;
        puissance1=puissance1+1;
     }
     printf ("nombre de points traites pour le fichier 1 : 2^%d=%ld\n",puissance1,dim1);
     
     while (nlign2>dim2)
     {
        dim2 = dim2*2;
        puissance2 = puissance2+1;
     }
     printf ("nombre de points traites pour le fichier 2 : 2^%d=%ld\n",puissance2,dim2);
     
     dim1 = dim1*2; /*Pour contenir les tableaux réels et imaginaires*/
     dim2 = dim2*2;
     
     printf("dim1 = %ld\n",dim1); 
     printf("dim2 = %ld\n",dim2);
      
    /*On alloue les tableaux tab1 et tab2*/
     tab1 = malloc( (dim1) * sizeof(double));
     tab2 = malloc( (dim2) * sizeof(double));
     printf("voix2\n");
    
     if(tab1 == NULL){
       exit(0);
     }
     
     if(tab2 == NULL){
      exit(0);
     }
    printf("voix3.");
    /*---------------------initialisation des tableaux dynamiques--------------------*/
    for(i=0; i<dim1; i++)
    {
      tab1[i]=0;
    }
    for(i=0; i<dim2; i++)
    {
      tab2[i]=0;
    }
    printf("voix1");
    /*On stocke les données du fichier 1  dans tab1*/
    strcpy(dir1,  "../../Données/TSign_AStat/Data_vibration/48k Drive End Bearing Fault Data/");
    strcat(dir1, filename1);
    fichier1 = fopen(dir1,"r");
    for(i=0;i<nlign1;i++)
      {
       /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(fichier1,"%lf", &tab1[2*i]);
        viderbuffer(fichier1);
      }
  
    /*On stocke les données du fichier 1  dans tab1*/
    strcpy(dir1,  "../../Données/TSign_AStat/Data_vibration/48k Drive End Bearing Fault Data/");
    strcat(dir1, filename2);
    fichier2 = fopen(dir1,"r");
    for(i=0;i<nlign2;i++)
      {
       /*lecture des echantillons et enregistrement dans le tableau*/
        fscanf(fichier2,"%lf", &tab2[2*i]);
        viderbuffer(fichier2);
      }
    
    /*On ouvre le fichier3*/
     /*fichier3 = fopen(filename3, "w");*/
    
    /*comparaison des dimensions*/
      if(dim1<=dim2){
        tab3 = malloc( (dim1) * sizeof(double));
        if(tab3 == NULL){
          exit(0);
        }
        for(i=0; i<dim1; i++){
          tab3[i]=0;
        }
        for(i=0;i<dim1;i++){
          tab3[i] = tab2[i] - tab1[i];
        }
       /* for(i=dim1;i<dim2;i++){
          tab3[i] = tab2[i];
        }*/
        /*fft*/
        call_fft(re, im, tab3, dim1, puissance1, filename3, freq);
      }
      else{
        tab3 = malloc( (dim2) * sizeof(double));
        if(tab3 == NULL){
          exit(0);
        }
        for(i=0; i<dim2; i++){
          tab3[i]=0;
        }
        for(i=0;i<dim2;i++){
          tab3[i] = tab1[i] - tab2[i];
        }
       /* for(i=dim2;i<dim1;i++){
          tab3[i] = tab1[i];
        }*/
        /*fft*/
        call_fft(re, im, tab3, dim2, puissance2, filename3, freq);
      }
    
    /*fft sans filtre*/
      call_fft(re, im, tab1, dim1, puissance1, filename1, freq);
      call_fft(re, im, tab2, dim2, puissance2, filename2, freq);
      /*fft sans filtre*/
      
      /*fft avec filtre de welsch??*/
   /*   ft_filtre(tab1,dim1,nlign1,filename1);
      ft_filtre(tab2,dim2,nlign2,filename2);*/
      /*fin fft avec filtre de Welsch*/
     
     /*****
      MFCC : prend en entrée le nom d'un fichier de données et écrit les
      coefficients cemptraux dans un fichier mfcc.txt
      *****/
      mfcc(filename1,freq,type_analyse);
      mfcc(filename2,freq,type_analyse);
     /*--------------------fin mfcc---------------------*/
    
    
     /*****
      stft : prend en entrée le nom d'un fichier de données et la fréq d'échantillonnage correspondante
      puis écrit le résultat dans un fichier stft.txt
      *****/
      stft(filename1,freq,type_analyse);
      stft(filename2,freq,type_analyse);
    
    
    /*------------------Libération de la mémoire-----------------*/
    
    fclose(fichier1);
    fichier1 = NULL;
    fclose(fichier2);
    fichier2 = NULL;
    /*fclose(fichier3);
    fichier3 = NULL;*/
    free(tab1);
    tab1 = NULL;
    free(tab2);
    tab2 = NULL;
    free(tab3);
    tab3 = NULL;
    free(re);
    re = NULL;
    free(im);
    im = NULL;
    
    
  }    


    /*---------------------liberation de la memoire----------------------------------*/
    /*liberation de la ram des malloc*/
    free(file);
    file = NULL;
    free(fichieraudio);
    fichieraudio = NULL;
    /*---------------------fin de liberation de la memoire---------------------------*/


    return 0;
}

