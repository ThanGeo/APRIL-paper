#include "april-main.h"

/*
*-------------------------------------------------------
*
*     GLOBAL VARIABLES
*       
*
*-------------------------------------------------------
*/

//temp container 32-bit
CONTAINER* all = new CONTAINER[1000000];
CONTAINER* f = new CONTAINER[1000000];

uint8_t out[10000000];

double intervalizationTime;
double rasterizationTime;
double preprocessingTime;

vector<double> openGLRasterizationTime;

uint TOTAL_INTERVAL_LENGTH = 0;
uint TOTAL_INTERVAL_COUNT = 0;
uint TOTAL_INTERVAL_GAP = 0;
uint TOTAL_GAP_COUNT = 0;
uint MAX_INTERVAL_LENGTH = 0;
uint MAX_GAP_LENGTH = 0;


/*
*-------------------------------------------------------
*
*     COMPUTE APRIL FOR POLYGONS
*       
*
*-------------------------------------------------------
*/


void computeIntervalsPolygons(string &argument, int flag){
	clock_t timer;	
	uint lineCounter = 0;
	uint recID;
	int totalPolygonCount, vertexCount;
	double x,y;
	double polxMin,polyMin,polxMax,polyMax;
	double MBRarea;
	vector<Point> originalVertices;
	uint C;
	int K;
	polygon bPolygon;

	//timers
	// rasterizationTime = 0;
	// intervalizationTime = 0;	
	// partial_cell_time = 0; 
	// intervalization_time = 0;
	// pip_time = 0;
	// total_pip_tests = 0;
	preprocessingTime = 0;
	
	//geometry input file
	string filename = getBinaryGeometryFilename(flag);
	ifstream fin(filename, fstream::in | ios_base::binary);
	//APRIL output files
	ofstream foutALL(getIntervalALLBinaryFilename(flag), ios_base::out | ios_base::binary);
	ofstream foutF(getIntervalFBinaryFilename(flag), ios_base::out | ios_base::binary);
	
	//read total polygon count from binary geometry file
	fin.read((char*) &totalPolygonCount, sizeof(int));
	//write H
	foutALL.write((char*)(&H), sizeof(uint));
	foutF.write((char*)(&H), sizeof(uint));
	//write order N for whole dataset, if so
	if(DIFF_GRANULARITY_FIXED && (argument == "T3NA" || argument == "O6_Oceania")){
		foutALL.write((char*)(&DESIGNATED_ORDER), sizeof(uint));
		foutF.write((char*)(&DESIGNATED_ORDER), sizeof(uint));
	}
	//write data space object count total
	foutALL.write((char*)&DATA_SPACE.totalObjects, sizeof(uint));
	foutF.write((char*)&DATA_SPACE.totalObjects, sizeof(uint));

	// size_t pixelBits = 0;
	// size_t intervalBits = 0;
	// size_t MBRbits = 0;
	// size_t indicesBits = 0;

	//---BUILD POLYGON LOOP---
	while(lineCounter < totalPolygonCount){
		//read pol id
		fin.read((char*) &recID, sizeof(int));
		//initialize MBR
		polxMin = numeric_limits<int>::max();
		polyMin = numeric_limits<int>::max();
		polxMax = -numeric_limits<int>::max();
		polyMax = -numeric_limits<int>::max();
		//read vertex count for polygon & reserve space
		fin.read((char*) &vertexCount, sizeof(int));		
		bPolygon.outer().clear();
		originalVertices.clear();
		originalVertices.reserve(vertexCount);
		//read points polygon
		for(int i=0; i<vertexCount; i++){
			//read x, y
			fin.read((char*) &x, sizeof(double));
			fin.read((char*) &y, sizeof(double));
			//store vertices
			originalVertices.emplace_back(x,y);
			bPolygon.outer().emplace_back(x,y);
			//keep original mbr
			polxMin = min(x, polxMin);
			polyMin = min(y, polyMin);
			polxMax = max(x, polxMax);
			polyMax = max(y, polyMax);
		}
		//correct
		boost::geometry::correct(bPolygon);
		//set to object's mbr
		MBR originalMBR(polxMin, polyMin, polxMax, polyMax);
		MBRarea = originalMBR.getArea();
		//calculate polygon's area

		timer = clock();
		//get the intersecting sections of the polygon's MBR in the data space
		vector<Section> sects = DATA_SPACE.getSectionsOfMBR(polxMin, polyMin, polxMax, polyMax);
		// std::cout << "locate section " << (clock()-timer) / (double)(CLOCKS_PER_SEC) << endl;

		//rasterize separately for each section (if there are more than 1 partitions, otherwise only 1 section; the entire data space)
		for(int i = 0; i<sects.size(); i++){
			Section sec = sects.at(i);
			
			//create polygon
			Polygon pol(recID);
			pol.mbr = originalMBR;
			pol.vertices = originalVertices;
			pol.orderN = 16;

			// if (pol.recID == 2143) {
			
			// 	exit(0);
			// }
			//combined rasterization and intervalization with no flood filling
			timer = clock();
			// printf("Rasterizing object %d with %ld vertices.\n", pol.recID, pol.vertices.size());
			// rasterizeAndIntervalizeScanline(pol, sec);
			// rasterizeAndIntervalizeHybridDDAScanline(pol, sec);
			// rasterizeAndIntervalizeFloodFill(pol, sec);
			intervalizeOneStep(pol, sec);
			// printf("Rasterized and intervalized in %f seconds.\n", (clock()-timer) / (double)(CLOCKS_PER_SEC));
			preprocessingTime += (clock()-timer) / (double)(CLOCKS_PER_SEC);
			
			//---SAVE ON DISK---
			switch(COMPRESSION){
				case 0:
					saveBinaryIntervalsUncompressed(pol, sec.sectionID, foutALL, foutF);
					break;
				case 1:
					saveBinaryIntervalsCompressed(pol, sec.sectionID, foutALL, foutF);
					break;
			}		
		

			// test only: count how much memory it would take if we used 2 bits per cell
			// for (int i=0; i<pol.uncompressedALL.size(); i+=2) {
			// 	pixelBits += (pol.uncompressedALL[i+1] - pol.uncompressedALL[i] + 1) * 2;
			// 	indicesBits += (pol.uncompressedALL[i+1] - pol.uncompressedALL[i] + 1) * 32;
			// 	intervalBits += 2 * 32;
			// }
			// for (int i=0; i<pol.uncompressedF.size(); i+=2) {
			// 	pixelBits += (pol.uncompressedF[i+1] - pol.uncompressedF[i] + 1) * 2;
			// 	indicesBits += (pol.uncompressedF[i+1] - pol.uncompressedF[i] + 1) * 32;
			// 	intervalBits += 2 * 32;
			// }
			// normalizeXYToSectionHilbert(originalMBR.pMin.x, originalMBR.pMin.y, sec.rasterxMin, sec.rasteryMin, sec.rasterxMax, sec.rasteryMax, HILBERT_n);
			// normalizeXYToSectionHilbert(originalMBR.pMax.x, originalMBR.pMax.y, sec.rasterxMin, sec.rasteryMin, sec.rasterxMax, sec.rasteryMax, HILBERT_n);
			// // get the hilbert cells min/max
			// pol.minHilbertX = (uint) originalMBR.pMin.x;
			// pol.minHilbertY = (uint) originalMBR.pMin.y;
			// pol.maxHilbertX = (uint) originalMBR.pMax.x;
			// pol.maxHilbertY = (uint) originalMBR.pMax.y;
			// MBRbits += (pol.maxHilbertX - pol.minHilbertX + 1) * (pol.maxHilbertY - pol.minHilbertY + 1) * 2 + 64;
		}


		lineCounter++;
	}

	// std::cout << "  Rasterization time: " << rasterizationTime << " sec." << endl;
	// std::cout << "  Intervalization time: " << intervalizationTime << " sec." << endl;

	// std::cout << "Pip tests per polygon: " << (double) total_pip_tests / totalPolygonCount << endl;
	// std::cout << "	partial cells time: " << partial_cell_time << " sec." << endl;
	// std::cout << "	intervalization time: " << intervalization_time << " sec." << endl;
	// std::cout << "		pip time: " << pip_time << " seconds." << endl;


	std::cout << "  Pre-processing time: " << preprocessingTime << " sec." << std::endl;
	// printf("MB needed if we were to keep 2 bits per non-overlapping cell (no referencing): %f\n", ((pixelBits) / (double) 8) / 1000000);
	// printf("MB needed if we were to keep 2 16-bit indices and 2 bits per non-overlapping cell: %f\n", ((pixelBits + (indicesBits/2)) / (double) 8) / 1000000);
	// printf("MB needed if we were to keep 2 32-bit indices and 2 bits per non-overlapping cell: %f\n", ((pixelBits + indicesBits) / (double) 8) / 1000000);
	// printf("MB needed if we were to keep a 2-d array with 2 bits per cell: %f\n", ((pixelBits+MBRbits) / (double) 8) / 1000000);
	// printf("MB needed for APRIl as it is: %f\n", (intervalBits / (double) 8) / 1000000);

	fin.close();
	foutALL.close();
	foutF.close();
}

