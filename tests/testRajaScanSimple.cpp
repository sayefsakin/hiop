#include <string>
#include <iostream>
#include <cassert>
#include <cstring>
#include <limits>
#include <cmath>

#include "mpi.h"

#include "cuda.h"

#include <umpire/Allocator.hpp>
#include <umpire/ResourceManager.hpp>
#include <RAJA/RAJA.hpp>


#define MY_RAJA_GPU_BLOCK_SIZE 128

using my_raja_exec   = RAJA::cuda_exec<MY_RAJA_GPU_BLOCK_SIZE>;
using my_raja_reduce = RAJA::cuda_reduce;
using my_raja_atomic = RAJA::cuda_atomic;

#define RAJA_LAMBDA [=] __device__
 
int main(int argc, char** argv)
{
  const int m1 = 100;
  const int m2 = 50;

  std::string mem_space_ = "UM";

  auto& resmgr = umpire::ResourceManager::getInstance();
  umpire::Allocator devalloc = resmgr.getAllocator(mem_space_);
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

  return 0;
}
