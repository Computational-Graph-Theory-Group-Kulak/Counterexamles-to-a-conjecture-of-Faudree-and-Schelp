#!/bin/bash

dir=../data/algorithms/hamcon/connected/$1-vertices
cycledir=../data/algorithms/hamcon/cycles/$1-vertices

mkdir -p $dir

mkdir -p $cycledir

for i in {0..29}
do
  ../snarkhunter/snarkhunter $1 3 s o g m $i 30 2> $dir/nauty.e$i | .././hamConCycles $cycledir/cycles.e$i 2> $dir/hamcon.e$i 1>/dev/null &
done
wait 
echo "All done"

