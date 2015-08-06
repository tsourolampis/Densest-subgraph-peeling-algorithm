#include <algorithm>
#include <cstdlib>
#include <cstring>
using namespace std;

/////////////Hash chart
namespace HASH{
  const int HASHSIZE = 30000001;
  const int MAXELEMENTS = 6000000;

  int hash[HASHSIZE + 10];

  int list[MAXELEMENTS][2];/////maximum number of elements kept
  int total;

  void init() {
    memset(hash, -1, sizeof(hash));
    total = 0;
  }

  inline int hashFunction(int u, int v) {
    int p = u + (v>>4) + ((u<<16)^v);
    if (p < 0) p = -p;
    return p % HASHSIZE;
  }

  inline int insert(int u, int v) {
    int p = hashFunction(u, v);
    int del = 1; //quadratic probing
    while(hash[p] >= 0) {
      if(u == list[hash[p]][0] && v == list[hash[p]][1])
        return 0;
      p = p + del;
      del ++;
      if (p >= HASHSIZE) p -= HASHSIZE;
      if (del >= HASHSIZE) del -= HASHSIZE;
    }
    if(total == MAXELEMENTS) {
      puts("uhoh, hash chart full...");
      exit(0);
    }
    list[total][0] = u;
    list[total][1] = v;
    hash[p] = total;
    total++;
    return 1;
  }

  inline int find(int u, int v) {
    if(u>v) swap(u, v);
    int p = hashFunction(u, v);
    int del = 1; //quadratic probing
    while(hash[p] >= 0) {
      if(u == list[hash[p]][0] &&
          v == list[hash[p]][1])
        return 1;
      p = p + del;
      del ++;
      if (p >= HASHSIZE) p -= HASHSIZE;
      if (del >= HASHSIZE) del -= HASHSIZE;
    }
    return 0;
  }
}


