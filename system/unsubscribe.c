/* unsubscribe.c - unsubscribe */

#include <xinu.h>

/*-------------------------------------------
 *  unsubscribe  -  Unsubscribe from a topic
 *-------------------------------------------
 */

syscall unsubscribe(topic16 topic) {
	if(topicstab[topic % NTOPICS][currpid].gid == -1) {
		return SYSERR;
	}
	topicstab[topic % NTOPICS][currpid].gid = -1;
	return OK;
}