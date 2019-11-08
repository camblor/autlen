#-----------------------
# Makefile para Autómatas y Lenguajes
#-----------------------

CC = gcc -ansi -pedantic
CFLAGS = -Wall
EXE = ejemplo1

all : $(EXE)

.PHONY : clean
clean :
	rm -f ejemplo1.o afnd.o transforma.o core $(EXE)

$(EXE) : % : %.o afnd.o transforma.o
	@echo "#---------------------------"
	@echo "# Generando $@ "
	@echo "# Depende de $^"
	@echo "# Ha cambiado $<"
	$(CC) $(CFLAGS) -o $@ $@.o afnd.o transforma.o

afnd.o : afnd.c afnd.h
	$(CC) $(CFLAGS) -c $<

transforma.o : transforma.c transforma.h
	$(CC) $(CFLAGS) -c $<

ejemplo1_test:
	./ejemplo1
