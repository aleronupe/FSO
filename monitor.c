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
int position = 0; //Indica a posição do primeiro aluno da fila de espera
int students; //Quantidade de alunos no programa

pthread_mutex_t locker; //mutex para barrar acesso a fila
pthread_mutex_t office; //mutex para controlar acesso ao escritório
pthread_mutex_t atendance;
sem_t sem_line; //semáforo para controlar acesso à fila

int main(int argc, char *argv[]){

  // Cria os estudantes
  srand(time(NULL));
  //students = (rand() % 38) + 3; --------------------------------
  students = 7;
  int chairs = students/2;
  int cont;

  //Printa cabeçalho na tela
  printf("[Projeto do Monitor]\n\n");
  printf("Alunos: %d\n", students);
  printf("Cadeiras: %d\n", chairs);
  printf("--------------------------------------------\n");

  //Lista de IDs
  pthread_t assistant_id;
  pthread_t *id_list;
  id_list = (pthread_t *) malloc(students*sizeof(pthread_t));

  //Alocar dinâmicamente o vetor
  line = (int *) calloc(students*3, sizeof(int));

  // line[position] = 69;
  // printf("line[position] = %d\n", line[position]);
  // printf("&line[position] = %d\n", &line[position]);

  //Inicializa Mutex e Semáforo
  pthread_mutex_init(&locker, NULL);
  pthread_mutex_init(&office, NULL);
  pthread_mutex_init(&atendance, NULL);
  sem_init(&sem_line, 0, chairs);

  //Cria o vetor de id de Threads
  int *vec_students;
  vec_students = (int *) calloc(students, sizeof(int));

  //Locka o atendance
  pthread_mutex_lock(&atendance);

  //Inicializa threads
  for(cont = 0; cont < students; cont++){
    *(vec_students + cont) = cont+1;
    pthread_create(id_list+cont, NULL, student, vec_students+cont);
  }
  pthread_create(&assistant_id, NULL, assistant, NULL);


  //Finaliza Threads
  for(cont = 0; cont < students; cont++){
    pthread_join(*(id_list+cont), NULL);
  }
  pthread_join(assistant_id, NULL);

  free(line);
  free(vec_students);
  return 0;

}

//////////////////////////////////Student Thread////////////////////
void *student(void *id_aux){
  //Inicializa ID e ajudas
  int id = *((int *)id_aux);
  printf("[Inicio da thread de id %.2d]\n", id);
  int helps = 0, cont = 0;

  while(helps < 3){
    //Verifica cadeiras vazias na fila
    pthread_mutex_lock(&locker);

    //Fluxo do Estudante na Fila
    if(sem_trywait(&sem_line) == 0){
      //Verifica se o estudante pode esperar na fila
      if(position > 0){
        printf("[%.2d] Estudante entrou na Fila\n", id);
      }

      //Ocupa uma posição na fila pelo semáforo
      for(cont = 0; cont < students*3; cont++){
        if(line[cont] == 0){
          line[cont] = id;
          break;
        }
      }
      pthread_mutex_unlock(&locker);


      //Aluno verifica se ele é o próximo a ser chamado
      while(1){
        pthread_mutex_lock(&atendance);
        if(cont == position && line[position] == id){
          pthread_mutex_unlock(&atendance);
          break;
        }
        pthread_mutex_unlock(&atendance);
      }

      printf("[%.2d] Estudante sendo atendido pelo Monitor (%d)\n", id, helps+1);

      pthread_mutex_lock(&office); //Aluno fica parado aqui
      printf("[%.2d] Estudante terminou  de ser atendido\n", id);
      helps++;
      pthread_mutex_unlock(&office);
    }

    //Fluxo do Estudante em espera
    else {
      pthread_mutex_unlock(&locker);
              //printf("[%.2d] Estudante não tinha lugar na fila e vai estudar\n", id);
      sleep(randomTime());
    }
  }

  printf("[%.2d]Estudante foi atendido todas as %d vezes e vai embora\n", id, helps);
  pthread_exit(0);
}

//////////////////////////////////////Retorno de número aleatório//////////////////
int randomTime(){
  srand(time(NULL));
  int waiter = (rand() % 3)+1;
  return waiter;
}

/////////////////////////////////Assistant Thread////////////////////
void *assistant(void *param){

  while( position < students*3 ) {
    //Caso o monitor não tenha quem atender
    if(line[position] == 0 && position > 0){
      //Monitor inicia tirando um cochilo
      printf("[Monitor] está dormindo\n");

      while(line[position] == 0){
        //Busy wait
      }
      //Monitor é acordado
      printf("[Monitor] foi acordado pelo aluno %d\n", line[position]);
    }



    //Monitor inicia atendimento do alunos
    pthread_mutex_lock(&office);
    printf("[Monitor] está atendendo o aluno %d\n", line[position]);
    sem_post(&sem_line); //Libera o lugar do aluno atendido na fila
    pthread_mutex_unlock(&atendance);
    //Libera para o aluno entrar
    sleep(randomTime());
    printf("[Monitor] finalizou o atendimento do aluno %d\n", line[position]);
    pthread_mutex_lock(&atendance);
    position++;
    pthread_mutex_unlock(&office);

    //Monitor barra o acesso para modificar o semáforo e o position
  //  pthread_mutex_lock(&locker);

  //  pthread_mutex_unlock(&locker);

  }

  printf("Não atendo mais ninguém seus alunos peba\n");
  pthread_exit(0);
}
