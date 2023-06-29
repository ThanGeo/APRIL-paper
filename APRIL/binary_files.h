#pragma once

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <iomanip>
#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <algorithm>
#include <vector>
#include <math.h>
#include <cmath>
#include <ctime>
#include <bitset>
#include <unordered_map>

#include "containers.h"
#include "dataset_data.h"


using namespace std;


/*
*-------------------------------------------------------
*
*     GEOMETRY BINARY DATA
*       
*
*-------------------------------------------------------
*/

//offset map
unordered_map<uint,unsigned long> loadOffsetMap(int flag);


/*
*-------------------------------------------------------
*
*     INTERVAL BINARY DATA
*       
*
*-------------------------------------------------------
*/

void saveBinaryIntervalsCompressed(Polygon &pol, uint &sectionID, ofstream &foutALL, ofstream &foutF);
void saveBinaryIntervalsUncompressed(Polygon &pol, uint &sectionID, ofstream &foutALL, ofstream &foutF);

void loadAprilCompressed(Dataset &set, string argument, int flag);
void loadAprilUncompressed(Dataset &set, string argument, int flag);