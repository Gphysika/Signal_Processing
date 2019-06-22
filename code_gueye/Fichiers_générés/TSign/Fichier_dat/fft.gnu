# : permet de commenter dans un script gnuplot

#titre de l'axe des abscisses
set xlabel "Frequence (Hz)"

#titre de l'axe des ordonnées
set ylabel "Amplitude (dB)" font "Times new roman,16"

#titre du graphe
set title "Densite spectrale" font "Times new roman,16"

#la position de la légende
set key right top

#initialiser l'échelle sur l'axe des abscisses
#set xrange [0:22500]
set xrange [*:*]

#génère un fichier de sortie au format png
set terminal pngcairo

#nom du fichier de sortie
set output '../../../Résultat_image/fft_audio-engine.png'

#tracer la courbe : colonne 5 du fichier représente les fréquences (Hz) et et la colonne 4 les amplitudes (dB)
plot "E_audio-engine.wav_data.dat_FFT.dat" u 5:4 title "Amplitude" w l lt rgb "blue"
#replot "O_Split-Bearing_D.wav_data.dat_FFT.dat" u 5:4 title "Amplitude défectueux" w l lt rgb "red"
