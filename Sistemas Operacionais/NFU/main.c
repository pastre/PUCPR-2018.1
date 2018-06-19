#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MASK 512
//#define 16 0x1000000000

typedef struct my_struct {
    int num;
    struct my_struct * next;
    int idade;
} ELEMENTO;

typedef struct my_list {
    ELEMENTO * ancora;
    ELEMENTO * tail;
} LISTA;

//Funcoes de fifo
LISTA * push(LISTA * , int, int *);
LISTA * pull(LISTA * );
LISTA * init_list(void);
LISTA * free_list(LISTA * );
void print_list(LISTA * );
void print_list_element(ELEMENTO * );

//Funcoes do programa
void simula_mmv(LISTA *,int *, int*, int, char *);
void erro(char *);
char * file_name(char *, char *);

int main1(){
    int a = 14;
    int b = a & 7;
    printf("%d\n",b);
    return 0;
}

int main(void) {
    LISTA * leitura = NULL;
    int trocas, reqs,i ;
    char fname[13];
    char * files[] = {
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo1.txt",
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo2.txt",
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo3.txt",
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo3.txt",
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo3.txt",
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo3.txt",
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo4.txt",
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo4.txt",
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo4.txt",
        "C:\\Aulas PUC\\Workspace\\Simula_FIFO\\arquivo4.txt"
    };
    int paginas[] = {3, 3, 63, 255, 1023, 2047, 63, 255, 1023, 2047};
   /**
    for(i = 0; i < 10; i ++){
        leitura = init_list();
        trocas = 0;
        reqs = 0;
        file_name( files[i], fname);
        simula_mmv(leitura, &trocas, &reqs, paginas[i], files[i]);
        printf("Arquivo: %s\nQuadros de memoria: %d, Paginas requisitadas: %d, FIFO: %d Page faults\n\n", fname, paginas[i] + 1, reqs, trocas);
    }
    */

    leitura = init_list();
   // simula_nfu(leitura, files[1], 3, 1);
    simula_nfu(leitura, files[2], 2048, 500);
    return 0;
}



void simula_nfu(LISTA * head, char * path, int max_quadros, int max_ciclos){
    int buffer, index = 0, tam_lista = 0, n_reqs = 0, pg_faults = 0;
    FILE * arquivo = fopen(path, "r");
    if(!arquivo)  erro("Nao foi possivel abrir o arquivo!");
    while(1){
        fscanf(arquivo, "%d", &buffer);
        if(feof(arquivo)) break;
        n_reqs ++;
       // printf("Readed %d\n", buffer);
        if(element_in_fifo(head -> ancora, buffer))
                age_page(head -> ancora, buffer);
        else{
            if(tam_lista < max_quadros){
                head = push(head, buffer, &tam_lista);
               // printf("tam lista %d\n", tam_lista);
            }
            else {
                //printf("Substituindo...\n");
                substitui_pagina(head -> ancora, buffer);
                pg_faults ++;
            }
        }
        if(n_reqs % max_ciclos == 0)
            end_cycle(head -> ancora);
      // d  if(n_reqs % 20 == 0)
           // print_list(head);
    }

    printf ("%5d paginas, %7d requisicoes, %5d Page Faults\n", max_quadros, n_reqs, pg_faults) ;
}
void end_cycle(ELEMENTO * ancora){
    if (ancora == NULL) return 0;
    ancora -> idade = ancora -> idade >> 1;
    end_cycle(ancora -> next);
}


void age_page(ELEMENTO * ancora, int num){
    if (ancora == NULL) return;
    if (ancora -> num == num){
      //  printf("OLD %d %d %x\n", ancora -> idade, ancora -> num, ancora);
        ancora -> idade = ancora -> idade | MASK;
    //    printf("NEW %d %d\n", ancora -> idade);
        return;
    }
    age_page(ancora -> next, num);
}

void encontra_mais_velha(ELEMENTO * ancora, ELEMENTO * curr_winner){
    if (ancora == NULL) return;
    if (curr_winner == NULL){
        curr_winner = ancora;
    }
    if(ancora -> idade < curr_winner -> idade ){
        curr_winner -> num = ancora -> num;
        curr_winner -> idade = ancora -> idade;
    }
    encontra_mais_velha(ancora -> next, curr_winner);
}


void substitui_pagina(ELEMENTO * ancora, int new_num){
    ELEMENTO pg_velha;
    pg_velha.num = 0;
    pg_velha.idade = MASK;
    encontra_mais_velha(ancora, &pg_velha);
    find_replace_fifo(ancora, pg_velha.num, new_num);
}
int element_in_fifo(ELEMENTO * ancora, int num){
    if (ancora == NULL){return 0;}
    if (ancora -> num == num){ return 1;}
    if (ancora -> next == NULL) {return 0;}
    return element_in_fifo(ancora -> next, num);
}

int find_replace_fifo(ELEMENTO * ancora, int num, int new_num){
    if (ancora == NULL) return 0;
    if (ancora -> num == num){
            //printf("Replaced %d for %d\n", num, new_num);
            ancora -> num = new_num;
            ancora -> idade = MASK;
            return 1;
    }
    if (ancora -> next == NULL) return 0;
    return find_replace_fifo(ancora -> next, num, new_num);
}

LISTA * push(LISTA * s, int val, int * cont) {
    ELEMENTO * p = malloc(1 * sizeof( * p));

    if (NULL == p) {
        fprintf(stderr, "IN %s, %s: malloc() failed\n", __FILE__, "list_add");
        return s;
    }

    p -> num = val;
    p -> next = NULL;
    p -> idade = MASK;
    if (NULL == s) {
        printf("Fila nao inicializada!\n");
        free(p);
        return s;
    } else if (NULL == s -> ancora && NULL == s -> tail) {
        s -> ancora = s -> tail = p;
        return s;
    } else if (NULL == s -> ancora || NULL == s -> tail) {
        printf("Erro grave de sincronizacao de ancora/tail \n");
        free(p);
        return NULL;
    } else {
        s -> tail -> next = p;
        s -> tail = p;
    }
    (*cont) ++;
    return s;
}

LISTA * pull(LISTA * s) {
    ELEMENTO * h = NULL;
    ELEMENTO * p = NULL;

    if (NULL == s) {
        printf("Lista vazia!\n");
        return s;
    } else if (NULL == s -> ancora && NULL == s -> tail) {
        printf("Lista vazia!\n");
        return s;
    } else if (NULL == s -> ancora || NULL == s -> tail) {
        printf("Erro grave de sincronizacao de ancora/tail \n");
        return s;
    }

    h = s -> ancora;
    p = h -> next;
    free(h);
    s -> ancora = p;
    if (NULL == s -> ancora) s -> tail = s -> ancora;

    return s;
}

LISTA * free_list(LISTA * s) {
    while (s -> ancora) {
        pull(s);
    }

    return s;
}

LISTA * init_list(void) {
    LISTA * p = malloc(1 * sizeof( * p));

    if (NULL == p) {
        fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
    }

    p -> ancora = p -> tail = NULL;

    return p;
}

void print_list(LISTA * ps) {
    ELEMENTO * p = NULL;

    if (ps) {
        for (p = ps -> ancora; p; p = p -> next) {
            print_list_element(p);
        }
    }

    printf("------------------\n");
}

void print_list_element(ELEMENTO * p) {
    if (p) {
        printf("Num = %d, Idade = %d\n", p -> num, p -> idade);
    } else {
        printf("Struct nula\n");
    }
}

void erro(char * mensagem){
    printf("asdasdaadas");
    printf("%s\n", mensagem);
    exit(1);
}
