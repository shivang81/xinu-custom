/*  main.c  - main */

#include <xinu.h>

pid32 sender1_id;
pid32 receiver1_id;
pid32 sender2_id;
pid32 receiver2_id;


process sender1(void)
{	
	umsg32 msgs[3] = {10,20,30};
	umsg32 msg = 12;

	printf("Sender pid: %d sending message via sendMsg to %d: %d\n",sender1_id, receiver1_id, msg );
	sendMsg(receiver1_id, msg);
	sleep(1);
	printf("Sender pid: %d sending message via sendMsg to %d: %d\n",sender1_id, receiver1_id, 20 );
	sendMsg(receiver1_id, 20);
	sleep(1);
	printf("Sender pid: %d sending message via sendMsg to %d: %d\n",sender1_id, receiver2_id, 30 );
	sendMsg(receiver2_id, 30);
	sleep(1);
	printf("Sender pid: %d sending message via sendMsg to %d: %d\n",sender1_id, receiver2_id, 40 );
	sendMsg(receiver2_id, 40);
	sleep(1);
	printf("Sender pid: %d sending message via sendMsg to %d: %d\n",sender1_id, receiver1_id, 50 );
	sendMsg(receiver1_id, 50);
	sleep(1);

	printf("Sender pid: %d sending message via send to %d: %d\n",sender1_id, receiver1_id, 199 );
	send(receiver2_id, 199);
	sleep(1);

	printf("Sender pid: %d sending messages via sendMsgs to %d: %d, %d, %d\n",sender1_id, receiver1_id, msgs[0], msgs[1], msgs[2] );
	sendMsgs(receiver1_id, msgs, 3);
	sleep(1);

	printf("Sender pid: %d sending messages via sendMsgs to %d: %d, %d, %d\n",sender1_id, receiver2_id, msgs[0], msgs[1], msgs[2] );
	sendMsgs(receiver2_id, msgs, 3);
	sleep(1);

	return OK;
}

process receiver1(void)
{
	int i;
	umsg32 msgs[5];

	printf("Receiver pid: %d received: %d by receiveMsg system call\n", receiver1_id, receiveMsg());
	printf("Receiver pid: %d received: %d by receiveMsg system call\n", receiver1_id, receiveMsg());

	printf("Receiver pid: %d received: %d by receive system call\n", receiver1_id, receive());

	receiveMsgs(msgs, 5);
	for (i = 0; i < 5; ++i)
	{
		printf("Receiver pid: %d received: %d by receiveMsgs call\n", receiver1_id, msgs[i]);
	}
	return OK;
}

process sender2(void)
{	
	umsg32 msg = 13;
	pid32 pids[2] = {receiver1_id, receiver2_id};

	printf("Sender pid: %d sending message via sendMsg to %d: %d\n",sender2_id, receiver2_id, 70 );
	sendMsg(receiver2_id, 70);
	sleep(1);

	printf("Sender pid: %d sending message via send to %d: %d\n",sender2_id, receiver1_id, 99 );
	send(receiver1_id, 99);
	sleep(1);

	printf("Sender pid: %d sending messages via sendnMsg to %d, %d: %d\n",sender2_id, receiver1_id, receiver2_id, msg );
	sendnMsg(2, pids, msg);
	sleep(1);
	 
	printf("....Sleeping for 10 seconds. Will send final message after that....");
	sleep(10);

	printf("Sender pid: %d sending message via sendMsg to %d: %d\n",sender2_id, receiver2_id, 1000 );
	sendMsg(receiver2_id, 1000);
	return OK;
}

process receiver2(void)
{
	int i;
	printf("Receiver pid: %d received: %d by receiveMsg system call\n", receiver2_id, receiveMsg());
	printf("Receiver pid: %d received: %d by receiveMsg system call\n", receiver2_id, receiveMsg());
	printf("Receiver pid: %d received: %d by receive system call\n", receiver2_id, receive());
	umsg32 msgs[6];
	receiveMsgs(msgs, 6);
	for (i = 0; i < 6; ++i)
	{
		printf("Receiver pid: %d received: %d by receiveMsgs system call\n", receiver2_id, msgs[i]);
	}
	return OK;
}

process	main(void)
{
	recvclr();

	sender1_id = create(sender1, 4096, 50, "sender1", 0);
	receiver1_id = create(receiver1, 4096, 50, "receiver1", 0);
	sender2_id = create(sender2, 4096, 50, "sender2", 0);
	receiver2_id = create(receiver2, 4096, 50, "receiver2", 0);

	resched_cntl(DEFER_START);
	resume(sender1_id);
	resume(receiver1_id);

	resume(sender2_id);
	resume(receiver2_id);
	resched_cntl(DEFER_STOP);

	return OK;
}
