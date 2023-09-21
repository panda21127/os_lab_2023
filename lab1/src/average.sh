#!/bin/bash
count=0
sum=0
#$ sh ./average_input.sh < numbers.txt
for an_arg in "$@" ; do
  sum=$(($sum + ${an_arg}))
  count=$(($count+1))
done
expr $sum / $count
expr $count

