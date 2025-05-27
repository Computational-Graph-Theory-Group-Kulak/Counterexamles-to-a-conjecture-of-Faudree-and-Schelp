#!/bin/bash

dir=../data/faudreeSchelp/$1-vertices/any/$(echo $2 | tr -d ' ') 

mkdir -p $dir

for i in {0..29}
do
 ../nauty/geng $2  $1 $i/30 2> $dir/nauty.e$i | .././hamConPaths any 2> $dir/hamcon.e$i 1>$dir/results-$i.txt &
done
wait 
echo "All done"