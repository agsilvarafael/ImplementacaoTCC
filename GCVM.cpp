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
vector<bool> v_in_C;
vector<bool> melhor_C;
int solucao_melhor_C = INT32_MAX;
int indice_melhor_troca = 0;
int melhor_solucao = INT32_MAX;
int solucao_atual;
int melhor_qtd_iter;

int solucao() {
        int sum = 0;
        //aumenta o custo em 1 para cada vertice na cobertura. 
        for (int i = 0; i < g.qtdVertices; i++) {
            if(v_in_C[i]){
                sum ++;
            }
        }
        for (int i = 0; i < g.qtdArestas; i++) {
            //verificando se nenhum dos vertices da aresta esta na cobertura.
            if(!v_in_C[g.arestas[i].first-1] && !v_in_C[g.arestas[i].second-1]){
                //aumenta o custo em |V| para cada aresta nao coberta
                sum += g.qtdVertices;
            }
        }
        return sum;
    }

bool randBool() {
    return rand()%2>0;
}

//Passar qtd de iteracoes e qtd de tentativas
int main(int argc, char const *argv[])
{
    //Grafo g ja foi criado globalmente. O construtor existente le o grafo da entrada
    clock_t start = clock();
    int qtd_iter = atoi(argv[1]);
    int qtd_tentativas = atoi(argv[2]);
    
    srand (time(NULL));

    //Criando cobertura aleatoria
    for (int i = 0; i < g.qtdVertices; i++)
    {
        v_in_C.push_back(randBool());
    }    

    for (int tentativa = 0; tentativa < qtd_tentativas; tentativa++)
    {
        melhor_solucao = INT32_MAX;
        int iteracao = 0;
        for (; iteracao < qtd_iter; iteracao++)
        {
            indice_melhor_troca = -1;
            for (int i = 0; i < g.qtdVertices; ++i) {
                //Troca o valor do vertice e testa o valor da nova cobertura
                v_in_C[i]  = !v_in_C[i];
                solucao_atual = solucao();
                if(solucao_atual<melhor_solucao){
                    melhor_solucao = solucao_atual;
                    indice_melhor_troca = i;
                }
                v_in_C[i]  = !v_in_C[i];
            }
            
            if(indice_melhor_troca>=0){
                //Troca o vertice que trara maior ganho
                v_in_C[indice_melhor_troca] = !v_in_C[indice_melhor_troca];
            }
            else{
                //melhor solucao atual ja encontrada
                break;
            }
        }
        if(solucao_melhor_C > melhor_solucao){
            solucao_melhor_C = melhor_solucao;
            melhor_C = v_in_C;
            melhor_qtd_iter = iteracao;
        }
    }

    int qtd_vertices = 0;
    cout<< "Vertices na Cobertura:" << endl;
    for (int i = 0; i < g.qtdVertices; i++) {
        if(melhor_C[i]){
            cout<< i+1 <<" ";
            qtd_vertices++;
        }
    }
    cout<< "\n\nTotal de Vertices na Cobertura: "<< qtd_vertices << endl;
    if(qtd_vertices!=solucao_melhor_C){
        printf("\nO CONJUNTO ACIMA NAO E' UMA COBERTURA: Total de vertices != Funcao Fitness (%d  !=  %d)\nCOBERTURA NAO ENCONTRADA!",qtd_vertices,solucao_melhor_C);
    }
    cout<< "\n\nQuantidade de Iteracoes para Encontrar a Cobertura: "<< melhor_qtd_iter << endl;

    clock_t nowtime = clock(); 
    double totaltime = double(nowtime - start) / CLOCKS_PER_SEC;
    cout<< "\n\nTempo total: "<< totaltime << endl;

    return 0;
}
