#include <stdio.h>
#include <stdlib.h>

// Struct que será utilizada para montar a TLB
typedef struct tlb{
  int page_numb;
  int frame_numb;
  int frequence;
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
Address *load_addresses(char *name_file, int *number_of_addresses);
char *read_of_backing_store(int number_of_frame);
void LRU(TLB *table, int page_number, int frame_number);

//---------------------------------------------MAIN--------------------------------------------------------------//
int main(int argc, char *argv[]){

  // ponteiro de endereços a ser criado
  Address *logical;
  // ponteiro para os endereços da Memória Física
  // int **simul_ram;
  // ponteiro pra ponteiro que aponta para o byte do endereço da Memória Física
  char **RAM;
  // numero de endereços do arquivo lido
  int number_of_addresses = 0;
  // numero de falhas de pagina
  int page_faults = 0;
  // TLB
  TLB *table;
  table = (TLB *)malloc(16 * sizeof(TLB));
  for(int i = 0; i < 16; i++) {
    table[i].frequence = -1;
    table[i].page_numb = -1;
    table[i].frame_numb = -1;
  }
  // Vetor da tabela de página
  int TP[256];
  for(int i = 0; i < 256; i++) {
    TP[i] = -1;
  }

  // Chamada da função que preenche a struct de enderços com o numero de pagina e o numero de deslocamento a partir do numero puro
  logical = load_addresses(argv[1], &number_of_addresses);

  // Define vetor que atua como memória Física
  RAM = (char **)malloc(256 * sizeof(char *));
  for(int i = 0; i < 256; i++) {
    *(RAM + i) = (char *)malloc(256 * sizeof(char));
    for(int j = 0; j < 256; j++){
      *(*(RAM + i) + j) = -1;
    }
  }

  // Acesso a TP
  int page_position = 0, frame_disp = 0, flag = 0, tlb_sucess = 0, found = -1;
  for(int i = 0; i < number_of_addresses; i++) {
    flag = 0;
    // numero de pagina do numero (8 bits que indica o numero da pagina)
    page_position = (*(logical + i)).page_numb;
    // deslocamento do numero (8 bits de deslocamento)
    frame_disp = (*(logical + i)).disp_numb;

    found = -1;
    for(int z = 0; z < 16; z++) {
      if((table[z].page_numb == page_position)) {
        // achou na tlb
        tlb_sucess++;
        found = z;
        flag = 1;
        break;
      }
    }
    if(flag == 1) {
      printf("\nO número de página foi encontrado na TLB\n");
      LRU(table, table[found].page_numb, table[found].frame_numb);
      printf("Número de Página: %d\n", page_position);
      printf("O endereço lógico que está sendo traduzido: %d\n", (*(logical + i)).pure_numb);
      printf("O valor do offset: %d\n", (*(logical + i)).disp_numb);
      printf("Endereço na memória física: %d\n", table[found].frame_numb);
      printf("Deslocamento na memória física: %d\n", (256 * table[found].frame_numb) + frame_disp); // valor do offset
      printf("Valor encontrado na memória física: [%c]\n\n", RAM[table[found].frame_numb][frame_disp]); // valor do byte
    } else {
      // Tenta acessar pela tp, caso não tenha ele aumenta o page_faults e preenche pelo backing store na memoria ram e preenche a posição na TP
      if(TP[page_position] == -1) {
        printf("\nO número do frame não existe na tabela de página\n");
        page_faults++;
        // Procura a primeira posição livre na RAM
        for(int j = 0; j < 256; j++){
          //Verifica qual a primeira posição de deslocamento na RAM possui somente -1
          if(RAM[j][1] == -1) {
            //Salva o bloco referente ao número da página na primeira posição livre na RAM
            *(RAM + j) = read_of_backing_store(page_position);
            //Salva, na TP, a referência para a posição da RAM com o bloco de memória
            TP[page_position] = j;
            break;
          }
        }
      }
      printf("\nO número de página não foi encontrado na TLB, entrou para a TP\n");
      LRU(table, page_position, TP[page_position]);
      printf("Número de Página: %d\n", page_position);
      printf("O endereço lógico que está sendo traduzido: %d\n", (*(logical + i)).pure_numb);
      printf("O valor do offset: %d\n", (*(logical + i)).disp_numb);
      printf("Endereço na memória física: %d\n", TP[page_position]);
      printf("Deslocamento na memória física: %d\n", (256*TP[page_position]) + frame_disp); // valor do offset
      printf("Valor encontrado na memória física: [%c]\n\n", RAM[TP[page_position]][frame_disp]); // valor do byte
    }
  }
  printf("\nO número de endereços: %d\n", number_of_addresses);
  printf("O número de acessos pela TLB: %d\nA técnica de substituição de página utilizada foi LRU\n", tlb_sucess);
  printf("O número de erros de página: %d\n", page_faults);
  printf("Taxa de sucesso da TLB: %.2f%%\n", ((float)tlb_sucess/(float)number_of_addresses) * 100);
  printf("Taxa de erro: %.2f%%\n", ((float)page_faults/(float)number_of_addresses) * 100);

  // FREE
  free(table);
  for(int i = 0; i < 256; i++) {
    free(*(RAM + i));
  }
  free(RAM);
  free(logical);

  return 0;
}
//--------------------------------------------load_adresses-----------------------------------------------------//
Address *load_addresses(char *name_file, int *number_of_addresses) {
  // Lê a quantidade de endereços no arquivo a partir do número de espaços mais um
  FILE *fp;
  char add;
  int cont;
  Address *logical;
  (*number_of_addresses) = 0;

  fp = fopen(name_file, "r");
  while( fscanf(fp, "%c", &add) != EOF){
    // quando tiver espaço em branco ele incrementa
    if(add == 32){
      (*number_of_addresses)++;
    }
  }
  (*number_of_addresses)++;

  printf("O Programa contém %d enderecos\n", (*number_of_addresses));

  // Modifica o endereço apontado pelo ponteiro na main com endereços
  logical = (Address *) malloc((*number_of_addresses)*sizeof(Address));

  rewind(fp);

  unsigned int pure_address = 0;
  int page_address = 0, disp_address = 0;

  // Pega os numeros de pagina e os deslocamentos de cada numero do arquivo
  for(cont = 0; cont < (*number_of_addresses); cont++){
    fscanf(fp, " %u", &pure_address);
    page_address = ((0xFFFF & pure_address) >> 8);
    disp_address = (0x00FF & pure_address);

    (*(logical + cont)).pure_numb = pure_address;
    (*(logical + cont)).page_numb = page_address;
    (*(logical + cont)).disp_numb = disp_address;

    printf("[%d]", cont);
    printf("endereço lido: %u\n", (*(logical + cont)).pure_numb);
    printf("pagina: %d\n", (*(logical + cont)).page_numb);
    printf("deslocamentos: %d\n\n", (*(logical + cont)).disp_numb);
  }

  fclose(fp);
  return logical;
}

//--------------------------------------------Backing Store-----------------------------------------------------//
char *read_of_backing_store(int number_of_frame) {

    //Declaração das variáveis de função
    char *frame;
    frame = (char *) malloc(256 * sizeof(char));
    FILE *bs;

    //Abre arquivo do backing store
    bs = fopen("BACKING_STORE.bin", "r");
    fseek(bs, 256 * number_of_frame, SEEK_SET);
    fread(frame, 256, 1, bs);
    fclose(bs);

    // Imprime na tela os dados lidos
    // for(int cont = 0; cont < 256; cont++){
    //   printf("DEBUG BACKING STORE %d - [%c]\n", cont, *(frame + cont));
    // }

    return frame;
}

//--------------------------------------------LRU-----------------------------------------------------//
void LRU(TLB *table, int page_number, int frame_number) {
  int cont = 0, pos = -1, maior = -1, flag = 0;
  for(cont = 0; cont < 16; cont++){

    //Verifica se ainda existe espaço não preenchido na TLB
    if((*(table+cont)).page_numb == -1 ){
      (*(table+cont)).frequence = 0;
      (*(table+cont)).page_numb = page_number;
      (*(table+cont)).frame_numb = frame_number;
      flag = 1;
      break;
    }
    //Caso em que o elemento já está na tabela
    if( ((*(table+cont)).page_numb == page_number)){
      (*(table+cont)).frequence = -1;
      flag = 1;
    }

    //Não altera dados da struct, somente guarda informação de maior frequence e posição
    if((*(table+cont)).frequence > maior){
      maior = (*(table+cont)).frequence;
      pos = cont;
    }

    (*(table+cont)).frequence++;
  }

  if(flag == 0){
    (*(table+pos)).frequence = 0;
    (*(table+pos)).page_numb = page_number;
    (*(table+pos)).frame_numb = frame_number;
  }

  // DEBUG LRU
  // for(cont = 0; cont < 16; cont++){
  //   printf("TLB page numb %d\n", table[cont].page_numb);
  //   printf("TLB frame numb %d\n", table[cont].frame_numb);
  //   printf("TLB freq %d\n", table[cont].frequence);
  // }

}
