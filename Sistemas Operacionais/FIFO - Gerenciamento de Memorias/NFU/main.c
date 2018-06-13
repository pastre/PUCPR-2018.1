#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct my_struct {
    int num;
    struct my_struct * next;
    char idade;
} DADO;

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

void envelhece_quadro(ELEMENTO * head, int num){
    if (head == NULL) return;
    if (head -> num == num){
        head -> idade = head -> idade > 1;
        return;
    }
    element_in_fifo(head -> next, num);
}

void computa_ciclo(ELEMENTO * head, int index, int * saida){
    if (head == NULL) return;
    if(head -> idade & 16) saida[index] = 1;
    else saida[index] = 0;
    computa_ciclo(head -> prox, num, index ++, saida);
}

void simula_nfu(LISTA * head, int num, int n_quadros, FILE * file){
    int i;
    int * acessos = (int *) malloc(sizeof(int) * num);
    int next_index, tam = 1, ciclo = 0;
    char buffer;
    while(1){
        fgets(buffer, 9, file);
        if(feof(file)) break;
        if (ciclo == num) computa_ciclo(head, 0, acessos);

        find_in_fifo(head, buffer, &next_index);
        //Se existe na fifo, envelheca o quadro
        if(next_index)envelhece_quadro(head, buffer)
        else{
            if(tam < n_quadros){
                head = push(head, buffer, &tam);
            }else{
                replace_oldest(head, buffer);
            }
        }

        ciclo += 1;
    }
}
int element_in_fifo(ELEMENTO * head, int num){
    if (head == NULL) return 0;
    if (head -> num == num) return 1;
    if (head -> next == NULL) return 0;
    return element_in_fifo(head -> next, num);
}

int find_in_fifo(ELEMENTO * head, int num, int * index){
    if (head == NULL) return 0;
    if (head -> num == num) return 1;
    if (head -> next == NULL) return 0;
    *(index) += 1;
    return element_in_fifo(head -> next, num);
}

LISTA * push(LISTA * s, int val, int * cont) {
    ELEMENTO * p = malloc(1 * sizeof( * p));

    if (NULL == p) {
        fprintf(stderr, "IN %s, %s: malloc() failed\n", __FILE__, "list_add");
        return s;
    }

    p -> num = val;
    p -> next = NULL;
    p -> idade = 0;
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
