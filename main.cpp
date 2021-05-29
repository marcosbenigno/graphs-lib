#include <iostream>

#include <string>

#include <bits/stdc++.h>

#include <fstream>

#include <vector>

#include <cmath>

#include <chrono>


using namespace std;

class Grafo {

  typedef pair < int, double > Par;

  private:
    int numeroDeVertices;
  int numeroDeArestas;
  string tipoDeDado;
  vector < vector < double >> matrizComPesos;
  vector < vector < int >> estruturaSemPesos;
  vector < vector < int >> matriz;
  vector < vector < vector < double >>> vetorComPesos;
  bool temPesos = false;
  bool temPesosNegativos = false;
  public:

    Grafo(string _fileName, string _tipoDeDado) {
      this -> tipoDeDado = _tipoDeDado;
      criarGrafo(getArestasDeArquivo(_fileName));
    //   relatorio();
    }

  vector < vector < double >> getArestasDeArquivo(string fileName) {

    ifstream file;

    string line;
    file.open(fileName, ios:: in );
    int numeroDaLinha = 0;
    int i = 0;
    vector < vector < double >> arestas;
    if (file.is_open()) {

      while (getline(file, line)) {
        if (numeroDaLinha == 0) {

          setNumeroDeVertices(stoi(line));

        } else {

          arestas.push_back(split(line));

          i++;
        }
        numeroDaLinha++;

      }

      this -> numeroDeArestas = i;
      file.close();

    }

    return arestas;

  }

  vector < double > split(string str) {

    vector < double > vetor;
    istringstream ss(str);
    string palavra;
    int i = 0;
    while (ss >> palavra) {
      // cout<<palavra<<endl;
      if (i == 2) {
        vetor.push_back(stod(palavra));
        if (stod(palavra) < 0) {
          this -> temPesosNegativos = true;
        }

      } else {
        vetor.push_back(stod(palavra) - 1.0);

      }
      i = i + 1;
    }
    //    cout<<vetor[0]<<" "<<vetor[1]<<" "<<vetor[2]<<endl;
    return vetor;
  }

  void criarGrafo(vector < vector < double >> _arestas) {
    arestasTemPesos(_arestas);
    if (this -> tipoDeDado == "matriz") {

      for (int i = 0; i < getNumeroDeVertices(); i++) {

        if (this -> temPesos) {
          vector < double > coluna(getNumeroDeVertices(), -1);

          this -> matrizComPesos.push_back(coluna);
        } else {
          vector < int > coluna(getNumeroDeVertices(), 0);

          this -> estruturaSemPesos.push_back(coluna);
        }
      }

      for (int i = 0; i < _arestas.size(); i++) {
        if (this -> temPesos) {

          this -> matrizComPesos[_arestas[i][0]][_arestas[i][1]] = _arestas[i][2];
          this -> matrizComPesos[_arestas[i][1]][_arestas[i][0]] = _arestas[i][2];
        } else {

          this -> estruturaSemPesos[_arestas[i][0]][_arestas[i][1]] = 1;
          this -> estruturaSemPesos[_arestas[i][1]][_arestas[i][0]] = 1;

        }
      }

    } else if (this -> tipoDeDado == "vetor") {

      for (int i = 0; i < getNumeroDeVertices(); i++) {

        if (this -> temPesos) {
          vector < vector < double > > vertice;
          this -> vetorComPesos.push_back(vertice);
        } else {
          vector < int > vertice;
          this -> estruturaSemPesos.push_back(vertice);
        }
      }

      for (int i = 0; i < _arestas.size(); i++) {
        if (this -> temPesos) {
          vector < double > vec = {
            _arestas[i][1],
            _arestas[i][2]
          };
          this -> vetorComPesos[_arestas[i][0]].push_back(vec);
          vector < double > vec2 = {
            _arestas[i][0],
            _arestas[i][2]
          };
          this -> vetorComPesos[_arestas[i][1]].push_back(vec2);
        } else {
          this -> estruturaSemPesos[_arestas[i][0]].push_back(_arestas[i][1]);
          this -> estruturaSemPesos[_arestas[i][1]].push_back(_arestas[i][0]);
        }
      }
      if (!(this -> temPesos)) {
        for (int i = 0; i < getNumeroDeVertices(); i++) {
          sort(estruturaSemPesos[i].begin(), estruturaSemPesos[i].end());
        }
      } else {
        for (int i = 0; i < getNumeroDeVertices(); i++) {
          sort(vetorComPesos[i].begin(), vetorComPesos[i].end(), [](auto & left, auto & right) {
            return left[0] < right[0];

          });
        }
      }

    }
  }
  void arestasTemPesos(vector < vector < double >> _arestas) {
    //cout<<_arestas[0].size();
    if (_arestas[0].size() == 3) {
      this -> temPesos = true;

    } else {
      this -> temPesos = false;
    }
  }
  int grauDeVertice(int _vertice) {

    _vertice = _vertice - 1;
    int grau = 0;
    if (this -> tipoDeDado == "matriz") {

      for (int i = 0; i < matriz[_vertice].size(); i++) {
        if (!(this -> temPesos)) {
          grau += estruturaSemPesos[_vertice][i];
        } else {
          if (estruturaSemPesos[_vertice][i] != -1) {
            grau += 1;
          }
        }
      }

    } else if (this -> tipoDeDado == "vetor") {
      grau = matriz[_vertice].size();
    }
    return grau;
  }

