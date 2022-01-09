#!/bin/bash
#SBATCH -A kurs00051
#SBATCH -p kurs00051
#SBATCH --reservation=kurs00051
#SBATCH -J integral
#SBATCH --mail-type=ALL
#SBATCH -e "/home/kurse/kurs00051/wy17hosa/Lab2/Task1/test1.err.%j"
#SBATCH -o "/home/kurse/kurs00051/wy17hosa/Lab2/Task1/test1.out.%j"
#SBATCH -n 8
#SBATCH -c 1
#SBATCH --mem-per-cpu=3800
#SBATCH -t 00:03:00

module purge
module load gcc
module load openmpi


make
mpirun -np 8 ./integral 1000000000