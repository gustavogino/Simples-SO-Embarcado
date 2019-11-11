#include <pic18f4520.h>
#include <xc.h>
#include "user_tasks.h"
#include "kernel.h"
#include "sralloc.h"
#include "semaphore.h"
#include "pipe.h"

t_sem pedagio, ponte;
t_pipe msg;

// Função de definição
void user_tasks_config()  
{     
  //Todos os pinos da porta D tem que ser outputs
  TRISDbits.RD0 = 0;
  TRISDbits.RD1 = 0;
  TRISDbits.RD2 = 0;
  TRISDbits.RD3 = 0;
  TRISDbits.RD4 = 0;
  TRISDbits.RD5 = 0;
  TRISDbits.RD6 = 0;
  TRISDbits.RD7 = 0;
  
  TRISCbits.RC0 = 0;
  
  //Valor inicial das portas
  PORTDbits.RD0 = 0;
  PORTDbits.RD1 = 0;
  PORTDbits.RD2 = 1;
  PORTDbits.RD3 = 0;
  PORTDbits.RD4 = 0;
  PORTDbits.RD5 = 0;
  PORTDbits.RD6 = 0;
  PORTDbits.RD7 = 1;
  PORTCbits.RC0 = 0;
  
  
  sem_create(&pedagio, 0);
  sem_create(&ponte, 1);
  pipe_create(&msg);
    
}
// Tarefas do sistema
void task_sinaleira_a() //Prioridade Baixa = 1
{     
    for(;;){
        //Se pedagio disponível define como ocupado
        sem_tryWait(&pedagio);

        //Fecha a sinaleira B
        PORTDbits.RD5 = 0;
        PORTDbits.RD6 = 0;
        PORTDbits.RD7 = 1;

        //Coloca pra aguardar a sua sinaleira (Sinaleira A)
        PORTDbits.RD0 = 0;
        PORTDbits.RD1 = 1;
        PORTDbits.RD2 = 0; 
        
        //Delay para fins de simulação
        delay_os(1500);
        
        //Se ponte disponível define como ocupado
        sem_tryWait(&ponte);

        //Abre a sinaleira A
        PORTDbits.RD0 = 1;
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 0; 

        // Aguarda o carro passar
        delay_os(1500);

        //Libera utilização da ponte
        sem_post(&ponte);  

        //Libera utilização do pedágio
        sem_post(&pedagio);

        //Fecha a sinaleira A
        PORTDbits.RD0 = 0;
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 1;
        
        //Para fins de simulação
        delay_os(1500);
    }
}

void task_sinaleira_b() //Prioridade Baixa = 1
{     
    for(;;){
        //Se pedagio disponível define como ocupado
        sem_tryWait(&pedagio);

        //Fecha a sinaleira A
        PORTDbits.RD0 = 0;
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 1;

        //Coloca pra aguardar a sua sinaleira (Sinaleira B)
        PORTDbits.RD5 = 0;
        PORTDbits.RD6 = 1;
        PORTDbits.RD7 = 0;    

        //Se ponte disponível define como ocupado
        sem_tryWait(&ponte);
        
        //Apenas para fins de visualiazação na simulação
        delay_os(1500);

        //Abre a sinaleira A
        PORTDbits.RD5 = 1;
        PORTDbits.RD6 = 0;
        PORTDbits.RD7 = 0; 

        // Aguarda o carro passar
        delay_os(1500);

        //Libera utilização da ponte
        sem_post(&ponte);  

        //Libera utilização do pedágio
        sem_post(&pedagio);

        //Fecha a sinaleira A
        PORTDbits.RD5 = 0;
        PORTDbits.RD6 = 0;
        PORTDbits.RD7 = 1;
        
        //Para fins de simulação
        delay_os(1500);
    }
}

void task_barco_chegando() //Prioridade Média = 3
{
    byte dado;
    for(;;){
        pipe_read(&msg, &dado);
        
        if (dado == 't'){
            PORTCbits.RC0 = ~PORTCbits.RC0;
        }
        
        //Define ambas as sinaleiras como Aguarde
        PORTDbits.RD0 = 0;
        PORTDbits.RD1 = 1;
        PORTDbits.RD2 = 0;

        PORTDbits.RD5 = 0;
        PORTDbits.RD6 = 1;
        PORTDbits.RD7 = 0;

        //Se ponte disponível define como ocupado
        sem_tryWait(&ponte);

        //Apenas para fins de visualiazação na simulação
        delay_os(1500);

        //Define ambas as sinaleiras como Bloqueado
        PORTDbits.RD0 = 0;
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 1;

        PORTDbits.RD5 = 0;
        PORTDbits.RD6 = 0;
        PORTDbits.RD7 = 1;

        //Ativa o levantamento da ponte
        PORTDbits.RD4 = 1;

        //Aguarda o tempo para o barco passar pela ponte
        delay_os(1500);

        //Libera utilização da ponte
        sem_post(&ponte);

        //Desativa o levantamento da ponte
        PORTDbits.RD4 = 0; 
    }
}

void task_alarme() //Prioridade Alta = 5
{
    for(;;){
        pipe_write(&msg, 't');
        //Define ambas as sinaleiras como Bloqueado
        PORTDbits.RD0 = 0;
        PORTDbits.RD1 = 0;
        PORTDbits.RD2 = 1;

        PORTDbits.RD5 = 0;
        PORTDbits.RD6 = 0;
        PORTDbits.RD7 = 1;
        
        //Desativa ponte
        PORTDbits.RD4 = 0;
    
        // Pisca a luz de alerta e bloqueia o estado da ponte    
        delay_os(500);
        PORTDbits.RD3 = ~PORTDbits.RD3;       
    }
    pipe_clean(&msg);
}