  vector < int > grausDeVertices() {

    vector < int > graus;

    for (int i = 1; i <= getNumeroDeVertices(); i++) {
      graus.push_back(grauDeVertice(i));

    }

    return graus;
  }

  int grauMax() {

    vector < int > graus = grausDeVertices();
    return *max_element(graus.begin(), graus.end());
  }
  int grauMin() {

    vector < int > graus = grausDeVertices();
    return *min_element(graus.begin(), graus.end());
  }
  double grauMedio() {

    vector < int > graus = grausDeVertices();
    int somaDosGraus = 0;
    for (int i = 0; i < graus.size(); i++) {
      somaDosGraus += graus[i];
    }
    return (somaDosGraus * 1.0) / graus.size();
  }
  void relatorio() {
    if (!(this -> temPesos)) {
    string relatorio;

    relatorio += "Numero de vertices: " + to_string(this -> getNumeroDeVertices()) + '\n';
    relatorio += "Numero de arestas: " + to_string(this -> numeroDeArestas) + '\n';
    relatorio += "Grau minimo de vertice: " + to_string(this -> grauMin()) + '\n';
    relatorio += "Grau maximo de vertice: " + to_string(this -> grauMax()) + '\n';
    relatorio += "Grau medio de vertices: " + to_string(this -> grauMedio()) + '\n';
    relatorio += "Mediana dos graus de vertices: " + to_string(this -> grauMediana()) + '\n';
    relatorio += "Dados sobre componentes conexas:\n" + this -> componentesConexas() + '\n';

    ofstream outfile("grafos_relatorio.txt");

    outfile << relatorio << std::endl;

    outfile.close();
    } else {
    cout<<"Grafo com pesos\n";
    }
  }
  double grauMediana() {

    vector < int > graus = grausDeVertices();
    sort(graus.begin(), graus.end());
    int tamanhoDoVetor = graus.size();
    if ((tamanhoDoVetor % 2) == 0) {
      return ((graus[tamanhoDoVetor / 2] + graus[(tamanhoDoVetor / 2) - 1]) * 1.0) / 2;
    } else {
      return (graus[tamanhoDoVetor / 2] * 1.0);
    }

  }

  int getNumeroDeVertices() {

    return numeroDeVertices;
  }

  void setNumeroDeVertices(int _numeroDeVertices) {

    this -> numeroDeVertices = _numeroDeVertices;
  }

