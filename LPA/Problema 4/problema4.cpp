#include <iostream>
#include <cstdio>
#include <algorithm>
#include <vector>
using namespace std;


struct Node {
  int id;
  int days;
  int days_sum;
  bool visited;
  int n_connections;
  int *connections;
  int  n_parents;
};

vector<Node> graph;
vector<Node> graph2;
int nclasses; // Numero de aulas no inicio
int corresp[801]; // Faz corresponder ao nó i do graph o seu nó no novo graph2
int sccchildren[801]; // Marca a 1 os nós que são filhos de um certo nó ao criar o graph2
int nscc = 0; // Numero de SCC's criados
vector<int> stack;
int scc[801][802]; // Guarda para cada SCC encontrado os seus nós, 0 - Contador 1+ - Numero dos nós
int dfs[801];
int low[801];
int index = 0;
int res = 0;


void tarjan(int v) {
  dfs[v] = index;
  low[v] = index;
  index++;
  stack.push_back(v);
  int w;

  for(int i = 0; i < graph[v].n_connections; i++){
    w = graph[v].connections[i];
    if(dfs[w] == -1){
      tarjan(w);
      low[v] = min(low[v],low[w]);
    }
    // if stack contains w
    else if (find(stack.begin(),stack.end(), w) != stack.end()){
      low[v] = min(low[v],dfs[w]);
    }
  }

  if(low[v] == dfs[v]) {
    int aux_days = 0;

    do{
      w = stack.back();
      stack.pop_back();
      corresp[w] = nscc;
      aux_days += graph[w].days;
      scc[nscc][scc[nscc][0] + 1] = w;
      scc[nscc][0]++;
    } while(w != v);

    Node aux;
    aux.id = nscc;
    aux.days = aux_days;
    aux.visited = false;
    aux.n_connections = 0;
    aux.n_parents = 0;
    aux.days_sum = aux_days;
    graph2.push_back(aux);
    nscc++;
  }
}

void topological(){
  int node = 0;

  while(node < nscc) {
    if(!graph2[node].visited && graph2[node].n_parents == 0) {
      res = max(res, graph2[node].days_sum);

      for(int i = 0; i < graph2[node].n_connections; i++) {
        int j = graph2[node].connections[i];
        graph2[j].n_parents--;
        graph2[j].days_sum = max(graph2[j].days_sum, graph2[j].days + graph2[node].days_sum);
      }

      graph2[node].visited = true;
      node = 0;
    }
    else {
      node++;
    }
  }
  graph2.clear();
}


void tarjan_algorithm(){
  stack.clear();
  for(int i = 0; i < nclasses; i++){
    if(dfs[i] == -1){
      tarjan(i);
    }
  }

  for(int i = 0; i < nscc; i++){
    graph2[i].connections = new int[nscc];

    for(int j = 0; j < scc[i][0]; j++){
      Node *aux = &graph[scc[i][j + 1]];
      for(int l = 0; l < aux->n_connections; l++) {
        if(corresp[aux->connections[l]] != i) {
          sccchildren[corresp[aux->connections[l]]] = 1;
        }
      }
    }

    for(int j = 0; j < nscc; j++){
      if(sccchildren[j] == 1){
        graph2[i].connections[graph2[i].n_connections] = j;
        graph2[i].n_connections++;
        graph2[j].n_parents++;
        sccchildren[j] = 0;
      }
    }
  }
}


void reset_mem(){
  index = 0;
  for(int i = 0; i < nclasses; i++){
    dfs[i]  = -1;
    scc[i][0] = 0;
    low[i] = 0;
  }
}


void print_graph(int c) {
  cout << "PRITING GRAPH\n\n";

  for (int i = 0; i < c; i++) {
    cout << "NODE: " << graph[i].id << "\n";
    cout << "visited: " << graph[i].visited << "\n";
    cout << "number of connections: " << graph[i].n_connections << "\n";
    for (int j = 0; j < graph[i].n_connections; j++) {
      cout << graph[i].connections[j] << " ";
    }
    cout << "\n\n";
  }
}


int main(int argc, char const *argv[]) {
  int n, c, r, days;
  scanf("%d", &n);

  for (int i = 0; i < n; i++) {
    scanf("%d %d", &c, &r);
    nclasses = c;
    nscc = 0;
    index = 0;
    res = 0;
    reset_mem();

    for (int j = 0; j < c; j++) {
      scanf("%d ", &days);
      Node aux;
      aux.id = j;
      aux.days = days;
      aux.visited = false;
      aux.n_connections = 0;
      aux.connections = new int[nclasses];
      graph.push_back(aux);
    }

    for (int j = 0; j < r; j++) {
      int dependent, independent;
      scanf("%d %d", &dependent, &independent);
      dependent--; independent--;
      graph[dependent].connections[graph[dependent].n_connections] = independent;
      graph[dependent].n_connections++;
    }

    //print_graph(c);
    tarjan_algorithm();
    graph.clear();
    topological();
    printf("%d\n",res);
  }

  return 0;
}
