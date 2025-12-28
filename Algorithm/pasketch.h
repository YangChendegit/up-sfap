#ifndef PASKECH_H
#define PASKECH_H
#include "Abstract.h"

class pasketch{
public: 
     struct cell{
      int32_t count;
      int32_t coldcount;//和P-sketch意义相反
      DATA_TYPE item;
      uint8_t status;
   };
    pasketch(uint32_t _MEMORY, int _HASH_NUM, int _windowsize, string _name = "SPLIT_O") {
        NAME = _name;
        MEMORY = _MEMORY;
        HASH_NUM = _HASH_NUM;
        cell_num = _MEMORY / (sizeof(int)*2 + sizeof(DATA_TYPE) + sizeof(uint8_t)); 
 	LENGTH = cell_num / HASH_NUM;
	window_size =  _windowsize;
        sum = -1;
        cc = new cell*[HASH_NUM];
        for(int i = 0; i < HASH_NUM; i++) {
                 cc[i] =  new cell[LENGTH];
                 for(int j = 0; j < LENGTH; j++){
                      cc[i][j].count = 0;
                      cc[i][j].coldcount = 0;
		      cc[i][j].status = 0;
		 }
                 //memset(cc[i], 0, sizeof(Cell) * LENGTH);
          }
      }

    ~pasketch() {
	    for(uint32_t i = 0; i < HASH_NUM; i ++) {
                 delete[] cc[i];
         }
         delete [] cc;
    }
   int max(int a, int b) { return a>b? a:b; }

    void Insert(DATA_TYPE item) {
	    COUNT_TYPE min = 0x7fffffff;
	    //COUNT_TYPE min = 9999999;
            uint32_t minPos = hash2(item, 0) % LENGTH;
            uint32_t minHash = 0;


  
            sum ++;
            if(sum % window_size == 0) {
                    NewWindow();
             }
	    
            for(int i = 0; i < HASH_NUM; i++) {
		    uint32_t pos = hash2(item, i) % LENGTH;
                    if(cc[i][pos].item == item) {
			    if(cc[i][pos].status == 0) {
				    cc[i][pos].count += 1;
				    cc[i][pos].coldcount = ( cc[i][pos].coldcount >  1 ? cc[i][pos].coldcount -1 : 0) ;
				    cc[i][pos].status = 1;
			    }
			    return;
		    }

	            if(cc[i][pos].count  < min) {
			    min = cc[i][pos].count;
			    minHash = i;
                            minPos = pos;

		    }	    
	    } 
            if(cc[minHash][minPos].status == 1)
		    return;
             
	    if(cc[minHash][minPos].count == 0) {
                cc[minHash][minPos].item = item;
                cc[minHash][minPos].count = 1;
                cc[minHash][minPos].coldcount =  ( cc[minHash][minPos].coldcount >  1 ? cc[minHash][minPos].coldcount -1 : 0);
                cc[minHash][minPos].status = 1;
	    } 
	    else if(rng() % ((max(1, 50- cc[minHash][minPos].coldcount)* cc[minHash][minPos].count)  +1) == 0) {
            
	        cc[minHash][minPos].item = item;
                cc[minHash][minPos].count = 1;
                cc[minHash][minPos].status = 1;
		cc[minHash][minPos].coldcount = 0;

	    }

    }


     COUNT_TYPE query(DATA_TYPE item) {
       COUNT_TYPE ret = INT_MAX;
          for(uint32_t i=0; i<HASH_NUM;i++) {
                  int32_t pos = hash2(item, i) % LENGTH;
                  if(cc[i][pos].item == item) return cc[i][pos].count;

                  ret = MIN(ret, cc[i][pos].count);
          }

        return ret;
   }


    void NewWindow() {
      for(int i = 0; i < HASH_NUM; i++) { 
	   for(int j = 0; j < LENGTH; j++) {
              if(cc[i][j].status == 0) {
                     cc[i][j].coldcount ++;
	      } else {
		      cc[i][j].coldcount = (cc[i][j].coldcount >1 ? cc[i][j].coldcount -1 :0);
	      }

		   cc[i][j].status = 0;
           }
       }
    }
    repMap Rep(COUNT_TYPE HIT) {
          repMap ret;

          for(uint32_t i = 0; i < HASH_NUM; i++)
              for(uint32_t j = 0; j < LENGTH; j++)
                 if(cc[i][j].count >= HIT) {
                    ret[cc[i][j].item] = cc[i][j].count-1;
                 }

    
	  return ret;
    }

private:
	uint64_t sum;
        uint32_t LENGTH;
        uint32_t HASH_NUM;
	uint32_t window_size;
	string NAME;
        cell ** cc;
	uint32_t MEMORY;
	uint32_t cell_num;

};
#endif

