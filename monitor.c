#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>
#include <unistd.h>

//Declarações de Funções
void *student(void *param);
void *assistant(void *param);
int randomTime();

int busy; //Estado do monitor
int *line; //Vetor com os ID's das threads
int position; //Indica a posição do primeiro aluno da fila de espera
int students; //Quantidade de alunos no programa

pthread_mutex_t locker; //mutex para barrar acesso a fila
pthread_mutex_t office; //mutex para controlar acesso ao escritório
sem_t semStudent; // um semaforo pode ser vetor? tipo semAtendimento[5]

int main(int argc, char *argv[]){

  // Cria os estudantes
  srand(time(NULL));
  students = (rand() % 38) + 3;
  int chairs = students/2;
  int cont;

  //Printa cabeçalho na tela
  printf("Alunos: %d\n", students);
  printf("Cadeiras: %d\n", chairs);
  printf("------------------------\n");

  //Lista de IDs
  pthread_t assistant_id;
  pthread_t *id_list;
  id_list = (pthread_t *) malloc(chairs*sizeof(pthread_t));

  //Alocar dinâmicamente o vetor
  line = (int *) calloc(students*3, sizeof(int));

  //Inicializa Mutex e Semáforo
  pthread_mutex_init(&locker, NULL);
  sem_init(&semStudent, 0, chairs);


  //Inicializa threads
  for(cont = 0; cont < chairs; cont++){
    pthread_create(id_list+cont, NULL, student, &cont);
  }
  pthread_create(&assistant_id, NULL, assistant, NULL);


  //Finaliza Threads
  for(cont = 0; cont < chairs; cont++){
    pthread_join(*(id_list+cont), NULL);
  }
  pthread_join(assistant_id, NULL);


  return 0;

}

// void *assistant(void *param){
//
//   while (1) {
//     // duvida se aqui ele tem que estar dormindo para dar um sem_wait
//     // for(position = 1; position < (chairs*3)-1; position++) {
//     printf("Estou atendendo o aluno %d\n", line[position] );
//
//     pthread_mutex_lock(&locker)
//     position++;
//
//     pthread_mutex_lock(&locker)
//
//     sem_post(&semStudent);
//
//     sleep(randomTime);
// }
//     if (line[position] == ((chairs*3) -1) {
//       pthread_mutex_unlock(&locker)
//       printf("Não atendo mais ninguém seus alunos peba\n");
//     }
//     pthread_mutex_unlock(&locker)
//   }
// }

//////////////////////////////////Student Thread////////////////////
void *student(void *id_aux){
  //Inicializa ID e ajudas
  int id = *((int *)id_aux) + 1;
  int helps = 0, empty_chairs = 0, cont = 0;


  while(helps < 3){
    //Verifica cadeiras vazias na fila
    pthread_mutex_lock(&locker);
    sem_getvalue(&semStudent, &empty_chairs);

    //Fluxo do Estudante na Fila
    if(empty_chairs > 0){
      //Verifica se o estudante pode esperar na fila
      printf("Estudante %d está na Fila\n", id);
      //Ocupa uma posição na fila pelo semáforo
      sem_wait(&semStudent);
      for(cont = 0; cont < students*3; cont++){
        if(line[cont] == 0){
          line[cont] = id;
          break;
        }
      }
      pthread_mutex_unlock(&locker);
      while( (line[position] != id) && (busy != 1) ){
        //Busy wait
      }
      printf("Estudante %d sendo atendido pelo Monitor pela %d ª vez\n", id, helps);
      pthread_mutex_lock(&office);
      helps++;
      pthread_mutex_unlock(&office);
    }

    //Fluxo do Estudante em espera
    else {
      pthread_mutex_unlock(&locker);
      sleep(randomTime());
    }
  }

  printf("Estudante %d foi atendido todas as %d vezes e vai embora\n", id, helps);
  pthread_exit(0);
}

int randomTime(){
  srand(time(NULL));
  int waiter = (rand() % 10) + 1;
  return waiter;
}

/////////////////////////////////Assistant Thread////////////////////
void *assistant(void *param){

  while( position < (students/2)*3 ) {

    //Caso o monitor não tenha quem atender
    if(line[position] == 0){
      //Monitor inicia tirando um cochilo
      printf("Monitor está dormindo\n");

      while(line[position] == 0){
        //Busy wait
      }

      //Monitor é acordado
      printf("Monitor foi acordado pelo aluno %d\n", line[position]);
    }

    //Monitor inicia atendimento do alunos
    pthread_mutex_lock(&office);
    busy = 1;
    printf("Monitor está atendendo o aluno %d\n", line[position]);
    sleep(randomTime());
    pthread_mutex_unlock(&office);

    //Monitor barra o acesso para modificar o semáforo e o position
    pthread_mutex_lock(&locker);
    position++;
    sem_post(&semStudent);
    pthread_mutex_lock(&locker);

  }

  printf("Não atendo mais ninguém seus alunos peba\n");
  pthread_exit(0);
}
