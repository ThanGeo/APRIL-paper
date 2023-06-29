#!/bin/bash
echo "-------------TEST 4 BEGINS-------------" > script-output/test4-output.txt
echo $'\n' >> script-output/test4-output.txt

echo "1. T1NA T8NA polygon-linestring intersection join with APRIL creation and 10 pipeline iterations"
echo "1. T1NA T8NA polygon-linestring intersection join with APRIL creation and 10 pipeline iterations" >> script-output/test4-output.txt
./sj -p 1000 -c -f -q -e -l T1NA T8NA >> script-output/test4-output.txt
echo $'\n' >> script-output/test4-output.txt

echo "2. T2NA T8NA polygon-linestring intersection join with APRIL creation and 10 pipeline iterations"
echo "2. T2NA T8NA polygon-linestring intersection join with APRIL creation and 10 pipeline iterations" >> script-output/test4-output.txt
./sj -p 1000 -c -f -q -e -l T2NA T8NA >> script-output/test4-output.txt
echo $'\n' >> script-output/test4-output.txt

echo "3. T3NA T8NA polygon-linestring intersection join with APRIL creation and 10 pipeline iterations"
echo "3. T3NA T8NA polygon-linestring intersection join with APRIL creation and 10 pipeline iterations" >> script-output/test4-output.txt
./sj -p 1000 -c -f -q -e -l T3NA T8NA >> script-output/test4-output.txt
echo $'\n' >> script-output/test4-output.txt
