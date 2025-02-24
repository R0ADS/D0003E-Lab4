// The Timber compiler <timber-lang.org>
// 
// Copyright 2008-2012 Johan Nordlander <nordland@csee.ltu.se>
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 
// 3. Neither the names of the copyright holder and any identified
//    contributors, nor the names of their affiliations, may be used to 
//    endorse or promote products derived from this software without 
//    specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE CONTRIBUTORS ``AS IS'' AND ANY EXPRESS
// OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS OR CONTRIBUTORS BE LIABLE FOR
// ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
// OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

/*
 * 
 * TinyTimber.c
 *
 */

#include "TinyTimber.h"

#include <setjmp.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define STACKSIZE       96
#define NMSGS           15
#define NTHREADS        4

#define STATUS()        (SREG & 0x80)
#define DISABLE(s)      { s = STATUS(); cli(); }
#define ENABLE(s)       if (s) sei();
#define SLEEP()         { SMCR = 0x01; __asm__ __volatile__ ("sleep" ::); }
#define PANIC()         { LCDDR0 = 0xFF; LCDDR1 = 0xFF; LCDDR2 = 0xFF; while (1) SLEEP(); }
                        // Light up the display...
#define SETSTACK(buf,a) { *((unsigned int *)(buf)+8) = (unsigned int)(a) + STACKSIZE - 4; \
                          *((unsigned int *)(buf)+9) = (unsigned int)(a) + STACKSIZE - 4; }
#define SETPC(buf, a)   *((unsigned int *)((unsigned char *)(buf) + 21)) = (unsigned int)(a)

#define TIMER_INIT()    { CLKPR = 0x80; CLKPR = 0x00; \
                          TCNT1 = 0x0000; TCCR1B = 0x04; TIMSK1 = 0x01; }
                        // No system clock prescaling
                        // Normal mode, clk/256 prescaling, enable timer overflow interrupts
#define TIMER_OCLR()    // No timer overflow interrupt clear necessary
#define TIMER_CCLR()    // No timer compare interrupt clear necessary
#define TIMERGET(x)     (x) = ((Time)overflows << 16) | (unsigned int)TCNT1; \
                        if (TIFR1 & 0x01) (x) = ((Time)(overflows+1) << 16) | (unsigned int)TCNT1;
#define TIMERSET(x)     { if ((x) && (HIGH16((x)->baseline) == overflows)) { \
                             OCR1A = MAX(LOW16((x)->baseline), (unsigned int)TCNT1+1); \
                             TIMSK1 |= 0x02; \
                          } else \
                             TIMSK1 &= ~0x02; \
                        }
#define HIGH16(x)       (int)((x) >> 16)
#define LOW16(x)        (unsigned int)((x) & 0xffff)
#define MAX(a,b)        ( (a)-(b) <= 0 ? (b) : (a) )
#define INFINITY        0x7fffffffL
#define INF(a)          ( (a)==0 ? INFINITY : (a) )

typedef struct thread_block *Thread;

#define INSTALLED_TAG (Thread)1

struct msg_block {
    Msg next;                // for use in linked lists
    Time baseline;           // event time reference point
    Time deadline;           // absolute deadline (=priority)
    Object *to;              // receiving object
    Method method;           // code to run
    int arg;                 // argument to the above
};

struct thread_block {
    Thread next;             // for use in linked lists
    Msg msg;                 // message under execution
    Object *waitsFor;        // deadlock detection link
    jmp_buf context;         // machine state
};

struct stack {
    unsigned char stack[STACKSIZE];
};

struct msg_block    messages[NMSGS];
struct thread_block threads[NTHREADS];
struct stack        stacks[NTHREADS];

struct thread_block thread0;

Msg msgPool         = messages;
Msg msgQ            = NULL;
Msg timerQ          = NULL;
Time timestamp      = 0;
int overflows       = 0;

Thread threadPool   = threads;
Thread activeStack  = &thread0;
Thread current      = &thread0;

Method  mtable[N_VECTORS];
Object *otable[N_VECTORS];

static void schedule(void);

#define TIMER_COMPARE_INTERRUPT  ISR(TIMER1_COMPA_vect)
#define TIMER_OVERFLOW_INTERRUPT ISR(TIMER1_OVF_vect)

#define IRQ(n,v) ISR(v) { TIMERGET(timestamp); if (mtable[n]) mtable[n](otable[n],n); schedule(); }

