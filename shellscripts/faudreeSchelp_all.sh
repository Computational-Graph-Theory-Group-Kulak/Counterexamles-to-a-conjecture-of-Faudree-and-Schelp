#!/bin/bash

dir=../data/faudreeSchelp/$1-vertices/all/$(echo $2 | tr -d ' ') 

mkdir -p $dir

for i in {0..47}
do
 ../plantri53/plantri -g $2  $1 $i/48 2> $dir/nauty.e$i | .././hamConPaths all 2> $dir/hamcon.e$i 1>$dir/results-$i.txt &
done
wait 
echo "All done"