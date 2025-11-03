# 2025.2-Atividades-05-Tarefas-Threads

Atividade avaliativa sobre threads em C para alunos iniciantes de Ciências da Computação.

## 📋 Descrição da Atividade

Esta atividade prática tem como objetivo demonstrar o comportamento de threads POSIX em C através de três cenários diferentes:

1. **Thread CPU**: Executa operações intensivas de processamento (cálculo de números primos)
2. **Thread I/O**: Realiza operações de entrada/saída (leitura e escrita de arquivos)
3. **Thread Mista**: Combina operações de CPU e I/O

## 🎯 Objetivos de Aprendizado

- Compreender o conceito de threads e concorrência
- Observar o comportamento de diferentes tipos de operações em threads
- Aprender a compilar e executar programas C com threads (pthread)
- Utilizar Docker para criar ambientes isolados e reproduzíveis

## 📁 Estrutura do Projeto

```
.
├── atividade_threads.c  # Código-fonte principal do programa
├── Makefile             # Arquivo para compilação automatizada
├── Dockerfile           # Configuração do container Fedora
├── executar.sh          # Script auxiliar para execução
├── RELATORIO.md         # Template para relatório da atividade
└── README.md            # Este arquivo
```

## 🚀 Como Executar

### Método Rápido: Script Auxiliar (Recomendado para Iniciantes)

Execute o script auxiliar que guiará você pelo processo:

```bash
./executar.sh
```

O script oferece opções para:
1. Executar localmente (se você tiver GCC e Make instalados)
2. Executar com Docker (se você tiver Docker instalado)

### Método 1: Executar Localmente (sem Docker)

**Pré-requisitos:**
- GCC (GNU Compiler Collection)
- Make
- Biblioteca pthread (geralmente incluída no Linux)

**Passos:**

1. Compile o programa:
```bash
make
```

2. Execute o programa:
```bash
./atividade_threads
```

3. Para limpar os arquivos gerados:
```bash
make clean
```

### Método 2: Executar com Docker (Recomendado)

**Pré-requisitos:**
- Docker instalado no sistema

**Passos:**

1. Construa a imagem Docker:
```bash
docker build -t atividade-threads .
```

2. Execute o container:
```bash
docker run --rm atividade-threads
```

**Opção alternativa** - Execute em modo interativo para explorar o container:
```bash
docker run -it --rm atividade-threads /bin/bash
```

Dentro do container, você pode executar:
```bash
./atividade_threads
```

**Nota sobre problemas de SSL:** Se encontrar erros relacionados a certificados SSL durante o build do Docker, isso é comum em alguns ambientes de CI/CD. Nesse caso, use o Método 1 (execução local) ou tente executar em sua máquina pessoal.

## 📝 Elaboração do Relatório

Após executar o programa, utilize o arquivo `RELATORIO.md` como template para documentar suas observações:

1. Faça uma cópia do arquivo:
```bash
cp RELATORIO.md MEU_RELATORIO.md
```

2. Preencha as seções com suas observações e análises

3. Execute o programa múltiplas vezes para comparar resultados

4. Responda às questões propostas com base nas suas observações

## 🔍 O Que Observar

Ao executar o programa, preste atenção aos seguintes aspectos:

1. **Ordem de Execução**: As threads são criadas em ordem, mas podem completar em qualquer ordem
2. **Tempos de Execução**: Compare os tempos de cada tipo de operação
3. **Saída do Console**: Observe como as mensagens das diferentes threads podem se entrelaçar
4. **Comportamento Concorrente**: Todas as três threads executam simultaneamente

### Perguntas para Reflexão

1. Qual thread terminou primeiro? Por quê?
2. Por que os tempos de execução variam entre diferentes execuções?
3. Como o sistema operacional gerencia a execução das threads?
4. Qual seria o impacto de aumentar o número de threads?
5. O que aconteceria se executássemos as mesmas operações sequencialmente?

## 📝 Detalhes Técnicos

### Thread 1 - CPU (funcao_cpu)
- **Operação**: Cálculo de números primos até 1.000.000
- **Características**: 
  - Uso intensivo de CPU
  - Não realiza operações de I/O
  - Tempo de execução depende da capacidade de processamento

### Thread 2 - I/O (funcao_io)
- **Operação**: Escrita e leitura de 10.000 linhas em arquivo
- **Características**:
  - Operações de entrada/saída no disco
  - Menor uso de CPU
  - Tempo de execução depende da velocidade do disco

### Thread 3 - Mista (funcao_mista)
- **Operação**: Cálculos de fatorial intercalados com escrita em arquivo
- **Características**:
  - Combina CPU e I/O
  - Demonstra um padrão comum em aplicações reais
  - Tempo de execução equilibrado entre CPU e I/O

## 🛠️ Modificações Sugeridas

Para aprofundar o aprendizado, experimente:

1. Aumentar/diminuir o `NUM_ITERACOES` para ver o impacto na Thread CPU
2. Adicionar mais threads do mesmo tipo
3. Modificar a prioridade das threads (requer permissões especiais)
4. Adicionar sincronização entre threads usando mutexes
5. Medir o uso de CPU e memória durante a execução

## 📚 Conceitos Importantes

### Threads POSIX (pthread)
- **pthread_create()**: Cria uma nova thread
- **pthread_join()**: Aguarda uma thread terminar
- **pthread_exit()**: Termina a thread atual

### Compilação com pthread
O flag `-pthread` é necessário para linkar a biblioteca de threads POSIX.

## 🐛 Resolução de Problemas

**Erro: "undefined reference to pthread_create"**
- Solução: Certifique-se de compilar com o flag `-pthread`

**Erro: "Permission denied" ao executar**
- Solução: Torne o executável com `chmod +x atividade_threads`

**Docker não encontrado**
- Solução: Instale o Docker seguindo as instruções em https://docs.docker.com/get-docker/

## 📖 Referências

- [POSIX Threads Programming](https://computing.llnl.gov/tutorials/pthreads/)
- [GCC Compiler](https://gcc.gnu.org/)
- [Docker Documentation](https://docs.docker.com/)
- Tanenbaum, A. S. "Modern Operating Systems"

## 👨‍🏫 Para o Professor

Esta atividade pode ser avaliada considerando:
- Compreensão do comportamento das threads
- Capacidade de executar o programa em diferentes ambientes
- Análise crítica dos resultados observados
- Documentação das observações e conclusões

## 📄 Licença

Este projeto está sob a licença especificada no arquivo LICENSE.
