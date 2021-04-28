#include <iostream>

#include <string>

#include <bits/stdc++.h>

#include <fstream>

#include <vector>

#include <cmath>

#include <chrono>

using namespace std;

class Grafo {

  private:
    int numeroDeVertices;
  int numeroDeArestas;
  string tipoDeDado;
  vector < vector < int >> matriz;

  public:

    Grafo(string _fileName, string _tipoDeDado) {
      this -> tipoDeDado = _tipoDeDado;
      criarGrafo(getArestasDeArquivo(_fileName));
      relatorio();
      

    }

  vector < vector < int >> getArestasDeArquivo(string fileName) {

    ifstream file;

    string line;
    file.open(fileName, ios:: in );
    int numeroDaLinha = 0;
    int i = 0;
    vector < vector < int >> arestas;
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

  vector < int > split(string str) {

    vector < int > vetor;
    istringstream ss(str);
    string palavra;
    while (ss >> palavra) {
      vetor.push_back(stoi(palavra) - 1);
    }
    return vetor;
  }

  void criarGrafo(vector < vector < int >> _arestas) {

    if (this -> tipoDeDado == "matriz") {

      for (int i = 0; i < getNumeroDeVertices(); i++) {
        vector < int > coluna(getNumeroDeVertices(), 0);

        this -> matriz.push_back(coluna);
      }
      for (int i = 0; i < _arestas.size(); i++) {

        this -> matriz[_arestas[i][0]][_arestas[i][1]] = 1;
        this -> matriz[_arestas[i][1]][_arestas[i][0]] = 1;
      }

      for (int i = 0; i < getNumeroDeVertices(); i++) {

        for (int j = 0; j < getNumeroDeVertices(); j++) {
          cout << this -> matriz[i][j] << " ";
        }
        cout << "\n";
      }
    } else if (this -> tipoDeDado == "vetor") {

      for (int i = 0; i < getNumeroDeVertices(); i++) {
        vector < int > vertice;
        this -> matriz.push_back(vertice);

      }

      for (int i = 0; i < _arestas.size(); i++) {

        this -> matriz[_arestas[i][0]].push_back(_arestas[i][1]);
        this -> matriz[_arestas[i][1]].push_back(_arestas[i][0]);

      }

      for (int i = 0; i < getNumeroDeVertices(); i++) {
        sort(matriz[i].begin(), matriz[i].end());
      }
      for (int i = 0; i < getNumeroDeVertices(); i++) {

        for (int j = 0; j < this -> matriz[i].size(); j++) {

          cout << this -> matriz[i][j] << " ";
        }
        cout << "\n";
      }

    }
  }

  int grauDeVertice(int _vertice) {

    _vertice = _vertice - 1;
    int grau = 0;
    if (this -> tipoDeDado == "matriz") {

      for (int i = 0; i < matriz[_vertice].size(); i++) {
        grau += matriz[_vertice][i];

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
          if (matriz[verticeVisitado][i] == 1 && (!visitado[i])) {

            temVisitado = 1;
            arvoreGeradora += "- Vertice: " + to_string(i + 1) + "(nivel " + to_string(nivel) + ") e seu pai e " + to_string(verticeVisitado + 1) + '\n';
            visitado[i] = true;
            fila.push_back(i);

          }
          if ((i == matriz[verticeVisitado].size() - 1) && temVisitado == 1) {

            nivel = nivel + 1;
          }
        }
      } else if (this -> tipoDeDado == "vetor") {
        for (auto i = matriz[verticeVisitado].begin(); i != matriz[verticeVisitado].end(); i++) {

          if (!visitado[ * i]) {
            temVisitado = 1;

            arvoreGeradora += "- Vertice: " + to_string(( * i) + 1) + "(nivel " + to_string(nivel) + ") e seu pai e " + to_string(verticeVisitado + 1) + '\n';
            visitado[ * i] = true;

            fila.push_back( * i);
          }
          if ((i == matriz[verticeVisitado].end() - 1) && temVisitado == 1) {

            nivel = nivel + 1;
          }

        }
      }

    }

    ofstream outfile("grafo_bfs_arvore.txt");

    outfile << arvoreGeradora << std::endl;

    outfile.close();
  }

