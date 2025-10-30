#include <bits/stdc++.h>
#include "graph.hpp"
using namespace std;

/*
  Execução:
    g++ -std=c++17 -O2 -o grafos main.cpp
    ./grafos --in=grafo_W_1.txt --rep=list --src=10 --dst=20 --bench-dijkstra-heap=100
*/

static vector<int> build_path(int s, int t, const vector<int>& pai) {
    vector<int> path;
    if (t<0 || t>=(int)pai.size()) return path;
    if (pai[t]==-1) return path;
    for (int v=t; v!=s && v!=-1; v=pai[v]) path.push_back(v);
    path.push_back(s);
    reverse(path.begin(), path.end());
    return path;
}

int main(int argc, char** argv) {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string in, repStr;
    int benchVec = 0, benchHeap = 0;
    int src = 10, dst = 20; // padrão

    for (int i = 1; i < argc; i++) {
        string a = argv[i];
        if (a.rfind("--in=", 0) == 0) in = a.substr(5);
        else if (a.rfind("--rep=", 0) == 0) repStr = a.substr(6);
        else if (a.rfind("--bench-dijkstra-vec=", 0) == 0)
            benchVec = stoi(a.substr(string("--bench-dijkstra-vec=").size()));
        else if (a.rfind("--bench-dijkstra-heap=", 0) == 0)
            benchHeap = stoi(a.substr(string("--bench-dijkstra-heap=").size()));
        else if (a.rfind("--src=", 0) == 0) src = stoi(a.substr(6));
        else if (a.rfind("--dst=", 0) == 0) dst = stoi(a.substr(6));
    }

    Graph::Rep rep = (repStr == "matrix" ? Graph::MATRIX : Graph::LIST);
    Graph* G = loadGraph(in, rep);

    cout << "V=" << G->n << " E=" << G->m
         << " rep=" << (rep == Graph::LIST ? "LIST" : "MATRIX") << "\n";

    // --- (1) sempre imprime distância e caminho (uma execução com heap) ---
    {
        DijkstraHeap dij(G);
        vector<double> dist = dij.run(src);

        cout.setf(ios::fixed);
        cout << setprecision(3);

        if (dst>=1 && dst<=G->n && dist[dst] < Graph::INF/2) {
            vector<int> path = build_path(src, dst, dij.pai);
            cout << "dist("<<src<<","<<dst<<")="<<dist[dst]<<" path:";
            for (int v : path) cout << " " << v;
            cout << "\n";
        } else {
            cout << "dist("<<src<<","<<dst<<")=INF path:\n";
        }

        cout.unsetf(ios::floatfield);
    }

    // --- (2) benchmarks (média) ---
    mt19937_64 rng(123);
    uniform_int_distribution<int> pick(1, G->n);

    auto bench = [&](bool heap, int k) {
        using clk = chrono::steady_clock;
        auto t0 = clk::now();

        double soma = 0; // evita otimização

        if (heap) {
            DijkstraHeap dij(G);
            for (int i = 0; i < k; i++) {
                auto dist = dij.run(pick(rng));
                soma += dist[1];
            }
        } else {
            DijkstraVector dij(G);
            for (int i = 0; i < k; i++) {
                auto dist = dij.run(pick(rng));
                soma += dist[1];
            }
        }

        auto t1 = clk::now();
        volatile double evitar_otimizacao = soma; (void)evitar_otimizacao;

        return chrono::duration<double, milli>(t1 - t0).count() / k;
    };

    if (benchVec > 0) {
        double tempo = bench(false, benchVec);
        cout.setf(ios::fixed);
        cout << setprecision(3);
        cout << "bench_dijkstra_vec_ms=" << tempo << "\n";
    }
    if (benchHeap > 0) {
        double tempo = bench(true, benchHeap);
        cout.setf(ios::fixed);
        cout << setprecision(3);
        cout << "bench_dijkstra_heap_ms=" << tempo << "\n";
    }

    return 0;
}
