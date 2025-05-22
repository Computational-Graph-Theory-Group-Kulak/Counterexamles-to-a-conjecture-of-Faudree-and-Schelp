#!/bin/bash

dir=/home/u0149846/data/algorithms/hamcon/thomassen/connected/$1-vertices-counterex

mkdir -p $dir

for i in {0..29}
do
  /home/u0149846/codebase/plantri53/plantri -g -p $1 $i/30 2> $dir/plantri.e$i | /home/u0149846/codebase/hamCon/./hamConCycles $dir/counterex-$i.txt last 2> $dir/hamcon.e$i 1>$dir/results-$i &
done
wait 
echo "All done"