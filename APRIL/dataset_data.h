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
#include <map>


#include "containers.h"

using namespace std;

/* geometry files */
string getBinaryGeometryFilename(int flag);

/* interval files */
string getIntervalALLBinaryFilename(int argument);

string getIntervalFBinaryFilename(int flag);

/* geometry offset map */
string getOffsetMap(int flag);

void getUniversalCoordinates(int setcode);

void buildFilePaths(string &argument1, string &argument2);