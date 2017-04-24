//#include<stdio.h>
//#include<pthread.h>
#include <semaphore.h>

#ifndef _RWLOCK_H_
#define _RWLOCK_H_

class RWLock{
private:
#ifdef RWLOCK
	pthread_cond_t readCV = PTHREAD_COND_INITIALIZER;
	pthread_cond_t writeCV = PTHREAD_COND_INITIALIZER;
	int AW;
	int WW;
	int AR;
	int WR;
#endif
	pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; 
 
public:
		RWLock();
    	~RWLock();
    //Reader
    	void startRead();
    	void doneRead();
    // Writer
    	void startWrite();
    	void doneWrite();
};

#endif