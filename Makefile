# Makefile para compilar a atividade de threads
# Para compilar: make
# Para limpar: make clean
# Para executar: make run

CC = gcc
CFLAGS = -Wall -pthread
LDFLAGS = -lm
TARGET = atividade_threads
SOURCE = atividade_threads.c

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CC) $(CFLAGS) $(SOURCE) -o $(TARGET) $(LDFLAGS)
	@echo "Compilação concluída com sucesso!"
	@echo "Execute com: ./$(TARGET)"

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(TARGET)
	rm -f thread_*_io.txt thread_*_misto.txt
	@echo "Arquivos limpos!"

.PHONY: all run clean
