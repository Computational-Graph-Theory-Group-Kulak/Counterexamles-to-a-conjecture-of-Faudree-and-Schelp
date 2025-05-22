#!/bin/bash

dir=/home/u0149846/data/algorithms/hamcon/connected/$1-vertices-gadgets
edges_upper_bound=$(($1*2-1))
edges_upper_bound=$(($edges_upper_bound*$edges_upper_bound))
edges_upper_bound=$(($edges_upper_bound/4))
edges_upper_bound=$(($edges_upper_bound-3))
edges_upper_bound=$(($edges_upper_bound/2))
echo "edges_upper_bound: $edges_upper_bound"

mkdir -p $dir

for i in {0..29}
do
  /home/u0149846/codebase/nauty/geng -c $1 0:$edges_upper_bound $i/30 2> $dir/nauty.e$i | /home/u0149846/codebase/hamCon/./hamConGadgets /home/u0149846/data/gadget_test.txt 2 2> $dir/hamcon.e$i 1>/dev/null &
done
wait 
echo "All done"