  int distancia(int inicio, int fim) {
    inicio = inicio - 1;
    fim = fim - 1;
    vector < int > distanciass = distancias(inicio);
    int distancia = distanciass[fim];
    cout << distancia << " distancia" << endl;
    for (int i = 0; i < distanciass.size(); i++) {
      if (i == fim) {

      }
    }
    return distancias(inicio)[fim];

  }

  vector < int > distancias(int inicio) {
    
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
          if (matriz[verticeVisitado][i] == 1 && (!visitado[i])) {

            temVisitado = 1;

            dist[i] = nivel;
            visitado[i] = true;
            fila.push_back(i);

          }
          if ((i == matriz[verticeVisitado].size() - 1) && temVisitado == 1) {

            nivel = nivel + 1;
          }
        }
      } else if (this -> tipoDeDado == "vetor") {
        for (auto i = matriz[verticeVisitado].begin(); i != matriz[verticeVisitado].end(); i++) {

          if (!visitado[ * i]) {
            temVisitado = 1;
            dist[ * i] = nivel;

            visitado[ * i] = true;

            fila.push_back( * i);
          }
          if ((i == matriz[verticeVisitado].end() - 1) && temVisitado == 1) {

            nivel = nivel + 1;
          }

        }
      }

    }

    return dist;
  }

  string componentesConexas() {
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
              if (matriz[verticeVisitado][i] == 1 && (!visitado[i])) {

                fila.push_back(i);

                visitado[i] = true;
              }
            }
          } else if (this -> tipoDeDado == "vetor") {
            for (auto i = matriz[verticeVisitado].begin(); i != matriz[verticeVisitado].end(); i++) {

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
  }

  int diametro() {
    vector < int > distanciass;
    for (int i = 0; i < this -> getNumeroDeVertices(); i++) {
      vector < int > distanciaDaAresta = distancias(i);
      distanciass.push_back( * max_element(distanciaDaAresta.begin(), distanciaDaAresta.end()));
    }

    int diametro = * max_element(distanciass.begin(), distanciass.end());

    return diametro;
  }

};

int main(void) {

  //Grafo grafo("grafo_2.txt", "vetor");
  //Grafo grafo("grafo_2.txt", "matriz");
  
  
  // using std::chrono::high_resolution_clock;
      // using std::chrono::duration_cast;
      // using std::chrono::duration;
      // using std::chrono::milliseconds;
      //TESTE: Mil BFS's e tempo de execucao
      // auto t1 = high_resolution_clock::now();
      //    for (int i = 0; i < 1000; i++) {
      //      grafo.BFS(i);
      //  
      //	}
      // auto t2 = high_resolution_clock::now();
      // auto ms_int = duration_cast<milliseconds>(t2 - t1);
      // cout <<"Mil bfs's: "<<ms_int.count()<<" ms";
//------
      //TESTE: Diametro e tempo de execucao
      // auto tinicio = high_resolution_clock::now();      
      //cout<<"diametro :"<<grafo.diametro()<<endl;
      // auto tfim = high_resolution_clock::now();
      // auto ms_int_2 = duration_cast<milliseconds>(tfim - tinicio);
      // cout <<"Diametro: "<<ms_int_2.count()<<" ms";
//------
      //TESTE: Distancia entre vertices
      //cout<<"distancia :"<<"10,20 "<<grafo.distancia(10,20)<<endl;
      //cout<<"distancia :"<<"10,30 "<<grafo.distancia(10,30)<<endl;
      //cout<<"distancia :"<<"20,30 "<<grafo.distancia(20,30)<<endl;
  //------    
      //TESTE: Componentes conexas:
      //cout<<grafo.componentesConexas(); 
//-------
  	//TESTE: Paternidade de vertices:
  	//grafo.BFS(1);
  	//grafo.BFS(2);
  	//grafo.BFS(3);
  return 0;

}
