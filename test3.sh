#!/bin/bash
echo "-------------TEST 3 BEGINS-------------" > script-output/test3-output.txt
echo $'\n' >> script-output/test3-output.txt

echo "1. T1NA Q polygon selection query (1000 polygonal windows) with APRIL creation and 10 pipeline iterations"
echo "1. T1NA Q polygon selection query (1000 polygonal windows) with APRIL creation and 10 pipeline iterations" >> script-output/test3-output.txt
./sj -p 1000 -c -f -q -e -s T1NA Q >> script-output/test3-output.txt
echo $'\n' >> script-output/test3-output.txt

echo "2. T2NA Q polygon selection query (1000 polygonal windows) with APRIL creation and 10 pipeline iterations"
echo "2. T2NA Q polygon selection query (1000 polygonal windows) with APRIL creation and 10 pipeline iterations" >> script-output/test3-output.txt
./sj -p 1000 -c -f -q -e -s T2NA Q >> script-output/test3-output.txt
echo $'\n' >> script-output/test3-output.txt
