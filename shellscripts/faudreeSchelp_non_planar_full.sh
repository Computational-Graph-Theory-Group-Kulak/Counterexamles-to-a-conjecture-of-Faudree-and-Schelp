#!/bin/bash

dir=../data/faudreeSchelp/$1-vertices/full/$(echo $2 | tr -d ' ') 

mkdir -p $dir

for i in {0..149}
do
 ../nauty/geng $2  $1 $i/150 2> $dir/nauty.e$i | .././hamConPaths full 2> $dir/hamcon.e$i 1>$dir/results-$i.txt &
done
wait 
echo "All done"