# Curva di risonanza corda vibrante

Programma per calcolare la curva di risonanza nel caso in cui la frequenza di massimo dipenda dall'ampiezza, come succede nella corda vibrante.

Si ottiene una curva asimmetrica che però dipende da se si parte da frequenze maggiori o da frequenze minori.

Partendo da destra si ottiene la seguente curva:

![curva da destra](/curva_da_destra.png)

Mentre da sinistra si ha:

![curva da sinistra](/curva_da_sinistra.png)

I due grafici sovrapposti danno il seguente risultato

![Sovrapposti](/curva_1.png)

Il programma fa uso di gnuplot e della libreria [gplot++](https://github.com/ziotom78/gplotpp) per fare grafici con c++.
