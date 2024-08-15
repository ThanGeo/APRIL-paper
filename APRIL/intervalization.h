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
#include <iomanip>
#include <unistd.h>
#include <inttypes.h>

#include "containers.h"

#include "../libvbyte-master/vbyte.h"
#include "../libvbyte-master/varintdecode.h"

using namespace std;

void intervalize(Polygon &pol);
void printIntervals(vector<ID> &intervals);
void createIntervalList(Polygon &pol, CellPackage &cp, int COLOR);

void compressLinestringCells(Polygon &pol);