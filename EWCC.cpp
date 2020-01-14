//g++ -o -O2 -std=c++11
#include <bits/stdc++.h>
#define memcle(a) memset(a, 0, sizeof(a))
#define debug(x) cerr << #x << " = " << x << ' '
#define deln(x) cerr << #x << " = " << x << endl
using namespace std;

typedef pair<int, int> PI;
const int N = 5010;
const int M = 1500010;
const int inf = 0x7fffffff;

char *inputPath, *outputPath;
int utl;
PI S[N * 2];
int n, m, en, mm;
int sb, jump;
//vector<int> adj[N], eid[N];
int last[N], nex[M * 2], eid[M * 2], adj[M * 2];
int lastuncover[M], weight[M], dscore[N];
int C[N], ansC[N], tmpC[N];
bool confChange[N];
int ub, step;
int Lcnt, Ccnt;
int cost;
PI tmp[N];
int tn;
clock_t start;

int delta;
int maxSteps;

int lastremove;
int lastadd;

void read(int &x)
{
    char c = getchar(); 
    for (; c < '0' || c > '9'; ) c = getchar();
    x = 0;
    for (; c >= '0' && c <= '9'; ) x = x * 10 + c - '0', c = getchar();
}

void readp()
{
    char c = getchar(); 
    for (;c == 'c'; c = getchar()){ 
        //se a primeira letra for um c, entao a linha e um comentario
        while(c!='\n'){
            c = getchar();
        }
    }
    for (; ; c = getchar()){ 
        //if(c=='\n'){
           // c = getchar();
            if(c == 'p'){
                break;
            }
        //}
    }
}

double reportTime()
{
    clock_t nowtime = clock(); 
    double totaltime = double(nowtime - start) / CLOCKS_PER_SEC;
    deln(totaltime);
    return totaltime;
}

class Edge
{
    public : 
    int x, y; 
    Edge(int _x = 0, int _y = 0) : x(_x), y(_y) {}
} edge[M]; 

class EdgeinL
{
    public :
    int id, last;
    EdgeinL(int _i = 0, int _l = 0) {id = _i, last = _l;}
};

list<EdgeinL> L, UL;

void link(int x, int y)
{
    edge[++en] = Edge(x, y);
    /*adj[x].push_back(y);
    eid[x].push_back(en);
    adj[y].push_back(x);
    eid[y].push_back(en);
    */
    adj[++mm] = y, eid[mm] = en, nex[mm] = last[x], last[x] = mm;
    adj[++mm] = x, eid[mm] = en, nex[mm] = last[y], last[y] = mm;
}

void init()
{
 //   cout << "Comeco Leitura\n";
    freopen(inputPath, "r", stdin);
    readp();
    read(n); read(m);
    for (int i = 1; i <= m; i++)
    {
        int x, y;
        read(x); read(y);
 //cout<<x<<"   "<<y<<endl;
        link(x, y);
    }
    fclose(stdin);
  //  cout << "FIM Leitura\n";
}

struct node
{
    int x, deg;
    node(int _x, int _deg) {x = _x, deg = _deg;}
};

bool operator <(const node &a, const node &b) {return a.deg < b.deg;}

void Greedy(int *pick) // extends C and return the size of extend pick result
{
    copy(C + 1, C + 1 + n, pick + 1);
    //deg = uncover adjcent edge number 
    priority_queue<node> heap;
    int deg[N];
    memcle(deg);
    int res = 0; // the numbers of picked vertexes
    for (int i = 1; i <= n; i++)
        if (!pick[i])
        {
            for (int p = last[i]; p; p = nex[p])
                if (!pick[adj[p]])
                    deg[i]++;
            heap.push(node(i, deg[i]));
        } else res++;

    for (;;)
    {
        node now = heap.top(); heap.pop();
        if (now.deg == 0) break;
        int x = now.x;
        int dg = now.deg;
        if (deg[x] != dg || pick[x]) continue;
        pick[x] = 1;
        res++;
        for (int p = last[x]; p; p = nex[p])
        {
            int y = adj[p];
            if (pick[y]) continue;
            deg[y]--;
            heap.push(node(y, deg[y]));
        }
    }

    if (res < ub)
    {
        ub = res;
        deln(step);
        deln(ub);
        double ti = reportTime();
        char s[10]; double oldtime; int oldnum = 100000;
        try
        {
            freopen(outputPath, "r", stdin);
            cin >> s >> s >> oldtime;
            cin >> oldnum;
            fclose(stdin);
        } catch (...)
        {

        }
        if (ub < oldnum || (ub <= oldnum && oldtime > ti))
        {
            deln(inputPath);
            deln(outputPath);
            cerr << "update !!!!!!!!!!!!" << endl;
            freopen(outputPath, "w", stdout);
            printf("totaltime = %.3f\n", ti);
            printf("%d\n", ub);
            for (int i = 1; i <= n; i++) if (pick[i]) printf("%d ", i); puts("");
            fclose(stdout);
        }
    }}

