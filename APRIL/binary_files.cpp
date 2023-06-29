#include "binary_files.h"

uint8_t loadSpace[10000000];
ID loadSpace32[10000000];

/*
*-------------------------------------------------------
*
*     GEOMETRY BINARY DATA
*       
*
*-------------------------------------------------------
*/

//offset map
unordered_map<uint,unsigned long> loadOffsetMap(int flag){
	unsigned long offset;
	uint lineCounter = 0;

	uint recID;

	ifstream fin(getOffsetMap(flag), fstream::out | ios_base::binary);

	unordered_map<uint,unsigned long> offset_map;

	int totalLines;

	//read total lines
	fin.read((char*) &totalLines, sizeof(int));

	while(lineCounter < totalLines){
		//read rec id
		fin.read((char*) &recID, sizeof(int));
		//read byte offset
		fin.read((char*) &offset, sizeof(unsigned long));

		offset_map.insert(make_pair(recID, offset));		
		lineCounter++;
	}
	

	fin.close();

	return offset_map;
}


/*
*-------------------------------------------------------
*
*     INTERVAL BINARY DATA
*       
*
*-------------------------------------------------------
*/

void saveBinaryIntervalsUncompressed(Polygon &pol, uint &sectionID, ofstream &foutALL, ofstream &foutF){
	//write ID
	//cout << "Polygon " << pol.recID << " ALL intervals: " << pol.numIntervalsALL << " (" << pol.numBytesALL << " bytes)" << endl; 
	foutALL.write((char*)(&pol.recID), sizeof(uint));
	//write section
	foutALL.write((char*)(&sectionID), sizeof(uint));
	//write number of intervals
	foutALL.write((char*)(&pol.numIntervalsALL), sizeof(uint));	
	//write number of elements in vector
	foutALL.write((char*)(&pol.numBytesALL), sizeof(uint));
	//write the uncompressed data
	foutALL.write((char*)(&pol.uncompressedALL.data()[0]), pol.numBytesALL * sizeof(ID));	

	if(pol.numIntervalsF > 0){
		foutF.write((char*)(&pol.recID), sizeof(uint));
		foutF.write((char*)(&sectionID), sizeof(uint));
		foutF.write((char*)(&pol.numIntervalsF), sizeof(uint));
		foutF.write((char*)(&pol.numBytesF), sizeof(uint));
		foutF.write((char*)(&pol.uncompressedF.data()[0]), pol.numBytesF * sizeof(ID));
	}
}

void saveBinaryIntervalsCompressed(Polygon &pol, uint &sectionID, ofstream &foutALL, ofstream &foutF){
	//write ID
	//cout << "Polygon " << pol.recID << " ALL intervals: " << pol.numIntervalsALL << " (" << pol.numBytesALL << " bytes)" << endl; 
	foutALL.write((char*)(&pol.recID), sizeof(uint));
	//write section
	foutALL.write((char*)(&sectionID), sizeof(uint));
	//write number of intervals
	foutALL.write((char*)(&pol.numIntervalsALL), sizeof(uint));	
	//write number of bytes of the compressed intervals
	foutALL.write((char*)(&pol.numBytesALL), sizeof(uint));
	//write the compressed data
	foutALL.write((char*)(&pol.compressedALL.data()[0]), pol.numBytesALL);	

	if(pol.numIntervalsF > 0){
		foutF.write((char*)(&pol.recID), sizeof(uint));
		foutF.write((char*)(&sectionID), sizeof(uint));
		foutF.write((char*)(&pol.numIntervalsF), sizeof(uint));
		foutF.write((char*)(&pol.numBytesF), sizeof(uint));
		foutF.write((char*)(&pol.compressedF.data()[0]), pol.numBytesF);
	}
}

