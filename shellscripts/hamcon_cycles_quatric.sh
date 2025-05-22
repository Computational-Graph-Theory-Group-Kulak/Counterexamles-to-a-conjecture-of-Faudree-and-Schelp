#!/bin/bash

dir=/home/u0149846/data/algorithms/hamcon/quartic/connected/$1-vertices
cycledir=/home/u0149846/data/algorithms/hamcon/cycles/quartic/$1-vertices

cores=8
mkdir -p $dir

mkdir -p $cycledir

for (( i=0; i<=$cores; i++ ))
do
  /home/u0149846/codebase/nauty/genquarticg -C $1 $i/$cores 2> $dir/nauty.e$i | /home/u0149846/codebase/hamCon/./hamConCycles $cycledir/cycles.e$i 2> $dir/hamcon.e$i 1>/dev/null &
done
wait 
echo "All done"

