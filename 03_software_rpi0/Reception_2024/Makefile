# Makefile

# Nom du fichier binaire généré
TARGET = reception

# Compilateurs croisés
CC = arm-linux-gnueabihf-gcc
CXX = arm-linux-gnueabihf-g++

# Options de compilation et d'édition de liens
CFLAGS = -Wall -O2 -march=armv6 -mfpu=vfp -mfloat-abi=hard -marm
CXXFLAGS = -Wall -O2 -march=armv6 -mfpu=vfp -mfloat-abi=hard -marm -lpthread -lrt
LDFLAGS =

# Fichiers source et objets
SRCS = reception.cpp GestionFile.cpp
OBJS = $(SRCS:.cpp=.o)

# Règle de compilation
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJS)

.PHONY: all clean
