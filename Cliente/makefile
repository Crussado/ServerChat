CLIENTE = cli_stream
BANDERAS = -Wall -Wextra -Werror -g -std=gnu99 -pthread
NOMBRE = cliente
armar: $(NOMBRE)

$(NOMBRE): $(CLIENTE).c $(CLIENTE).h 
	gcc -o $(NOMBRE)  $(CLIENTE).c $(BANDERAS)
clean: 
	rm *.o