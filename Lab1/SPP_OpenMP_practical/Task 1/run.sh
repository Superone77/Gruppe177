#! /bin/bash
#SBATCH -A kurs00051
#SBATCH -p kurs00051
#SBATCH --reservation=kurs00051
#SBATCH -J daxpy
#SBATCH --mail-type=ALL
#SBATCH -e /work/home/kurse/kurs00051/wy17hosa/test.err.%j
#SBATCH -o /work/home/kurse/kurs00051/wy17hosa/test.out.%j
#SBATCH --mail-type=ALL
#SBATCH -n 2
#SBATCH -c 2
#SBATCH --mem-per-cpu=3800
#SBATCH -t 00:10:00

module purge
module load gcc

./daxpy 1 > out.txt
./daxpy 2 > out.txt
./daxpy 4 > out.txt
./daxpy 8 > our.txt
./daxpy 16 > out.txt