/*
*-------------------------------------------------------
*
*     COMPUTE APRIL FOR LINESTRINGS
*       
*
*-------------------------------------------------------
*/


void computeIntervalsLinestrings(string &argument, int flag){
	uint lineCounter = 0;
	string line;
	clock_t timer;	
	uint recID;
	int totalObjectCount, vertexCount;
	double x,y;
	double polxMin,polyMin,polxMax,polyMax;
	vector<Point> originalVertices;

	linestring bLinestring;

	//timers
	rasterizationTime = 0;
	intervalizationTime = 0;
	
	//geometry input file
	string filename = getBinaryGeometryFilename(flag);
	ifstream fin(filename, fstream::in | ios_base::binary);

	//APRIL output files
	ofstream foutALL(getIntervalALLBinaryFilename(flag), ios_base::out | ios_base::binary);
	ofstream foutF(getIntervalFBinaryFilename(flag), ios_base::out | ios_base::binary);
	
	//read total polygon count from binary geometry file
	fin.read((char*) &totalObjectCount, sizeof(int));
	//write H
	foutALL.write((char*)(&H), sizeof(uint));
	foutF.write((char*)(&H), sizeof(uint));

	//write data space object count total
	foutALL.write((char*)&DATA_SPACE.totalObjects, sizeof(uint));
	foutF.write((char*)&DATA_SPACE.totalObjects, sizeof(uint));

	//---BUILD POLYGON LOOP---
	while(lineCounter < totalObjectCount){
		//read pol id
		fin.read((char*) &recID, sizeof(int));		

		//initialize MBR
		polxMin = numeric_limits<int>::max();
		polyMin = numeric_limits<int>::max();
		polxMax = -numeric_limits<int>::max();
		polyMax = -numeric_limits<int>::max();

		//read vertex count for polygon & reserve space
		fin.read((char*) &vertexCount, sizeof(int));		
		
		bLinestring.clear();
		originalVertices.clear();
		originalVertices.reserve(vertexCount);
		//read points
		for(int i=0; i<vertexCount; i++){
			//read x, y
			fin.read((char*) &x, sizeof(double));
			fin.read((char*) &y, sizeof(double));			
			//store vertices
			originalVertices.emplace_back(x,y);
			bLinestring.emplace_back(x,y);
			//keep original mbr
			polxMin = min(x, polxMin);
			polyMin = min(y, polyMin);
			polxMax = max(x, polxMax);
			polyMax = max(y, polyMax);
		}
		
		//set to object's mbr
		MBR originalMBR(polxMin, polyMin, polxMax, polyMax);

		//get the intersecting sections of the polygon's MBR in the data space
		timer = clock();
		vector<Section> sects = DATA_SPACE.getSectionsOfMBR(polxMin, polyMin, polxMax, polyMax);

		//rasterize separately for each section
		for(int i = 0; i<sects.size(); i++){
			Section sec = sects.at(i);

			//create polygon (linestring)
			Polygon pol(recID);
			pol.mbr = originalMBR;
			pol.vertices = originalVertices;
			pol.orderN = 16;

			//---RASTERIZE LINESTRING---
			timer = clock();			
			//rasterize using a modified simple algorithm
			rasterizeSimpleLinestring(pol, sec);
			rasterizationTime += (clock()-timer) / (double)(CLOCKS_PER_SEC);

			//no intervalization, keep individual cells
			
			//---SAVE ON DISK---
			switch(COMPRESSION){
				case 0:
					//save uncompressed
					saveBinaryIntervalsUncompressed(pol, sec.sectionID, foutALL, foutF);
					break;
				case 1:
					//compress
					compressLinestringCells(pol);
					//and save
					saveBinaryIntervalsCompressed(pol, sec.sectionID, foutALL, foutF);
					break;
			}
		}
		lineCounter++;
	}

	std::cout << "  Rasterization time: " << rasterizationTime << endl;
	std::cout << "  Intervalization time: " << intervalizationTime << " seconds." << endl;

	fin.close();
	foutALL.close();
	foutF.close();
}