  void BFS(int inicio) {
    if (!this -> temPesos) {
      string arvoreGeradora = "Arvore geradora (BFS) do grafo a partir do vertice " + to_string(inicio) + '\n';
      inicio = inicio - 1;
      bool * visitado = new bool[getNumeroDeVertices()];
      for (int i = 0; i < getNumeroDeVertices(); i++)
        visitado[i] = false;

      vector < int > fila;

      visitado[inicio] = true;
      fila.push_back(inicio);

      int nivel = 0;
      int verticeVisitado;
      while (!fila.empty()) {
        verticeVisitado = fila[0];

        fila.erase(fila.begin());
        if (nivel == 0) {
          arvoreGeradora += "- Vertice: " + to_string(inicio + 1) + "(nivel " + to_string(nivel) + ") e seu pai e --\n";
          nivel = nivel + 1;
        }

        int temVisitado = 0;
        if (this -> tipoDeDado == "matriz") {
          for (int i = 0; i < getNumeroDeVertices(); i++) {
            if (estruturaSemPesos[verticeVisitado][i] == 1 && (!visitado[i])) {

              temVisitado = 1;
              arvoreGeradora += "- Vertice: " + to_string(i + 1) + "(nivel " + to_string(nivel) + ") e seu pai e " + to_string(verticeVisitado + 1) + '\n';
              visitado[i] = true;
              fila.push_back(i);

            }
            if ((i == estruturaSemPesos[verticeVisitado].size() - 1) && temVisitado == 1) {

              nivel = nivel + 1;
            }
          }
        } else if (this -> tipoDeDado == "vetor") {
          for (auto i = estruturaSemPesos[verticeVisitado].begin(); i != estruturaSemPesos[verticeVisitado].end(); i++) {

            if (!visitado[ * i]) {
              temVisitado = 1;

              arvoreGeradora += "- Vertice: " + to_string(( * i) + 1) + "(nivel " + to_string(nivel) + ") e seu pai e " + to_string(verticeVisitado + 1) + '\n';
              visitado[ * i] = true;

              fila.push_back( * i);
            }
            if ((i == estruturaSemPesos[verticeVisitado].end() - 1) && temVisitado == 1) {

              nivel = nivel + 1;
            }

          }
        }

      }

      ofstream outfile("grafo_bfs_arvore.txt");

      outfile << arvoreGeradora << std::endl;

      outfile.close();
    } else {
      cout << "Grafo com pesos." << endl;
    }
  }

  double distancia(int inicio, int fim) {
    if (!this -> temPesos) {
      inicio = inicio - 1;
      fim = fim - 1;
      vector < int > distanciass = distancias(inicio);
      int distancia = distanciass[fim];
      cout << distancia << " distancia" << endl;
      for (int i = 0; i < distanciass.size(); i++) {
        if (i == fim) {

        }
      }
      return distancias(inicio)[fim] * 1.0;
    } else {
      cout << "Grafo com pesos" << endl;

      return distanciaECaminhoComPesos(inicio, fim);
    }
  }

  vector < int > distancias(int inicio) {
    if (!this -> temPesos) {
      vector < int > dist(this -> getNumeroDeVertices(), 0);

      bool * visitado = new bool[getNumeroDeVertices()];
      for (int i = 0; i < getNumeroDeVertices(); i++)
        visitado[i] = false;

      vector < int > fila;

      visitado[inicio] = true;
      fila.push_back(inicio);

      int nivel = 0;
      int verticeVisitado;
      while (!fila.empty()) {
        verticeVisitado = fila[0];

        fila.erase(fila.begin());
        if (nivel == 0) {

          dist[inicio] = nivel;
          nivel = nivel + 1;
        }

        int temVisitado = 0;
        if (this -> tipoDeDado == "matriz") {
          for (int i = 0; i < getNumeroDeVertices(); i++) {
            if (estruturaSemPesos[verticeVisitado][i] == 1 && (!visitado[i])) {

              temVisitado = 1;

              dist[i] = nivel;
              visitado[i] = true;
              fila.push_back(i);

            }
            if ((i == estruturaSemPesos[verticeVisitado].size() - 1) && temVisitado == 1) {

              nivel = nivel + 1;
            }
          }
        } else if (this -> tipoDeDado == "vetor") {
          for (auto i = estruturaSemPesos[verticeVisitado].begin(); i != estruturaSemPesos[verticeVisitado].end(); i++) {

            if (!visitado[ * i]) {
              temVisitado = 1;
              dist[ * i] = nivel;

              visitado[ * i] = true;

              fila.push_back( * i);
            }
            if ((i == estruturaSemPesos[verticeVisitado].end() - 1) && temVisitado == 1) {

              nivel = nivel + 1;
            }

          }
        }

      }

      return dist;
    } else {
      vector < int > vec(this -> getNumeroDeVertices(), -1);
      return vec;
    }
  }

