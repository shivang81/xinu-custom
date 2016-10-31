/* subscribe.c - subscribe */

#include <xinu.h>

/*----------------------------------------------------
 *  subscribe  -  Subscribe to a topic with a handler
 *----------------------------------------------------
 */

syscall subscribe(topic16 topic, void (*hnd)(topic16, void*, uint32)) {
	if(topicstab[topic % NTOPICS][currpid].gid != -1) {
		return SYSERR;
	}
	topicstab[topic % NTOPICS][currpid].gid = topic/NTOPICS;
	topicstab[topic % NTOPICS][currpid].handler = hnd;
	return OK;
}