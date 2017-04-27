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
public:
      HashMap();
      int get(int key);
      void put(int key, int value); 
      void remove(int key); 
      ~HashMap(); 
};