void loadAprilCompressed(Dataset &set, string argument, int flag){
	uint totalPolygons, recID, fileH, sectionID;
	uint8_t szbyte, bytesUsedForStart, bytesUsedForDiff;
	uint firstStart;
	uint numIntervals;
	long lSize;
	uint8_t* buffer;
	size_t result;

	uint8_t temp;
	uint readOrder;

	FILE* pFile = fopen(getIntervalALLBinaryFilename(flag).c_str(), "rb");

	/* ---------- ALL ---------- */
	//read each one individually into memory
	if(pFile == NULL) {cout << "File error: " + getIntervalALLBinaryFilename(flag) << endl; exit (1);}
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);
	// allocate memory to contain the whole file:
	buffer = (uint8_t*) malloc (sizeof(uint8_t)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	//init read indices
	long bufferIndex = 0;
	long loadIndex;

	
	//read H
	memcpy(&fileH, &buffer[0], sizeof(uint));
	bufferIndex += sizeof(uint);
	if(fileH != H){
		cout << "file error: Read H = " << fileH << " but program has specified H = " << H << endl; 
	}

	//read order of dataset (if designated)
	if(DIFF_GRANULARITY_FIXED && (argument == "T3NA" || argument == "O6_Oceania")){
		memcpy(&readOrder, &buffer[0], sizeof(uint));
		bufferIndex += sizeof(uint);
		if(readOrder != DESIGNATED_ORDER){
			cout << "DESIGNATED_ORDER = " << DESIGNATED_ORDER << " but read order " << readOrder << endl;
		}
	}


	//read total polygons
	memcpy(&totalPolygons, &buffer[bufferIndex], sizeof(uint));
	bufferIndex += sizeof(uint);

	
	//read loop
	uint polCounter = 0;
	while(bufferIndex < lSize){

		loadIndex = 0;
		//id
		memcpy(&recID, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);
		Polygon pol(recID);

		//section
		memcpy(&sectionID, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);

		//set objects N if designated
		if(DIFF_GRANULARITY_FIXED){
			if(argument == "T3NA" || argument == "O6_Oceania"){
				pol.orderN = DESIGNATED_ORDER;
			}else{
				pol.orderN = 16;
			}
		}
		
		//total intervals in polygon
		memcpy(&pol.numIntervalsALL, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);

		//total bytes for the compressed data
		memcpy(&pol.numBytesALL, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);

		//copy compressed data
		memcpy(&loadSpace[loadIndex], &buffer[bufferIndex], pol.numBytesALL * sizeof(uint8_t));
		loadIndex += pol.numBytesALL;
		bufferIndex += pol.numBytesALL;
		

		//add to polygon
		pol.compressedALLarray = (uint8_t*) malloc(loadIndex * sizeof(uint8_t));
		memcpy(&pol.compressedALLarray[0], &loadSpace[0], loadIndex * sizeof(uint8_t));


		//add to data space's object map
		switch(flag){
			case 0:
				{
					//R
					auto itR = DATA_SPACE.objectMapR.find(recID);
					if(itR != DATA_SPACE.objectMapR.end()){
						//already exists
						itR->second.push_back(sectionID);
					}else{
						//create new entry
						vector<uint> vec = {sectionID};
						DATA_SPACE.objectMapR.insert(make_pair(recID, vec));
					}
					break;
				}
			case 1:
				{
					//S
					auto itS = DATA_SPACE.objectMapS.find(recID);
					if(itS != DATA_SPACE.objectMapS.end()){
						//already exists
						itS->second.push_back(sectionID);
					}else{
						//create new entry
						vector<uint> vec = {sectionID};
						DATA_SPACE.objectMapS.insert(make_pair(recID, vec));
					}
					break;
				}
		}
		//add to set
		auto secIT = set.sectionObjectsMap.find(sectionID);
		if(secIT != set.sectionObjectsMap.end()){
			secIT->second.insert(make_pair(pol.recID, pol));
		}else{
			//add to polygon map of this section
			unordered_map<uint,Polygon> polygons = {make_pair(pol.recID, pol)};
			set.sectionObjectsMap.insert(make_pair(sectionID, polygons));
		}

		//cout << "Loaded: polygon " << recID << ", section " << sectionID << endl; 

		polCounter++;
	}
	// terminate this file
	fclose (pFile);
	
	//free memory
	free (buffer);

	/* ---------- F ---------- */
	pFile = fopen(getIntervalFBinaryFilename(flag).c_str(), "rb");
	if(pFile == NULL) {cout << "File error: " + getIntervalFBinaryFilename(flag) + ", section " << sectionID << endl; exit (1);}
	//for each file
	unordered_map<uint,Polygon> polygons;

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);
	// allocate memory to contain the whole file:
	buffer = (uint8_t*) malloc (sizeof(uint8_t)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	//init read indices
	bufferIndex = 0;
	//read H
	memcpy(&fileH, &buffer[0], sizeof(uint));
	bufferIndex += sizeof(uint);
	if(fileH != H){
		cout << "file error: Read H = " << fileH << " but program has specified H = " << H << endl; 
	}
	
	//read order of dataset (if designated)
	if(DIFF_GRANULARITY_FIXED && (argument == "T3NA" || argument == "O6_Oceania")){
		memcpy(&readOrder, &buffer[0], sizeof(uint));
		bufferIndex += sizeof(uint);
		if(readOrder != DESIGNATED_ORDER){
			cout << "DESIGNATED_ORDER = " << DESIGNATED_ORDER << " but read order " << readOrder << endl;
		}
	}

	//read total polygons
	memcpy(&totalPolygons, &buffer[bufferIndex], sizeof(uint));
	bufferIndex += sizeof(uint);

	//read loop
	polCounter = 0;
	
	while(bufferIndex < lSize){
		loadIndex = 0;
		//id
		memcpy(&recID, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);		

		//section
		memcpy(&sectionID, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);	

		//retrieve the polygon
		Polygon *pol = set.getPolygonByIDAndSection(sectionID, recID);

		//set objects N if designated
		if(DIFF_GRANULARITY_FIXED){
			if(argument == "T3NA" || argument == "O6_Oceania"){
				pol->orderN = DESIGNATED_ORDER;
			}else{
				pol->orderN = 16;
			}
		}

		//total intervals in polygon
		memcpy(&numIntervals, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);
		pol->numIntervalsF = numIntervals;

		//total bytes for the compressed data
		memcpy(&pol->numBytesF, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);

		//copy diffs
		memcpy(&loadSpace[loadIndex], &buffer[bufferIndex], pol->numBytesF);
		loadIndex += pol->numBytesF;
		bufferIndex += pol->numBytesF;

		//add to polygon
		pol->compressedFarray = (uint8_t*) malloc(loadIndex * sizeof(uint8_t));
		memcpy(&pol->compressedFarray[0], &loadSpace[0], loadIndex * sizeof(uint8_t));
		pol->F = true;
		//pol->compressedF.insert(pol->compressedF.begin(), &loadSpace[0], loadSpace + loadIndex);

		polCounter++;
	}

	// terminate this file
	fclose (pFile);
	
	//free memory
	free (buffer);
}

