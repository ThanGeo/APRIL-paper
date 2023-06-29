#!/bin/bash
echo "-------------RASTERIZATION TESTS BEGINS-------------" > script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "1. T1NA T3NA"
echo "1. T1NA T3NA" >> script-output/test_rasterization-output.txt
./sj -p 1000 -c -f -q T1NA T3NA >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "2. T1NA T2NA"
echo "2. T1NA T2NA" >> script-output/test_rasterization-output.txt
./sj -p 1000 -c -f -q T1NA T2NA >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "3. O5_Africa O6_Africa"
echo "3. O5_Africa O6_Africa" >> script-output/test_rasterization-output.txt
./sj -p 1000 -c -f -q O5_Africa O6_Africa >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "4. O5_Asia O6_Asia"
echo "4. O5_Asia O6_Asia" >> script-output/test_rasterization-output.txt
./sj -p 1000 -c -f -q O5_Asia O6_Asia >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "5. O5_Europe O6_Europe"
echo "5. O5_Europe O6_Europe" >> script-output/test_rasterization-output.txt
./sj -p 1000 -c -f -q O5_Europe O6_Europe >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "6. O5_NorthAmerica O6_NorthAmerica"
echo "6. O5_NorthAmerica O6_NorthAmerica" >> script-output/test_rasterization-output.txt
./sj -p 1000 -c -f -q O5_NorthAmerica O6_NorthAmerica >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "7. O5_Oceania O6_Oceania"
echo "7. O5_Oceania O6_Oceania" >> script-output/test_rasterization-output.txt
./sj -p 1000 -c -f -q O5_Oceania O6_Oceania >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "8. O5_SouthAmerica O6_SouthAmerica"
echo "8. O5_SouthAmerica O6_SouthAmerica" >> script-output/test_rasterization-output.txt
./sj -p 1000 -c -f -q O5_SouthAmerica O6_SouthAmerica >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

echo "------------------------------------------------------------------------------------------------------------------"  >> script-output/test_rasterization-output.txt
echo $'\n' >> script-output/test_rasterization-output.txt

