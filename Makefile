EXECS = risonanza_corda
HEADS = EquazioniDifferenziali.h VectorOperations.h FunzioniVettoriali.h

all: gplot++.h $(EXECS); 

gplot++.h: ; 
	@echo "Downloading gplot++.h from github"
	@curl -sL https://tinyurl.com/yyoeskq7 > gplot++.h

risonanza_corda: risonanza_corda.o; g++ -o $@ $^
risonanza_corda.o: risonanza_corda.cpp $(HEADS); g++ -c $<

#serve per tenere i .o  
.SECONDARY:

#-f serve così non mi avvisa ogni volta che non riesce ad eliminarli
clean: ; rm -f *.o ${EXECS} gplot++.h