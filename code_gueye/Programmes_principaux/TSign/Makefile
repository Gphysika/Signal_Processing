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
	rm -f *.o ../../Modules_fonctions/TSign/*.o
	rm -f ../../Fichiers_intermédiaires/TSign/Fichier_dat/*.dat
	rm -f ../../Fichiers_générés/TSign/Fichier_dat/*.dat
	rm -f ../../Fichiers_générés/TSign/Audio_MT/*.wav

# ------------------------------------------
# Exécutables
# ------------------------------------------
diagnostic : diagnostic.o ../../Modules_fonctions/TSign/wave.o ../../Modules_fonctions/TSign/ft.o ../../Modules_fonctions/TSign/ft_filtre.o ../../Modules_fonctions/TSign/fft.o ../../Modules_fonctions/TSign/call_fft.o ../../Modules_fonctions/TSign/libmfcc.o ../../Modules_fonctions/TSign/mfcc.o ../../Modules_fonctions/TSign/stft.o ../../Modules_fonctions/TSign/extract_data_1file.o ../../Modules_fonctions/TSign/extract_data_2file.o ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.o ../../Modules_fonctions/TSign/supp_chaine_caractere.o
	$(CC) $(LFLAGS) diagnostic.o ../../Modules_fonctions/TSign/wave.o ../../Modules_fonctions/TSign/ft.o ../../Modules_fonctions/TSign/ft_filtre.o ../../Modules_fonctions/TSign/fft.o ../../Modules_fonctions/TSign/call_fft.o ../../Modules_fonctions/TSign/libmfcc.o ../../Modules_fonctions/TSign/mfcc.o ../../Modules_fonctions/TSign/stft.o ../../Modules_fonctions/TSign/extract_data_1file.o ../../Modules_fonctions/TSign/extract_data_2file.o ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.o ../../Modules_fonctions/TSign/supp_chaine_caractere.o -o diagnostic $(LINK)

#fft : fft.o
#	$(CC) $(LFLAGS) fft.o -o fft $(LINK)

#readmat : readmat.o
#	$(CC) $(LFLAGS) readmat.o -o readmat $(LINK)

#wave : wave.o
#	$(CC) $(LFLAGS) wave.o -o wave $(LINK)

# ------------------------------------------
# Fichiers intermédiaires (*.o)
# ------------------------------------------
diagnostic.o : diagnostic.c ../../Modules_fonctions/TSign/wave.h ../../Modules_fonctions/TSign/ft.h ../../Modules_fonctions/TSign/ft_filtre.h ../../Modules_fonctions/TSign/fft.h ../../Modules_fonctions/TSign/call_fft.h ../../Modules_fonctions/TSign/libmfcc.h ../../Modules_fonctions/TSign/mfcc.h ../../Modules_fonctions/TSign/stft.h ../../Modules_fonctions/TSign/extract_data_1file.h ../../Modules_fonctions/TSign/extract_data_2file.h ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.h ../../Modules_fonctions/TSign/supp_chaine_caractere.h
	$(CC) $(CFLAGS) $(IFLAGS) -c diagnostic.c

#readmat.o : readmat.c mat.h
#	$(CC) $(CFLAGS) $(IFLAGS) -c readmat.c

wave.o : ../../Modules_fonctions/TSign/wave.c ../../Modules_fonctions/TSign/wave.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/wave.c

ft.o : ../../Modules_fonctions/TSign/ft.c 
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/ft.c

ft_filtre.o : ../../Modules_fonctions/TSign/ft_filtre.c ../../Modules_fonctions/TSign/ft_filtre.h ft.h ../../Modules_fonctions/TSign/wave.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/ft_filtre.c

fft.o : ../../Modules_fonctions/TSign/fft.c 
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/fft.c

call_fft.o : ../../Modules_fonctions/TSign/call_fft.c ../../Modules_fonctions/TSign/call_fft.h ../../Modules_fonctions/TSign/fft.h ../../Modules_fonctions/TSign/supp_chaine_caractere.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/call_fft.c

libmfcc.o : ../../Modules_fonctions/TSign/libmfcc.c ../../Modules_fonctions/TSign/libmfcc.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/libmfcc.c

mfcc.o : ../../Modules_fonctions/TSign/mfcc.c ../../Modules_fonctions/TSign/mfcc.h ../../Modules_fonctions/TSign/libmfcc.h ../../Modules_fonctions/TSign/fft.h ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/mfcc.c

stft.o : ../../Modules_fonctions/TSign/stft.c ../../Modules_fonctions/TSign/stft.h ../../Modules_fonctions/TSign/libmfcc.h ../../Modules_fonctions/TSign/fft.h ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/stft.c

extract_data_1file.o : ../../Modules_fonctions/TSign/extract_data_1file.c ../../Modules_fonctions/TSign/extract_data_1file.h wave.h ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/extract_data_1file.c

extract_data_2file.o : ../../Modules_fonctions/TSign/extract_data_2file.c ../../Modules_fonctions/TSign/extract_data_2file.h wave.h ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/extract_data_2file.c
	
viderbuffer_supp_saut_ligne.o : ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.c ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/viderbuffer_supp_saut_ligne.c

supp_chaine_caractere.o : ../../Modules_fonctions/TSign/supp_chaine_caractere.c ../../Modules_fonctions/TSign/supp_chaine_caractere.h
	$(CC) $(CFLAGS) $(IFLAGS) -c ../../Modules_fonctions/TSign/supp_chaine_caractere.c

