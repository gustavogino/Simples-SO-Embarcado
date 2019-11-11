#include "timer0.h"
#include "kernel.h"

void interrupt() timer_zero_int() {
    dispacher(READY);
}
