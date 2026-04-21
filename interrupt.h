/* interrupt.h */
#ifndef INTERRUPT_H
#define INTERRUPT_H

void Init_Interrupts(void);
void eint0_isr(void) __irq;
void eint1_isr(void) __irq;

#endif
