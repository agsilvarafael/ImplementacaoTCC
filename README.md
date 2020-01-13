# ImplementacaoTCC

Compilado utilizando o Makefile

Como foi executado:

Todos foram executados com "timeout -k 30m 30m "

.\ed <grafo.txt >saidaed.txt
.\ewcc grafo.txt saidaewcc.txt 200
.\GCVM 1000 2 <grafo.txt >saidagcvm.txt
.\ssa 10000 <grafo.txt >saidassa.txt
.\ssa2 10000 <grafo.txt >saidassa2.txt
.\ssagcvm 10000 <grafo.txt >saidassagcvm.txt
.\ssagcvm2 10000 <grafo.txt >saidassagcvm2.txt
