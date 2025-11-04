# 🧮 Trabalho de Grafos – Parte 2  
### COS242 – Teoria dos Grafos | UFRJ  

![C++](https://img.shields.io/badge/language-C++17-blue.svg)  
![Status](https://img.shields.io/badge/status-Conclu%C3%ADdo-success.svg)  
![License](https://img.shields.io/badge/license-MIT-lightgrey.svg)  
![UFRJ](https://img.shields.io/badge/UFRJ-Engenharia%20de%20Computa%C3%A7%C3%A3o%20e%20Informa%C3%A7%C3%A3o-yellow.svg)

---

## 🎯 Objetivo

Nesta Parte 2, o projeto reestrutura a solução da Parte 1 usando **orientação a objetos**, com **classes abstratas e herança** para evitar duplicação de código e permitir trocar de representação de grafo/estrutura de prioridade com mínima mudança:

- **Graph (abstrata)** → interfaces comuns (`forEachAdj`, `addEdge`)  
  - `GraphVector` (vetor de adjacência, memória `O(n+m)`)  
  - `GraphMatrix` (matriz de adjacência, memória `O(n^2)`)  
- **Dijkstra (abstrata)** → fluxo comum (`run`, `init`, `extrair`, `atualiza`)  
  - `DijkstraVector` (extração por varredura, `O(n^2)`)  
  - `DijkstraHeap` (heap/priority queue, `O((n+m) log n)`)

Além da arquitetura, esta parte inclui **estudos de caso** com medições e gráficos comparando as variantes.

---

## 🧩 Estrutura do Projeto

.
├── main.cpp # CLI (flags, construção e orquestração)
├── graph.hpp # Classes: Graph (abstrata), GraphVector, GraphMatrix,
│ # Dijkstra (abstrata), DijkstraVector, DijkstraHeap
├── trabalho-P2.pdf # Enunciado oficial da Parte 2
├── Grafos_Parte1.pdf # Relatório da Parte 1 (referência)
├── Relatorio_Parte2.pdf # Relatório desta parte (compilado do LaTeX)
├── Estudo_de_Casos_Tabelas_2.xlsx# Tabelas preenchidas (tempos, distâncias, etc.)
├── diagrama.png # Diagrama da arquitetura (usado no relatório)
├── grafico1.png # Tempo médio: Dijkstra vetor vs heap
├── grafico3.png # Speedup (vetor/heap)
└── README.md # Este arquivo

yaml
Copiar código

> **Observação:** Os nomes dos arquivos de grafo seguem o padrão do enunciado (ex.: `grafo_W_1.txt` … `grafo_W_5.txt`).

---

## ⚙️ Compilação

Requer **g++** compatível com **C++17**:

```bash
g++ -std=c++17 -O2 main.cpp -o grafos
▶️ Execução (CLI)
🔧 Representação do Grafo
--rep=vector → vetor de adjacência

--rep=matrix → matriz de adjacência

🧠 Flags principais
Flag	Descrição
--in=arquivo.txt	arquivo com o grafo (V na 1ª linha, depois arestas com pesos)
`--rep=vector	matrix`
--src=X --dst=Y	vértices de origem e destino para o caminho mínimo
--bench-dijkstra-vec=K	executa Dijkstra (varredura) K vezes e imprime tempo médio (ms)
--bench-dijkstra-heap=K	executa Dijkstra (heap) K vezes e imprime tempo médio (ms)

Saída padrão: sempre mostra distância e caminho mínimo (src → dst).
Benchmarks: imprimem a média em ms (com checagem para evitar otimizações “zerarem” o tempo).

📌 Exemplos
(1) Caminho mínimo + tempo médio com heap (100 repetições):

bash
Copiar código
./grafos --in=grafo_W_2.txt --rep=vector --src=10 --dst=40 --bench-dijkstra-heap=100
(2) Apenas distância/caminho com varredura:

bash
Copiar código
./grafos --in=grafo_W_1.txt --rep=matrix --src=10 --dst=20
(3) Benchmark comparativo (troque apenas a flag):

bash
Copiar código
./grafos --in=grafo_W_3.txt --rep=vector --src=10 --dst=20 --bench-dijkstra-vec=100
./grafos --in=grafo_W_3.txt --rep=vector --src=10 --dst=20 --bench-dijkstra-heap=100
🏗️ Arquitetura (resumo)


Herança (setas contínuas):
GraphVector/GraphMatrix ← Graph (abstrata)
DijkstraVector/DijkstraHeap ← Dijkstra (abstrata)

Dependência (seta tracejada):
Dijkstra usa um Graph (chama forEachAdj(u, f) para relaxamentos).

Complexidades:

Dijkstra Vector: O(n^2)

Dijkstra Heap: O((n+m) log n)

GraphVector memória O(n+m) • GraphMatrix memória O(n^2)

📚 Estudos de Caso
As tabelas em Estudo_de_Casos_Tabelas_2.xlsx incluem:

Distância e caminho mínimo (ex.: src=10, dst ∈ {20, 30, 40, 50, 60});

Tempo médio (em ms) do Dijkstra com vetor e com heap (para K repetições);

Observações de falha por std::bad_alloc em representações matriciais grandes.

Importante: Nos testes, heap foi consistentemente mais rápido, com speedup crescente conforme V e E aumentam (grafos esparsos).

Gráficos usados no relatório
grafico1.png – Tempo médio (ms): Dijkstra (vetor) vs Dijkstra (heap)

grafico3.png – Speedup (vetor/heap) — > 1 indica vantagem do heap

🧪 Notas de Benchmark
O tempo mostrado é em milissegundos (ms) e corresponde ao tempo médio sobre K execuções.

Para evitar que o compilador elimine o trabalho (otimização), o código usa o resultado (ex.: soma de dist[dst] em um checksum).

I/O de arquivos não entra na cronometria; medimos apenas o núcleo do algoritmo.

⚠️ Limitações e decisões
std::bad_alloc em matriz: grafos grandes com representação matricial (O(n^2)) podem exceder a memória disponível → optamos por vetor de adjacência para escalar.

Precisão de tempos: rodar em ambiente estável (sem outros processos pesados) e usar K adequado para reduzir ruído.

Uniformização das saídas: distância e caminho sempre são exibidos, independente da representação/variante do Dijkstra.

💻 Ambiente
Linguagem: C++17

Compilador: g++ (MinGW/WSL/Linux)

Otimizador: -O2

SO: Windows/Linux (testado em ambos)

🧍 Autor
Pedro Tuttman Cysne Soares — DRE 124023584
Engenharia de Computação e Informação — UFRJ
Repositório: https://github.com/pedrotuttman/Grafos-Parte2

📄 Licença
Distribuído sob MIT License. Livre para uso/modificação com atribuição.

📘 Referências
Enunciado Parte 2: trabalho-P2.pdf

Relatório Parte 1 (referência): Grafos_Parte1.pdf

Relatório desta parte: Grafos_Parte2.pdf

Tabelas: Estudo_de_Casos_Tabelas_2.xlsx

