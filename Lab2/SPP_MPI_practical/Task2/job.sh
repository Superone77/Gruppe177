#!/bin/bash
#SBATCH -A kurs00051
#SBATCH -p kurs00051
#SBATCH --reservation=kurs00051
#SBATCH -J integral
#SBATCH --mail-type=ALL
#SBATCH -e "/home/kurse/kurs00051/wy17hosa/Lab2/Task2/test1.err.%j"
#SBATCH -o "/home/kurse/kurs00051/wy17hosa/Lab2/Task2/test1.out.%j"
#SBATCH -n 8
#SBATCH -c 1
#SBATCH --mem-per-cpu=3800
#SBATCH -t 00:03:00

module purge
module load gcc
module load openmpi
module load scorep
export SCOREP_ENABLE_TRACING=true
export SCOREP_ENABLE_PROFILING=false
export SCOREP_EXPERIMENT_DIRECTORY=$HPC_SCRATCH/Task2

scorep mpicc -O2 integral.c -o integral
mpirun -np 8 ./integral 100000000