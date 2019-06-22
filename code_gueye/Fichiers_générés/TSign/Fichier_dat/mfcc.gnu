# : permet de commenter dans un script gnuplot

#titre de l'axe des abscisses
set xlabel "frame"

#titre de l'axe des ordonnées
set ylabel "coeff" font "Times new roman,16"

#titre du graphe
set title "Coefficients Mel" font "Times new roman,16"

#génère un fichier de sortie au format png
set terminal pngcairo

#nom du fichier de sortie
set output '../../../Résultat_image/mfcc_Split-Bearing_ND.png'

#la position de la légende
set key right top

#initialiser l'échelle par défaut
set xrange [0:3]
set yrange [*:*]

#tracer la courbe : colonne 5 du fichier représente les fréquences (Hz) et et la colonne 4 les amplitude (db)
#plot "R_Split-Bearing_N.wav_data.dat_mfcc.dat" u 2:1 title "coeff Mels" w l lt rgb "blue"
replot "O_Split-Bearing_D.wav_data.dat_mfcc.dat" u 2:1 title "coeff Mels" w l lt rgb "red"
