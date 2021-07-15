#include <string>
#include <iostream>
#include <cassert>
#include <cstring>
#include <limits>
#include <cmath>

#include "cuda.h"

#include <umpire/Allocator.hpp>
#include <umpire/ResourceManager.hpp>
#include <RAJA/RAJA.hpp>


#define MY_RAJA_GPU_BLOCK_SIZE 128

using my_raja_exec   = RAJA::cuda_exec<MY_RAJA_GPU_BLOCK_SIZE>;

#define RAJA_LAMBDA [=] __device__
 
int main(int argc, char** argv)
{
  // setting this to nonzeros fail the program
  int docopy = 0;
  if(argc==2)
    docopy = atoi(argv[1]);

  const int m1 = 100;
  const int m2 = 50;

  std::string mem_space_ = "UM";

  auto& resmgr = umpire::ResourceManager::getInstance();
  umpire::Allocator devalloc = resmgr.getAllocator(mem_space_);
  umpire::Allocator hosalloc = resmgr.getAllocator("HOST");
  int *ar_dev;
  int *ar_hos;
  
  /* copying from host  reproduce the problem*/
  int nnz = 200;
  if(docopy!=0) {
    ar_hos = static_cast<int*>(hosalloc.allocate(nnz * sizeof(int)));
    ar_dev = static_cast<int*>(devalloc.allocate(nnz * sizeof(int)));
    for(int i=0; i<nnz; i++) {
      ar_hos[i] = 1;
    }
    resmgr.copy(ar_dev,ar_hos);
  }

  int* m1_row_start = static_cast<int*>(devalloc.allocate((m1+1)*sizeof(int)));
  RAJA::forall<my_raja_exec>(
    RAJA::RangeSegment(0, m1+1),
    RAJA_LAMBDA(RAJA::Index_type i)
    {
      if(i>0 && i<m2+1) {
        m1_row_start[i] = 1;
      } else { 
        m1_row_start[i] = 0;
      }
    }
  );
     
  RAJA::inclusive_scan_inplace<my_raja_exec>(m1_row_start,m1_row_start+m1+1,RAJA::operators::plus<int>());

  std::cout << "\nDone!\n\n";

  if(docopy) {
    hosalloc.deallocate(ar_hos);
    devalloc.deallocate(ar_dev);
  }
  devalloc.deallocate(m1_row_start);
  return 0;
}
