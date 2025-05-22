#!/bin/bash

if [ -z "$1" ]
  then
    path=""
else
  path="$1"
fi

numberread=`cat $path/hamcon.e* | grep "Checked" | wc -l`
echo "Read $numberread files."
  
  total_generated=0
  counter=1
  cat $path/nauty.e* | grep ">Z" | cut -d" " -f2 | while read graphs
  do
    total_generated=`echo $total_generated+ $graphs | bc`
    if [ $counter -eq $numberread ]
      then
        echo "Total number of generated graphs : $total_generated"
    fi
        counter=`expr $counter + 1`   
  
  done

  total_generated=0
  counter=1
  cat $path/plantri.e* | grep "written to stdout;" | cut -d" " -f1 | while read graphs
  do
    total_generated=`echo $total_generated+ $graphs | bc`
    if [ $counter -eq $numberread ]
      then
        echo "Total number of generated graphs : $total_generated"
    fi
        counter=`expr $counter + 1`   
  
  done

  	total_checked=0
  counter=1
  cat $path/pickg.e* | grep ">Z" | cut -d" " -f2 | while read graphs
  do
    total_checked=`echo $total_checked + $graphs | bc`
    if [ $counter -eq $numberread ]
      then
        echo "Total number of filtered graphs : $total_checked"
    fi
        counter=`expr $counter + 1`   
  
  done

total_passed=0
  counter=1
  cat $path/pickg.e* | grep ">Z" | cut -d" " -f7 | while read graphs
  do
    total_passed=`echo $total_passed + $graphs | bc`
    if [ $counter -eq $numberread ]
      then
        echo "Total number of graphs after filter : $total_passed"
    fi
        counter=`expr $counter + 1`   
  
  done

  total_checked=0
  counter=1
  cat $path/hamcon.e* | grep "Checked" | cut -d" " -f2 | while read graphs
  do
    total_checked=`echo $total_checked + $graphs | bc`
    if [ $counter -eq $numberread ]
      then
        echo "Total number of checked graphs : $total_checked"
    fi
        counter=`expr $counter + 1`   
  
  done

  total_passed=0
  counter=1
  cat $path/hamcon.e* | grep "Checked" | cut -d" " -f7 | while read graphs
  do
    total_passed=`echo $total_passed + $graphs | bc`
    if [ $counter -eq $numberread ]
      then
        echo "Total number of passed graphs : $total_passed"
    fi
        counter=`expr $counter + 1`   
  
  done


 
  highest=0
  while read gaps
  do
  	if [ $gaps -gt $highest ]
	  then
	    highest=`echo $gaps | bc`
	fi
  
  done <<< "$( cat $path/hamcon.e* | grep "of length:" | cut -d" " -f9 )"
  echo "Largest missing path length : $highest"

 total_steps=0
  counter=1
  cat $path/hamcon.e* | grep "Checked" | cut -d" " -f10 | while read graphs
  do
    total_steps=`echo $total_steps + $graphs | bc`
    if [ $counter -eq $numberread ]
      then
        echo "Total number of steps : $total_steps"
    fi
        counter=`expr $counter + 1`   
  
  done

  total_time=0
  counter=1
  max=0
  min=`echo 365*24*3600 | bc`
  cat $path/hamcon.e* | grep "Checked" | cut -d" " -f5 | while read time
	do
		time_rounded=`echo $time/2*2 | bc` # round to second
		total_time=`echo $total_time + $time | bc`
		if [ "$time_rounded" -gt "$max" ]
		then
			max=$time_rounded
		fi
		if [ "$time_rounded" -lt "$min" ]
		then
			min=$time_rounded
		fi
		if [ $counter -eq $numberread ]
		then 
			echo "Total CPU time (s): $total_time"
			time_hours=`echo $total_time/3600 | bc`
			echo "Total CPU time (h): $time_hours"
			time_avg=`echo $time_hours/$numberread | bc`
			echo "Average CPU time (h): $time_avg"
			min_hours=`echo $min/3600 | bc`
			max_hours=`echo $max/3600 | bc`
			echo "Max CPU time (h): $max_hours"
			echo "Min CPU time (h): $min_hours"
		fi
		counter=`expr $counter + 1`
	done

total_time=0
  counter=1
  max=0
  min=`echo 365*24*3600 | bc`
  cat $path/pickg.e* | grep ">Z" | cut -d" " -f11 | while read time
	do
		time_rounded=`echo $time/2*2 | bc` # round to second
		total_time=`echo $total_time + $time | bc`
		if [ "$time_rounded" -gt "$max" ]
		then
			max=$time_rounded
		fi
		if [ "$time_rounded" -lt "$min" ]
		then
			min=$time_rounded
		fi
		if [ $counter -eq $numberread ]
		then 
			echo "Total CPU time pickg (s): $total_time"
			time_hours=`echo $total_time/3600 | bc`
			echo "Total CPU time pickg (h): $time_hours"
			time_avg=`echo $time_hours/$numberread | bc`
		#	echo "Average CPU time (h): $time_avg"
		#	min_hours=`echo $min/3600 | bc`
		#	max_hours=`echo $max/3600 | bc`
		#	echo "Max CPU time (h): $max_hours"
		#	echo "Min CPU time (h): $min_hours"
		fi
		counter=`expr $counter + 1`
	done

  total_time=0
  counter=1
  max=0
  min=`echo 365*24*3600 | bc`
  cat $path/nauty.e* | grep ">Z" | cut -d" " -f6 | while read time
	do
		time_rounded=`echo $time/2*2 | bc` # round to second
		total_time=`echo $total_time + $time | bc`
		if [ "$time_rounded" -gt "$max" ]
		then
			max=$time_rounded
		fi
		if [ "$time_rounded" -lt "$min" ]
		then
			min=$time_rounded
		fi
		if [ $counter -eq $numberread ]
		then 
			echo "Total CPU time geng (s): $total_time"
			time_hours=`echo $total_time/3600 | bc`
			echo "Total CPU time geng (h): $time_hours"
			time_avg=`echo $time_hours/$numberread | bc`
		#	echo "Average CPU time (h): $time_avg"
		#	min_hours=`echo $min/3600 | bc`
		#	max_hours=`echo $max/3600 | bc`
		#	echo "Max CPU time (h): $max_hours"
		#	echo "Min CPU time (h): $min_hours"
		fi
		counter=`expr $counter + 1`
	done

  total_time=0
  counter=1
  max=0
  min=`echo 365*24*3600 | bc`
  cat $path/plantri.e* | grep "written to stdout;" | rev | cut -d" " -f2 | rev | cut -c 5- | while read time
	do
		time_rounded=`echo $time/2*2 | bc` # round to second
		total_time=`echo $total_time + $time | bc`
		if [ "$time_rounded" -gt "$max" ]
		then
			max=$time_rounded
		fi
		if [ "$time_rounded" -lt "$min" ]
		then
			min=$time_rounded
		fi
		if [ $counter -eq $numberread ]
		then 
			echo "Total CPU time plantri (s): $total_time"
			time_hours=`echo $total_time/3600 | bc`
			echo "Total CPU time plantri (h): $time_hours"
			time_avg=`echo $time_hours/$numberread | bc`
		#	echo "Average CPU time (h): $time_avg"
		#	min_hours=`echo $min/3600 | bc`
		#	max_hours=`echo $max/3600 | bc`
		#	echo "Max CPU time (h): $max_hours"
		#	echo "Min CPU time (h): $min_hours"
		fi
		counter=`expr $counter + 1`
	done
