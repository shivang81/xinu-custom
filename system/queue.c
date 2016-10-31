/* queue.c - enqueue, dequeue */
/*
Modified by Shivang Gupta
Added functions to add, remove messages from queue. 
Also added functions to check the status(empty/full) of queue and display the messages in queue.
*/

#include <xinu.h>

struct qentry   queuetab[NQENT];    /* Table of process queues  */

/*------------------------------------------------------------------------
 *  enqueue  -  Insert a process at the tail of a queue
 *------------------------------------------------------------------------
 */
pid32   enqueue(
      pid32     pid,        /* ID of process to insert  */
      qid16     q       /* ID of queue to use       */
    )
{
    qid16   tail, prev;     /* Tail & previous node indexes */

    if (isbadqid(q) || isbadpid(pid)) {
        return SYSERR;
    }

    tail = queuetail(q);
    prev = queuetab[tail].qprev;

    queuetab[pid].qnext  = tail;    /* Insert just before tail node */
    queuetab[pid].qprev  = prev;
    queuetab[prev].qnext = pid;
    queuetab[tail].qprev = pid;
    return pid;
}

/*------------------------------------------------------------------------
 *  dequeue  -  Remove and return the first process on a list
 *------------------------------------------------------------------------
 */
pid32   dequeue(
      qid16     q       /* ID queue to use      */
    )
{
    pid32   pid;            /* ID of process removed    */

    if (isbadqid(q)) {
        return SYSERR;
    } else if (isempty(q)) {
        return EMPTY;
    }

    pid = getfirst(q);
    queuetab[pid].qprev = EMPTY;
    queuetab[pid].qnext = EMPTY;
    return pid;
}

/*----------------------------------------------
 *  isMQfull  -  Check if message queue is full
 *----------------------------------------------
 */
uint16  isMQfull(struct procent *prptr)
{                     
    if( (prptr->qfront == prptr->qrear + 1) || (prptr->qfront == 0 && prptr->qrear == NMSGS - 1)) 
        return (uint16)1;
    return (uint16)0;
}
 
/*------------------------------------------------
 *  isMQempty  -  Check if message queue is empty
 *------------------------------------------------
 */ 
uint16 isMQempty(struct procent *prptr)
{
    if(prptr->qfront == -1) 
        return (uint16)1;
    return (uint16)0;
}

/*------------------------------------------
 *  MQinsert  -  Insert message into queue
 *------------------------------------------
 */ 
void MQinsert(umsg32 msg, struct procent *prptr)
{                      
    if( isMQfull(prptr)) 
        kprintf("\n\n Overflow!!!!\n\n");
    else {
        if(prptr->qfront == -1) 
            prptr->qfront = 0;
        prptr->qrear = (prptr->qrear+1) % NMSGS;
        prptr->prmsgs[prptr->qrear] = msg;
    }
}

/*------------------------------------------
 *  MQdelete  -  Remove message from queue
 *------------------------------------------
 */ 
umsg32 MQdelete(struct procent *prptr)
{                      
    umsg32 msg;
    if(isMQempty(prptr)) { 
        kprintf("\n\nUnderflow!!!!\n\n");
        return(-1); 
    }
    else {
        msg = prptr->prmsgs[prptr->qfront];
        if(prptr->qfront == prptr->qrear) { 
                prptr->qfront = -1; 
                prptr->qrear = -1;
            }
        else
            prptr->qfront = (prptr->qfront+1) % NMSGS;
        return(msg);
    }
}

/*------------------------------------------
 *  displayMQ  -  Display messages in queue
 *------------------------------------------
 */ 
void displayMQ(struct procent *prptr)
{       
    int i;
    if(isMQempty(prptr)) 
        printf("Empty Queue\n");
    else {
        for(i = prptr->qfront; i != prptr->qrear; i = (i + 1) % NMSGS)
            kprintf("%d ", prptr->prmsgs[i]);
        kprintf("%d \n", prptr->prmsgs[i]);
    }
}