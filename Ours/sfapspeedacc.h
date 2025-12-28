#ifndef SFAPSPEEDACC_H
#define SFAPSPEEDACC_H
#include "Abstract.h"

class sfapspeedacc{
public: 
      struct cell{
       uint32_t count;
      DATA_TYPE item;  
   };
    sfapspeedacc(uint32_t _MEMORY, int _HASH_NUM, int _windowsize, int __alpha = 1, string _name = "SPLIT_O") {
        NAME = _name;
        MEMORY = _MEMORY;
        HASH_NUM = _HASH_NUM;
	cell_num = (int)(_MEMORY / (sizeof(uint32_t) +sizeof(DATA_TYPE) + 1.0/8));
 	LENGTH = cell_num / HASH_NUM;
	window_size =  _windowsize;
        sum = -1;
	psum = 0;
        cc = new cell*[HASH_NUM];
	bitmap = new BitMap(cell_num);
        for(int i = 0; i < HASH_NUM; i++) {
                 cc[i] =  new cell[LENGTH];
                 for(int j = 0; j < LENGTH; j++)
                      cc[i][j].count = 0;
          }
      alpha = __alpha;
    
    }

    ~sfapspeedacc() {
	    for(uint32_t i = 0; i < HASH_NUM; i ++) {
                 delete[] cc[i];
         }
         delete [] cc;
    }


      float setpara(int i, int j) {
           if(cc[i][j].count > psum/(4*cell_num)) {
                   return 10;
           } else {
                  return 1;
           }

   }


    void Insert(DATA_TYPE item) {  
	    COUNT_TYPE min = 0x7fffffff;
            uint32_t minPos;
            uint32_t minHash;
            sum ++;
            if(sum % window_size == 0) {
                    NewWindow();
             }
	    uint32_t pos = hash2(item, 2) % LENGTH;
	    for(int i = 0; i < HASH_NUM; i++) {
                    if(cc[i][pos].item == item) {
			    if(bitmap->Get(i*LENGTH +pos) == 0) {
				    cc[i][pos].count += 1;
				    psum++;
				    bitmap->Set(i*LENGTH +pos);
			    }
			    return;
		    }
	            if(cc[i][pos].count  < min) {
			    min = cc[i][pos].count;
			    minHash = i;
                            minPos = pos;
		    }	    
	    }
            if(bitmap->Get(minHash * LENGTH + minPos))
		    return;
	    if(cc[minHash][minPos].count == 0) {
                cc[minHash][minPos].item = item;
                cc[minHash][minPos].count = 1;
		psum++;
		bitmap->Set(minHash*LENGTH +minPos);
	    } else {
	      int ccss = ((int) ((cc[minHash][minPos].count +1)*setpara(minHash,minPos)*(1.0*sum/( psum))));  
	      if(ccss < 1) ccss = 1;

	     if(rng() % ccss == 0) {
	      psum += 1;
	      cc[minHash][minPos].item = item;
	      cc[minHash][minPos].count = 1;
	      }

	    } 

    }
    void NewWindow() {   
	    bitmap->Clear();
    }
    repMap Rep(COUNT_TYPE HIT) {
          repMap ret;

          for(uint32_t i = 0; i < HASH_NUM; i++)
              for(uint32_t j = 0; j < LENGTH; j++)
                 if(cc[i][j].count >= HIT) {
                    ret[cc[i][j].item] = cc[i][j].count;
                 }

    
	  return ret;
    }

private:
	uint64_t sum;
	uint64_t psum;
        uint32_t LENGTH;
        uint32_t HASH_NUM;
	uint32_t window_size;
	string NAME;
        cell** cc;
	uint32_t MEMORY;
	uint32_t cell_num;
	float alpha;
        BitMap* bitmap;
};
#endif

