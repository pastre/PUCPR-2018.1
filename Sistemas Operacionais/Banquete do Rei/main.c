#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define MAX_PRATOS 12
#define MAX_CONVIDADOS 20
#define COOK_TIME 100
#define TRUE 1
#define FALSE 0
#define EAT_TIME 3

int pratos_disp = MAX_PRATOS;
int chamaCozinheiro = FALSE;
int cozinhando = FALSE;
int nServidos = 0;

HANDLE comeMutex;
//TODO: TYPEDEF STRUCT COM UM ID, PARA MARCAR NUM VETOR QUEM COMEU QUANTO

DWORD WINAPI init_convidado(void* data){
    printf("Convidado chegou! \n");
    DWORD dwWaitResult;
    while(1)
    {
        if(cozinhando)
            Sleep(1);
        dwWaitResult = WaitForSingleObject(comeMutex, INFINITE);
        switch (dwWaitResult)
        {// The thread got ownership of the mutex
        case WAIT_OBJECT_0:
            if(pratos_disp >0)
            {
                Sleep(EAT_TIME);
                pratos_disp --;
                printf("%d comeu! Pratos: %d\n", GetCurrentThreadId(), pratos_disp);
            }
            else if(!cozinhando && !chamaCozinheiro)
            {
                printf("Chamei o cozinheiro...\n");
                chamaCozinheiro = TRUE;
            }
            if (! ReleaseMutex(comeMutex))
            {
                printf("Falha opra liberar");
            }
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
    HANDLE cozinheiro = CreateThread(NULL, 0, init_cozinheiro, NULL, 0, NULL);;
    HANDLE * convidados = (HANDLE * ) malloc(MAX_CONVIDADOS * sizeof(HANDLE));
    int sai = FALSE;
    for (int i = 0; i < MAX_CONVIDADOS; i++)
        convidados[i] = CreateThread(NULL, 0, init_convidado, NULL, 0, NULL);

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
        if(chamaCozinheiro){
            for (int i = 0; i < MAX_CONVIDADOS; i++)
                SuspendThread(convidados[i]);
            ResumeThread(cozinheiro);
            SuspendThread(cozinheiro);
            for (int i = 0; i < MAX_CONVIDADOS; i++)
                ResumeThread(convidados[i]);
//
        }
    }

    return 0;
}
