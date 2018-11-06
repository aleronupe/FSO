#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>

int sudoku[10][9][9];
int status[10] = {};
pthread_mutex_t locker;

typedef struct Quadrado {
  int begin_line;
  int begin_column;
  int numb;
} quadrado;

void *vertical(void *param);

int main(){

  int cont;
  FILE *fp;
  pthread_mutex_init(&locker, NULL);
  pthread_t id_list[11];
  ///////////Função de Leitura dos Códigos/////////////
  for(cont = 1; cont <= 10; cont++){
    status[cont-1] = 0;
    char nome[30] = "sudokus/";
    if(cont == 10){
      nome[8] = '1';
      nome[9] = '0';
      nome[10] = '.';
      nome[11] = 't';
      nome[12] = 'x';
      nome[13] = 't';
    }
    else {
      char numb = '0' + cont;
      nome[8] = numb;
      nome[9] = '.';
      nome[10] = 't';
      nome[11] = 'x';
      nome[12] = 't';
    }

    fp = fopen(nome, "r");
    int i, j;
    char letter;
    for(i = 0; i < 9; i++){
      for(j = 0; j < 9; j++){
        fscanf(fp, " %c", &letter);
        sudoku[cont-1][i][j] = letter;
      }
    }
    fclose(fp);
  }

  quadrado argumento;
  for(cont = 0; cont < 10; cont++){
    argumento.numb = cont;


  }





  return 0;
}

void *vertical(void *number){
  int numb = *((int *) number);

  if(status[numb] == 1){
    pthread_exit(0);
  }
  int i, j, value;
  int list[9] = {};

  for(i = 0; i < 9; i++){
    //////////////Loop para ler a coluna/////////////
    for(j = 0; j < 9; j++){
      value = sudoku[numb][j][i];
      list[value]++;
    }

    //////////////Loop para conferir o rolê////////////////
    for(j = 0; j < 9; j++){
      if(list[j] != 1){
        pthread_mutex_lock(&locker); ////////Tranca para modificar a variável status
        status[numb] = 1;
        pthread_mutex_unlock(&locker);
        pthread_exit(0);
        printf(" O sudoku de n° %d falhou. Vertical numero %i\n", numb, j);
      }
    }
  }

  pthread_exit(0);
}

void *horizontal(void *number) {
  int numb = *((int *) number);

  if (status[numb] == 1) {
    pthread_exit(0);
  }
  int i, j, value;
  int list[9] = {};

  for(j = 0; j < 0; j++) {
    for(i = 0; i < 0; i++) {
      value = sudoku[numb][i][j];
      list[value]++;
    }

    for(i = 0; i < 9; i++) {
      if (list[i] != 1) {
        pthread_mutex_lock(&locker);
        status[numb] = 1;
        pthread_mutex_unlock(&locker);
        pthread_exit(0);
        printf("O sudoku de nº %d faio :/ Horizontal de numero %i\n", numb, j);
      }
    }
  }
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
      list[value]++;
    }
  }

  for(j = 0; j < 9; j++){
    if(list[j] != 1){
      pthread_mutex_lock(&locker); ////////Tranca para modificar a variável status
      status[numb] = 1;
      pthread_mutex_unlock(&locker);
      pthread_exit(0);
      printf(" O sudoku de n° %d falhou. Linha: %d, Coluna: %d\n", numb, matriz.begin_line, matriz.begin_column);
    }
  }

  pthread_exit(0);
}
