#!/bin/bash


filesdir=$1
searchstr=$2


if [ $# -lt 2 ]   
  then
    echo "not enough arguments"
    exit 1

elif [ $# -gt 2 ]
  then
   echo "too many arguments"
   exit 1

elif [ ! -d $filesdir ] 
 then
  echo "directory doesn't exist"
  exit 1


fi
 a=$( find "$filesdir" -type f -print | wc -l)
 b=$( grep -r -o "$searchstr" "$filesdir" | wc -l)
 
 echo "The number of files are $a and the number of matching lines are $b"
 

