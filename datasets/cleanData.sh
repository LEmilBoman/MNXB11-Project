#!/bin/bash

#script for cleaning smhi-opendata csv files. NOTE: Run this script inside the dataset directory (containing the csv file to be cleaned).

#Add the file names of the datasets to be treated to the list. Format: '<name>'
listDatasets=()

#For each file in the list: 1. Extract the first 3 ; separated columns (containing date;time;temperature) into a temp-file
#                           2. Count the number of lines untill the data starts
#                           3. Extract the the data (starting from the line determined in 2), replace all ";" with ",", and pipe too final file
#                           4. Remove temp file.
for list in "${listDatasets[@]}"; do
  cut -d';' -f1-3 $list >> ${list/.csv/_temp.csv}
  lineStart=$(grep -n "Datum;Tid (UTC);Lufttemperatur" ${list/.csv/_temp.csv} | cut -d':' -f1)
  #echo "+$lineStart"
  tail -n "+$lineStart" ${list/.csv/_temp.csv} | sed 's/;/,/' >> ${list/.csv/_clean.csv}
  rm ${list/.csv/_temp.csv}
done