#include <stdio.h>
#include <stdlib.h>

// Struct que será utilizada para montar a TLB
typedef struct tlb{
  int page_numb;
  int frame_numb;
} TLB;

/*
Struct para guardar os endereços que serão lidos, sendo:
 - pure_numb o número puro lido do arquivo para o endereço
 - page_numb o número da página que será acessada
 - disp_numb o número do deslocamento dentro do frame apontado pela página
 */
typedef struct address{
  unsigned int pure_numb;
  int page_numb;
  int disp_numb;
} Address;

// Declaração da thread que lê os endereçoss
void load_addresses(Address *logical, char *name_file);

//---------------------------------------------MAIN--------------------------------------------------------------//
int main(int argc, char *argv[]){

  // ponteiro de endereços a ser criado
  Address *logical;

  // Vetor da tabela de página
  int TP[256];
  for(int i = 0; i < 256; i++) {
    TP[i] = -1;
  }

  // Chamada da função que preenche a struct de enderços com o numero de pagina e o numero de deslocamento a partir do numero puro
  load_addresses(logical, argv[1]);



  // TODO - FREE (LIBERAR MEMORIAS UTILIZADAS)
  // free da Struct de endereços

  return 0;
}
//--------------------------------------------load_adresses-----------------------------------------------------//
void load_addresses(Address *logical, char *name_file) {
  // Lê a quantidade de endereços no arquivo a partir do número de espaços mais um
  FILE *fp;
  char add;
  int add_numb = 0, cont;

  fp = fopen(name_file, "r");
  while( fscanf(fp, "%c", &add) != EOF){
    if(add == 32){
      add_numb++;
    }
  }
  add_numb++;

  printf("O Programa contém %d enderecos\n", add_numb);

  // Modifica o endereço apontado pelo ponteiro na main com endereços
  logical = (Address *) malloc(add_numb*sizeof(Address));

  rewind(fp);

  unsigned int pure_address = 0;
  int page_address = 0, disp_address = 0;

  // Pega os numeros de pagina e os deslocamentos de cada numero do arquivo
  for(cont = 0; cont < add_numb; cont++){
    fscanf(fp, " %u", &pure_address);
    page_address = ((0xFFFF & pure_address) >> 8);
    disp_address = (0x00FF & pure_address);

    (*(logical + cont)).pure_numb = pure_address;
    (*(logical + cont)).page_numb = page_address;
    (*(logical + cont)).disp_numb = disp_address;

    printf("[%d]", cont);
    printf("endereço lido: %u\n", (*(logical + cont)).pure_numb);
    printf("pagina: %d\n", (*(logical + cont)).page_numb);
    printf("deslocamentos: %d\n", (*(logical + cont)).disp_numb);
  }

  fclose(fp);
  return;
}

//--------------------------------------------Backing Store-----------------------------------------------------//
void read_Of_Backing_Store() {

    int number_of_frame = 4;
    FILE *bs;
    bs = fopen()

}