IRQ(IRQ_INT0,            INT0_vect);
IRQ(IRQ_PCINT0,          PCINT0_vect);
IRQ(IRQ_PCINT1,          PCINT1_vect);
IRQ(IRQ_TIMER2_COMP,     TIMER2_COMP_vect);
IRQ(IRQ_TIMER2_OVF,      TIMER2_OVF_vect);
IRQ(IRQ_TIMER0_COMP,     TIMER0_COMP_vect);
IRQ(IRQ_TIMER0_OVF,      TIMER0_OVF_vect);
IRQ(IRQ_SPI_STC,         SPI_STC_vect);
IRQ(IRQ_USART0_RX,       USART0_RX_vect);
IRQ(IRQ_USART0_UDRE,     USART0_UDRE_vect);
IRQ(IRQ_USART0_TX,       USART0_TX_vect);
IRQ(IRQ_USI_START,       USI_START_vect);
IRQ(IRQ_USI_OVERFLOW,    USI_OVERFLOW_vect);
IRQ(IRQ_ANALOG_COMP,     ANALOG_COMP_vect);
IRQ(IRQ_ADC,             ADC_vect);
IRQ(IRQ_EE_READY,        EE_READY_vect);
IRQ(IRQ_SPM_READY,       SPM_READY_vect);
IRQ(IRQ_LCD,             LCD_vect);

/* queue manager */
void enqueueByDeadline(Msg p, Msg *queue) {
    Msg prev = NULL, q = *queue;
    while (q && (q->deadline <= p->deadline)) {
        prev = q;
        q = q->next;
    }
    p->next = q;
    if (prev == NULL)
        *queue = p;
    else
        prev->next = p;
}

void enqueueByBaseline(Msg p, Msg *queue) {
    Msg prev = NULL, q = *queue;
    while (q && (q->baseline <= p->baseline )) {
        prev = q;
        q = q->next;
    }
    p->next = q;
    if (prev == NULL)
        *queue = p;
    else
        prev->next = p;
}

Msg dequeue(Msg *queue) {
    Msg m = *queue;
    if (m)
        *queue = m->next;
    else
        PANIC();  // Empty queue, kernel panic!!!
    return m;
}

void insert(Msg m, Msg *queue) {
    m->next = *queue;
    *queue = m;
}

void push(Thread t, Thread *stack) {
    t->next = *stack;
    *stack = t;
}

Thread pop(Thread *stack) {
    Thread t = *stack;
    *stack = t->next;
    return t;
}

static int remove(Msg m, Msg *queue) {
    Msg prev = NULL, q = *queue;
    while (q && (q != m)) {
        prev = q;
        q = q->next;
    }
    if (q) {
        if (prev)
            prev->next = q->next;
        else
            *queue = q->next;
        return 1;
    }
    return 0;
}

TIMER_OVERFLOW_INTERRUPT {
    TIMER_OCLR();
    overflows++;
    TIMERSET(timerQ);
}

TIMER_COMPARE_INTERRUPT {
    Time now;
    TIMER_CCLR();
    TIMERGET(now);
    while (timerQ && (timerQ->baseline - now <= 0))
        enqueueByDeadline( dequeue(&timerQ), &msgQ );
    TIMERSET(timerQ);
    schedule();
}

/* context switching */
static void dispatch( Thread next ) {
    if (setjmp( current->context ) == 0) {
        current = next;
        longjmp( next->context, 1 );
    }
}

static void run(void) {
    while (1) {
        Msg this = current->msg = dequeue(&msgQ);
        Msg oldMsg;
        char status = 1;
        
        ENABLE(status);
        SYNC(this->to, this->method, this->arg);
        DISABLE(status);
        insert(this, &msgPool);
       
        oldMsg = activeStack->next->msg;
        if (!msgQ || (oldMsg && (msgQ->deadline - oldMsg->deadline > 0))) {
            Thread t;
            push(pop(&activeStack), &threadPool);
            t = activeStack;  // can't be NULL, may be &thread0
            while (t->waitsFor) 
	            t = t->waitsFor->ownedBy;
            dispatch(t);
        }
    }
}

static void idle(void) {
    schedule();
    ENABLE(1);
    while (1) {
        SLEEP();
    }
}

static void schedule(void) {
    Msg topMsg = activeStack->msg;
    if (msgQ && threadPool && ((!topMsg) || (msgQ->deadline - topMsg->deadline < 0))) {
        push(pop(&threadPool), &activeStack);
        dispatch(activeStack);
    }
}

