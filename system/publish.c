/* publish.c - publish */

#include <xinu.h>

/*--------------------------------------
 *  publish  -  Publish data to a topic
 *--------------------------------------
 */

syscall publish(topic16 topic, void* data, uint32 size) {
	struct topicdata td;
	td.topic = topic;
	td.data = data;
	td.size = size;
	topicQinsert(td);
	return OK;		
}