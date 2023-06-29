#!/bin/bash
echo "-------------TEST 1 BEGINS-------------" > script-output/test1-output.txt
echo $'\n' >> script-output/test1-output.txt

echo "1. T1NA T3NA polygon-polygon intersection join with APRIL creation and 10 pipeline iterations"
echo "1. T1NA T3NA polygon-polygon intersection join with APRIL creation and 10 pipeline iterations" >> script-output/test1-output.txt
./sj -p 1000 -c -f -q -e T1NA T3NA >> script-output/test1-output.txt
echo $'\n' >> script-output/test1-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test1-output.txt
echo $'\n' >> script-output/test1-output.txt

echo "2. T1NA T3NA polygon-polygon compressed intersection join with compressed APRIL creation and 10 pipeline iterations"
echo "2. T1NA T3NA polygon-polygon compressed intersection join with compressed APRIL creation and 10 pipeline iterations" >> script-output/test1-output.txt
./sj -p 1000 -c -f -q -e -z T1NA T3NA >> script-output/test1-output.txt
echo $'\n' >> script-output/test1-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test1-output.txt
echo $'\n' >> script-output/test1-output.txt

echo "3. T1NA T3NA polygon-polygon within join 10 iterations"
echo "3. T1NA T3NA polygon-polygon within join 10 iterations" >> script-output/test1-output.txt
./sj -p 1000 -f -q -w -e T1NA T3NA >> script-output/test1-output.txt
echo $'\n' >> script-output/test1-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test1-output.txt
echo $'\n' >> script-output/test1-output.txt
