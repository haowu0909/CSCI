#include "matvec.h"
#include <stdlib.h>

int optimized_matrix_trans_mult_vec(matrix_t mat, vector_t vec, vector_t res){
  if(mat.rows != vec.len){
    printf("mat.rows (%ld) != vec.len (%ld)\n",mat.rows,vec.len);
    return 1;
  }
  if(mat.cols != res.len){
    printf("mat.cols (%ld) != res.len (%ld)\n",mat.cols,res.len);
    return 2;
  }
  for(int j=0; j<mat.rows; j++)
    VSET(res,j,0); // initialize res[j] to zero

  int n = mat.cols - 4;       // use for cleanup loop
  for(int i=0; i<mat.rows; i++){
    int veci = VGET(vec,i); //get vector elements,and set it as local variable
    int j;
    for(j=0; j<n; j+=4){
      //get result vector elements
      //use pipeline
      int resj = VGET(res,j);
      int resj2 = VGET(res,j+1);
      int resj3 = VGET(res,j+2);
      int resj4 = VGET(res,j+3);
      //get matrix elements
      int elij = MGET(mat,i,j);
      int elij2 = MGET(mat,i,j+1);
      int elij3 = MGET(mat,i,j+2);
      int elij4 = MGET(mat,i,j+3);


      //update result
      resj += elij * veci;
      resj2 += elij2 * veci;
      resj3 += elij3 * veci;
      resj4 += elij4 * veci;
      //store to vector
      VSET(res,j, resj);
      VSET(res,j+1, resj2);
      VSET(res,j+2, resj3);
      VSET(res,j+3, resj4);
    }

      //add on remain elements
    for(; j < mat.cols; j++){

      int resj = VGET(res,j);
      int elij = MGET(mat,i,j);
      resj += elij * veci;
      VSET(res,j, resj);

        }
}



  return 0;
}
