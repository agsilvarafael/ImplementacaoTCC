#include<bits/stdc++.h>

using namespace std;

class Grafo {
public:
    int qtdArestas;
    int qtdVertices;
    vector<pair<int,int>> arestas;
    Grafo(){
        string s;
        int a,b;

        //Ler comentarios ate encontrar o "p"
        cin >> s;
        while(s.compare("p")!=0){
            cin >> s;
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
        }
    }
};

Grafo g;
vector<bool> solution;
vector<int> L;

int main(int argc, char const *argv[])
{
    //Grafo g ja foi criado globalmente. O construtor existente le o grafo da entrada
    
    clock_t start = clock();
    solution.assign(g.qtdVertices,false);

    //Preencher L -> arestas nao cobertas
    for (int i = 0; i < g.qtdArestas; i++) {
        L.push_back(i);
    }

    int indice_aleatorio;
    while(L.size()>0){
        indice_aleatorio = rand() % L.size();

        //Se nenhum dos vertices esta na cobertura
        if(!solution[g.arestas[L[indice_aleatorio]].first-1] && !solution[g.arestas[L[indice_aleatorio]].second-1]){
            if(rand() % 2 == 0){
                //Escolhe o primeiro vertice
                solution[g.arestas[L[indice_aleatorio]].first-1] = true;
            }else{
                //Escolhe o segundo vertice
                solution[g.arestas[L[indice_aleatorio]].second-1] = true;
            }
        }
        L.erase(L.begin()+indice_aleatorio);
    }

    int qtd_vertices = 0;
    cout<< "Vertices na Cobertura:" << endl;
    for (int i = 0; i < solution.size(); i++) {
        if(solution[i]){
            cout<< i+1 <<" ";
            qtd_vertices++;
        }
    }
    cout<< "\n\nTotal de Vertices na Cobertura: "<< qtd_vertices << endl;
    
    clock_t nowtime = clock(); 
    double totaltime = double(nowtime - start) / CLOCKS_PER_SEC;
    cout<< "\n\nTempo total: "<< totaltime << endl;

    return 0;
}
