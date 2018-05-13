#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct my_struct {
    int num;
    struct my_struct * next;
} ELEMENTO;

typedef struct my_list {
    ELEMENTO * head;
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
    for(i = 0; i < 10; i ++){
        leitura = init_list();
        trocas = 0;
        reqs = 0;
        file_name( files[i], fname);
        simula_mmv(leitura, &trocas, &reqs, paginas[i], files[i]);
        printf("Arquivo: %s\nQuadros de memoria: %d, Paginas requisitadas: %d, FIFO: %d Page faults\n\n", fname, paginas[i] + 1, reqs, trocas);
    }

    return 0;
}
char * file_name(char * to_format, char * res){
    int i, j;
    char ret[13];
    for (i =35, j = 0; i < 47; i++, j++)
        res[j] = to_format[i];
}
int element_in_fifo(ELEMENTO * head, int num){
    if (head == NULL) return 0;
   // printf("SCANNING %d", head -> num);
    if (head -> num == num) return 1;
    if (head -> next == NULL) return 0;
    return element_in_fifo(head -> next, num);
}
void simula_mmv(LISTA * lista,int * trocas, int * reqs, int MAX_PAGINAS, char * PATH_TO_FILE){
    FILE * file;
    char buffer[10];
    if((file = fopen(PATH_TO_FILE, "r")) == NULL) erro("Nao foi possivel abrir o arquivo!");
    int cont = 0;
    while(1){
        fgets(buffer, 9, file);
        if(feof(file)) break;
        (*reqs)++;
        if(element_in_fifo(lista -> head, atoi(buffer)) == 1) continue;

        if(cont >= MAX_PAGINAS){
            pull(lista);
            (*trocas) ++;
            //printf("%d, %x\n", *trocas, trocas);
        }
        push(lista, atoi(buffer), &cont);
    }
}

LISTA * push(LISTA * s, int val, int * cont) {
    ELEMENTO * p = malloc(1 * sizeof( * p));

    if (NULL == p) {
        fprintf(stderr, "IN %s, %s: malloc() failed\n", __FILE__, "list_add");
        return s;
    }

    p -> num = val;
    p -> next = NULL;

    if (NULL == s) {
        printf("Fila nao inicializada!\n");
        free(p);
        return s;
    } else if (NULL == s -> head && NULL == s -> tail) {
        s -> head = s -> tail = p;
        return s;
    } else if (NULL == s -> head || NULL == s -> tail) {
        printf("Erro grave de sincronizacao de head/tail \n");
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
    } else if (NULL == s -> head && NULL == s -> tail) {
        printf("Lista vazia!\n");
        return s;
    } else if (NULL == s -> head || NULL == s -> tail) {
        printf("Erro grave de sincronizacao de head/tail \n");
        return s;
    }

    h = s -> head;
    p = h -> next;
    free(h);
    s -> head = p;
    if (NULL == s -> head) s -> tail = s -> head;

    return s;
}

LISTA * free_list(LISTA * s) {
    while (s -> head) {
        pull(s);
    }

    return s;
}

LISTA * init_list(void) {
    LISTA * p = malloc(1 * sizeof( * p));

    if (NULL == p) {
        fprintf(stderr, "LINE: %d, malloc() failed\n", __LINE__);
    }

    p -> head = p -> tail = NULL;

    return p;
}

void print_list(LISTA * ps) {
    ELEMENTO * p = NULL;

    if (ps) {
        for (p = ps -> head; p; p = p -> next) {
            print_list_element(p);
        }
    }

    printf("------------------\n");
}

void print_list_element(ELEMENTO * p) {
    if (p) {
        printf("Num = %d\n", p -> num);
    } else {
        printf("Struct nula\n");
    }
}

void erro(char * mensagem){
    printf("%s\n", mensagem);
    exit(1);
}
