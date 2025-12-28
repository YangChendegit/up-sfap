#ifndef ONOFFESTIMA_H 
#define ONOFFESTIMA_H
#include "Abstract.h"

class onoffestima{
public: 
      struct cell{
       uint32_t count;
     // short int hotcount;
     // DATA_TYPE item;  // uint8_t status;
   };
    onoffestima(uint32_t _MEMORY, int _HASH_NUM, int _windowsize, int __alpha = 1, string _name = "SPLIT_O") {

        HASH_NUM = _HASH_NUM;
	window_size = _windowsize;
	int M = _MEMORY /(sizeof(cell) +1);
        bucketBitsets = new BitMap(M);
	bucketBitsets-> Clear();
        length = M/HASH_NUM;
        packets =0;
	//sketchBitsets = new BitMap(length);
	buckets =new  cell*[HASH_NUM];
	for(int i=0; i< HASH_NUM; i++) {
             buckets[i] = new cell[length];
	     memset(buckets[i], 0, sizeof(cell) * length);
	} 

    }
    /*
    void check( ) {
          for(int i = 0; i < HASH_NUM; i++) {
               for(int j = 0; j < LENGTH; j++)
                     if(cc[i][j] < 0) 

    }*/

    ~onoffestima() {
         delete bucketBitsets;
        for(uint32_t i = 0; i < HASH_NUM; i ++) {
               delete[] buckets[i];
         }
         delete [] buckets;

    }

    void Insert(DATA_TYPE item) {
          if(packets % window_size == 0) {
                NewWindow();
        }
        packets ++;
    
         for(uint32_t i=0; i< HASH_NUM; i++) {
               int32_t pos = hash2(item, i) % length; 

              if (bucketBitsets->Get(i*length + pos) == 0) {
                         buckets[i][pos].count += 1;
			 bucketBitsets->Set(i*length + pos);
              }
	 }	 
    }

   COUNT_TYPE query(DATA_TYPE item) {
       COUNT_TYPE ret = INT_MAX;
          for(uint32_t i=0; i<HASH_NUM;i++) {
		  int32_t pos = hash2(item, i) % length;
		  ret = MIN(ret, buckets[i][pos].count);
	  }

    	return ret;   
   }	   



    void NewWindow() {
    bucketBitsets-> Clear(); 
    }
    repMap Rep(COUNT_TYPE HIT) {
          repMap ret;
          
            
    
	  return ret;
    }

private:

   uint32_t length;

   BitMap * bucketBitsets;
   cell ** buckets;
   uint64_t packets;
   BitMap * sketchBitsets;
   COUNT_TYPE* sketch;
   int HASH_NUM;
   int window_size;

};
#endif

