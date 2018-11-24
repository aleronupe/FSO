#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

//Struct que será utilizada para montar a TLB
typedef struct tlb{
  int page_numb;
  int frame_numb;
} TLB;

/*Struct para guardar os endereços que serão lidos, sendo:
 - pure_numb o número puro lido do arquivo para o endereço
 - page_numb o número da página que será acessada
 - disp_numb o número do deslocamento dentro do frame apontado pela página*/
typedef struct address{
  unsigned int pure_numb;
  int page_numb;
  int disp_numb;
} Adress;

//Declaração da thread que lê os endereçoss
void *load_addresses(void *param);

int main(int argc, char *argv[]){

  //ponteiro de endereços a ser criado
  Adress *logical;

  //Testa a thread "load_Addresses" que carrega os endereços pra um vetor de Adress
  pthread_t load_id;
  pthread_create(&load_id, NULL, load_addresses, logical);
  pthread_join(load_id, NULL);



  return 0;
}

void *load_addresses(void *param) {
  //Lê a quantidade de endereços no arquivo a partir do número de espaços mais um
  Adress *logical = *((address *) param)
  FILE *fp;
  char add;
  int add_numb = 0, cont;

  fp = fopen("addresses.txt", "r");
  while( fscanf(fp, "%c", &add) != EOF){
    if(add == 32){
      add_numb++;
    }
  }
  add_numb++;

  printf("O Programa contém %d enderecos\n", add_numb);

  //Modifica o endereço apontado pelo ponteiro na main com endereços
  logical = (Address *) calloc(add_numb, sizeof(Address));

  unsigned int pure_address = 0;
  int page_address = 0, disp_address = 0;

  for(cont = 0; cont < add_numb; cont++){
    



  }


  pthread_exit(0);
}
