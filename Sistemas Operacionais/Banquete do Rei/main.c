#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>

#define MAX_PRATOS 12
#define MAX_CONVIDADOS 30
#define COOK_TIME 1000
#define TRUE 1
#define FALSE 0
#define TEMPO_REI 5000

int pratos_disp = MAX_PRATOS;
int chamaCozinheiro = FALSE;
int cozinhando = FALSE;
int nServidos = 0;
int refeicoesPorSeg = 0;
int a[MAX_CONVIDADOS];
HANDLE comeMutex;
//TODO: TYPEDEF STRUCT COM UM ID, PARA MARCAR NUM VETOR QUEM COMEU QUANTO

int eat_time(){
    srand(time(NULL));
    int ret = (rand() % 4 + 2)*100;
    return ret;
}

DWORD WINAPI init_convidado(void* data){
    DWORD dwWaitResult;
    int id = (int) data;
    printf("Convidado chegou! ID: %d\n", id);
    while(1)
    {
        if(cozinhando){
            Sleep(1);
            continue;
        }
        dwWaitResult = WaitForSingleObject(comeMutex, INFINITE);
        switch (dwWaitResult)
        {// The thread got ownership of the mutex
        case WAIT_OBJECT_0:
            if(pratos_disp >0)
            {
                printf("%d comeu! Pratos disponiveis: %d\n", id, pratos_disp);
                pratos_disp --;
                refeicoesPorSeg ++;
                a[id]++;
            }
            else if(!cozinhando && !chamaCozinheiro)
            {
                printf("Chamei o cozinheiro...\n");
                chamaCozinheiro = TRUE;
            }
            if (! ReleaseMutex(comeMutex))
            {
                printf("Falha opra liberar");
            }else{
                Sleep(eat_time());}
            break;
        case WAIT_ABANDONED:
            printf("OOPS");
            break;
        }
    }
    printf("Convidado indo embora...\n");
    return 0;
}

DWORD WINAPI init_cozinheiro(void* data){
    printf("Cozinheiro no role!\n");
    while(1)
    {
        if (chamaCozinheiro)
        {
            printf("Estou cozinhando mais...\n");
            cozinhando = TRUE;
            Sleep(COOK_TIME);
            pratos_disp = MAX_PRATOS;
            chamaCozinheiro = FALSE;
            cozinhando = FALSE;
            printf("Comida pronta!\n");
        }
        else
        {
            Sleep(1);
        }
    }

    printf("Flw cozinheiro\n");
    return 0;
}

int main(){
    HANDLE cozinheiro = CreateThread(NULL, 0, init_cozinheiro, NULL, 0, NULL);
    HANDLE * convidados = (HANDLE * ) malloc(MAX_CONVIDADOS * sizeof(HANDLE));
    DWORD dwWaitResult;
    int sai = FALSE;
    for (int i = 0; i < MAX_CONVIDADOS; i++)
        convidados[i] = CreateThread(NULL, 0, init_convidado, (void *) i, 0, NULL);
    int tmp;
    comeMutex = CreateMutex(NULL, FALSE, NULL);
  //  SuspendThread(cozinheiro);
    while(1)
    {
        for (int i = 0; i < MAX_CONVIDADOS; i++)
        {
            if (convidados[i])
                break;
            sai = TRUE;
        }
        if (sai)
            break;
        Sleep(TEMPO_REI);
        dwWaitResult = WaitForSingleObject(comeMutex, INFINITE);
        switch (dwWaitResult)
        {// The thread got ownership of the mutex
        case WAIT_OBJECT_0:
            printf("Pratos total: %d\n", refeicoesPorSeg);
            if (! ReleaseMutex(comeMutex))
            {
                printf("Falha opra liberar");
            }
            for(int j = 0; j < MAX_CONVIDADOS; j++){
                printf("\t%d comeu %d pratos\n", j, a[j]);
            }
            if(refeicoesPorSeg >=3000){
                for(int j = 0; j < MAX_CONVIDADOS; j++){
                    TerminateThread(convidados[j], 0);
                    printf("O convidado %d foi embora\n", j);
                }
                TerminateThread(cozinheiro, 0);
                printf("O cozinheiro foi embora\nO banquete acabou!\n");
                system("pause");
                return 0;
            }
            Sleep(10000);
            break;
        case WAIT_ABANDONED:
            printf("OOPS");
            break;
        }
    }


    return 0;
}
