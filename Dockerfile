# Dockerfile para executar a atividade de threads em Fedora
# Nota: Se encontrar problemas de SSL/certificados, use a versão com tag específica
FROM fedora:40

# Define o diretório de trabalho
WORKDIR /app

# Copia os arquivos do projeto
COPY atividade_threads.c .
COPY Makefile .

# Atualiza o sistema e instala ferramentas necessárias
# Nota: Em ambientes com problemas de SSL, você pode pular o 'dnf update'
RUN dnf install -y gcc make && \
    dnf clean all

# Compila o programa
RUN make

# Comando padrão ao executar o container
CMD ["./atividade_threads"]
