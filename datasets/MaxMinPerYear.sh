#!/bin/bash

#script for extracing all days with negative temperature from cleaned csv file.
#the time period treated is 1964-end date of file.
#csv files to be treated are given as input to the script-
listDatasets=( "$@" )


for list in "${listDatasets[@]}"; do
  #line number of first entry in 1949
  lineStart=$(grep -m 1 -n "1949" $list | cut -d':' -f1)
  #all years in file after 1949 that contains entries
  years=($(tail -n "+$lineStart" $list | cut -c -4 | sort -u))
  #first line of file needed to parse the file using lazycsv
  echo "date,time,temperature" >> ${list/.csv/_MaxMinTemp.csv}
  #go through each year individually and find max and min temperature.
  for year in "${years[@]}"; do
    temp=$(grep "$year" $list|cut -d "," -f3|sort -g|sed -n '1p; $p')
    grep "$year" $list|grep -w -e "$temp" >> ${list/.csv/_MaxMinTemp.csv}
  done
done