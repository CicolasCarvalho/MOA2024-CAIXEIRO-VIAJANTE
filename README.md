### Caixeiro Viajante com Algoritmos Heurísticos

analises de dados:
https://docs.google.com/spreadsheets/d/1uNeRed3rUFXcJDx3tkvcuFqQe6Ef3JCYaPZpodyrSOc/edit?usp=sharing
https://docs.google.com/spreadsheets/d/1xTFg_zdOMiRkKhCgjkVeopu4B62PplPZ3cUGDd-Ux8g/edit?usp=sharing

---

#### **Descrição**
Este projeto implementa uma solução para o problema do **Caixeiro Viajante (TSP)** utilizando algoritmos heurísticos. Ele suporta construção de caminhos por meio de *Nearest Neighbor* e *Nearest Insertion* e pode otimizar os caminhos utilizando *Pair Swap* e *2-opt*.

---

#### **Requisitos**
- **Sistema Operacional:** Linux ou Windows. (_testado apenas em Linux_)
- **Compilador:** GCC (recomendado).
- **Ferramentas Adicionais:** Make.

---

#### **Compilação**
1. Certifique-se de que o `gcc` e o `make` estão instalados.
2. Compile o projeto executando:
   ```bash
   make
   ```
3. O executável será gerado no diretório `build` com o nome `out`.

---

#### **Execução**
O programa espera os seguintes argumentos na linha de comando:
```bash
./build/out [--nearest_neighbor|--nearest_insertion] [--pair_swap|--2opt|--none] <caminho_do_arquivo>
```

**Argumentos:**
1. `--nearest_neighbor` ou `--nearest_insertion`: Define o algoritmo heurístico para construção inicial do caminho.
2. `--pair_swap`, `--2opt`, ou `--none`: Escolhe a técnica de otimização a ser aplicada ao caminho construído.
3. `<caminho_do_arquivo>`: Caminho para o arquivo no formato TSP contendo os dados do problema.

**Exemplo de uso:**
```bash
./build/out --nearest_neighbor --2opt resources/test8.tsp
```

---

#### **Formato do Arquivo de Entrada**
O arquivo de entrada deve estar no formato `.tsp`, que contém:
- Número de vértices.
- Coordenadas ou matriz de distâncias entre os vértices.

**Exemplo:**
```text
NAME: test8
TYPE: TSP
COMMENT: 8-city test
DIMENSION: 8
EDGE_WEIGHT_TYPE : EUC_2D
NODE_COORD_SECTION
1 -1 0
2 1 1
3 0 1
4 5 3
5 0 0
6 10 3
7 -4 1
8 0 5
EOF
```

---

#### **Modos de Saída**
- **Modo Benchmark** (`IS_BENCHMARK = 1` em `defs.h`): Apenas exibe o custo total e o tempo de execução.
- **Modo Debug** (`IS_BENCHMARK = 0` em `defs.h`): Exibe detalhes sobre as arestas do caminho e outras informações.

---

#### **Comandos Úteis**
1. **Compilação:**
   ```bash
   make
   ```
2. **Limpeza de arquivos compilados:**
   ```bash
   make clean
   ```

---

#### **Exemplo de Saída**
```text
---#----Graph Size: (8)----#---

Nearest Insertion:
---#----------#---
Total Distance: 34.126161

Tempo decorrido: 0.000126s

Pair Swap:
----------
Total Distance: 33.989112

Tempo decorrido: 0.000020s
```
