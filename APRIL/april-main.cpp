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
		// cout << "locate section " << (clock()-timer) / (double)(CLOCKS_PER_SEC) << endl;

		//rasterize separately for each section (if there are more than 1 partitions, otherwise only 1 section; the entire data space)
		for(int i = 0; i<sects.size(); i++){
			Section sec = sects.at(i);
			
			//create polygon
			Polygon pol(recID);
			pol.mbr = originalMBR;
			pol.vertices = originalVertices;
			pol.orderN = 16;

			//combined rasterization and intervalization with no flood filling
			timer = clock();
			rasterizeAndIntervalizeNoFloodFill(pol, sec);
			preprocessingTime += (clock()-timer) / (double)(CLOCKS_PER_SEC);

			// if(true){
			// 	//combined rasterization and intervalization with no flood filling
			// 	rasterizeAndIntervalizeNoFloodFill(pol, sec);
			// }else{
			// 	//---RASTERIZE POLYGON---
			// 	timer = clock();
			// 	if(DIFF_GRANULARITY_FIXED && (argument == "T3NA" || argument == "O6_Oceania")){
			// 		//---rasterize to fixed granularity (using the simple method, since granularity is lower than 16)---
			// 		//have defined specific order for all polygons of this dataset
			// 		pol.orderN = DESIGNATED_ORDER;
			// 		//simple rasterization
			// 		rasterizeSimple(pol, sec);
			// 	}else{
			// 		//---calculate the value of K (described in paper)---
			// 		// number of cells in the order 16 grid that are intersected by the object's MBR
			// 		C = MBRarea / sec.cellArea;
			// 		if(C <= 1500){
			// 			// cout << "formula1 fixed: 16 " << endl;
			// 		}else{
			// 			K = 16 - ceil(log2(MBRarea / (sec.cellArea * (double) 1500)) / 2);
			// 		}
			// 		//dont let it go under 8 (no point)
			// 		K = max(8,K);
			// 		//---Decide whether to rasterize using the simple or 2-grid algorithm---
			// 		if(K > 11){
			// 			//rasterize using the simple algorithm					
			// 			rasterizeSimple(pol, sec);
			// 		}else{					
			// 			//rasterize using the 2-grid rasterization algorithm
			// 			rasterize2grid(pol, sec, K);
			// 		}

			// 	}
			// 	rasterizationTime += (clock()-timer) / (double)(CLOCKS_PER_SEC);

			// 	//---INTERVALIZE POLYGON---
			// 	timer = clock();
			// 	intervalize(pol);
			// 	// cout << fixed << setprecision(14) << "Intervalization time: " << (clock()-timer) / (double)(CLOCKS_PER_SEC) << " seconds." << endl;
			// 	intervalizationTime += (clock()-timer) / (double)(CLOCKS_PER_SEC);

			// 	// cout << "ALL and F intervals total: " << pol.uncompressedALL.size()/2 << " and " << pol.uncompressedF.size()/2 << endl;
			// 	// //print partial
			// 	// cout << "PARTIAL" << endl;
			// 	// for(auto it = pol.uncompressedALL.begin(); it != pol.uncompressedALL.end(); it+=2){		
			// 	// 	cout << "[" << *it << "," << *(it+1) << ")" << endl;
			// 	// }
			// 	// //print full
			// 	// cout << "FULL" << endl;
			// 	// for(auto it = pol.uncompressedF.begin(); it != pol.uncompressedF.end(); it+=2){		
			// 	// 	cout << "[" << *it << "," << *(it+1) << ")" << endl;
			// 	// }

			// 	// exit(0);

			// }
			
			//---SAVE ON DISK---
			switch(COMPRESSION){
				case 0:
					saveBinaryIntervalsUncompressed(pol, sec.sectionID, foutALL, foutF);
					break;
				case 1:
					saveBinaryIntervalsCompressed(pol, sec.sectionID, foutALL, foutF);
					break;
			}		
		}


		lineCounter++;
	}

	// cout << "  Rasterization time: " << rasterizationTime << " sec." << endl;
	// cout << "  Intervalization time: " << intervalizationTime << " sec." << endl;

	// cout << "Pip tests per polygon: " << (double) total_pip_tests / totalPolygonCount << endl;
	// cout << "	partial cells time: " << partial_cell_time << " sec." << endl;
	// cout << "	intervalization time: " << intervalization_time << " sec." << endl;
	// cout << "		pip time: " << pip_time << " seconds." << endl;



	cout << "  Pre-processing time: " << preprocessingTime << " sec." << endl;

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

	cout << "  Rasterization time: " << rasterizationTime << endl;
	cout << "  Intervalization time: " << intervalizationTime << " seconds." << endl;

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
		cout << "***************************************************" << endl;
		cout << "Creating APRIL approximation for polygon dataset " << argument << endl;
		clock_t timer;
		timer = clock();
		computeIntervalsPolygons(argument, flag);
		cout << fixed << setprecision(6) << "Finished in " << (clock()-timer) / (double)(CLOCKS_PER_SEC) << " seconds." << endl;
		cout << "***************************************************" << endl;
	}else if(flag == 1 && DATA_TYPE == LINESTRING_TYPE){
		//linestrings
		cout << "***************************************************" << endl;
		cout << "Creating APRIL approximation for linestring dataset " << argument << endl;
		clock_t timer;
		timer = clock();
		computeIntervalsLinestrings(argument, flag);
		cout << fixed << setprecision(6) << "Finished in " << (clock()-timer) / (double)(CLOCKS_PER_SEC) << " seconds." << endl;
		cout << "***************************************************" << endl;
	}	
}

void loadApproximations(Dataset &dataset, string argument, int flag){
	clock_t timer;
	timer = clock();
	//  load data
	// cout << "Loading Raster Intervals for " << argument << "..." << endl;
	switch(COMPRESSION){
		case 0:
			loadAprilUncompressed(dataset, argument, flag);
			break;
		case 1:
			loadAprilCompressed(dataset, argument, flag);
			break;
	}
	// cout << fixed << setprecision(6) << "Finished in " << (clock()-timer) / (double)(CLOCKS_PER_SEC) << " seconds" << endl;		
}
