#!/bin/bash
i=0
n=1
delay=1
while [ $i -le 0 ]
do
  echo "Start test $n"
  echo exit | ./host_main ../sw-kernel/sw_kernel.rawbinary 
  sleep $delay
  n=$((n+1))
done
