#!/bin/bash

dir=/home/u0149846/data/algorithms/hamcon/connected/$1-vertices

mkdir -p $dir

for i in {0..29}
do
  /home/u0149846/codebase/nauty/geng -c $1 $i/30 2> $dir/nauty.e$i | /home/u0149846/codebase/hamCon/./hamCon 2> $dir/hamcon.e$i 1>/dev/null &
done
wait 
echo "All done"

