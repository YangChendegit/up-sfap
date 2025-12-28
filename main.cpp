#include <iostream>
#include <stdlib.h>
#include "benchmark.h"
#include "trace.h"


#include <sys/stat.h>
#include <fstream>

#include <chrono>
#include <vector>

#include "Baseline.h"
#include "Ours.h"

#include <sys/stat.h>
#include <fstream>

#include <chrono>
#include <vector>
#include "MMap.h"
#include <chrono>
#include <vector>
#include <vector>
#include "psketch.h"
#include "pasketch.h"
//#include "dec_sketch.h"
#include "sfapacc.h"
//#include "dec_sketch2.h"
#include "sfap.h"
#include "sfapspeed.h"
#include "sfapspeedacc.h"
//#include "dec_sketch3.h"
//#include "dec_sketch4.h"
//#include "dec_sketch5.h"
#include "onoffsketch.h"
#include "pontus.h"
#include "Abstract2.h"
#include "Filter.h"
#include "HotStorage.h"
#include "Hyper.h"

std::unordered_map<DATA_TYPE, COUNT_TYPE>  checkError(std::vector<ItemPair>Items, int windowsize) {
     std::unordered_map<DATA_TYPE, TIME_TYPE>last_time;
     std::unordered_map<DATA_TYPE, COUNT_TYPE>count_num;
     for(uint32_t ii = 0; ii <Items.size(); ii++) {
         if(last_time.find(Items[ii].item) != last_time.end()) {
		 if(last_time[Items[ii].item] /windowsize != ii/ windowsize ) {
	              count_num[Items[ii].item] += 1;
		     // alll ++;
		  }
		 last_time[Items[ii].item] = ii;
	} else {
	    last_time[Items[ii].item] = Items[ii].time;
	    count_num[Items[ii].item] = 1;
	}
     }
     std::cout <<"the different number:" <<count_num.size() << std::endl;
     return count_num;
}


void Error_report(std::unordered_map<DATA_TYPE, COUNT_TYPE>count_num, uint32_t HIT,   repMap result) {
     double real = 0, estimate = 0, both = 0;
     double aae = 0, are = 0, cr = 0, pr = 0, f1 = 0, aee =0;

     double hot = 0;


     //for(auto x:result) {
     //    estimate++;
     //}
     estimate = result.size();
     for(auto it = count_num.begin(); it != count_num.end(); ++it) {
	     if(it->second > HIT) {
		   real +=1;
		   if(result.find(it->first) != result.end()) {
			   both += 1;
			   COUNT_TYPE value = result[it->first];
			   aae += abs(it->second - value);
			   are += abs(it->second - value) / (double)(it->second);
			   aee +=  value - it->second;
		   }
	     }
     } 
     if(both <= 0) {
	  std::cout << "Error" << std::endl;
     } else {
	     aae /= both;
	     are /= both;
     }
     cr = both / real;
     if(estimate <=0) {
	     std::cout << "Not Find" << std::endl;
     } else {
            pr = both / estimate;
     }

     std::cout << "AAE:" << aae << std::endl
	     << "AEE" << aee << std::endl
	     << "ARE:" <<are << std::endl
	     << "Recall-Rate:" << cr << std::endl
	     << "Precision-Rate:" << pr << std::endl
	     << "F1-Score:"  <<  2*cr*pr/(cr + pr)<< std::endl<<std::endl << std::endl;
            

}


  void FPICheckError(Abstract2<DATA_TYPE, COUNT_TYPE>* sketch, COUNT_TYPE HIT,  std::unordered_map<DATA_TYPE, COUNT_TYPE>mp){
        double f1 = 0, are = 0;
        double tp = 0, fn = 0, fp = 0, tn = 0;
        double precision = 0, recall = 0, fnr = 0, fpr = 0;

        for(auto it = mp.begin();it != mp.end();++it){
            COUNT_TYPE predict = sketch->Query(it->first);
            COUNT_TYPE real = it->second;

            if(real > HIT){
                if(predict > HIT){
                    tp++;
                    are += (double) abs(real - predict) / real * 1.0;
                }else{
                    fn++;
                }
            }else{
                if(predict > HIT){
                    fp++;
                }else{
                    tn++;
                }
            }
        }

        if(tp <= 0){
            std::cout << "Not Find Real Persistent" << std::endl;
        }else{
            are /= tp;
        }

        precision = tp / (tp + fp + 0.0);
        recall = tp / (tp + fn + 0.0);
        f1 = 2 * precision * recall / (precision + recall);
        fnr = fn / (fn + tp);
        fpr = fp / (fp + tn);

        std::cout << sketch->getName() << ":" << std::endl;
        
	
	 std::cout << "AAE:" << 0 << std::endl
             << "AEE" << 0 << std::endl

	       
		 << "ARE:" <<are << std::endl
             << "Recall-Rate:" << recall << std::endl
             << "Precision-Rate:" << precision << std::endl
             << "F1-Score:"  << f1 << std::endl<<std::endl << std::endl;


    }



