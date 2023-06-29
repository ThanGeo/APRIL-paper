#!/bin/bash
echo "-------------TEST 2 BEGINS-------------" > script-output/test2-output.txt
echo $'\n' >> script-output/test2-output.txt

echo "1. T1NA T2NA polygon-polygon intersection join with APRIL creation and 10 pipeline iterations"
echo "1. T1NA T2NA polygon-polygon intersection join with APRIL creation and 10 pipeline iterations" >> script-output/test2-output.txt
./sj -p 1000 -c -f -q -e T1NA T2NA >> script-output/test2-output.txt
echo $'\n' >> script-output/test2-output.txt

echo "2. T1NA T2NA polygon-polygon compressed intersection join with compressed APRIL creation and 10 pipeline iterations"
echo "2. T1NA T2NA polygon-polygon compressed intersection join with compressed APRIL creation and 10 pipeline iterations" >> script-output/test2-output.txt
./sj -p 1000 -c -f -q -e -z T1NA T2NA >> script-output/test2-output.txt
echo $'\n' >> script-output/test2-output.txt

echo "3. T2NA T1NA polygon-polygon within join 10 iterations"
echo "3. T2NA T1NA polygon-polygon within join 10 iterations" >> script-output/test2-output.txt
./sj -p 1000 -f -q -w -e T2NA T1NA >> script-output/test2-output.txt
echo $'\n' >> script-output/test2-output.txt

echo "4. T1NA T2NA polygon-polygon intersection join with APRIL creation, 10 pipeline iterations and 4 partitions"
echo "4. T1NA T2NA polygon-polygon intersection join with APRIL creation, 10 pipeline iterations and 4 partitions" >> script-output/test2-output.txt
./sj -p 1000 -c -f -q -n 4 -e T1NA T2NA >> script-output/test2-output.txt
echo $'\n' >> script-output/test2-output.txt