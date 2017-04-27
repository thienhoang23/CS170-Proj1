//#include<stdio.h>
//#include<pthread.h>
#include <semaphore.h>

#ifndef _RWLOCK_H_
#define _RWLOCK_H_

class RWLock{
private:
#ifdef RWLOCK
	pthread_cond_t readCV;
	pthread_cond_t writeCV;
	int AW;
	int WW;
	int AR;
	int WR;
#endif
	pthread_mutex_t lock; 

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