int ra(int x) {return ((rand() << 15) + (rand())) % x;}

void remove(int u)
{
    if (!C[u]) 
        cerr << "remove a vertex not in C!!" << endl;
    dscore[u] = 0;
    confChange[u] = 0;
    for (int p = last[u]; p; p = nex[p])
    {
        int v = adj[p];
        int cur = eid[p];
        if (!C[v])
        {
            confChange[v] = 1;
            cost += weight[cur];
            lastuncover[cur] = step; 
            L.push_back(EdgeinL(cur, step));
            Lcnt++;

            UL.push_back(EdgeinL(cur, step));
            dscore[v] += weight[cur];
            dscore[u] += weight[cur];
        } else dscore[v] -= weight[cur];
    }
    C[u] = 0;
    Ccnt--;
    lastremove = u;
}

void add(int u)
{
    if (C[u]) cerr << "add a vertex already in C!!" << endl;
    dscore[u] = 0;
    confChange[u] = 0;
    for (int p = last[u]; p; p = nex[p])
    {
        int v = adj[p];
        int cur = eid[p];
        if (!C[v])
        {
            confChange[v] = 1;
            lastuncover[cur] = -1;
            cost -= weight[cur];
            dscore[v] -= weight[cur];
            dscore[u] -= weight[cur];
            Lcnt--;
            //L and UL use lazydelete
        } else 
        {
            dscore[v] += weight[cur];
        }
    }
    C[u] = 1;
    Ccnt ++;
    lastadd = u;
}

void remove_some_vertices()
{
    /*int a[N];
    int nn = 0;
    for (int i = 1; i <= n; i++) if (C[i]) a[++nn] = i;
    random_shuffle(a + 1, a + 1 + nn, ra);
    for (int i = 1; i <= delta && i < nn; i++)
        remove(a[i]);*/
    int nn = 0;
    for (int i = 1; i <= n; i++) if (C[i]) tmp[++nn] = make_pair(dscore[i], i);
    sort(tmp + 1, tmp + 1 + nn, greater<PI>());
    int i = 1; 
    for (; Lcnt == 0; i++) remove(tmp[i].second);
    for (; i <= delta && i < nn; i++)
        remove(tmp[i].second);
}

void append_new_pair(int v, PI *S, int &sn)
{
    for (int p = last[v]; p; p = nex[p])
    {
        int u = adj[p];
        int cur = eid[p];
        if (C[u] && dscore[v] + dscore[u] + weight[cur] > 0 && (u != lastadd || v != lastremove) && (confChange[v] == 1)) 
            S[sn++] = make_pair(u, v);
            //S.push_back(make_pair(u, v));
    }
    //if (S.size() != 0) return;
    /*
    int l = lower_bound(tmp + 1, tmp + 1 + tn, make_pair(-dscore[v] + 1, 0)) - tmp;
    for (int i = l; i <= tn; i++)
        if (v != lastremove || tmp[i].second != lastadd)
            S.push_back(make_pair(tmp[i].second, v));
    */
    for (int i = 1; i <= n; i++)
        if (C[i] && (v != lastremove || i != lastadd) && dscore[i] + dscore[v] > 0 && confChange[v] == 1)
            S[sn++] = make_pair(i, v);
}

PI ChooseExchangePair() // return the label of edge
{
    //tn = 0; ///////////// ??? 
    //for (int i = 1; i <= n; i++) if (C[i]) tmp[++tn] = make_pair(dscore[i], i);
    //sort(tmp + 1, tmp + 1 + tn);
    int u, v; 
    // in L we search the oldest edge 
    auto it = L.begin();
    for ( ; it != L.end(); )
    {
        if (lastuncover[it -> id] != it -> last) it = L.erase(it); else break;
    }
    if (it == L.end()) return make_pair(0, 0);

    int sn = 0;
    int cur = it -> id;
    append_new_pair(edge[cur].x, S, sn);
    append_new_pair(edge[cur].y, S, sn);
    if (sn > 0) 
    {
        ++jump;
        return S[ra(sn)];
    }

    it = UL.begin(); 
    for (; it != UL.end(); )
    {
        if (lastuncover[it -> id] != it -> last) it = UL.erase(it); else 
        {
            int cur = it -> id;              
            sn = 0;
            append_new_pair(edge[cur].x, S, sn);
            append_new_pair(edge[cur].y, S, sn);
            it = UL.erase(it);
            //if (S.size() > 0) return S[ra(S.size())];
            if (sn > 0) return S[ra(sn)];
        }
    }

    return make_pair(0, 0);
} 

