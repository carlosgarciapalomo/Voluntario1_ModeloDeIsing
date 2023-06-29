

# Configuración del estilo de la línea y puntos
set style line 1 lc rgb "black" lw 2 pt 7 ps 1.5 # Puntos negros y gruesos
set style line 2 lc rgb "red" lw 1 pt 7 ps 1.5   # Puntos rojos y gruesos


set label 1 "Correlacion N=16" at graph 0.5,0.95 font "arial bold,12" center

# Configuración de los ejes
set xlabel "Temperatura"
set ylabel "f(3)"
set grid

# Cargar los datos de la tabla
# Asumiendo que los datos están en el archivo "datos.txt" y están separados por espacios
# La primera columna es X, la segunda columna es Y, la tercera columna es el error de Y
plot "Correlacion.txt" using 1:2:3 with yerrorbars title "ERROR" ls 2 lc rgb "red", \
     "" using 1:2 with points title "VALOR MEDIO" ls 1 lc rgb "black"

