CARPETASLIST = SList/
SLIST = Slist.o
CARPETAINTERPRETE = interprete/
INTERPRETE = interprete.o
SERVIDOR = srv_stream
BANDERAS = -Wall -Wextra -Werror -g -std=gnu99 -pthread
NOMBRE = server
armar: $(NOMBRE)

$(NOMBRE): $(SERVIDOR).c $(SERVIDOR).h $(SLIST) $(INTERPRETE)
	gcc -o server $(SERVIDOR).c $(CARPETASLIST)$(SLIST) $(CARPETAINTERPRETE)$(INTERPRETE) $(BANDERAS)
$(SLIST):
	cd $(CARPETASLIST) && make
$(INTERPRETE):
	cd $(CARPETAINTERPRETE) && make
clean: 
	rm *.o