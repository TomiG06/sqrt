CC = gcc
AS = as

ASM_SRC = heron.s
C_SRC = sqrt.c

OBJ = heron.o sqrt.o

TARGET = sqrt

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -no-pie $(OBJ) -lm -o $(TARGET)

heron.o: heron.s
	$(AS) $< -o $@

sqrt.o: sqrt.c
	$(CC) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

