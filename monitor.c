#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <semaphore.h>

void *students(void *param);
void *assistent(void *param);

int randomTime();
int busy;
int status;

pthread_mutex_t locker;

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

  pthread_mutex_init(&locker, NULL);

  //Inicializa threads
  for(cont = 0; cont < chairs; cont++){
    pthread_create(id_list+cont, NULL, aluno, NULL);
  }
  pthread_create(&monitor_id, NULL, monitor, NULL)

  for(cont = 0; cont < chairs; cont++){
    pthread_join(*(id_list+cont), NULL);
  }
  pthread_join(monitor_id, NULL)



}

void *monitor(void *param){
  pthread_mutex_lock(&locker)
  busy = 1;
  printf("Estou atendendo o aluno \n", );
  pthread_mutex_unlock(&locker)

}

void *aluno(void *param){
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
