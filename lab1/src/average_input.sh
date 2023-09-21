#!/bin/bash
for i in `seq 150`; do od -vAn -N2 -tu2 < /dev/random; done > numbers.txt
count=0
sum=0
while read numbers; do
    sum=$(($sum + ${numbers}))
    count=$(($count+1))
done
expr $sum / $count
expr $count