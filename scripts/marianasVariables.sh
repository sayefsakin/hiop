if [ ! -v BUILDDIR ]; then
  echo BUILDDIR is not set! Your paths may be misconfigured.
fi
export MY_CLUSTER="marianas"
export PROJ_DIR=/qfs/projects/exasgd
export APPS_DIR=/share/apps
export SPACK_ARCH=linux-centos7-broadwell
#  NOTE: The following is required when running from Gitlab CI via slurm job
source /etc/profile.d/modules.sh
source $PROJ_DIR/src/spack/share/spack/setup-env.sh
module use -a /share/apps/modules/Modules/versions
module use -a $MODULESHOME/modulefiles/environment
module use -a $MODULESHOME/modulefiles/development/mpi
module use -a $MODULESHOME/modulefiles/development/mlib
module use -a $MODULESHOME/modulefiles/development/compilers
module use -a $MODULESHOME/modulefiles/development/tools
module use -a $MODULESHOME/modulefiles/apps
module use -a $MODULESHOME/modulefiles/libs
module use -a $PROJ_DIR/src/spack/share/spack/modules/$SPACK_ARCH/
export MY_GCC_VERSION=7.3.0
export MY_CUDA_VERSION=10.2.89
export MY_OPENMPI_VERSION=3.1.3
export MY_CMAKE_VERSION=3.15.3
export MY_MAGMA_VERSION=2.5.4-gcc-7.3.0-bwwsayw
export MY_METIS_VERSION=5.1.0
export MY_NVCC_ARCH="sm_60"

export NVBLAS_CONFIG_FILE=$PROJ_DIR/$MY_CLUSTER/nvblas.conf
module load gcc/7.3.0
module load cuda/10.2.89
module load openmpi/3.1.3
module load cmake/3.19.6

spack env activate exago-v0-99-2-hiop-v0-3-99-2-marianas
