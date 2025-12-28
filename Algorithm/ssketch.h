#ifndef SSKECH_H
#define SSKECH_H
#include "Abstract.h"

class ssketch{
public: 
     struct cell{
      uint32_t count;
      uint32_t hotcount;
      DATA_TYPE item;
      uint8_t status;
   };
    ssketch(uint32_t _MEMORY, int _HASH_NUM, int _windowsize, string _name = "SPLIT_O") {
        NAME = _name;
        MEMORY = _MEMORY;
        HASH_NUM = _HASH_NUM;
        cell_num = _MEMORY / sizeof(cell);
 	LENGTH = cell_num / HASH_NUM;
	window_size =  _windowsize;
        sum = -1;
        cc = new cell*[HASH_NUM];
        for(int i = 0; i < HASH_NUM; i++) {
                 cc[i] =  new cell[LENGTH];
                 for(int j = 0; j < LENGTH; j++){
                      cc[i][j].count = 0;
                      cc[i][j].hotcount = 0;
		      cc[i][j].status = 0;
		 }
                 //memset(cc[i], 0, sizeof(Cell) * LENGTH);
          }
      }

    ~ssketch() {
	    for(uint32_t i = 0; i < HASH_NUM; i ++) {
                 delete[] cc[i];
         }
         delete [] cc;
    }

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
				    cc[i][pos].hotcount += 1;
				    cc[i][pos].status = 1;
			    }
			    return;
		    }
		    //std::cout << cc[i][pos].count << "\t"  << cc[i][pos].hotcount << std::endl;
	            if(cc[i][pos].count + cc[i][pos].hotcount < min) {
			    min = cc[i][pos].count + cc[i][pos].hotcount;
			    minHash = i;
                            minPos = pos;
			 //   std::cout << "happens!!";
		    }	    
	    } 
            if(cc[minHash][minPos].status == 1)
		    return;
             
	    if(cc[minHash][minPos].count == 0) {
                cc[minHash][minPos].item = item;
                cc[minHash][minPos].count = 1;
                cc[minHash][minPos].hotcount = 1;
                cc[minHash][minPos].status = 1;
	    } 
	    else if(rng() %((cc[minHash][minPos].count * cc[minHash][minPos].hotcount) +1) == 0) {
            
	        //cc[minHash][minPos].item = item;
                cc[minHash][minPos].count = (cc[minHash][minPos].count  >  1 ? cc[minHash][minPos].count -1  :0);
                if(cc[minHash][minPos].count == 0) {
			cc[minHash][minPos].item = item;
			cc[minHash][minPos].count = 1;
                        cc[minHash][minPos].hotcount = (cc[i][j].hotcount > 1 ? cc[i][j].hotcount-1:0);
                }
                     cc[minHash][minPos].status = 1;
	    }

    }
    void NewWindow() {
      for(int i = 0; i < HASH_NUM; i++) { 
	   for(int j = 0; j < LENGTH; j++) {
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