  string componentesConexas() {
    if (!this -> temPesos) {
      string componentesInfo;
      vector < vector < int >> componentes;
      bool * visitado = new bool[getNumeroDeVertices()];
      for (int i = 0; i < getNumeroDeVertices(); i++)
        visitado[i] = false;
      for (int k = 0; k < getNumeroDeVertices(); k++) {
        while (visitado[k] == false) {

          int s = k;

          int numero = 0;

          vector < int > fila;

          visitado[s] = true;
          fila.push_back(s);
          int verticeVisitado;
          vector < int > componente;
          while (!fila.empty()) {

            verticeVisitado = fila[0];
            componente.push_back(verticeVisitado);
            fila.erase(fila.begin());

            if (this -> tipoDeDado == "matriz") {
              for (int i = 0; i < getNumeroDeVertices(); i++) {
                if (estruturaSemPesos[verticeVisitado][i] == 1 && (!visitado[i])) {

                  fila.push_back(i);

                  visitado[i] = true;
                }
              }
            } else if (this -> tipoDeDado == "vetor") {
              for (auto i = estruturaSemPesos[verticeVisitado].begin(); i != estruturaSemPesos[verticeVisitado].end(); i++) {

                if (!visitado[ * i]) {
                  visitado[ * i] = true;
                  fila.push_back( * i);
                }
              }
            }
          }

          componentes.push_back(componente);

        }
      }
      sort(componentes.begin(), componentes.end(), [](const vector < int > & a,
        const vector < int > & b) {
        return a.size() > b.size();
      });
      componentesInfo += "componentes: " + to_string(componentes.size()) + '\n';
      for (int h = 0; h < componentes.size(); h++) {
        int componenteIndice = h + 1;
        componentesInfo += to_string(componentes[h].size()) + " vertices de componente " + to_string(componenteIndice);
        componentesInfo += ": ";
        for (int g = 0; g < componentes[h].size(); g++) {
          componentesInfo += to_string(componentes[h][g] + 1) + " ";
        }
        componentesInfo += "\n\n";
      }
      return componentesInfo;
    } else {

      return "Grafo com pesos";
    }
  }

  int diametro() {
    if (!this -> temPesos) {
      vector < int > distanciass;
      for (int i = 0; i < this -> getNumeroDeVertices(); i++) {
        vector < int > distanciaDaAresta = distancias(i);
        distanciass.push_back( * max_element(distanciaDaAresta.begin(), distanciaDaAresta.end()));
      }

      int diametro = * max_element(distanciass.begin(), distanciass.end());

      return diametro;

    } else {

      return -1;
    }
  }

