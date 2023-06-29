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
#include <set>
#include <deque>

#include "containers.h"
#include "../libvbyte-master/vbyte.h"
#include "../libvbyte-master/varintdecode.h"

using namespace std;

extern double partial_cell_time, intervalization_time, pip_time;

void rasterize2grid(Polygon &pol, Section &sec, int &fromPower);


void rasterizeSimple(Polygon &pol, Section &sec);

void rasterizeSimpleLinestring(Polygon &pol, Section &sec);
void rasterizeAndIntervalizeNoFloodFill(Polygon &pol, Section &sec);