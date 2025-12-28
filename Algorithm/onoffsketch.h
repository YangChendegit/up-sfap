#ifndef ONOFF_H 
#define ONOFF_H
#include "Abstract.h"

class onoffsketch{
public: 
      struct cell{
       uint32_t count;
     // short int hotcount;
      DATA_TYPE item;  // uint8_t status;
   };
    onoffsketch(uint32_t _MEMORY, int _HASH_NUM, int _windowsize, int __alpha = 1, string _name = "SPLIT_O") {

        packets = 0;
        HASH_NUM = _HASH_NUM;
	window_size = _windowsize;
	length = _MEMORY /int((HASH_NUM* sizeof(cell) +sizeof(uint32_t) + HASH_NUM + 1.0/8));
        bucketBitsets = new BitMap(length * HASH_NUM);
	sketchBitsets = new BitMap(length);
        sketch = new COUNT_TYPE[length];
	memset(sketch, 0, sizeof(sketch));
	buckets =new  cell*[length];
	for(int i=0; i< length; i++) {
             buckets[i] = new cell[HASH_NUM];
	     memset(buckets[i], 0, sizeof(cell) * HASH_NUM);
	} 

    }
    /*
    void check( ) {
          for(int i = 0; i < HASH_NUM; i++) {
               for(int j = 0; j < LENGTH; j++)
                     if(cc[i][j] < 0) 

    }*/

    ~onoffsketch() {

	    
	    for(uint32_t i = 0; i < HASH_NUM; i ++) {
                 delete[] buckets[i];
         }
         delete [] buckets;
         delete sketch;
    }

    void Insert(DATA_TYPE item) {

	if(packets % window_size == 0) {
		NewWindow();
	}
	packets ++;
        uint32_t pos = hash2(item, 1) % length;
        uint32_t bucketBitPos = pos * HASH_NUM;

	for(auto i = 0; i < HASH_NUM; i++) {
	     if(buckets[pos][i].item == item) {
		    if (bucketBitsets->Get(bucketBitPos + i) == 0)
			    buckets[pos][i].count += 1;
		    bucketBitsets->Set(bucketBitPos + i);
		   // buckets[pos][i].count += (!bucketBitsets->SetNGet(bucketBitPos + i));
		   return;
	     }
       } 
       if(!sketchBitsets->Get(pos)) {
	       for(uint32_t i = 0; i < HASH_NUM; i++) {
		       if( buckets[pos][i].count == sketch[pos]) {
                               buckets[pos][i].item = item;
			       buckets[pos][i].count += 1;
			       bucketBitsets->Set(bucketBitPos + i);
			       return;
		       }
	       }
             sketch[pos] += 1;
	     sketchBitsets->Set(pos);
       }
       //packets ++;
    }
    void NewWindow() {
	   bucketBitsets->Clear();
	   sketchBitsets->Clear(); 
    }
    repMap Rep(COUNT_TYPE HIT) {
          repMap ret;
          
          for(uint32_t i = 0; i < length; i++)
              for(uint32_t j = 0; j < HASH_NUM; j++)
                 if(buckets[i][j].count >= HIT) {
                    ret[buckets[i][j].item] = buckets[i][j].count;
                 }
            
    
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

