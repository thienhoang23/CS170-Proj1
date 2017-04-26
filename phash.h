/**
 * Code is based on
 *http://www.algolist.net/Data_structures/Hash_table/Chaining
 *
 **/

#include "rwlock.h"

class LinkedHashEntry {
private:
      int key;
      int value;
      LinkedHashEntry *next;
public:
      LinkedHashEntry(int key, int value); 
      int getKey(); 
      int getValue();
      void setValue(int value);
 
      LinkedHashEntry *getNext(); 
      void setNext(LinkedHashEntry *next); 
};


class HashMap {
private:
      LinkedHashEntry **table;
      #ifndef FINEGRAIN
            RWLock rwlock;
      #else
            RWLock* rwlocks;
      #endif
      // double get_total_time;
      // double put_total_time;
      // double remove_total_time;
public:
      HashMap();
      // double getTotalGetTime(){ return get_total_time;}
      // double getTotalPutTime(){ return put_total_time;}
      // double getTotalRemoveTime(){ return remove_total_time;}
      int get(int key);
      void put(int key, int value); 
      bool remove(int key); 
      ~HashMap(); 
};