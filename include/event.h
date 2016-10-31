/* event.h */
	
#define topicQSIZE 256							
#define NTOPICS	   256							/* Total number of topics in a group */
#define MAXSUB	   8							/* Maximum subscriber for a topic */

struct topic
{
	int16 gid;									/* Group id for topic */
	void (*handler)(topic16, void*, uint32);	//Function pointer of subscriber 
};

struct topicdata								/* Structure for topic and data published */
{
	topic16 topic;
	void* data;
	int32 size;
};

extern struct topicdata topicQ[];				/* queue to store topicdata */
extern int topicQf;								/* front of topicQ */
extern int topicQr;								/* rear of topicQ */

extern	struct 	topic topicstab[][MAXSUB];		/* table containing topics for subscription info */