#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int sudoku[10][9][9];
int status[10] = {0, 0, 0, 0, 0, 0, 0, 0};
pthread_mutex_t locker;

typedef struct Quadrado {
  int begin_line;
  int begin_column;
  int numb;
} quadrado;

void *vertical(void *number);
void *horizontal(void *number);
void *quadrado3x3(void *param);

int main(){

  int cont;
  FILE *fp;
  pthread_mutex_init(&locker, NULL);
  pthread_t id_list[11];
  ///////////Função de Leitura dos Códigos/////////////
  for(cont = 1; cont <= 10; cont++){
    status[cont-1] = 0;
    char nome[30] = "sudokus/";

    sprintf(nome, "sudokus/%d.txt", cont);
    fp = fopen(nome, "r");
    int value;
    for(int i = 0; i < 9; i++){
      for(int j = 0; j < 9; j++){
        fscanf(fp, "%d", &value);
        sudoku[cont-1][i][j] = value;
      }
    }
    fclose(fp);
  }



  quadrado argumento;
  for(cont = 0; cont < 10; cont++){
    int i, j,cont_list = 0;
    pthread_create(&id_list[cont_list], NULL, vertical, &cont);
    // printf("Thread criada\n");
    cont_list++;
    pthread_create(&id_list[cont_list], NULL, horizontal, &cont);
    cont_list++;
    // printf("Thread criada\n");


    argumento.numb = cont;

    for(i = 0; i <= 6; i += 3){
      for(j = 0; j <= 6; j += 3){
        argumento.begin_line = i;
        argumento.begin_column = j;
        pthread_create(&id_list[cont_list], NULL, quadrado3x3, &argumento);
        // printf("Thread criada\n");

        cont_list++;
      }
    }
  }

  for(cont = 0; cont < 11; cont++) {
    pthread_join(id_list[cont], NULL);
  }

  for(cont = 0; cont < 10; cont++){
    printf("sudoku %d", cont+1);
    if(status[cont] == 0){
      printf(" esta correto.\n");
    }
    else {
      printf(" falhou.\n");
    }
  }

  return 0;
}

void *vertical(void *number){
  int numb = *((int *) number);

  if(status[numb] == 1){
    pthread_exit(0);
  }
  int i, j, value;
  int list[9] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

  for(i = 0; i < 9; i++){

    for(j = 0; j < 9; j++){
      value = sudoku[numb][j][i];
      list[value - 1]++;
    }


    for(j = 0; j < 9; j++){
      if(list[j] != 1){
        pthread_mutex_lock(&locker);
        status[numb] = 1;
        pthread_mutex_unlock(&locker);
        //printf(" O sudoku de n° %d falhou. Coluna: [ %d ] Falhou o: [ %d ]\n", numb+1, i, j+1);
        pthread_exit(0);
      }
      else{
        list[j] = 0;
      }
    }
  }

  pthread_exit(0);
}

void *horizontal(void *number){
  int numb = *((int *) number);

  if (status[numb] == 1) {
    pthread_exit(0);
  }

  int i, j, value;
  int list[9] = {};

  for(j = 0; j < 0; j++) {
    for(i = 0; i < 0; i++) {
      value = sudoku[numb][i][j];
      list[value - 1]++;
    }

    for(i = 0; i < 9; i++) {
      if (list[i] != 1) {
        pthread_mutex_lock(&locker);
        status[numb] = 1;
        pthread_mutex_unlock(&locker);
        //printf("O sudoku de nº %d faio :/ Horizontal de numero %i\n", numb++, j);
        pthread_exit(0);
      }
      else{
        list[j] = 0;
      }
    }
  }
  pthread_exit(0);
}

void *quadrado3x3(void *param){

  quadrado matriz = *((quadrado *) param);

  int numb = matriz.numb;

  if(status[numb] == 1){
    pthread_exit(0);
  }

  int i, j, value;
  int beg_lin = matriz.begin_line, beg_col = matriz.begin_column;
  int list[9] = {};

  for(i = beg_lin; i < beg_lin + 3; i++){
    for(j = beg_col; j < beg_col + 3; j++){
      value = sudoku[numb][i][j];
      list[value - 1]++;
    }
  }

  for(j = 0; j < 9; j++){
    if(list[j] != 1){
      pthread_mutex_lock(&locker); ////////Tranca para modificar a variável status
      status[numb] = 1;
      pthread_mutex_unlock(&locker);
      //printf(" O sudoku de n° %d falhou. Linha: %d, Coluna: %d\n", numb+1, matriz.begin_line, matriz.begin_column);
      pthread_exit(0);
    }
    else{
      list[j] = 0;
    }
  }

  pthread_exit(0);
}
