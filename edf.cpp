#include <algorithm>
#include <iomanip>
#include <iostream>
#include <queue>
#include <vector>

#define MAX_N 26
#define MAX_T 100000

using namespace std;

class Tarefa {
 private:
  unsigned c, p, d, id, r;

 public:
  Tarefa(unsigned parC = 0, unsigned parP = 0, unsigned parD = 0) {
    define(parC, parP, parD);
    r = c;
  }
  unsigned obtemId() { return id; }
  unsigned obtemC() { return c; }
  unsigned obtemP() { return p; }
  unsigned obtemD() { return d; }
  unsigned obtemR() { return r; }
  void defineC(unsigned parC) { c = parC; }
  void defineP(unsigned parP) { p = parP; }
  void defineD(unsigned parD) { d = parD; }
  void defineR(unsigned parR) { r = parR; }
  void define(unsigned parC, unsigned parP, unsigned parD) {
    c = parC;
    p = parP;
    d = parD;
    r = c;
  }
  void defineId(unsigned parId) { id = parId; }
};

// Função para comparar duas tarefas pelo deadline
bool comparaDeadline(Tarefa a, Tarefa b) { return a.obtemD() > b.obtemD(); }

int main() {
  Tarefa tarefas[MAX_N];
  unsigned n, t;

  while (true) {
    // Leitura
    cin >> n >> t;
    if (n == 0 || t == 0) break;
    for (int i = 0; i < n; i++) {
      unsigned c, p, d;
      cin >> c >> p >> d;
      tarefas[i].define(c, p, d);
      tarefas[i].defineId(i);
    }

    // Processamento
    char grade[MAX_T + 1];
    unsigned t_atual = 0;
    memset(grade, '\0', MAX_T + 1);
    grade[t_atual] = '\0';
    unsigned tr_contexto = 0, preemp = 0;
    double utilizacao = 0.0;

    // Cria uma fila de prioridade para armazenar as tarefas prontas
    priority_queue<Tarefa, vector<Tarefa>, decltype(&comparaDeadline)> fila(
        comparaDeadline);

    // Cria um vetor para armazenar os próximos tempos de chegada das tarefas
    vector<unsigned> chegadas(n);

    // Inicializa o vetor de chegadas com os períodos das tarefas
    for (int i = 0; i < n; i++) {
      chegadas[i] = 0;
      utilizacao +=
          (double)tarefas[i].obtemC() /
          tarefas[i].obtemP();  // Calcula a taxa de utilização do processador
    }

    while (t_atual < t) {  // Enquanto não terminar o tempo de simulação
      for (int i = 0; i < n; i++) {    // Para cada tarefa
        if (chegadas[i] == t_atual) {  // Se chegou uma nova instância da tarefa
          fila.push(tarefas[i]);       // Coloca na fila de prontas
          chegadas[i] +=
              tarefas[i]
                  .obtemP();  // Atualiza o próximo tempo de chegada da tarefa
        }
      }
      if (fila.empty()) {    // Se não há nenhuma tarefa pronta
        strcat(grade, ".");  // Adiciona uma unidade ociosa na grade
        if (t_atual > 0 && grade[t_atual - 1] != '.')
          tr_contexto++;  // Conta uma troca de contexto se o processador estava
                          // executando alguma tarefa antes
      } else {            // Se há alguma tarefa pronta
        Tarefa atual = fila.top();  // Pega a tarefa com menor deadline da fila
        fila.pop();                 // Remove a tarefa da fila
        char letra =
            'A' +
            atual.obtemId();  // Define a letra que representa a tarefa na grade
        if (t_atual >= atual.obtemD())
          letra +=
              'a' - 'A';  // Se a execução está em atraso, usa letra minúscula
        strncat(grade, &letra,
                1);  // Adiciona uma unidade de execução da tarefa na grade
        if (t_atual > 0 && grade[t_atual - 1] != letra)
          tr_contexto++;  // Conta uma troca de contexto se o processador estava
                          // executando outra tarefa antes
        atual.defineR(atual.obtemR() -
                      1);  // Diminui o tempo restante da tarefa em uma unidade.
                           // Adicione esta linha.
        if (atual.obtemR() >
            0) {  // Se a tarefa ainda não terminou. Substitua esta linha.
          fila.push(atual);  // Coloca de volta na fila de prontas
          preemp++;          // Conta uma preempção
        }
      }
      t_atual++;  // Incrementa o tempo atual
    }

    // Resultado
    cout << grade << endl;
    cout << tr_contexto << " " << preemp << endl;
    cout << fixed << setprecision(4) << utilizacao << " ";
    cout << (utilizacao <= 1.0 ? "OK" : "NOK") << endl << endl;
  }
  return 0;
}
