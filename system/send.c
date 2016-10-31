/* send.c - send */
/* Modified by Shivang Gupta
Added definitions for sendMsg, sendMsgs and sendnMsg system calls. 
*/

#include <xinu.h>

/*------------------------------------------------------------------------
 *  send  -  Pass a message to a process and start recipient if waiting
 *------------------------------------------------------------------------
 */
syscall	send(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg		/* Contents of message		*/
	)
{
	intmask	mask;			/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (prptr->prhasmsg) {
		restore(mask);
		return SYSERR;
	}
	prptr->prmsg = msg;		/* Deliver message		*/
	prptr->prhasmsg = TRUE;		/* Indicate message is waiting	*/

	/* If recipient waiting or in timed-wait make it ready */

	if (prptr->prstate == PR_RECV) {
		ready(pid);
	} else if (prptr->prstate == PR_RECTIM) {
		unsleep(pid);
		ready(pid);
	}
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*---------------------------------------------------------------------------------------------------
 *  sendMsg  -  Pass a message to the message queue for the process and start recipient if waiting
 *---------------------------------------------------------------------------------------------------
 */
syscall	sendMsg(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	msg			/* Contents of message		*/
	)
{
	intmask	mask;				/* Saved interrupt mask		*/
	struct	procent *prptr;		/* Ptr to process's table entry	*/

	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}

	prptr = &proctab[pid];
	if (isMQfull(prptr)) {	/* If queue is full the message can't be delivered */
		restore(mask);
		return SYSERR;
	}
	MQinsert(msg, prptr);		/* Deliver message		*/

	/* If recipient waiting */
	if (prptr->prstate == PR_RECQ) {
		ready(pid);
	} 
	restore(mask);		/* Restore interrupts */
	return OK;
}

/*----------------------------------------------------
 *  sendMsgs  -  Pass multiple messages to a process
 *----------------------------------------------------
 */
uint32	sendMsgs(
	  pid32		pid,		/* ID of recipient process	*/
	  umsg32	*msg,		/* Contents of messages		*/
	  uint32 	msg_count	/* count of messages */
	)
{	
	intmask mask;
	uint32 sent_count = 0;
	uint32 res;
	uint32 i;
	mask = disable();
	if (isbadpid(pid)) {
		restore(mask);
		return SYSERR;
	}
	resched_cntl(DEFER_START);
	for(i = 0; i < msg_count; i++) {
		res = sendMsg(pid, *msg++);
		if (res == OK)
			sent_count++;
	}
	if (sent_count == 0) {
		resched_cntl(DEFER_STOP);
		restore(mask);
		return SYSERR;
	}
	resched_cntl(DEFER_STOP);
	restore(mask);
	return sent_count;
}

/*-------------------------------------------------------
 *  sendnMsg  -  Pass the message to multiple processes
 *-------------------------------------------------------
 */
uint32	sendnMsg(
	  uint32 pid_count,		/* count of processes */
	  pid32		*pids,		/* IDs of recipient processes	*/
	  umsg32	msg			/* Contents of message		*/
	)
{	
	intmask mask;
	uint32 sent_count = 0;
	uint32 res;
	uint32 i;
	mask = disable();
	resched_cntl(DEFER_START);
	for(i = 0; i < pid_count; i++) {
		res = sendMsg(*pids++, msg);
		if (res == OK)
			sent_count++;
	}
	if (sent_count == 0) {
		resched_cntl(DEFER_STOP);
		restore(mask);
		return SYSERR;
	}
	resched_cntl(DEFER_STOP);
	restore(mask);
	return sent_count;
}