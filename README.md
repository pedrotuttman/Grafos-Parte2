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

