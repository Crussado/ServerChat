SLIST = Slist
BANDERAS = -Wall -Wextra -Werror -g -std=gnu99 

armar: $(SLIST).o

$(SLIST).o: $(SLIST).c $(SLIST).h
	gcc -c $(SLIST).c $(BANDERAS)
clean: 
	rm *.o
