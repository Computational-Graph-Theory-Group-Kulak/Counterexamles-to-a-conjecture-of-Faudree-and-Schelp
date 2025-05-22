#!/bin/bash

dir=/home/u0149846/data/faudreeSchelp/$1-vertices/-p-c4 

mkdir -p $dir

for i in {0..29}
do
 /home/u0149846/codebase/plantri53/plantri -g -p -c4  $1 $i/30 2> $dir/nauty.e$i | /home/u0149846/codebase/hamCon/./FaudreeSchelp last 2> $dir/hamcon.e$i 1>$dir/results-$i.txt &
done
wait 
echo "All done"