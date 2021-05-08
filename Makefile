#$< penso sia il primo parametro

EXECS = esercizio09.0 esercizio09.1 esercizio09.2 esercizio09.3 esercizio09.4 esercizio09.5 esercizio09.6 risonanza_corda
HEADS = EquazioniDifferenziali.h VectorOperations.h FunzioniVettoriali.h

all: $(EXECS);

#il 2> serve per non mostrarmi gli errori, perché c'è sempre quello di gnuplot
#in windows farei > NUL
esercizio09.%: esercizio09.%.o EquazioniDifferenziali.o; g++ -o $@ $^ 2> /dev/null
esercizio09.%.o: esercizio09.%.cpp $(HEADS); g++ -c $<

EquazioniDifferenziali.o: EquazioniDifferenziali.cpp $(HEADS); g++ -c $<

risonanza_corda: risonanza_corda.o EquazioniDifferenziali.o; g++ -o $@ $^
risonanza_corda.o: risonanza_corda.cpp $(HEADS); g++ -c $<

#serve per tenere i .o  
.SECONDARY:

#-f serve così non mi avvisa ogni volta che non riesce ad eliminarli
clean: ; rm -f *.o ${EXECS}
