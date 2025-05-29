#!/bin/bash

dir=../data/faudreeSchelp/$1-vertices/any/$(echo $3 | tr -d ' ')/$(echo $2 | tr -d ' ') 

mkdir -p $dir

for i in {0..47}
do
 ../plantri/plantri -g $2  $1 $i/48 2> $dir/plantri.e$i | ../nauty/pickg $3 2> $dir/pickg.e$i | ../hamConPaths any 2> $dir/hamcon.e$i 1>$dir/results-$i.txt &
done
wait 
echo "All done"