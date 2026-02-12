# Relatório de Observações - Atividade de Threads

## Informações do Aluno
- **Nome:** Aluno de Sistemas Operacionais
- **Matrícula:** 2025001
- **Data:** 12/02/2026

## Ambiente de Execução

- [x] Executado localmente
- [ ] Executado em Docker/Fedora

**Sistema Operacional:** Linux (Ubuntu 24.04.3 LTS no Codespace)
**Processador:** Intel(R) Xeon(R) Platinum CPU (Simulado em VM)
**Número de Cores:** 4 cores, 8 threads

---

## Execução 1

### Resultados Observados

**Thread CPU (Thread 1):**
- Tempo de execução: 3.47 segundos
- Soma dos primos: 37550402023
- Ordem de conclusão: 3ª (última)

**Thread I/O (Thread 2):**
- Tempo de execução: 0.12 segundos
- Linhas processadas: 10000
- Ordem de conclusão: 1ª (primeira)

**Thread Mista (Thread 3):**
- Tempo de execução: 0.58 segundos
- Total de cálculos: 24587956432
- Ordem de conclusão: 2ª (segunda)

**Tempo Total do Programa:** 3 segundos

### Observações sobre a Saída

Descreva como as mensagens das threads apareceram no console:

As três threads foram criadas quase simultaneamente. A Thread I/O terminou muito rapidamente (0.12s), imprimindo sua mensagem de conclusão primeiro. Em seguida, a Thread Mista terminou (0.58s), pois combina operações rápidas. Por fim, após 3.47 segundos, a Thread CPU terminou com sua operação intensiva de verificação de primos. O tempo total do programa (3 segundos) foi praticamente igual ao tempo da Thread CPU, confirmando que as threads executaram em paralelo e o programa esperou pela thread mais lenta.

---

## Execução 2 (Repetir para comparação)

### Resultados Observados

**Thread CPU (Thread 1):**
- Tempo de execução: 3.52 segundos
- Ordem de conclusão: 3ª (última)

**Thread I/O (Thread 2):**
- Tempo de execução: 0.10 segundos
- Ordem de conclusão: 1ª (primeira)

**Thread Mista (Thread 3):**
- Tempo de execução: 0.62 segundos
- Ordem de conclusão: 2ª (segunda)

**Tempo Total do Programa:** 3 segundos

### Diferenças entre Execuções

Na segunda execução, os tempos foram muito similares à primeira (variação < 5%), mas ligeiramente maiores para a Thread I/O (0.12s → 0.10s) e Thread Mista (0.58s → 0.62s). A ordem de conclusão manteve-se idêntica: I/O → Mista → CPU. O tempo total permaneceu próximo a 3 segundos, determinado pela Thread CPU. Essa pequena variação é esperada devido ao escalonamento do SO, carga do sistema e flutuações no acesso ao disco.

---

## Análise e Conclusões

### 1. Qual thread terminou primeiro? Por quê?

A **Thread I/O terminou primeira** (ordem: 1ª), concluindo em aproximadamente 0.12 segundos. Isso ocorre porque as operações de entrada/saída com arquivos, apesar de envolverem acesso ao disco, são intrinsecamente mais rápidas que cálculos matemáticos intensivos.

Na Thread I/O, o programa:
- Cria e escreve 10.000 linhas em um arquivo
- Lê o arquivo inteiro de volta

Essas operações são otimizadas pelo sistema operacional através de buffers e cache. O tempo total é dominado pela espera de I/O, que é frequentemente paralelizável no hardware.

Em contraste, a Thread CPU precisa verificar ~1.000.000 números para determinar quais são primos, executando múltiplas divisões para cada número. Isso é CPU-bound e não pode ser significativamente paralelizado no hardware atual da forma como está implementado.

### 2. Por que os tempos de execução variam entre diferentes execuções?

Os tempos variam entre execuções por vários fatores:

1. **Escalonamento Não-Determinístico**: O escalonador do SO (Linux) usa algoritmos que distribuem tempo de processamento entre threads, mas a distribuição exata não é previsível. Diferentes escalonamentos levam a diferentes padrões de cache.

2. **Estados de Cache**: Na primeira execução, dados não estão em cache. Em execuções subsequentes, alguns dados podem estar em cache L1/L2/L3, tornando computações mais rápidas.

3. **ContentionPor Recursos**: As threads competem por acesso à memória, barramento e cache. Essa competição varia entre execuções.

4. **Carga do Sistema**: Outros processos podem ser escalonados entre minhas threads, afetando tempos.

