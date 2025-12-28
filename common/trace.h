#ifndef _TRACE_H_
#define _TRACE_H_
#include <vector>
#include <cstdio>
#include <cinttypes>
#include <utility>
#include <fstream>
using namespace std;
vector<pair<uint32_t, float>> loadCAIDA(const char *filename = "./CAIDA.dat") {
  printf("Open %s \n", filename);
  FILE *pf = fopen(filename, "rb");
  if (!pf) {
    printf("%s not found!\n", filename);
    exit(-1);
  }

  vector<pair<uint32_t, float>> vec;
  double ftime = -1;
  char trace[30];
  while (fread(trace, 1, 21, pf)) {
    uint32_t tkey = *(uint32_t *)(trace);
    double ttime = *(double *)(trace + 13);
    if (ftime < 0)
      ftime = ttime;
    vec.push_back(pair<uint32_t, float>(tkey, ttime - ftime));
  }
  fclose(pf);
  return vec;
}

vector<pair<uint32_t, uint64_t>>load_all_critep(const char *filename) {
      uint64_t a,b;
      vector<pair<uint32_t, uint64_t>> vec;
      ifstream fp(filename);
      while(!fp.eof()) {
	      fp >> a >>b;
              vec.push_back(pair<uint32_t,uint64_t>(b,a));
      }
      fp.close();
      return vec;
}

vector<pair<uint32_t, float>>
loadCRITEO(const char *filename = "./CRITEO.log") {
  fprintf(stderr, "Open %s \n", filename);
  FILE *pf = fopen(filename, "rb");
  if (!pf) {
    printf("%s not found!\n", filename);
    exit(-1);
  }

  vector<pair<uint32_t, float>> vec;
  char trace[40];
  int ttime = 0;
  while (fscanf(pf, "%s", trace) != EOF) {
    uint64_t tkey;
    sscanf(trace + 10, "%" SCNu64, &tkey);
    vec.push_back(pair<uint32_t, float>(tkey, ++ttime));
  }
  fprintf(stderr, "Close %s \n", filename);
  fclose(pf);
  return vec;
}
#endif // _TRACE_H_
