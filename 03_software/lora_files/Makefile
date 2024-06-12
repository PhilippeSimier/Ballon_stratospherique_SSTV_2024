# Nom du programme final
PROG = lora_files

# Fichiers source C
SRC = lora_files.c LoRa.c readConfig.c

# Objets compilés
OBJ = $(SRC:.c=.o)

# Bibliothèques
LIBS = -lpigpio -lpthread -lm

# Compilateur et options
CC = gcc
CFLAGS = -Wall -O2

# Règle de compilation principale
$(PROG): $(OBJ)
	$(CC) $(CFLAGS) -o $(PROG) $(OBJ) $(LIBS)

# Règle de compilation pour chaque objet
.PHONY: all clean

all: $(PROG)

clean:
	-rm $(OBJ) $(PROG)
