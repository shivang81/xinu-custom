/*  main.c  - main */

#include <xinu.h>

pid32 a_id;
pid32 b_id;
pid32 c_id;
pid32 d_id;
pid32 broker_id;
sid32 publishsem;

void foo(topic16 topic, void* data, uint32 size) {
	kprintf("- Function foo() is called with topic = %d and data = %d, %d, %d and data-size=%d\n", topic, ((int*) data)[0], ((int*) data)[1], ((int*) data)[2], size);
	signal(publishsem);  // Now the publisher can edit the data
}

void bar(topic16 topic, void* data, uint32 size) {
	kprintf("- Function bar() is called with topic = %d and data = %d, %d and data-size=%d\n", topic, ((int*) data)[0], ((int*) data)[1], size);
	signal(publishsem);	// Now the publisher can edit the data
}

void processA() {
	topic16 topic = 10;
	kprintf("Process A subscribes to topic %d with handler foo()\n", topic);
	subscribe(topic, foo);
	sleep(5);
}

void processC() {
	sleep(1);
	topic16 topic = 12;
	kprintf("Process C subscribes to topic %d with handler bar()\n", topic);
	subscribe(topic, bar);
	sleep(15); 		//Sleeping so that the process doesnt end!!
}

void processB() {
	sleep(2);
	topic16 topic = 10;
	uint32 data[3] = {1,2,3};
	kprintf("Process B publishes %d, %d, %d to topic %d\n", ((int*) data)[0], ((int*) data)[1], ((int*) data)[2], topic);
	publish(topic, data, 3);
	wait(publishsem); 
	
	data[2] = 444; // changing the value of data[2]..

	sleep(2);

	uint32 data2[3] = {44,55};
	kprintf("Process B publishes %d, %d, to topic %d\n", ((int*) data2)[0], ((int*) data2)[1], topic);
	publish(12, data2, 2);
	wait(publishsem);
}

void call_handlers(topic16 topic, void* data, int32 size) {
	int topicid = topic % NTOPICS;
	int gid = topic / NTOPICS;	
	int i;
	for(i = 0; i < MAXSUB; i++) {
		if(topicstab[topicid][i].gid == gid || (gid == 0 && topicstab[topicid][i].gid != -1)) {
			topicstab[topicid][i].handler(topic, data, size);	
		}	
	}
}

void broker(void)
{		
	int i;
	while(1) {	  
		if(!topicQempty()) {
		    struct topicdata elem;
	        elem=topicQ[topicQf];
	        if(topicQf==topicQr){ topicQf=-1; topicQr=-1;} 
	        else
	            topicQf=(topicQf+1) % topicQSIZE;		    
			call_handlers(elem.topic, elem.data, elem.size);
		}
	}
}

process	main(void)
{
	recvclr();

	a_id = create(processA, 4096, 50, "processA", 0);
	b_id = create(processB, 4096, 50, "processB", 0);
	c_id = create(processC, 4096, 50, "processC", 0);
	broker_id = create(broker, 4096, 50, "broker", 0);
	
	resume(a_id);
	resume(b_id);
	resume(c_id);
	resume(broker_id);

	publishsem = semcreate(1);


	return OK;
}
