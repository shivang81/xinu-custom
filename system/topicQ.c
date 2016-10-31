/* topicQ.c - topicQfull, topicQfull, topicQinsert, topicQdelete */

#include <xinu.h>

struct topicdata topicQ[topicQSIZE];
int topicQf = -1;
int topicQr = -1;

/*------------------------------------------------------------
 *  topicQfull  -  Returns whether topic queue is full or not
 *------------------------------------------------------------
 */
int  topicQfull()
{   
    if( (topicQf==topicQr+1) || (topicQf == 0 && topicQr== topicQSIZE-1)) return 1;
    return 0;
}


/*--------------------------------------------------------------
 *  topicQempty  -  Returns whether topic queue is empty or not
 *--------------------------------------------------------------
 */ 
int topicQempty()
{    
    if(topicQf== -1) return 1;
    return 0;
}

/*-------------------------------------------------------------
 *  topicQinsert  -  Inserts published data in the topic queue
 *-------------------------------------------------------------
 */  
void topicQinsert(struct topicdata elem)
{                       
    if( topicQfull()) {{kprintf("\n\ntopicQ Overflow!!!!\n\n");}}
    else
    {
        if(topicQf==-1)topicQf=0;
        topicQr=(topicQr+1) % topicQSIZE;
        topicQ[topicQr]=elem;
    }
}

/*-----------------------------------------------------------
 *  topicQdelete  -  Dequeues topicdata from the topic queue
 *-----------------------------------------------------------
 */ 
struct topicdata topicQdelete()
{   
    struct topicdata elem;
    if(topicQempty()){ kprintf("\n\ntopicQ Underflow!!!!\n\n");
    return elem; }
    else
    {
        elem=topicQ[topicQf];
        if(topicQf==topicQr){ topicQf=-1; topicQr=-1;} 
        else
            topicQf=(topicQf+1) % topicQSIZE;
        return elem;
    }
}  
  