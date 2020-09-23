#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "search.h"

typedef int (*search_t)(void* , int, int);// A pointer to the search function
typedef void* (*setup_t)(int);            // A pointer to the function which generate data
typedef void (*cleanup_t)(void*);         // A pointer to cleaning function
typedef struct searchalg_t_ {
  char description[50];   // Description of the algorithm
  char header[10];        // A short name of the algorithm which is used in result printing
  char symbol;            // A symbol which denoted the algorithm in a command line agument
  int enable;             // A boolean switch: 0 not use this algorithm
  search_t search_fun;
  setup_t setup_fun;
  cleanup_t cleanup_fun;
} searchalg_t;

int main(int argc, char *argv[]){
  //Table of search algorithm
  searchalg_t algs[] = {
    {"Linear Array Search", "array", 'a', 1,
      (search_t) linear_array_search, (setup_t) make_evens_array, (cleanup_t) free},
    {"Linked List Search", "list", 'l', 1,
      (search_t) linkedlist_search, (setup_t) make_evens_list, (cleanup_t) list_free},
    {"Binary Array Search", "binary", 'b', 1,
      (search_t) binary_array_search, (setup_t) make_evens_array, (cleanup_t) free},
    {"Binary Tree Search", "tree", 't', 1,
      (search_t) binary_tree_search, (setup_t) make_evens_tree, (cleanup_t) bst_free},
  };


  int n_algs = sizeof(algs) / sizeof(algs[0]);    //number of elements in algs[]

  if (argc < 4 || argc > 5) {
    printf("usage: %s <minpow> <maxpow> <repeats> [which]\n", argv[0] );
    printf(" which is a combination of:\n");
    for (int i=0; i<n_algs; i++)
      printf("  %c : %s\n", algs[i].symbol, algs[i].description);
    return 1;
  }

  int min_size_pow  = atoi(argv[1]);
  int max_size_pow = atoi(argv[2]);
  int repeats = atoi(argv[3]);

  if (argc > 4) {
    char *ch = argv[4];
    // disable all algorithms
    for (int i=0; i<n_algs; i++) {
      algs[i].enable = 0;
    }
    // switch on only desired
    while (*ch) {
      for (int i=0; i<n_algs; i++)
        if (*ch == algs[i].symbol)
          algs[i].enable = 1;
      ch++;
    }
  }

  // print the header
  printf("%10s%10s", "LENGTH", "SEARCHES");
  for (int i=0; i<n_algs; i++)
    if (algs[i].enable)
      printf("%12s", algs[i].header);
  printf("\n");

  clock_t begin, end;
  double cpu_time;


  for(int pow=min_size_pow; pow<=max_size_pow; pow++){
    long size = 1 << pow;
    long searches = 2*size;     // half succes and half fault searches
    printf("%10ld%10ld", size, searches*repeats);

    for (int i=0; i<n_algs; i++) {
      if (algs[i].enable) {      //algorithm enable
        void *data = algs[i].setup_fun(size);   // create data
        begin = clock();
        for (int iter=0; iter<repeats; iter++) {   //every repeat
          for (int query=0; query<searches; query++) {
            algs[i].search_fun(data, size, query);
          }
        }
        end = clock();
        cpu_time = ((double) (end - begin)) / CLOCKS_PER_SEC;
        printf("%12.3e", cpu_time);
        algs[i].cleanup_fun(data);   // cleaning
      }
    }

    printf("\n"); // begin new line
  }

  return 0;
}
