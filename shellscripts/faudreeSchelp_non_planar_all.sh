#!/bin/bash

dir=/home/u0149846/data/faudreeSchelp/$1-vertices/all/$(echo $2 | tr -d ' ') 

mkdir -p $dir

for i in {0..29}
do
 /home/u0149846/codebase/nauty/geng $2  $1 $i/30 2> $dir/nauty.e$i | /home/u0149846/codebase/hamCon/./hamConPaths all 2> $dir/hamcon.e$i 1>$dir/results-$i.txt &
done
wait 
echo "All done"