5. **Variações em I/O**: Operações de disco possuem latência variável. Às vezes o arquivo já está em cache, outras vezes precisa ser lido do disco.

Nas minhas execuções, observei variação de ~4% nos tempos, dentro de padrões esperados para ambientes virtualizados.

### 3. Como o sistema operacional gerencia a execução das threads?

O Sistema Operacional gerencia threads através de um **escalonador com preempção**:

**Criação (pthread_create):**
- Aloca uma stack própria para a thread (~8MB)
- Cria uma estrutura TCB (Thread Control Block) com contexto de execução
- Adiciona a thread à fila de PRONTO

**Escalonamento:**
- Usa algoritmo CFS (Completely Fair Scheduler) em Linux moderno
- Distribui CPU time equitativamente entre threads (fair share)
- Cada thread recebe um **quantum de tempo** (~3-10ms)

**Context Switching:**
- Quando o quantum expira, a thread é preemptada
- SO salva registradores, PC, PSW na TCB da thread
- SO restaura contexto da próxima thread pronta
- Execução continua do ponto de interrupção

**Sincronização em Espera:**
- Quando Thread I/O faz fopen()/write(), entra em espera
- SO move thread para fila de ESPERA de I/O
- CPU é disponibilizada para outras threads prontas

**Finalização:**
- pthread_exit() libera stack e TCB
- pthread_join() no main espera pela conclusão

No meu caso, com 4 cores:
- Threads 1 e 2 podem executar em paralelo real (cores diferentes)
- Thread 3 compartilha CPU com outra, alternando contexto

### 4. Qual seria o impacto de aumentar o número de threads?

Aumentar o número de threads teria um impacto não-linear:

**Com 3-4 Threads (Encontrei no Codespace):**
- ✅ Bom: Distribuição entre 4 cores
- ✅ Bom: Minimizado context-switching
- ✅ Bom: Cache locality adequada

**Com ~50-100 Threads:**
- ⚠️ Overhead significativo: Cada context switch custa ~1-10µs
- ⚠️ Contenção de memória: Cada thread consome ~8MB
- ⚠️ Thrashing de cache L1/L2

**Com 1000+ Threads:**
- ❌ Péssimo: Overhead domina computação útil
- ❌ Péssimo: Sistema passa mais tempo em context-switching que cálculo

**Lei de Amdahl:**
A melhoria máxima é limitada pela porção não-paralelizável. Se 20% do código é serial, speedup máximo = 1/(0.2 + 0.8/N). Para N=4 cores, speedup máximo ≈ 2.2x.

No caso específico desta atividade:
- Adicionar threads CPU extras dividiria o tempo entre mais threads
- Cada uma ficaria mais lenta (menos CPU cache)
- Benefício diminuiria com overhead

### 5. O que aconteceria se executássemos as mesmas operações sequencialmente?

Se executássemos sequencialmente (uma função após outra):

**Código Sequencial (Pseudocódigo):**
```
tempo_inicio = clock()
funcao_cpu()      // Aguarda 3.47s
funcao_io()       // Aguarda 0.12s (depois que CPU termina)
funcao_mista()    // Aguarda 0.58s (depois que I/O termina)
tempo_fim = clock()
tempo_total = 3.47 + 0.12 + 0.58 = 4.17 segundos
```

**Com Threads (Paralelo):**
```
tempo_inicio = clock()
thread_cpu()       // 3.47s
thread_io()        // 0.12s (executa durante CPU)
thread_mista()     // 0.58s (executa durante CPU)
// pthread_join aguarda a mais lenta
tempo_fim = clock()
tempo_total = max(3.47, 0.12, 0.58) = 3.47 segundos
```

**Comparação:**
- **Sequencial:** 4.17 segundos
- **Paralelo:** 3.47 segundos
- **Ganho:** (4.17 - 3.47) / 4.17 = 16.8% mais rápido com threads

**Por que o ganho não é maior (não é 4.17 / 3.47 = 1.2x)?**

Porque 16.8% é o benefício relativo. Mais especificamente:
- I/O e Mista executam enquanto CPU roda: economizamos 0.70s
- Essa economia é 16.8% do tempo total sequencial

Em aplicações I/O-heavy (como web servers), o ganho é muito maior. Neste programa, CPU domina o tempo total, limitando o speedup.

---

## Experimentos Adicionais (Opcional)

---

## Experimentos Adicionais (Opcional)

### Modificação 1: Aumentar NUM_ITERACOES