/* communication primitives */
Msg async(Time bl, Time dl, Object *to, Method meth, int arg) {
    Msg m;
    Time now;
    char status;
    DISABLE(status);
    m = dequeue(&msgPool);
    m->to = to; 
    m->method = meth; 
    m->arg = arg;
    m->baseline = (status ? current->msg->baseline : timestamp) + bl;
    m->deadline = m->baseline + (dl > 0 ? dl : INFINITY);
    
    TIMERGET(now);
    if (m->baseline - now > 0) {        // baseline has not yet passed
        enqueueByBaseline(m, &timerQ);
        TIMERSET(timerQ);
    } else {                            // m is immediately schedulable
        enqueueByDeadline(m, &msgQ);
        if (status && threadPool && (msgQ->deadline - activeStack->msg->deadline < 0)) {
            push(pop(&threadPool), &activeStack);
            dispatch(activeStack);
        }
    }
    
    ENABLE(status);
    return m;
}

int sync(Object *to, Method meth, int arg) {
    Thread t;
    int result;
    char status, status_ignore;
    
    DISABLE(status);
    t = to->ownedBy;
    if (t) {                            // to is already locked
        while (t->waitsFor) 
            t = t->waitsFor->ownedBy;
        if (t == current || !status) {  // deadlock!
            ENABLE(status);
            return -1;
        }
        if (to->wantedBy)               // must be a lower priority thread
            to->wantedBy->waitsFor = NULL;
        to->wantedBy = current;
        current->waitsFor = to;
        dispatch(t);
        if (current->msg == NULL) {     // message was aborted (when called from run)
            ENABLE(status);
            return 0;
        }
    }
    to->ownedBy = current;
    ENABLE(status && (to->wantedBy != INSTALLED_TAG));
    result = meth(to, arg);
    DISABLE(status_ignore);
    to->ownedBy = NULL; 
    t = to->wantedBy;
    if (t && (t != INSTALLED_TAG)) {      // we have run on someone's behalf
        to->wantedBy = NULL; 
        t->waitsFor = NULL;
        dispatch(t);
    }
    ENABLE(status);
    return result;
}

void ABORT(Msg m) {
    char status;
    DISABLE(status);
    if (remove(m, &timerQ) || remove(m, &msgQ))
        insert(m, &msgPool);
    else {
        Thread t = activeStack;
        while (t) {
            if ((t != current) && (t->msg == m) && (t->waitsFor == m->to)) {
	            t->msg = NULL;
	            insert(m, &msgPool);
	            break;
            }
            t = t->next;
        }
    }
    ENABLE(status);
}

void T_RESET(Timer *t) {
    t->accum = STATUS() ? current->msg->baseline : timestamp;
}

Time T_SAMPLE(Timer *t) {
    return (STATUS() ? current->msg->baseline : timestamp) - t->accum;
}

Time CURRENT_OFFSET(void) {
    char status;
    Time now;
    DISABLE(status);
    TIMERGET(now);
    ENABLE(status);
    return now - (status ? current->msg->baseline : timestamp);
}

    
/* initialization */
static void initialize(void) {
    int i;

    for (i=0; i<NMSGS-1; i++)
        messages[i].next = &messages[i+1];
    messages[NMSGS-1].next = NULL;
    
    for (i=0; i<NTHREADS-1; i++)
        threads[i].next = &threads[i+1];
    threads[NTHREADS-1].next = NULL;
    
    for (i=0; i<NTHREADS; i++) {
        setjmp( threads[i].context );
        SETSTACK( &threads[i].context, &stacks[i] );
        SETPC( &threads[i].context, run );
        threads[i].waitsFor = NULL;
    }

    thread0.next = NULL;
    thread0.waitsFor = NULL;
    thread0.msg = NULL;
    
    TIMER_INIT();
}

void install(Object *obj, Method m, enum Vector i) {
    if (i >= 0 && i < N_VECTORS) {
        char status;
        DISABLE(status);
        otable[i] = obj;
        mtable[i] = m;
        obj->wantedBy = INSTALLED_TAG;  // Mark object as subject to synchronization by interrupt disabling
        ENABLE(status);
    }
}

int tinytimber(Object *obj, Method m, int arg) {
    char status;
    DISABLE(status);
    initialize();
    ENABLE(1);
    if (m != NULL)
        //m(obj, arg);
		ASYNC(obj,m,arg);
    DISABLE(status);
    idle();
    return 0;
}
