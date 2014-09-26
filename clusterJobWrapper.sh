#!/bin/sh -login
#PBS -l nodes=1:ppn=1
#PBS -l walltime=02:00:00

module load gcc/4.9.1
module load boost/1.56.0

cd $HOME/Desktop/OnlineOptimisation/bin
./cli $OPTIMISATION_PROBLEM $NUMBER_OF_DIMENSIONS $POPULATION_SIZE
