#!/bin/bash
echo "-------------CREATING ALL APRIL APPROXIMATIONS -------------" > script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "1. T1NA T2NA"
./sj -p 1000 -c -f -q T1NA T2NA >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "2. T1NA T3NA"
./sj -p 1000 -c -f -q T1NA T3NA >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "3. O5_Africa O6_Africa"
./sj -p 1000 -c -f -q O5_Africa O6_Africa >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "4. O5_Asia O6_Asia"
./sj -p 1000 -c -f -q O5_Asia O6_Asia >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "5. O5_Europe O6_Europe"
./sj -p 1000 -c -f -q O5_Europe O6_Europe >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "6. O5_NorthAmerica O6_NorthAmerica"
./sj -p 1000 -c -f -q O5_NorthAmerica O6_NorthAmerica >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "7. O5_SouthAmerica O6_SouthAmerica"
./sj -p 1000 -c -f -q O5_SouthAmerica O6_SouthAmerica >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "8. O5_Oceania O6_Oceania"
./sj -p 1000 -c -f -q O5_Oceania O6_Oceania >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/create-output.txt
echo $'\n' >> script-output/create-output.txt