void loadAprilUncompressed(Dataset &set, string argument, int flag){
	uint totalPolygons, recID, fileH, sectionID;
	uint8_t szbyte, bytesUsedForStart, bytesUsedForDiff;
	uint firstStart;
	uint numIntervals;
	long lSize;
	uint8_t* buffer;
	size_t result;
	uint8_t temp;

	uint readOrder;

	FILE* pFile = fopen(getIntervalALLBinaryFilename(flag).c_str(), "rb");

	/* ---------- ALL ---------- */
	//read each one individually into memory
	if(pFile == NULL) {cout << "File error: " + getIntervalALLBinaryFilename(flag) << endl; exit (1);}
	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);
	// allocate memory to contain the whole file:
	buffer = (uint8_t*) malloc (sizeof(uint8_t)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	//init read indices
	long bufferIndex = 0;
	long loadIndex;

	//read H
	memcpy(&fileH, &buffer[0], sizeof(uint));
	bufferIndex += sizeof(uint);
	if(fileH != H){
		cout << "file error: Read H = " << fileH << " but program has specified H = " << H << endl; 
	}

	//read order of dataset (if designated)
	if(DIFF_GRANULARITY_FIXED && (argument == "T3NA" || argument == "O6_Oceania")){
		memcpy(&readOrder, &buffer[0], sizeof(uint));
		bufferIndex += sizeof(uint);
		if(readOrder != DESIGNATED_ORDER){
			cout << "DESIGNATED_ORDER = " << DESIGNATED_ORDER << " but read order " << readOrder << endl;
		}
	}

	//read total polygons
	memcpy(&totalPolygons, &buffer[bufferIndex], sizeof(uint));
	bufferIndex += sizeof(uint);

	//read loop
	uint polCounter = 0;
	while(bufferIndex < lSize){
		loadIndex = 0;
		//id
		memcpy(&recID, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);
		Polygon pol(recID);

		//section
		memcpy(&sectionID, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);

		//set objects N if designated
		if(DIFF_GRANULARITY_FIXED){
			if(argument == "T3NA" || argument == "O6_Oceania"){
				pol.orderN = DESIGNATED_ORDER;
				// cout << "pol " << pol.recID << " set order " << pol.orderN << endl;
			}else{
				pol.orderN = 16;
			}
		}
		
		//total intervals in polygon
		memcpy(&pol.numIntervalsALL, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);

		//total bytes for the compressed data
		memcpy(&pol.numBytesALL, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);

		//copy uncompressed data
		memcpy(&loadSpace32[loadIndex], &buffer[bufferIndex], pol.numBytesALL * sizeof(uint32_t));
		loadIndex += pol.numBytesALL;
		bufferIndex += pol.numBytesALL*sizeof(uint32_t);		

		//add to polygon
		pol.uncompressedALL.insert(pol.uncompressedALL.begin(), &loadSpace32[0], loadSpace32 + loadIndex);

		//add to data space's object map
		switch(flag){
			case 0:
				{
					//R
					auto itR = DATA_SPACE.objectMapR.find(recID);
					if(itR != DATA_SPACE.objectMapR.end()){
						//already exists
						itR->second.push_back(sectionID);
					}else{
						//create new entry
						vector<uint> vec = {sectionID};
						DATA_SPACE.objectMapR.insert(make_pair(recID, vec));
					}
					break;
				}
			case 1:
				{
					//S
					auto itS = DATA_SPACE.objectMapS.find(recID);
					if(itS != DATA_SPACE.objectMapS.end()){
						//already exists
						itS->second.push_back(sectionID);
					}else{
						//create new entry
						vector<uint> vec = {sectionID};
						DATA_SPACE.objectMapS.insert(make_pair(recID, vec));
					}
					break;
				}
		}
		//add to set
		auto secIT = set.sectionObjectsMap.find(sectionID);
		if(secIT != set.sectionObjectsMap.end()){
			secIT->second.insert(make_pair(pol.recID, pol));
		}else{
			//add to polygon map of this section
			unordered_map<uint,Polygon> polygons = {make_pair(pol.recID, pol)};
			set.sectionObjectsMap.insert(make_pair(sectionID, polygons));
		}
		polCounter++;
	}
	// terminate this file
	fclose (pFile);
	
	//free memory
	free (buffer);

	/* ---------- F ---------- */
	pFile = fopen(getIntervalFBinaryFilename(flag).c_str(), "rb");
	if(pFile == NULL) {cout << "File error: " + getIntervalFBinaryFilename(flag) + ", section " << sectionID << endl; exit (1);}
	//for each file
	unordered_map<uint,Polygon> polygons;

	// obtain file size:
	fseek (pFile , 0 , SEEK_END);
	lSize = ftell (pFile);
	rewind (pFile);
	// allocate memory to contain the whole file:
	buffer = (uint8_t*) malloc (sizeof(uint8_t)*lSize);
	if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}
	// copy the file into the buffer:
	result = fread (buffer,1,lSize,pFile);
	if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
	//init read indices
	bufferIndex = 0;
	//read H
	memcpy(&fileH, &buffer[0], sizeof(uint));
	bufferIndex += sizeof(uint);
	if(fileH != H){
		cout << "file error: Read H = " << fileH << " but program has specified H = " << H << endl; 
	}

	//read order of dataset (if designated)
	if(DIFF_GRANULARITY_FIXED && (argument == "T3NA" || argument == "O6_Oceania")){
		memcpy(&readOrder, &buffer[0], sizeof(uint));
		bufferIndex += sizeof(uint);
		if(readOrder != DESIGNATED_ORDER){
			cout << "DESIGNATED_ORDER = " << DESIGNATED_ORDER << " but read order " << readOrder << endl;
		}
	}

	//read total polygons
	memcpy(&totalPolygons, &buffer[bufferIndex], sizeof(uint));
	bufferIndex += sizeof(uint);

	//read loop
	polCounter = 0;
	
	while(bufferIndex < lSize){
		loadIndex = 0;
		//id
		memcpy(&recID, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);		

		//section
		memcpy(&sectionID, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);	

		//retrieve the polygon
		Polygon *pol = set.getPolygonByIDAndSection(sectionID, recID);

		//set objects N if designated
		if(DIFF_GRANULARITY_FIXED){
			if(argument == "T3NA" || argument == "O6_Oceania"){
				pol->orderN = DESIGNATED_ORDER;
			}else{
				pol->orderN = 16;
			}
		}

		//total intervals in polygon
		memcpy(&numIntervals, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);
		pol->numIntervalsF = numIntervals;

		//total bytes for the uncompressed data
		memcpy(&pol->numBytesF, &buffer[bufferIndex], sizeof(uint));
		bufferIndex += sizeof(uint);

		//copy uncompressed data
		memcpy(&loadSpace32[loadIndex], &buffer[bufferIndex], pol->numBytesF * sizeof(uint32_t));
		loadIndex += pol->numBytesF;
		bufferIndex += pol->numBytesF*sizeof(uint32_t);		

		//add to polygon
		pol->uncompressedF.insert(pol->uncompressedF.begin(), &loadSpace32[0], loadSpace32 + loadIndex);
		pol->F = true;

		polCounter++;
	}

	// terminate this file
	fclose (pFile);
	
	//free memory
	free (buffer);
}