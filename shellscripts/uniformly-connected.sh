#!/bin/bash

dir=../data/faudreeSchelp/$1-vertices/uniformly-connected
mkdir -p $dir

for i in {0..29}
do
 ../nauty/./geng $1 $2 $i/30 2> $dir/nauty.e$i | .././hamConPaths full 2> $dir/hamcon.e$i 1>$dir/results-$i.txt &
done
wait 
echo "All done"