int main(int argc, char *argv[]) {
  
    std::vector<std::pair<uint32_t, uint64_t>> tt0 =load_all_critep("./datasets/202409011400_1_1.txt");
   
   int   mem  = atoi(argv[1]);
   int   window_size = atoi(argv[2]);
   int hh = atoi(argv[3]); 
   int hit = atoi(argv[4]);
   
   std::cout << "mem:\t" << mem << "window_size:\t"  <<  window_size << "hh:\t" << hh << "hit:\t" << hit << "\n";
   
   std::vector<ItemPair> ttt1;
    
   for(int i = 0; i < tt0.size(); i++) {
	   ItemPair ii;
	   ii.item = tt0[i].first;
	   ii.time =tt0[i].second;
	   ttt1.push_back(ii);
   }   
   std::unordered_map<DATA_TYPE, COUNT_TYPE>count_num = checkError(ttt1,  window_size);
   onoffsketch *onoff = new onoffsketch(mem, 16, window_size);
   pasketch *pas = new pasketch(mem, hh, window_size);
   psketch *ps = new psketch(mem, hh, window_size);
   ssketch *sks = new ssketch(mem, hh, window_size);
   pontus *pon = new pontus(mem, hh, window_size);
   sfapacc *dds = new sfapacc(mem,hh, window_size);
   sfap *dds2 = new sfap(mem,hh, window_size);
   sfapspeedacc *adj = new sfapspeedacc(mem, hh, window_size);
   sfapspeed *simds = new  sfapspeed(mem,hh, window_size);
    


  Abstract2<DATA_TYPE, COUNT_TYPE>*  HH_PP = new Hyper<DATA_TYPE, COUNT_TYPE, 3, 3, 1>(mem/1024  , 0.6, hit > 300 ? 255 : hit - 25 - 20, 25, 85, window_size); 


   TP initial, finish;
  

     auto start = std::chrono::high_resolution_clock::now();  
     initial = now();
     for(int i = 0; i < ttt1.size(); i++) {
          onoff->Insert(ttt1[i].item);
     }
    finish = now();
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Function took " << duration.count() << " milliseconds to execute." << std::endl;
    std::cout << "Insertion Thp of onoffSketch: " << ttt1.size() / durationms(finish, initial) << std::endl;
   

   initial = now();
   for(int i = 0; i < ttt1.size(); i++) {
          pas->Insert(ttt1[i].item);
     }
    finish = now();
    std::cout << "Insertion Thp of PAAASketch: " << ttt1.size() / durationms(finish, initial) << std::endl;



   initial = now();
   for(int i = 0; i < ttt1.size(); i++) {
          ps->Insert(ttt1[i].item);	  
    }
   finish = now();
   std::cout << "Insertion Thp of PSketch: " << ttt1.size() / durationms(finish, initial) << std::endl;

   initial = now();
   for(int i = 0; i < ttt1.size(); i++) {
          sks->Insert(ttt1[i].item);
    }
   finish = now();
   std::cout << "Insertion Thp of stablesketch: " << ttt1.size() / durationms(finish, initial) << std::endl;

   initial = now();
   for(int i = 0; i < ttt1.size(); i++) {
          pon->Insert(ttt1[i].item);
    }
    finish = now();
    std::cout << "Insertion Thp of pontus: " << ttt1.size() / durationms(finish, initial) << std::endl;



  initial = now();
   for(int i =0; i< ttt1.size(); i++) {
           dds->Insert(ttt1[i].item);
   }
   finish = now();
   std::cout << "Insertion Thp of sfap-acc: " << ttt1.size() / durationms(finish, initial) << std::endl;


    initial = now();
    for(int i =0; i< ttt1.size(); i++) {
           dds2->Insert(ttt1[i].item);
    }
    finish = now();
   std::cout << "Insertion Thp of sfap: " << ttt1.size() / durationms(finish, initial) << std::endl;

   initial = now();
   for(int i = 0; i < ttt1.size(); i++) {
          adj->Insert(ttt1[i].item);
    }
   finish = now();
   std::cout << "Insertion Thp of sfap-acc-speed: " << ttt1.size() / durationms(finish, initial) << std::endl;



   initial = now();
   for(int i =0; i< ttt1.size(); i++) {
           simds->Insert(ttt1[i].item);
   }
   finish = now();
  std::cout << "Insertion Thp of sfapspeed: " << ttt1.size() / durationms(finish, initial) << std::endl;
  
  initial = now();
   for(int i =0; i< ttt1.size(); i++) {
           HH_PP->Insert(ttt1[i].item, i);
   }
   finish = now();
   std::cout << "Insertion Thp of HH_PP: " << ttt1.size() / durationms(finish, initial) << std::endl;



   repMap resultonoff = onoff->Rep(hit);
   repMap resultpaas = pas->Rep(hit);
   repMap resultps = ps->Rep(hit);
   repMap resultsks = sks->Rep(hit);
   repMap resultpon = pon->Rep(hit);
   repMap resultdds = dds->Rep(hit);
   repMap resultdds2 = dds2->Rep(hit);
   repMap resultadj = adj->Rep(hit);
   repMap resultsimd = simds->Rep(hit);

	Error_report(count_num, hit,resultonoff);
   Error_report(count_num, hit,resultpaas);
   Error_report(count_num, hit, resultps);
   Error_report(count_num, hit, resultsks);
   Error_report(count_num, hit, resultpon);
   Error_report(count_num, hit, resultdds);
   Error_report(count_num, hit, resultdds2);
   Error_report(count_num, hit, resultadj);
   Error_report(count_num, hit, resultsimd);
   FPICheckError(HH_PP, hit,  count_num);



  delete onoff;
  delete pas;

  delete ps;
  delete sks;
  delete pon;
  delete dds;
  delete dds2;
  delete adj;
  delete simds;
  delete HH_PP;

    
    return 0;
}
