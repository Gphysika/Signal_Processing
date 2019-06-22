# ====================================================================================== #
#                                   Notes de l'auteur                                    #
# ====================================================================================== #
# ! Makefile pour le projet "Diagnostic par analyse sonore" de Business&Decision.
# ! Ce Makefile a été adapté afin de fonctionner sur tous les systèmes
# ! GNU/Linux pour lesquels les librairies X11 et cfitsio ont été
# ! installées en passant par le gestionnaire de paquets de la distribution.

# ====================================================================================== #
#                                 Variables du Makefile                                  #
# ====================================================================================== #

# ------------------------------------------
# 1 - variable du compilateur à utiliser
# 2 - choix des options de compilation
# 3 - liste des répertoires d'en-têtes
# 4 - liste des répertoires de librairies
# 5 - librairies à lier
# ------------------------------------------
CC     = /usr/bin/gcc
CFLAGS = -ansi -Wall -Wextra -Werror
IFLAGS = -I /usr/include
LFLAGS = -L /usr/lib
LINK   = -lm

# ====================================================================================== #
#                                   Cibles du Makefile                                   #
# nom_cible : dependance                                                                 #
# <tabulation> commande                                                                  #
# ====================================================================================== #

# ------------------------------------------
# ! - all : Compile tous les exécutables
# ! - help : Affiche cette aide
# ! - clean : efface les fichiers objet *.o
# !           et les exécutables
# ------------------------------------------
all : diagnostic

help :
	@grep -E "^# !" Makefile | sed -e 's/# !/ /g'

clean : 
	rm -f *.o diagnostic
	rm -f *.dat
	rm -f ./Fichiers_intermédiaires/TSign/Fichier_dat/*.dat
	rm -f ./Fichiers_générés/TSign/Fichier_dat/*.dat
	rm -f ./Fichiers_générés/TSign/Audio_MT/*.wav

# ------------------------------------------
# Exécutables
# ------------------------------------------
diagnostic : diagnostic.o wave.o ft.o ft_filtre.o fft.o call_fft.o libmfcc.o mfcc.o stft.o extract_data_1file.o extract_data_2file.o viderbuffer_supp_saut_ligne.o supp_chaine_caractere.o
	$(CC) $(LFLAGS) diagnostic.o wave.o ft.o ft_filtre.o fft.o call_fft.o libmfcc.o mfcc.o stft.o extract_data_1file.o extract_data_2file.o viderbuffer_supp_saut_ligne.o supp_chaine_caractere.o -o diagnostic $(LINK)

#fft : fft.o
#	$(CC) $(LFLAGS) fft.o -o fft $(LINK)

#readmat : readmat.o
#	$(CC) $(LFLAGS) readmat.o -o readmat $(LINK)

#wave : wave.o
#	$(CC) $(LFLAGS) wave.o -o wave $(LINK)

# ------------------------------------------
# Fichiers intermédiaires (*.o)
# ------------------------------------------
diagnostic.o : diagnostic.c wave.h ft.h ft_filtre.h fft.h call_fft.h libmfcc.h mfcc.h stft.h extract_data_1file.h extract_data_2file.h viderbuffer_supp_saut_ligne.h supp_chaine_caractere.h
	$(CC) $(CFLAGS) $(IFLAGS) -c diagnostic.c

#readmat.o : readmat.c mat.h
#	$(CC) $(CFLAGS) $(IFLAGS) -c readmat.c

wave.o : wave.c wave.h
	$(CC) $(CFLAGS) $(IFLAGS) -c wave.c

ft.o : ft.c 
	$(CC) $(CFLAGS) $(IFLAGS) -c ft.c

ft_filtre.o : ft_filtre.c ft_filtre.h ft.h wave.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ft_filtre.c

fft.o : fft.c 
	$(CC) $(CFLAGS) $(IFLAGS) -c fft.c

call_fft.o : call_fft.c call_fft.h fft.h supp_chaine_caractere.h
	$(CC) $(CFLAGS) $(IFLAGS) -c call_fft.c

libmfcc.o : libmfcc.c libmfcc.h
	$(CC) $(CFLAGS) $(IFLAGS) -c libmfcc.c

mfcc.o : mfcc.c mfcc.h libmfcc.h fft.h viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c mfcc.c

stft.o : stft.c stft.h libmfcc.h fft.h viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c stft.c

extract_data_1file.o : extract_data_1file.c extract_data_1file.h wave.h viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c extract_data_1file.c

extract_data_2file.o : extract_data_2file.c extract_data_2file.h wave.h viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c extract_data_2file.c
	
viderbuffer_supp_saut_ligne.o : viderbuffer_supp_saut_ligne.c viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c viderbuffer_supp_saut_ligne.c

supp_chaine_caractere.o : supp_chaine_caractere.c supp_chaine_caractere.h
	$(CC) $(CFLAGS) $(IFLAGS) -c supp_chaine_caractere.c

