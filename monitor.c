#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

//Declarações de Funções
void *students(void *param);
void *assistent(void *param);
int randomTime();

int busy; //Estado do monitor
int *fila; //Vetor com os ID's das threads
int position //Indica a posição do primeiro aluno da fila de espera

pthread_mutex_t locker;
sem_t semAluno; // um semaforo pode ser vetor? tipo semAtendimento[5]

int main(int argc, char *argv[]){

  // Cria os estudantes
  srand(time(NULL));
  int students = (rand() % 38) + 3;
  int chairs = students/2;
  int cont;

  //Printa cabeçalho na tela
  printf("Alunos: %d\n", students);
  printf("Cadeiras: %d\n", chairs);
  printf("------------------------\n");

  //Lista de IDs
  pthread_t monitor_id;
  pthread_t *id_list;
  id_list = (pthread_t *) malloc(chairs*sizeof(pthread_t));

  //Alocar dinâmicamente o vetor
  fila = (int *) calloc(students*3, sizeof(int));

  //Inicializa Mutex
  pthread_mutex_init(&locker, NULL);

  //Inicializa threads
  for(cont = 0; cont < chairs; cont++){
    pthread_create(id_list+cont, NULL, aluno, cont+1);
  }
  pthread_create(&monitor_id, NULL, monitor, NULL)


  //Finaliza Threads
  for(cont = 0; cont < chairs; cont++){
    pthread_join(*(id_list+cont), NULL);
  }
  pthread_join(monitor_id, NULL)

  sem_init(&semAluno, 0, 5);



}

void *assistant(void *param){

  while (1) {
    // duvida se aqui ele tem que estar dormindo para dar um sem_wait
    pthread_mutex_lock(&locker)

    // if (fila = 0) {
    //   pthread_mutex_unlock(&locker)
    // }
    printf("Estou atendendo o aluno %d\n", vetorId[posicao] ); // isso aqui é quando o vetor for criado com as threads, ai pra falar qual aluno foi atendido, usar aposica. n sei se faz sentido pra vc
    //AI NESSE POST ELE LIBERA A POSIÇÃO ATUAL DA THREAD SENDO ATENDIDA, E TIRA ELA DA fila

     // se puder ser um vetor, colocar a posição dessa thread,
    // se tirar essa thread atendida da fila, modifica aquela variavel que conversamos que percorre a fila, mas n saquei muito se vai ser só uma variavel ou um array
    pthread_mutex_unlock(&locker)
    // o monitor tem que esperar um tempo aleatorio para acabar o atendimento, n sei se vai ser radom ou sleep

    sem_post(&semAluno);
    sleep(waiter); // posso fazer isso? senão defino um tempo mesmo
  }
}

void *student(void *id_aux){
  int id =
  int study_time = randomTime();
  sleep()
  while(){

  }
}


int randomTime(){
  srand(time(NULL));
  int waiter = (rand() % 10) + 1;
  return waiter;
}
