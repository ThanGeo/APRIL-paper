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

#include "containers.h"
#include "dataset_data.h"

using namespace std;


/*
*-------------------------------------------------------
*
*     GEOMETRY RETRIEVAL
*       
*
*-------------------------------------------------------
*/

Polygon loadPolygonGeometry(uint &recID, unordered_map<uint,unsigned long> &offsetMap, ifstream &fin);
polygon loadPolygonGeometryBOOST(uint &recID, unordered_map<uint,unsigned long> &offsetMap, ifstream &fin);


MBR getCMBR(Polygon &polA, Polygon &polB);


bool refinementWithIDs(uint &idA, uint &idB, unordered_map<uint,unsigned long> &offsetMapR, unordered_map<uint,unsigned long> &offsetMapS, ifstream &finR, ifstream &finS);
bool refinementWithIDsLinestring(uint &idA, uint &idB, unordered_map<uint,unsigned long> &offsetMapR, unordered_map<uint,unsigned long> &offsetMapS, ifstream &finR, ifstream &finS);

bool refinementWithinWithIDs(uint &idA, uint &idB, unordered_map<uint,unsigned long> &offsetMapR, unordered_map<uint,unsigned long> &offsetMapS, ifstream &finR, ifstream &finS);