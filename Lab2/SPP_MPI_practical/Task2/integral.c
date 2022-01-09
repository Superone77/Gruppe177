//  Author:
//
//    Yannic Fischler
//    Modfied for WS21 by Sebastian Kreutzer

#include <stdlib.h>
#include <stdio.h>
#include <mpi.h>


int world_size;
int world_rank;


int main(int argc, char** argv) {
  MPI_Init(&argc, &argv);
  // TODO: Store number of processes in world_size
  MPI_Comm_size(MPI_COMM_WORLD, &world_size);
  // TODO: Store current rank in world_rank
  MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

  if (argc != 2) {
    printf("Add number of sampling points as parameter\n");
    return 1;
  }

  int numberOfPoints = atoi(argv[1]);

  // TODO: Make sure that numberOfPoints is valid
  if(numberOfPoints<world_size){
      numberOfPoints=world_size;
  }

  if (world_rank == 0) {
    printf("Running with %d processes\n", world_size);
  }

  double result = 0;


  // TODO: Implement the Monte-Carlo simulation as described in the task.
  //       Store the solution in "result".
  srand(world_rank);

  double h = 2.0/(double)numberOfPoints;
  double temp = 0;
  for(int i = 0;(world_size)*i+world_rank < numberOfPoints;i++) {
        double x = -1.0+1.0*rand()/RAND_MAX *2;
        temp += 2.0 / (1 + x * x);
  }
  if(world_rank!=0){
        int error = MPI_Send(&temp, 1, MPI_DOUBLE, 0, 1, MPI_COMM_WORLD);
        if(error != MPI_SUCCESS){
            printf("process %d fail to send \n", world_rank);
        }
  }
  if(world_rank == 0){
      result+=temp;
      for(int i = 1; i<world_size;i++){
          double rcv;
          MPI_Recv(&rcv,1,MPI_DOUBLE,i,1,MPI_COMM_WORLD,MPI_STATUS_IGNORE);
          result+=rcv;
      }
  }


  if (world_rank == 0) {
      result *= h;
    printf("%f\n", result);
  }

  MPI_Finalize();

  return 0;

}



