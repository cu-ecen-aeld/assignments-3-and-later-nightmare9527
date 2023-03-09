#!/bin/bash

writefile=$1
writestr=$2

if [ $# -lt 2 ]   
then
  echo "not enough arguments"
  exit 1


elif [ $# -gt 2 ]
then
   echo "too many arguments"
   exit 1

else
  mkdir -p "$(dirname "$writefile")" 
  if  ! touch  "$writefile" 
  then 
     echo "fail to make directory"
     exit 1
  fi
     echo $writestr > $writefile
      
fi

