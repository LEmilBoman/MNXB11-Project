#!/bin/bash
#Provide the path to the data sets that need to be cut
dataSets=( "$@" )

echo "Provide the first date to be included in the dataset in format yyyy-mm-dd"
read start
echo "Provide the last date to be included in the dataset in format yyyy-mm-dd"
read end

for dataSet in "${dataSets[@]}"; do
    firstLine=$(cat $dataSet | grep -n $start | cut -d : -f 1|head -n 1)
    echo $firstLine
    lastLine=$(cat $dataSet | grep -n $end | cut -d : -f 1|tail -n 1)
    echo $lastLine
    cat $dataSet|head -n 1 >> ${dataSet/.csv/_cut.csv}
    cat $dataSet|head -n $lastLine| tail -n $(($lastLine - $firstLine + 1)) >> ${dataSet/.csv/_cut.csv}
done