  vector < vector < double >> dijkstra(int inicio) {
    vector < double > dist(this -> getNumeroDeVertices(), -1);
    vector < double > pais(this -> getNumeroDeVertices(), -1);
    if (this -> temPesosNegativos) {
      vector < vector < double >> vec;
      vec.push_back(dist);
      vec.push_back(pais);
      cout << "GRAFO POSSUI PESOS NEGATIVOS!" << endl;
      return vec;
    }
    inicio = inicio - 1;

    priority_queue < Par, vector < Par > , greater < Par > > filaDePrioridades;

    filaDePrioridades.push(make_pair(0, inicio));
    dist[inicio] = 0;

    while (!filaDePrioridades.empty()) {

      int verticeAtual = filaDePrioridades.top().second;
      filaDePrioridades.pop();

      if (this -> tipoDeDado == "vetor") {
        for (auto i = vetorComPesos[verticeAtual].begin(); i != vetorComPesos[verticeAtual].end(); i++) {

          int verticeVizinho = ( * i)[0];
          double peso = ( * i)[1];

          if ((dist[verticeVizinho] > dist[verticeAtual] + peso) || (dist[verticeVizinho] == -1)) {

            pais[verticeVizinho] = verticeAtual;
            dist[verticeVizinho] = dist[verticeAtual] + peso;

            filaDePrioridades.push(make_pair(dist[verticeVizinho], verticeVizinho));
          }
        }
      } else if (this -> tipoDeDado == "matriz") {
        for (int i = 0; i != this -> getNumeroDeVertices(); i++) {

          int verticeVizinho = i;
          double peso = matrizComPesos[verticeAtual][i];

          if (((dist[verticeVizinho] > dist[verticeAtual] + peso) or(dist[verticeVizinho] == -1)) && (matrizComPesos[verticeAtual][verticeVizinho] != -1)) {

            pais[verticeVizinho] = verticeAtual;
            dist[verticeVizinho] = dist[verticeAtual] + peso;

            filaDePrioridades.push(make_pair(dist[verticeVizinho], verticeVizinho));
          }
        }

      }

    }

    vector < vector < double >> vec;
    vec.push_back(dist);
    vec.push_back(pais);
    return vec;
  }

  double distanciaECaminhoComPesos(int inicio, int fim) {

    fim = fim - 1;
    vector < vector < double >> vec;
    vec = dijkstra(inicio);
    if (!(this -> temPesosNegativos)) {
      double distanciaComPesos = vec[0][fim];
      cout << "Distancia entre " << inicio << " e " << fim + 1 << ": " << distanciaComPesos << endl;

      cout << "Caminho de " << inicio << " para " << fim + 1 << endl;
      int verticeAtual = fim;
      string caminho = "";
      while (verticeAtual != -1) {

        caminho += " " + to_string(verticeAtual + 1);

        verticeAtual = vec[1][verticeAtual];
      }

      cout << caminho;
      cout << endl;
      return distanciaComPesos;
    }
    return -1.0;
  }

  void todasAsDisanciasComPesos(int inicio) {

    vector < vector < double >> vec;
    vec = dijkstra(inicio);
    if (!(this -> temPesosNegativos)) {

      cout << "Distancia:\n";
      for (int i = 0; i < this -> getNumeroDeVertices(); i++)
        cout << to_string(i + 1) << " " << vec[0][i] << endl;

      cout << "-----------CAMINHOS---------\n";
      for (int i = 0; i < this -> getNumeroDeVertices(); i++) {
        cout << inicio << " para " << i + 1 << endl;
        int verticeAtual = i;
        string caminho = "";
        while (verticeAtual != -1) {

          if (i == inicio - 1) {
            caminho += "#";
          } else {
            caminho += " " + to_string(verticeAtual + 1);
          }
          verticeAtual = vec[1][verticeAtual];

        }
        cout << caminho;
        cout << endl;
      }
    }
  }

