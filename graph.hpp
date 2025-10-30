#pragma once
#include <bits/stdc++.h>
using namespace std;

/*
  Trabalho de Grafos – Parte 1 e Parte 2
  Autor: (seu nome)
  Curso: Engenharia – 4º período

  Implementação com classes abstratas:
  - Graph: classe base abstrata.
  - GraphList e GraphMatrix: herdam de Graph.
  - Dijkstra: classe base abstrata.
  - DijkstraVector e DijkstraHeap: herdam de Dijkstra.
*/

struct Graph {
    enum Rep { LIST=0, MATRIX=1 };
    int n, m;
    Rep rep;
    static constexpr double INF = 1e100;

    Graph(int N, Rep R) : n(N), m(0), rep(R) {}
    virtual void addEdge(int u, int v, double w) = 0;
    virtual void forEachAdj(int u, const function<void(int,double)>& f) const = 0;
    virtual ~Graph() {}
};

// ---------- Lista de adjacência ----------
struct GraphList : public Graph {
    vector<vector<pair<int,double>>> adj;
    GraphList(int N) : Graph(N, LIST), adj(N+1) {}
    void addEdge(int u, int v, double w) override {
        adj[u].push_back(make_pair(v,w));
        adj[v].push_back(make_pair(u,w));
        m++;
    }
    void forEachAdj(int u, const function<void(int,double)>& f) const override {
        for (size_t i=0; i<adj[u].size(); i++)
            f(adj[u][i].first, adj[u][i].second);
    }
};

// ---------- Matriz de adjacência ----------
struct GraphMatrix : public Graph {
    vector<vector<double>> mat;
    GraphMatrix(int N) : Graph(N, MATRIX), mat(N+1, vector<double>(N+1, INF)) {
        for (int i=1; i<=N; i++) mat[i][i] = 0;
    }
    void addEdge(int u, int v, double w) override {
        mat[u][v] = w; 
        mat[v][u] = w;
        m++;
    }
    void forEachAdj(int u, const function<void(int,double)>& f) const override {
        for (int v=1; v<=n; v++)
            if (u!=v && mat[u][v] < INF)
                f(v, mat[u][v]);
    }
};

// ---------- Leitura de arquivo ----------
inline Graph* loadGraph(string path, Graph::Rep rep) {
    ifstream in(path);
    int N; in >> N;
    Graph *G = (rep==Graph::LIST)? (Graph*) new GraphList(N) : (Graph*) new GraphMatrix(N);
    int u,v; double w;
    while (in >> u >> v) {
        if (!(in >> w)) w = 1.0;
        G->addEdge(u,v,w);
    }
    return G;
}

// ---------- BFS e DFS únicos ----------
pair<vector<int>, vector<int>> bfs(const Graph* G, int s) {
    vector<int> pai(G->n+1,-1), dist(G->n+1,-1);
    queue<int> q; q.push(s);
    pai[s] = s; dist[s] = 0;
    while (!q.empty()) {
        int u = q.front(); q.pop();
        G->forEachAdj(u, [&](int v,double){
            if (dist[v]==-1) {
                dist[v]=dist[u]+1;
                pai[v]=u;
                q.push(v);
            }
        });
    }
    return make_pair(pai,dist);
}

pair<vector<int>, vector<int>> dfs(const Graph* G, int s) {
    vector<int> pai(G->n+1,-1), dist(G->n+1,-1);
    stack<int> st; st.push(s);
    pai[s]=s; dist[s]=0;
    while (!st.empty()) {
        int u = st.top(); st.pop();
        G->forEachAdj(u, [&](int v,double){
            if (pai[v]==-1) {
                pai[v]=u; dist[v]=dist[u]+1;
                st.push(v);
            }
        });
    }
    return make_pair(pai,dist);
}

// ---------- Dijkstra abstrato ----------
struct Dijkstra {
    Graph* G;
    vector<double> dist;
    vector<int> pai;
    vector<char> fechado;
    Dijkstra(Graph* g):G(g){}
    vector<double> run(int s){
        int n=G->n;
        dist.assign(n+1,Graph::INF);
        pai.assign(n+1,-1);
        fechado.assign(n+1,0);
        dist[s]=0; pai[s]=s;
        init(s);
        while(true){
            int u = extrair();
            if(u==-1) break;
            fechado[u]=1;
            G->forEachAdj(u,[&](int v,double w){
                double nd = dist[u]+w;
                if(!fechado[v] && nd<dist[v]){
                    dist[v]=nd; pai[v]=u;
                    atualiza(v);
                }
            });
        }
        return dist;
    }
    virtual void init(int s)=0;
    virtual int extrair()=0;
    virtual void atualiza(int v)=0;
    virtual ~Dijkstra(){}
};

// ---------- Dijkstra (vetor) ----------
struct DijkstraVector : public Dijkstra {
    DijkstraVector(Graph* g):Dijkstra(g){}
    void init(int s) override {}
    int extrair() override {
        int u=-1; double best=Graph::INF;
        for(int i=1;i<=G->n;i++)
            if(!fechado[i] && dist[i]<best){best=dist[i];u=i;}
        return u;
    }
    void atualiza(int v) override {}
};

// ---------- Dijkstra (heap) ----------
struct DijkstraHeap : public Dijkstra {
    priority_queue<pair<double,int>,vector<pair<double,int>>,greater<pair<double,int>>> pq;
    DijkstraHeap(Graph* g):Dijkstra(g){}
    void init(int s) override {
        while(!pq.empty()) pq.pop();
        pq.push(make_pair(0.0,s));
    }
    int extrair() override {
        while(!pq.empty()){
            pair<double,int> top = pq.top(); pq.pop();
            double d = top.first;
            int u = top.second;
            if(!fechado[u] && fabs(d - dist[u]) < 1e-12) return u;
        }
        return -1;
    }
    void atualiza(int v) override { pq.push(make_pair(dist[v],v)); }
};
