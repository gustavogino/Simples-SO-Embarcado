#include <pic18f4520.h>
#include <xc.h>
#include "kernel.h"
#include "user_tasks.h"
#include "sralloc.h"

#pragma config WDT      = OFF
#pragma config PBADEN   = OFF

__asm("GLOBAL _task_idle, _task_sinaleira_a, _task_sinaleira_b, _task_barco_chegando, _task_alarme");

int main(void) 
{  
    //Define as configurações do Sistema Operacional
    os_config();  
    
    SRAMInitHeap();
    
    //Cria as tarefas/Tasks
    if (create_task(3, 3, &task_barco_chegando) == OS_ERROR) return OS_ERROR;
    if (create_task(4, 5, &task_alarme) == OS_ERROR) return OS_ERROR;
    if (create_task(1, 1, &task_sinaleira_a) == OS_ERROR) return OS_ERROR;
    if (create_task(2, 2, &task_sinaleira_b) == OS_ERROR) return OS_ERROR;
    
   
    //Inicia o Sistema Operacional
    start_OS();
    
    while(1);
    
  return OS_OK;
}
