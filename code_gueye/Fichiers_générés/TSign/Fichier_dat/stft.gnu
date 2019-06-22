# : permet de commenter dans un script gnuplot

#titre de l'axe des abscisses
set xlabel "Frequence (Hz)"

#titre de l'axe des ordonnées
set ylabel "Amplitude (dB)" font "Times new roman,16"

#titre du graphe
set title "Densite spectrale sur une fenêtre de 512 points" font "Times new roman,16"

#génère un fichier de sortie au format png
set terminal pngcairo

#nom du fichier de sortie
set output '../../../Résultat_image/stft_Baw_Split-Bearing_ND.png'

#la position de la légende
set key right top

#initialiser l'échelle par défaut sur les deux axes
set xrange [*:*]
set yrange [*:*]

#tracer la courbe : colonne 5 du fichier représente les fréquences (Hz) et et la colonne 4 les amplitude (db)
#plot "R_Split-Bearing_N.wav_data.dat_stft_magn_BaW.dat" u 5:4 every ::0::511 title "Amplitude" w l lt 6
replot "O_Split-Bearing_D.wav_data.dat_stft_magn_BaW.dat" u 5:4 every ::0::511 title "Amplitude" w l lt rgb "red"