void init_of_ewls()
{
    for (int i = 1; i <= n; i++) confChange[i] = 1;
    memcle(C); // C is a tank which store the picked vertices 
    ub = inf;
    Greedy(C); // use greedy to initilaize C, ub is the size of answer
    Ccnt = ub;
    copy(C + 1, C + 1 + n, ansC + 1); // the best answer store in ansC
    
    //initialize L, UL, dscore, lastuncover
    step = 0;
    for (int i = 1; i <= m; i++)
    {
        weight[i] = 1;
        int x = edge[i].x;
        int y = edge[i].y;
        if (!C[x] && C[y]) swap(x, y);

        if (C[x] && C[y])
        {
            lastuncover[i] = -1;
        } else 
        if (C[x] && !C[y])
        {
            lastuncover[i] = -1;
            dscore[x] -= weight[i];
            //dscore[y] += weight[i];
        } else
        if (!C[x] && !C[y])
        {
            lastuncover[i] = 0;
            dscore[x] += weight[i];
            dscore[y] += weight[i];
            L.push_back(EdgeinL(i, 0)); Lcnt++;
            UL.push_back(EdgeinL(i, 0));
        }
    }

    remove_some_vertices();
}

void ewls()
{
    init_of_ewls();
    int rate = 0;
    for (step = 0; step < maxSteps; step++)
    {
        int nowrate = int(step * 100.0 / maxSteps);
        clock_t finish = clock();
        double ti = double(finish - start) / CLOCKS_PER_SEC;
        if (ti > utl)  break;
            
        if (nowrate != rate) 
        {
            rate = nowrate;
            // deln(rate);
            // reportTime();
            // deln(step);
            clock_t finish = clock();
            double ti = double(finish - start) / CLOCKS_PER_SEC;
            if (ti > utl)  break;
            // deln(ub);
            // deln(cost);
            // deln(jump);
        }
        //deln(step);
        //deln(ub);
        //deln(cost);
        PI ex = ChooseExchangePair();
        int u = ex.first;
        int v = ex.second;
        if (u == 0 && v == 0)
        {
            // update edge weight and find a random pair (u, v) 
            /*if (Lcnt == 0)
            {
                remove_conflict();  
            }*/
            int t = ra(Lcnt) + 1;
            for (auto it = L.begin(); it != L.end();)
            {
                int cur = it -> id;
                int tag = it -> last;
                if (lastuncover[cur] == tag)
                {
                    weight[cur] += 1;
                    cost += 1;
                    dscore[edge[cur].x] += 1;
                    dscore[edge[cur].y] += 1;
                    confChange[edge[cur].x] = 1;
                    confChange[edge[cur].y] = 1;

                    t--;
                    if (t == 0) 
                    {
                        if (ra(2) == 0) 
                            v = edge[cur].x; 
                        else v = edge[cur].y;
                    }

                    ++it;                    
                } else it = L.erase(it);
            }
            
            int a[N], nn = 0;
            for (int i = 1; i <= n; i++) if (C[i]) a[++nn] = i;
            u = a[ra(nn) + 1];
        }

        remove(u);
        add(v);

        if (Ccnt + Lcnt < ub) 
        {
            //ub = Ccnt + Lcnt;
            //sb = 0;
            Greedy(tmpC);
            remove_some_vertices();
        }
    }
} 

int main(int argc, char *argv[])
{
    inputPath = argv[1];
    deln(inputPath);
    outputPath = argv[2];
    deln(outputPath);
    utl = atoi(argv[3]);
    deln(utl);
    
    init();
    srand(time(0));
    start = clock();

    delta = 1;
    // maxSteps = 100000000;
    maxSteps = 1000000;
    ewls();

    // printf("%d\n", ub);
    // for (int i = 1; i <= n; i++) 
    //     if (ansC[i]) printf("%d ", i);
    // puts("");

    return 0;
}