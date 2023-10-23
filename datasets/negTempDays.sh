#!/bin/bash

#script for extracing all days with negative temperature from cleaned csv file.
#the time period treated is 1964-end date of file.
#csv files to be treated are given as input to the script-
listDatasets=( "$@" )


for list in "${listDatasets[@]}"; do
  #line number of first entry in 1964
  lineStart=$(grep -m 1 -n "1964" $list | cut -d':' -f1)
  #all days from 1964-end of file with any negative temperatures. sort removes double counting of days.
  negDays=($(tail -n "+$lineStart" $list | grep ",-" | cut -d',' -f1 | sort -u))
  #first line of file needed to parse the file using lazycsv
  echo "date,time,temperature" >> ${list/.csv/_neg.csv}
  #extract the data for each day with any negative termperatures
  for day in "${negDays[@]}"; do
    grep "$day" $list >> ${list/.csv/_neg.csv}
  done
done