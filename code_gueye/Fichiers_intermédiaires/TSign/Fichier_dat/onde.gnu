# : permet de commenter dans un script gnuplot

#titre de l'axe des abscisses
set xlabel "temps (secondes)"

#titre de l'axe des ordonnées
set ylabel "Intensité acoustique" font "Times new roman,16"

#titre du graphe
set title "Onde temporelle" font "Times new roman,16"

#initialiser l'échelle par défaut sur les deux axes
#set xrange [*:*]
set yrange [*:*]

#génère un fichier de sortie au format png
set terminal pngcairo

#nom du fichier de sortie
set output '../../../Résultat_image/Onde_machine_1.png'

#la position de la légende
set key right top

#tracer la courbe : colonne 4 du fichier représente les temps (secondes) et la colonne 1 les intensités acoustiques (SI : W/m² ou N/m²)
plot "E_machine_1.wav_data.dat_FFT-inverse-onde-time-freq.dat" u 4:1 title "Intensité acoustique" w l lt rgb "blue"
