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

#include "containers.h"
#include "join_geometry_refinement.h"


#include "../libvbyte-master/vbyte.h"
#include "../libvbyte-master/varintdecode.h"

using namespace std;



/*
*-------------------------------------------------------
*
*     COMMON
*       
*
*-------------------------------------------------------
*/

//performs the join between two compressed APRIL approximations
int joinPolygons_compressed(Polygon *polA, Polygon *polB);
int joinPolygons_uncompressed(Polygon *polA, Polygon *polB);

//performs the join between two compressed APRIL approximations of different granularity
int joinPolygons_compressed_different_granularities(Polygon *polA, Polygon *polB);
int joinPolygons_uncompressed_different_granularities(Polygon *polA, Polygon *polB);

int joinPolygons_compressed_linestrings(Polygon *polA, Polygon *polB);
int joinPolygons_uncompressed_linestrings(Polygon *polA, Polygon *polB);

int joinPolygonsWithin_uncompressed(Polygon *polA, Polygon *polB);
int joinPolygonsWithin_compressed(Polygon *polA, Polygon *polB);


int joinPolygons_uncompressed_galloping(Polygon *polA, Polygon *polB);