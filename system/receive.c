/* receive.c - receive */
/*
Modified by Shivang Gupta
Added definitions for receiveMsg and receiveMsgs system calls.
*/

#include <xinu.h>

/*------------------------------------------------------------------------
 *  receive  -  Wait for a message and return the message to the caller
 *------------------------------------------------------------------------
 */
umsg32	receive(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (prptr->prhasmsg == FALSE) {
		prptr->prstate = PR_RECV;
		resched();		/* Block until message arrives	*/
	}
	msg = prptr->prmsg;		/* Retrieve message		*/
	prptr->prhasmsg = FALSE;	/* Reset message flag		*/
	restore(mask);
	return msg;
} 

/*---------------------------------------------------------------------------------------
 *  receiveMsg  -  Wait for a message from sendMsg and return the message to the caller
 *---------------------------------------------------------------------------------------
 */
umsg32	receiveMsg(void)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/
	umsg32	msg;			/* Message to return		*/

	mask = disable();
	prptr = &proctab[currpid];
	if (isMQempty(prptr)) {
		prptr->prstate = PR_RECQ;
		resched();		/* Block until message arrives	*/
	}
	msg = MQdelete(prptr);		/* Retrieve message		*/
	restore(mask);
	return msg;
}

/*------------------------------------------------
 *  receiveMsgs  -  Wait for a group of messages 
 *------------------------------------------------
 */
syscall receiveMsgs(umsg32 *msgs, uint32 msg_count) {
	int i;
	for (i = 0; i < msg_count; ++i)
	{
		msgs[i] = receiveMsg();
	}
}