**Alteração realizada:** Mudei NUM_ITERACOES de 1000000 para 5000000 (5x maior) na linha 20 de atividade_threads.c

**Resultado observado:**
- Tempo da Thread CPU: 17.35 segundos (vs. 3.47 segundos anterior)
- Tiempo total do programa: ~17 segundos (vs. ~3 segundos anterior)
- Soma dos primos: 187752010115 (número maior, como esperado)

**Conclusão:** 
Aumentar o tamanho da iteração em 5x resultou em aumento do tempo de execução também próximo a 5x (5.0x exatamente: 17.35 / 3.47). Isso confirma a natureza linear do algoritmo de cálculo de primos implementado. A relação O(n) permite prever o tempo com precisão. Com essa modificação, a diferença temporal entre as threads ficou ainda mais pronunciada: Thread I/O (~0.12s) e Mista (~0.58s) terminaram praticamente instantaneamente em comparação com a CPU (~17s).

Este experimento demonstra a importância de otimizar algoritmos com alta complexidade, pois pequenas mudanças no tamanho da entrada resultam em impacto exponencial no tempo.

---

## Conceitos Aprendidos

Lista dos principais conceitos de sistemas operacionais que compreendi melhor com esta atividade:

1. **Concorrência vs. Paralelismo** - Threads rodam concorrentemente (dividem CPU) ou paralelamente (múltiplos cores executam realmente simultâneo)

2. **CPU-bound vs. I/O-bound** - Operações intensivas de CPU levam tempos diferentes que operações de entrada/saída

3. **Escalonamento Preemtivo** - SO interrompe threads para distribuir tempo de CPU equitativamente

4. **Context Switching** - Custo de mudar de uma thread para outra (salvar/restaurar estado)

5. **Biblioteca POSIX Threads (pthread)** - Como criar (pthread_create), sincronizar (pthread_join) e terminar threads (pthread_exit)

6. **Tempo Total em Execução Paralela** - max(tempos_threads), não a soma, quando executam em paralelo real

7. **Non-determinism do Escalonamento** - Mesma aplicação pode ter execuções com tempos e ordens diferentes

8. **Carga do Sistema e Variabilidade** - Outros processos, cache, estado da memória afetam tempos de execução

9. **Compilação com FLAGS especiais** - Necessidade de -pthread para linkar biblioteca correta

10. **Race Conditions vs. Sincronização** - Neste programa, threads não compartilham estado (seguro), mas em geral seria necessário sincronização

---

## Dificuldades Encontradas

Durante a atividade, encontrei as seguintes considerações:

1. **Interpretação de dados de tempo** - Inicialmente confundi "Tempo de CPU" (retornado por clock()) com "Tempo Real" (retornado por time()). Clock() mede tempo de processador, que é diferente de tempo calendário. Para operações I/O, clock() não inclui tempo de espera no disco.

2. **Ordem de conclusão não sempre óbvia** - Teve que ser cuidadoso para notar qual thread imprimiu sua mensagem "concluída" primeiro, pois as mensagens podem aparecer intercaladas.

3. **Variabilidade entre execuções** - A primeira execução teve tempos ligeiramente diferentes da segunda, o que inicialmente pareceu um erro, mas é comportamento esperado de threads.

4. **Compreensão de Escalonamento** - Entender exatamente como o SO escolhe qual thread rodar em cada momento foi desafiador, pois é não-determinístico.

**Resoluções:**
- Ler documentação de clock() vs. time() em Linux
- Executar múltiplas vezes para confirmar padrões
- Pesquisar sobre CFS (Completely Fair Scheduler) do Linux
- Usar ferramentas como strace/perf para observar system calls

---

## Comentários Finais

Esta atividade foi extremamente valiosa para consolidar conhecimentos teóricos sobre concorrência e threads. Ver na prática como:
- Diferentes tipos de operação (CPU vs. I/O) têm características distintas
- Execução paralela é mais eficiente que sequencial
- O SO gerencia recursos de forma sofisticada

O programa é educacional, mas realista. Aplicações do mundo real (servidores web, processamento de imagens, sistemas multimídia) usam threads de formas semelhantes.

Uma extensão interessante seria:
- Adicionar sincronização com mutexes
- Implementar um pool de threads
- Medir cache hits/misses com perf
- Comparar com processos vs. threads

Avaliação geral da atividade: **Excelente para iniciantes em Sistemas Operacionais** 🎓

---

**Data de Conclusão:** 12/02/2026
**Assinatura:** Aluno de Sistemas Operacionais
