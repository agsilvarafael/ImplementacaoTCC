#include "SSA.h"

using namespace std;

class Grafo {
public:
    int qtdArestas;
    int qtdVertices;
    vector<pair<int,int>> arestas;
    Grafo(){
        string s;
        int a,b;

        //Ler comentarios ate encontrar a palavra "p"
        cin >> s;
        while(s.compare("p")!=0){
            cin >> s;
            // cout << s << " ";
        }

        //Ler nome do grafo
        cin >> s;
        //Ler qtd de vertices
        cin >> s;
        qtdVertices = stoi(s);
        //Ler qtd de arestas
        cin >> s;
        qtdArestas = stoi(s);

        //Ler arestas
        for(int i = 0; i<qtdArestas; i++){
            //Ler o "e"
            cin >> s;
            while(s.compare("e")!=0){
                cin >> s;
            }
            //Ler o primeiro vertice
            cin >> s;
            a = stoi(s);
            //Ler o segundo vertice
            cin >> s;
            b = stoi(s);
            arestas.push_back({a,b});
            // cout << a<<" "<<b<<endl;
        }
    }
};

Grafo g;

class MyProblem : public Problem {
public:
    MyProblem(unsigned int dimension) : Problem(dimension) { }
    double eval(const std::vector<double>& solution) {
        double sum = 0.0;

        //aumenta o custo em 1 para cada vertice na cobertura. Se >=0.5, entao esta na cobertura
        for (int i = 0; i < solution.size(); i++) {
            if(solution[i]>=0.5){
                sum += 1;
            }
        }
        for (int i = 0; i < g.qtdArestas; i++) {
            //verificando se nenhum dos vertices da aresta esta na cobertura. Se <0.5, entao nao esta na cobertura
            if(solution[g.arestas[i].first-1]< 0.5 && solution[g.arestas[i].second-1]< 0.5){
                //aumenta o custo em |V| para cada aresta nao coberta
                sum += g.qtdVertices;
            }
        }
        return sum;
    }
};


int main(int argc, char *argv[]){
    SSA ssa(new MyProblem(g.qtdVertices), 30);
    ssa.run(atoi(argv[1]), 1.0, 0.7, 0.1);
    return 0;
}