/*
*-------------------------------------------------------
*
*     MAIN APPROXIMATION
*       
*
*-------------------------------------------------------
*/

void createApproximations(string argument, int flag){
	if(flag == 0 || DATA_TYPE == POLYGON_TYPE){
		//polygons
		std::cout << "***************************************************" << endl;
		std::cout << "Creating APRIL approximation for polygon dataset " << argument << endl;
		clock_t timer;
		timer = clock();
		computeIntervalsPolygons(argument, flag);
		std::cout << fixed << setprecision(6) << "Finished in " << (clock()-timer) / (double)(CLOCKS_PER_SEC) << " seconds." << endl;
		std::cout << "***************************************************" << endl;
	}else if(flag == 1 && DATA_TYPE == LINESTRING_TYPE){
		//linestrings
		std::cout << "***************************************************" << endl;
		std::cout << "Creating APRIL approximation for linestring dataset " << argument << endl;
		clock_t timer;
		timer = clock();
		computeIntervalsLinestrings(argument, flag);
		std::cout << fixed << setprecision(6) << "Finished in " << (clock()-timer) / (double)(CLOCKS_PER_SEC) << " seconds." << endl;
		std::cout << "***************************************************" << endl;
	}	
}

void loadApproximations(Dataset &dataset, string argument, int flag){
	clock_t timer;
	timer = clock();
	//  load data
	// std::cout << "Loading Raster Intervals for " << argument << "..." << endl;
	switch(COMPRESSION){
		case 0:
			loadAprilUncompressed(dataset, argument, flag);
			break;
		case 1:
			loadAprilCompressed(dataset, argument, flag);
			break;
	}
	// std::cout << fixed << setprecision(6) << "Finished in " << (clock()-timer) / (double)(CLOCKS_PER_SEC) << " seconds" << endl;		
}