  void MST(int inicio) {
    string arvoreMST = "";
    vector < double > pais(this -> getNumeroDeVertices(), -1);
    vector < double > dist(this -> getNumeroDeVertices(), -1);

    if (this -> temPesosNegativos) {

      arvoreMST += "GRAFO POSSUI PESOS NEGATIVOS!";

    } else {
      inicio = inicio - 1;

      priority_queue < Par, vector < Par > , greater < Par > > filaDePrioridades;
      filaDePrioridades.push(make_pair(0, inicio));
      dist[inicio] = 0;

      while (!filaDePrioridades.empty()) {
        int verticeAtual = filaDePrioridades.top().second;
        filaDePrioridades.pop();
        if (this -> tipoDeDado == "vetor") {
          for (auto i = vetorComPesos[verticeAtual].begin(); i != vetorComPesos[verticeAtual].end(); i++) {
            int verticeVizinho = ( * i)[0];
            double peso = ( * i)[1];
            if ((dist[verticeVizinho] > peso) || (dist[verticeVizinho] == -1)) {
              pais[verticeVizinho] = verticeAtual;
              dist[verticeVizinho] = peso;

              filaDePrioridades.push(make_pair(dist[verticeVizinho], verticeVizinho));
            }
          }
        } else if (this -> tipoDeDado == "matriz") {
          for (int i = 0; i != this -> getNumeroDeVertices(); i++) {
            int verticeVizinho = i;
            double peso = matrizComPesos[verticeAtual][i];
            if (((dist[verticeVizinho] > peso) or(dist[verticeVizinho] == -1)) && (matrizComPesos[verticeAtual][verticeVizinho] != -1)) {
              pais[verticeVizinho] = verticeAtual;
              dist[verticeVizinho] = peso;

              filaDePrioridades.push(make_pair(dist[verticeVizinho], verticeVizinho));
            }
          }

        }
      }
      arvoreMST += "----------MST de custo " + to_string(accumulate(dist.begin(), dist.end(), 0.00)) + "---------\n";
      arvoreMST += "Pai\tFilho\n";

      for (int i = 1; i < this -> getNumeroDeVertices(); ++i) {

        arvoreMST += to_string((int)(pais[i] + 1)) + '\t' + to_string(i + 1) + "\n";

      }

    }

    ofstream outfile("grafo_mst_arvore.txt");

    outfile << arvoreMST << std::endl;

    outfile.close();
  }

  double excentricidade(int inicio) {
    double ex = 0;
    vector < vector < double >> vec = dijkstra(inicio);

    ex = * max_element(vec[0].begin(), vec[0].end());

    cout << "Excentricidade do vertice " << inicio << ": " << ex << endl;
    return ex;
  }

};

int main(void) {

  Grafo grafo("grafo_W_1.txt", "vetor");
  //Grafo grafo("grafo_W_1.txt", "matriz");

  //Teste 1
  /*grafo.distancia(1,10);
  cout<<'\n';
  grafo.distancia(1,20);
  cout<<'\n';
  grafo.distancia(1,30);
  cout<<'\n';
  grafo.distancia(1,40);
  cout<<'\n';
  grafo.distancia(1,50);*/

  //Teste 2
  //grafo.excentricidade(10);
  //grafo.excentricidade(20);
  //grafo.excentricidade(30);
  //grafo.excentricidade(40);
  //grafo.excentricidade(50);

  //Teste 3
  //using std::chrono::high_resolution_clock;
  //using std::chrono::duration_cast;
  //using std::chrono::duration;
  //using std::chrono::milliseconds;

  //auto acumulador = 0;
  //    for (int i = 0; i < 101; i++) {
  //auto t1 = high_resolution_clock::now();
  //   grafo.excentricidade(i);
  //auto t2 = high_resolution_clock::now();
  //auto ms_int = duration_cast<milliseconds>(t2 - t1);
  //acumulador += ms_int.count();
  //	}  
  //cout <<"Media: "<<to_string(acumulador/100)<<" ms";

  //Teste 4
  //using std::chrono::high_resolution_clock;
  //using std::chrono::duration_cast;
  //using std::chrono::duration;
  //using std::chrono::milliseconds;

  //auto t1 = high_resolution_clock::now();
  //grafo.MST(1);
  //auto t2 = high_resolution_clock::now();
  //auto ms_int = duration_cast<milliseconds>(t2 - t1);
  //cout<<ms_int.count()<<"ms\n";
  return 0;

}
