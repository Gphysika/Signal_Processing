/**
 * Read and parse a wave file
 *
 **/
 
 /*PROGRAM diagnostic by sound/vibration analysis
  NAME:
    diagnostic

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
    commented "fprintf(fich,"Channel#%d : ", (xchannels+1));"
    fprintf(fich,"==========Sample %ld / %ld=============\n", i, num_samples);
    fprintf(fich," | ");
    change int in void
    commented "free(filename);"
  

  Compilation: 
    gcc -c -ansi -Wall -Wextra diagnostic.c -lm
    gcc diagnostic.o diagnostic.o -o
    

  Execution:
    ./wave
*/
 
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "wave.h"
#define TRUE 1 
#define FALSE 0

/* WAVE header structure*/

unsigned char buffer4[4];
unsigned char buffer2[2];

char* seconds_to_time(float seconds);
/*void supprime(char *texte, char x);*/

 FILE *ptr;
 /*char *filename;*/
 struct HEADER header;
 /*int argc;
 char **argv;*/
/*void main(int argc, char **argv) {*/
 void wave(char *filename, char **file, int nfile) {
 
 FILE *fich=NULL;
 FILE *fichier=NULL;
 
 /*filename = (char*) malloc(sizeof(char) * 1024);
 if (filename == NULL) {
   printf("Error in malloc\n");
   exit(1);
 }*/
 
    

 /* get file path*/
 /*char cwd[1024];
 if (getcwd(cwd, sizeof(cwd)) != NULL) {
   
	strcpy(filename, cwd);*/

	/* get filename from command line*/
/*	if (argc < 2) {
	  printf("No wave file specified\n");
	  return 1;
	}*/
	
/*	strcat(filename, "/");
	strcat(filename, argv[1]);
	printf("%s\n", filename);
 }*/ /*TODO*/

 /* open file*/
/* printf("Opening  file..\n");
 char dirptr[100]="";
 strcpy(dirptr,  "../../Données/TSign_AStat/Data_sound/Inputs_MT/Données_référence");
 strcat(dirptr,filnemae);*/
 /*printf("filename = %s\nfile1 = %s\n",filename,file1[0]);*/
/* ptr = fopen(dirptr, "rb");
 if (ptr == NULL) {
	printf("Error opening file\n");
	exit(1);
 }*/
 
 int read = 0;
 int i = 0;
 char src_uniq[100] ="";
 char src_uniqdata[100] ="";
 char src[100] ="";
 char src1[100] ="";
 char src2[100] ="";
 char src3[100] ="";
 /*char x[1] =".";*/
 /*else {
	printf("Erreur à l'ouverture du fichier test1.txt\n");
	return 1;
 }
 while(getchar()!='\n');
    getchar();*/
 /* read header parts*/
 
 /*On ouvre le fichier où on stock le header du fichier son"*/
  /*strcpy(src,  "Header");*/
  /*supprime(filename, x);*/
     for(i=0;i<2;i++){
      printf("file[%d] = %s\n",i,file[i]);
     }
if(nfile == 2){
  printf("on lit deux fichiers");  
  if (strcmp(filename, file[0]) == 0){
    /*strcpy(src,  "HeaderFILE_1");*/
    char dirptr[100]="";
    strcpy(dirptr,  "../../Données/TSign_AStat/Data_sound/Inputs_MT/Données_référence/");
    strcat(dirptr,filename);
    /*printf("filename = %s\nfile1 = %s\n",filename,file1[0]);*/
    ptr = fopen(dirptr, "rb");
    if (ptr == NULL) {
	printf("Error opening file\n");
	exit(1);
	}
    strcpy(src,  "R_");
    strcat(src, filename);
    strcat(src,  "_Header.dat");
    /*On écrit le header dans un répertoire*/
    char dir[100]="";
    strcpy(dir,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir, src);
    fichier = fopen(dir, "w");
    
    if (fichier != NULL) {
 	   printf("le fichier %s est ouvert\n", src);
    }
    if (fichier == NULL) {
 	   printf("Erreur à l'ouverture du fichier %s\n", src);
    }
    printf("file1 = %s\n",file[0]);
  }
  else{
    /*strcpy(src1,  "HeaderFILE_2");*/
    char dirptr1[100]="";
    strcpy(dirptr1,  "../../Données/TSign_AStat/Data_sound/Inputs_MT/Données_observées/");
    strcat(dirptr1,filename);
    /*printf("filename = %s\nfile1 = %s\n",filename,file1[0]);*/
    ptr = fopen(dirptr1, "rb");
    if (ptr == NULL) {
	  printf("Error opening file\n");
	  exit(1);
	}
    strcpy(src1,  "O_");
    strcat(src1, filename);
    strcat(src1,  "_Header.dat");
    /*On écrit le header dans un répertoire*/
    char dir1[100]="";
    strcpy(dir1,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
    strcat(dir1, src1);
    fichier = fopen(dir1, "w");
    
    if (fichier != NULL) {
 	   printf("le fichier %s est ouvert\n", src1);
    }
    if (fichier == NULL) {
 	   printf("Erreur à l'ouverture du fichier %s\n", src1);
    }
    printf("file2 = %s\n",filename);
  }
}/*if du nbre de fichier*/
else{
  printf("On lit le fichier unique");
  /*strcpy(src_uniq,  "HeaderFILE_uniq");*/
  char dirptr_uniq[100]="";
  strcpy(dirptr_uniq,  "../../Données/TSign_AStat/Data_sound/Inputs_MT/Données_obs_unique/");
  strcat(dirptr_uniq,filename);
  /*printf("filename = %s\nfile1 = %s\n",filename,file1[0]);*/
  printf("dirptr_uniq = %s", dirptr_uniq);
  ptr = fopen(dirptr_uniq, "rb");
  if (ptr == NULL) {
	printf("Error opening file\n");
	exit(1);
  }
  strcpy(src_uniq,  "E_");
  strcat(src_uniq, filename);
  strcat(src_uniq,  "_Header.dat");
  /*On écrit le header dans un répertoire*/
  char dir_uniq[100]="";
  strcpy(dir_uniq,  "../../Fichiers_intermédiaires/TSign/Fichier_dat/");
  strcat(dir_uniq, src_uniq);
  /*fichier = fopen(src_uniq, "w");*/
  fichier = fopen(dir_uniq, "w");
  
  if (fichier != NULL) {
 	 printf("le fichier %s est ouvert\n", src_uniq);
  }
  if (fichier == NULL) {
 	 printf("Erreur à l'ouverture du fichier %s\n", src_uniq);
  }
  printf("file_uniq = %s\n",filename);
}
 
 read = fread(header.riff, sizeof(header.riff), 1, ptr);
 fprintf(fichier,"(1-4): %s \n", header.riff); 

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fichier,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);
 
 /* convert little endian to big endian 4 byte int*/
 header.overall_size  = buffer4[0] | 
						(buffer4[1]<<8) | 
						(buffer4[2]<<16) | 
						(buffer4[3]<<24);

 fprintf(fichier,"(5-8) Overall size: bytes:%u, Kb:%u \n", header.overall_size, header.overall_size/1024);

 read = fread(header.wave, sizeof(header.wave), 1, ptr);
 fprintf(fichier,"(9-12) Wave marker: %s\n", header.wave);

 read = fread(header.fmt_chunk_marker, sizeof(header.fmt_chunk_marker), 1, ptr);
 fprintf(fichier,"(13-16) Fmt marker: %s\n", header.fmt_chunk_marker);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fichier,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 /* convert little endian to big endian 4 byte integer*/
 header.length_of_fmt = buffer4[0] |
							(buffer4[1] << 8) |
							(buffer4[2] << 16) |
							(buffer4[3] << 24);
 fprintf(fichier,"(17-20) Length of Fmt header: %u \n", header.length_of_fmt);

 read = fread(buffer2, sizeof(buffer2), 1, ptr); printf("%u %u \n", buffer2[0], buffer2[1]);
 
 header.format_type = buffer2[0] | (buffer2[1] << 8);
 char format_name[10] = "";
 if (header.format_type == 1)
   strcpy(format_name,"PCM"); 
 else if (header.format_type == 6)
  strcpy(format_name, "A-law");
 else if (header.format_type == 7)
  strcpy(format_name, "Mu-law");

 fprintf(fichier,"(21-22) Format type: %u %s \n", header.format_type, format_name);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 fprintf(fichier,"%u %u \n", buffer2[0], buffer2[1]);

 header.channels = buffer2[0] | (buffer2[1] << 8);
 fprintf(fichier,"(23-24) Channels: %u \n", header.channels);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fichier,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 header.sample_rate = buffer4[0] |
						(buffer4[1] << 8) |
						(buffer4[2] << 16) |
						(buffer4[3] << 24);

 fprintf(fichier,"(25-28) Sample rate: %u\n", header.sample_rate);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fichier,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 header.byterate  = buffer4[0] |
						(buffer4[1] << 8) |
						(buffer4[2] << 16) |
						(buffer4[3] << 24);
 fprintf(fichier,"(29-32) Byte Rate: %u , Bit Rate:%u\n", header.byterate, header.byterate*8);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 fprintf(fichier,"%u %u \n", buffer2[0], buffer2[1]);

 header.block_align = buffer2[0] |
					(buffer2[1] << 8);
 fprintf(fichier,"(33-34) Block Alignment: %u \n", header.block_align);

 read = fread(buffer2, sizeof(buffer2), 1, ptr);
 fprintf(fichier,"%u %u \n", buffer2[0], buffer2[1]);

 header.bits_per_sample = buffer2[0] |
					(buffer2[1] << 8);
 fprintf(fichier,"(35-36) Bits per sample: %u \n", header.bits_per_sample);

 read = fread(header.data_chunk_header, sizeof(header.data_chunk_header), 1, ptr);
 fprintf(fichier,"(37-40) Data Marker: %s \n", header.data_chunk_header);

 read = fread(buffer4, sizeof(buffer4), 1, ptr);
 fprintf(fichier,"%u %u %u %u\n", buffer4[0], buffer4[1], buffer4[2], buffer4[3]);

 header.data_size = buffer4[0] |
				(buffer4[1] << 8) |
				(buffer4[2] << 16) | 
				(buffer4[3] << 24 );
 fprintf(fichier,"(41-44) Size of data chunk: %u \n", header.data_size);


 /* calculate no.of samples*/
 long num_samples = (8 * header.data_size) / (header.channels * header.bits_per_sample);
 fprintf(fichier,"Number of samples:%lu \n", num_samples);

 long size_of_each_sample = (header.channels * header.bits_per_sample) / 8;
 fprintf(fichier,"Size of each sample:%ld bytes\n", size_of_each_sample);

 /* calculate duration of file*/
 float duration_in_seconds = (float) header.overall_size / header.byterate;
 fprintf(fichier,"Approx.Duration in seconds=%f\n", duration_in_seconds);
 fprintf(fichier,"Approx.Duration in h:m:s=%s\n", seconds_to_time(duration_in_seconds));



 /* read each sample from data chunk if PCM*/
 if (header.format_type == 1) { /* PCM*/
    printf("Dump sample data? Y/N?\n");
	char c = 'y';
	/*printf("coucou\n");*/
	/*scanf("%c", &c);*/
	if (/*c == 'Y' ||*/c == 'y') { 
		long i =0;
		char data_buffer[size_of_each_sample];
		int  size_is_correct = TRUE;

		/* make sure that the bytes-per-sample is completely divisible by num.of channels*/
		long bytes_in_each_channel = (size_of_each_sample / header.channels);
		if ((bytes_in_each_channel  * header.channels) != size_of_each_sample) {
			fprintf(fichier,"Error: %ld x %ud <> %ld\n", bytes_in_each_channel, header.channels, size_of_each_sample);
			size_is_correct = FALSE;
		}
 
		if (size_is_correct) { 
					/* the valid amplitude range for values based on the bits per sample*/
			long low_limit = 0l;
			long high_limit = 0l;

			switch (header.bits_per_sample) {
				case 8:
					low_limit = -128;
					high_limit = 127;
					break;
				case 16:
					low_limit = -32768;
					high_limit = 32767;
					break;
				case 32:
					low_limit = -2147483648;
					high_limit = 2147483647;
					break;
			}
			
			/*On ouvre le fichier où on stock les données"*/
		 if(nfile == 2){
			if (strcmp(filename, file[0]) == 0){
			   printf("On lit les données du fichier 1 :\n");
			   /*strcpy(src2,  "checkdataFILE_1");*/
			   strcpy(src2,  "R_");
			   strcat(src2, filename);
			   strcat(src2,  "_data.dat");
			   /*On écrit le header dans un répertoire*/
			   char dir2[100]="";
			   strcpy(dir2,  "../../Fichiers_générés/TSign/Fichier_dat/");
			   strcat(dir2, src2);
			   fich = fopen(dir2, "w");
			   if (fich != NULL) {
				  printf("le fichier %s est ouvert\n",src2);
			   }
			   if (fich == NULL) {
				  printf("Erreur à l'ouverture du fichier %s\n",src2);
			   }
			}
			else{
			   printf("On lit les données du fichier 2 :\n");
			   /*strcpy(src3,  "checkdataFILE_2");*/
			   strcpy(src3,  "O_");
			   strcat(src3, filename);
			   strcat(src3,  "_data.dat");
			   char dir3[100]="";
			   strcpy(dir3,  "../../Fichiers_générés/TSign/Fichier_dat/");
			   strcat(dir3, src3);
			   fich = fopen(dir3, "w");
			   if (fich != NULL) {
				  printf("le fichier %s est ouvert\n",src3);
			   }
			   if (fich == NULL) {
				  printf("Erreur à l'ouverture du fichier %s\n",src3);
			   }
			}
		}
		else{	
		  printf("On lit les données du fichier unique :\n");
		  /*strcpy(src_uniqdata,  "checkdataFILE_uniq");*/
		  strcpy(src_uniqdata,  "E_");
		  strcat(src_uniqdata, filename);
		  strcat(src_uniqdata,  "_data.dat");
		  char dir_uniqdata[100]="";
		  strcpy(dir_uniqdata,  "../../Fichiers_générés/TSign/Fichier_dat/");
		  strcat(dir_uniqdata, src_uniqdata);
		  fich = fopen(dir_uniqdata, "w");
		  if (fich != NULL) {
			printf("le fichier %s est ouvert\n",src_uniqdata);
		  }
		  if (fich == NULL) {
				  printf("Erreur à l'ouverture du fichier %s\n",src_uniqdata);
		  }
		}
			fprintf(fich,".Valid range for data values : %ld to %ld \n", low_limit, high_limit);
			for (i =1; i <= num_samples; i++) {
				/*fprintf(fich,"==========Sample %ld / %ld=============\n", i, num_samples);*/
				read = fread(data_buffer, sizeof(data_buffer), 1, ptr);
				if (read == 1) {
				
					/* dump the data read*/
					unsigned int  xchannels = 0;
					int data_in_channel = 0;

					for (xchannels = 0; xchannels < header.channels; xchannels ++ ) {
						/*fprintf(fich,"Channel#%d : ", (xchannels+1));*/
						/* convert data from little endian to big endian based on bytes in each channel sample*/
						if (bytes_in_each_channel == 4) {
							data_in_channel =	data_buffer[0] | 
												(data_buffer[1]<<8) | 
												(data_buffer[2]<<16) | 
												(data_buffer[3]<<24);
						}
						else if (bytes_in_each_channel == 2) {
							data_in_channel = data_buffer[0] |
												(data_buffer[1] << 8);
						}
						else if (bytes_in_each_channel == 1) {
							data_in_channel = data_buffer[0];
						}

						fprintf(fich,"%d ", data_in_channel);

						/* check if value was in range*/
						if (data_in_channel < low_limit || data_in_channel > high_limit)
							fprintf(fich,"**value out of range\n");

						/*fprintf(fich," | ");*/
					}

					fprintf(fich,"\n");
				}
				else {
					fprintf(fich,"Error reading file. %d bytes\n", read);
					break;
				}

			} /* 	for (i =1; i <= num_samples; i++) {*/

		} /*	if (size_is_correct) { */

	 } printf("coucou\n");/* if (c == 'Y' || c == 'y') { */
 } /*  if (header.format_type == 1) { */

 printf("Closing file..\n");
 /*fclose(ptr);*/
 fclose(fichier);
 fichier = NULL;
 fclose(fich);
 fich = NULL;
 fclose(ptr);
 ptr = NULL;
  /* cleanup before quitting*/
 /*free(filename);*/
 return;

}

/**
 * Convert seconds into hh:mm:ss format
 * Params:
 *	seconds - seconds value
 * Returns: hms - formatted string
 **/
 char* seconds_to_time(float raw_seconds) {
  char *hms;
  int hours, hours_residue, minutes, seconds, milliseconds;
  hms = (char*) malloc(100);

  sprintf(hms, "%f", raw_seconds);

  hours = (int) raw_seconds/3600;
  hours_residue = (int) raw_seconds % 3600;
  minutes = hours_residue/60;
  seconds = hours_residue % 60;
  milliseconds = 0;

  /* get the decimal part of raw_seconds to get milliseconds*/
  char *pos;
  pos = strchr(hms, '.');
  int ipos = (int) (pos - hms);
  char decimalpart[15];
  memset(decimalpart, ' ', sizeof(decimalpart));
  strncpy(decimalpart, &hms[ipos+1], 3);
  milliseconds = atoi(decimalpart);	

  
  sprintf(hms, "%d:%d:%d.%d", hours, minutes, seconds, milliseconds);
  return hms;
}
