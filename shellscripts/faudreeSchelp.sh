#!/bin/bash

dir=/home/u0149846/data/faudreeSchelp/$1-vertices/$(echo $2 | tr -d ' ') 

mkdir -p $dir

for i in {0..47}
do
 /home/u0149846/codebase/plantri53/plantri -g $2  $1 $i/48 2> $dir/nauty.e$i | /home/u0149846/codebase/hamCon/./hamConPaths last 2> $dir/hamcon.e$i 1>$dir/results-$i.txt &
done
wait 
echo "All done"