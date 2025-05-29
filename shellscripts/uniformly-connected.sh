#!/bin/bash

dir=../data/faudreeSchelp/$1-vertices/uniformly-connected
mkdir -p $dir

for i in {0..99}
do
 ../nauty/./geng -C -d3 $1 $2 $i/100 2> $dir/nauty.e$i | .././hamConPaths full 2> $dir/hamcon.e$i 1>$dir/results-$i.txt &
done
wait 
echo "All done"