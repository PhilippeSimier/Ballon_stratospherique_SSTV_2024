# Nom du compilateur
CC = gcc

# Drapeaux de compilation
CFLAGS = -Wall -Wextra -Werror -O2

# Drapeaux de liaison pour wiringPi
LDFLAGS = -lwiringPi

# Nom de l'exécutable
TARGET = stop

# Fichiers sources
SRCS = stop.c

# Fichiers objets
OBJS = $(SRCS:.c=.o)

# Règle par défaut : compile et lie
all: $(TARGET)

# Règle pour créer l'exécutable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Règle pour compiler les fichiers objets
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer les fichiers objets et l'exécutable
clean:
	rm -f $(OBJS) $(TARGET)

# Règle pour tout nettoyer y compris les fichiers temporaires
distclean: clean
	rm -f *~

# Indique à make que ces règles ne sont pas des fichiers réels
.PHONY: all clean distclean
