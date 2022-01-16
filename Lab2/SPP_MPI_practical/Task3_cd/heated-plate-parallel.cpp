#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <string>
#include <chrono>
#include <mpi.h>

using namespace std;

// Simple helper class to measure time.
struct Timer {
  using clock = std::chrono::steady_clock;

  clock::time_point startTime;

  Timer() : startTime(clock::now()) {}

  double getSeconds() {
    return std::chrono::duration_cast<std::chrono::duration<float>>(clock::now() - startTime).count();
  }
};

int main(int argc, char *argv[])

//
//  Purpose:
//
//    MAIN is the main program for HEATED_PLATE_MPI.
//
//  Discussion:
//
//    This code solves the steady state heat equation on a rectangular region.
//
//    The sequential version of this program needs approximately
//    18/epsilon iterations to complete.
//
//
//    The physical region, and the boundary conditions, are suggested
//    by this diagram;
//
//                   W = 0
//             +------------------+
//             |                  |
//    W = 100  |                  | W = 100
//             |                  |
//             +------------------+
//                   W = 100
//
//    The region is covered with a grid of M by N nodes, and an M by N
//    array W is used to record the temperature.  The correspondence between
//    array indices and locations in the region is suggested by giving the
//    indices of the four corners:
//
//                  I = 0
//          [0][0]-------------[0][N-1]
//             |                  |
//      J = 0  |                  |  J = N-1
//             |                  |
//        [M-1][0]-----------[M-1][N-1]
//                  I = M-1
//
//    The steady state solution to the discrete heat equation satisfies the
//    following condition at an interior grid point:
//
//      W[Central] = (1/4) * ( W[North] + W[South] + W[East] + W[West] )
//
//    where "Central" is the index of the grid point, "North" is the index
//    of its immediate neighbor to the "north", and so on.
//
//    Given an approximate solution of the steady state heat equation, a
//    "better" solution is given by replacing each interior point by the
//    average of its 4 neighbors - in other words, by using the condition
//    as an ASSIGNMENT statement:
//
//      W[Central]  <=  (1/4) * ( W[North] + W[South] + W[East] + W[West] )
//
//    If this process is repeated often enough, the difference between
//    successive estimates of the solution will go to zero.
//
//    This program carries out such an iteration, using a tolerance specified by
//    the user.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license.
//
//  Modified:
//
//    18 October 2011
//
//  Author:
//
//    Original C version by Michael Quinn.
//    C++ version by John Burkardt.
//
//  Reference:
//
//    Michael Quinn,
//    Parallel Programming in C with MPI and OpenMP,
//    McGraw-Hill, 2004,
//    ISBN13: 978-0071232654,
//    LC: QA76.73.C15.Q55.
//
//  Local parameters:
//
//    Local, double DIFF, the norm of the change in the solution from one
//    iteration to the next.
//
//    Local, double MEAN, the average of the boundary values, used to initialize
//    the values of the solution in the interior.
//
//    Local, double U[M][N], the solution at the previous iteration.
//
//    Local, double W[M][N], the solution computed at the latest iteration.
//
{
#define M 500
#define N 500

  double diff;
  double epsilon = 0.001;
  int i;
  int iterations;
  int iterations_print;
  int j;
  double mean;
  // TODO: Each process may only allocate a part of the matrix
    int myid, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &myid);
    MPI_Comm_size(MPI_COMM_WORLD,&size);
    if(size>M){
        cout<<"the number of rows is less than processes"<<endl;
    }
    int effectivRow = ceil((double)(M-2)/(double)size);
    int rowProProcess = effectivRow+2;
    int coverRow = (size * rowProProcess - M)/(size-1);
    int end = M-1-effectivRow*(size-1);

  // you may want to allocate the matrix wit malloc depening on the number of
  // lines each process may get
  double* u = (double*) malloc(rowProProcess*N*sizeof (double));
  double* w=(double*) malloc(rowProProcess*N*sizeof (double));
  double wtime;

  // TODO: Make sure that only one MPI process prints the output
  if(myid == 0) {
      cout << "\n";
      cout << "HEATED_PLATE\n";
      cout << "  C++ version\n";
      cout
              << "  A program to solve for the steady state temperature distribution\n";
      cout << "  over a rectangular plate.\n";
      cout << "\n";
      cout << "  Spatial grid of " << M << " by " << N << " points.\n";
      cout << "  The iteration will be repeated until the change is <= " << epsilon
           << "\n";
      cout<<"effectiv row:"<<effectivRow<<endl;
      cout<<"rowProProcess:" << rowProProcess<<endl;
      cout<<"coverRow:"<<coverRow<<endl;
      cout<<"end:"<<end<<endl;
  }
  MPI_Barrier(MPI_COMM_WORLD);

  // TODO: Get number of MPI processes
  long num_procs = size;
    if(myid == 0){
        cout << "  Number of processes available = " << num_procs << "\n";
    }


  //
  //  Set the boundary values, which don't change.
  // TODO: Be aware that each process hold a different part of the matrix and may only
  // initialize his own part
  //通信分组
  //https://www.codenong.com/41404826/
  mean = 0.0;
    if(myid == size-1){

        for(int j = 0; j<end;j++){
            w[N*j]=100.0;
            w[N*j+N-1]=100.0;
        }
        for (j = N*end; j < N*(end+1); j++) {
            w[j] = 100.0;
        }
        mean = mean+(end-1)*2*100.0;
        mean = mean+N*100.0;
    }
    else if(myid == 0){
        for (j = 0; j < N; j++) {
            w[j] = 0.0;
        }
        for(int j = 1; j<rowProProcess;j++){
            w[N*j]=100.0;
            w[N*j+N-1]=100.0;
        }
        mean = mean+effectivRow*2*100.0;

    } else if(myid>0 && myid < size-1){
        //TODO 左右两边为100
        for(int j = 0; j<rowProProcess;j++){
            w[N*j]=100.0;
            w[N*j+N-1]=100.0;
        }
        mean = mean+effectivRow*2*100.0;
    }
    MPI_Barrier(MPI_COMM_WORLD);

  //
  //  Average the boundary values, to come up with a reasonable
  //  initial value for the interior.
  //  TODO: Keep in mind that you need to average over all processes
  double mean_global = 0.0;
  MPI_Reduce(&mean, &mean_global, 1, MPI_DOUBLE,MPI_SUM,0,MPI_COMM_WORLD);
    if(myid == 0){
        mean_global = mean_global / (double)(2 * M + 2 * N - 4);
        cout << "\n";
        cout << "  MEAN = " << mean_global << "\n";
    }

   MPI_Bcast(&mean_global, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if(myid == size-1){
        for(int j = 0; j<end;j++){
            for(i=1;i<N-1;i++) {
                w[N * j+i] = mean_global;
            }
        }
    }
    else if(myid == 0){
        for(int j = 1; j<rowProProcess;j++){
            for(i=1;i<N-1;i++) {
                w[N * j+i] = mean_global;
            }
        }

    }else if(myid>0 && myid < size-1){
        //TODO
        for(int j = 0; j<rowProProcess;j++){
            for(i=1;i<N-1;i++) {
                w[N * j+i] = mean_global;
            }
        }
    }


  iterations = 0;
  iterations_print = 1;
  if(myid == 0){
      cout << "\n";
      cout << " Iteration  Change\n";
      cout << "\n";
  }


  Timer timer;

  diff = epsilon;

    MPI_Barrier(MPI_COMM_WORLD);


  // TODO: This statement should yield the same result on all processes, so that all
  // processes stop at the same iteration

  while(epsilon<=diff){

      if(myid == size-1){
          for(int j = 0; j<end+1;j++){
              for(i=0;i<N;i++) {
                  u[N * j+i] = w[N * j+i];
              }
          }
      }
      else if(myid == 0){
          for(int j = 0; j<rowProProcess;j++){
              for(i=0;i<N;i++) {
                  u[N * j+i] = w[N * j+i];
              }
          }

      } else if(myid>0 && myid < size-1){
          //TODO
          for(int j = 0; j<rowProProcess;j++){
              for(i=0;i<N;i++) {
                  u[N * j+i] = w[N * j+i];
              }
          }
      }
      MPI_Barrier(MPI_COMM_WORLD);

      //  Determine the new estimate of the solution at the interior points.
      //  The new solution W is the average of north, south, east and west
      //  neighbors.
      //  TODO: Here you may need parts of the matrix that are part of other processes
      if(myid == size-1){
          for(int j = 1; j<end;j++){
              for(i=1;i<N-1;i++) {
                  w[N*j+i] = (u[N*j+i-1] + u[N*j+i+1] + u[N*(j-1)+i] + u[N*(j+1)+i]) / 4.0;
              }
          }
      }
      else if(myid == 0){
          for(int j = 1; j<rowProProcess-1;j++){
              for(i=1;i<N-1;i++) {
                  w[N*j+i] = (u[N*j+i-1] + u[N*j+i+1] + u[N*(j-1)+i] + u[N*(j+1)+i]) / 4.0;
              }
          }

      } else if(myid>0 && myid < size-1){
          //TODO
          for(int j = 1; j<rowProProcess-1;j++){
              for(i=1;i<N-1;i++) {
                  w[N*j+i] = (u[N*j+i-1] + u[N*j+i+1] + u[N*(j-1)+i] + u[N*(j+1)+i]) / 4.0;
              }
          }
      }
      MPI_Barrier(MPI_COMM_WORLD);
      // TODO: Be aware that each process only computes its local diff here. You may
      // need to combine the results from all processes
      double loc_diff = 0.0;
      if(myid == size-1){
          for(int j = 1; j<end;j++){
              for(i=1;i<N-1;i++) {
                  if (loc_diff < fabs(w[N*j+i] - u[N*j+i])) {
                      loc_diff = fabs(w[N*j+i] - u[N*j+i]);
                  }
              }
          }
      }
      else if(myid == 0){
          for(int j = 1; j<rowProProcess-1;j++){
              for(i=1;i<N-1;i++) {
                  if (loc_diff < fabs(w[N*j+i] - u[N*j+i])) {
                      loc_diff = fabs(w[N*j+i] - u[N*j+i]);
                  }
              }
          }

      } else if(myid>0 && myid < size-1 ){
          //TODO
          for(int j = 1; j<rowProProcess-1;j++){
              for(i=1;i<N-1;i++) {
                  if (loc_diff < fabs(w[N*j+i] - u[N*j+i])) {
                      loc_diff = fabs(w[N*j+i] - u[N*j+i]);
                  }
              }
          }
      }
      diff = 0.0;
      MPI_Barrier(MPI_COMM_WORLD);
      MPI_Reduce(&loc_diff, &diff, 1, MPI_DOUBLE,MPI_MAX,0,MPI_COMM_WORLD);

      MPI_Bcast(&diff, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
      MPI_Request* request =(MPI_Request*)malloc(4*sizeof(MPI_Request));
      if(myid == size-1){
          MPI_Isend(w+(coverRow-1)*N, N, MPI_DOUBLE, size-2, 0,MPI_COMM_WORLD, request);
          MPI_Irecv(w,N,MPI_DOUBLE,size-2,0,MPI_COMM_WORLD,request+1);
//          MPI_Sendrecv(w+(coverRow-1)*N, N, MPI_DOUBLE, size-2, 0,w,N,MPI_DOUBLE,size-2,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      }
      else if(myid == 0){
          MPI_Isend(w+N*(rowProProcess-coverRow),N,MPI_DOUBLE,1,0,MPI_COMM_WORLD, request);
          MPI_Irecv(w+N*(rowProProcess-1), N, MPI_DOUBLE,1,0,MPI_COMM_WORLD, request+1);
//          MPI_Sendrecv(w+N*(rowProProcess-coverRow),N,MPI_DOUBLE,1,0,w+N*(rowProProcess-1), N, MPI_DOUBLE,1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);

      } else if(myid > 0 && myid < size-1){
          MPI_Isend(w+(coverRow-1)*N, N, MPI_DOUBLE, myid-1, 0,MPI_COMM_WORLD, request);
          MPI_Irecv(w+N*(rowProProcess-1), N, MPI_DOUBLE,myid+1,0,MPI_COMM_WORLD, request+1);
          MPI_Isend(w+N*(rowProProcess-coverRow),N,MPI_DOUBLE,myid+1,0,MPI_COMM_WORLD, request+2);
          MPI_Irecv(w,N,MPI_DOUBLE,myid-1,0,MPI_COMM_WORLD, request+3);
//          MPI_Sendrecv(w+(coverRow-1)*N, N, MPI_DOUBLE, myid-1, 0,w+N*(rowProProcess-1), N, MPI_DOUBLE,myid+1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
//          MPI_Sendrecv(w+N*(rowProProcess-coverRow),N,MPI_DOUBLE,myid+1,0,w,N,MPI_DOUBLE,myid-1,0,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
      }
      iterations++;

      if (iterations == iterations_print) {
          if(myid == 0)
            cout << "  " << setw(8) << iterations << "  " << diff << "\n";
          iterations_print = 2 * iterations_print;
      }
      if(myid == 0)
        cout<<"DIFF:"<<diff<<endl;
      if(myid == size-1){
          MPI_Wait(request, MPI_STATUS_IGNORE);
          MPI_Wait(request+1, MPI_STATUS_IGNORE);
      }
      else if(myid == 0){
          MPI_Wait(request, MPI_STATUS_IGNORE);
          MPI_Wait(request+1, MPI_STATUS_IGNORE);
      } else if(myid > 0 && myid < size-1){
          MPI_Wait(request, MPI_STATUS_IGNORE);
          MPI_Wait(request+1, MPI_STATUS_IGNORE);
          MPI_Wait(request+2, MPI_STATUS_IGNORE);
          MPI_Wait(request+3, MPI_STATUS_IGNORE);
      }
      free(request);

  }

    // TODO: Insert a Barrier before time measurement
    MPI_Barrier(MPI_COMM_WORLD);
  if(myid == 0) {
      wtime = timer.getSeconds();

      // TODO: Make sure that only one MPI process prints the output
      cout << "\n";
      cout << "  " << setw(8) << iterations << "  " << diff << "\n";
      cout << "\n";
      cout << "  Error tolerance achieved.\n";
      cout << "  Wallclock time = " << setprecision(3) << wtime << "s\n";
      //
      //  Terminate.
      //
      cout << "\n";
      cout << "HEATED_PLATE:\n";
      cout << "  Normal end of execution.\n";
  }
  free(u);
  free(w);
    return 0;
#undef M
#undef N
}
