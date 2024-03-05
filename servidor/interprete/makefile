INTERPRETE = interprete
BANDERAS = -Wall -Wextra -Werror -g -std=gnu99 

armar: $(INTERPRETE).o

$(INTERPRETE).o: $(INTERPRETE).c $(INTERPRETE).h
	gcc -c $(INTERPRETE).c $(BANDERAS)
clean: 
	rm *.o