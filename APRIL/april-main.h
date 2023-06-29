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
#include <omp.h>

#include "containers.h"
#include "rasterization.h"
#include "intervalization.h"
#include "join.h"
#include "binary_files.h"

using namespace std;

void createApproximations(string argument, int flag);

void loadApproximations(Dataset &dataset, string argument, int flag);