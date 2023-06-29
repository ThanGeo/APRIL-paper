#include "dataset_data.h"

string intervalsRfilepath_A;
string intervalsSfilepath_A;
string intervalsRfilepath_F;
string intervalsSfilepath_F;

string R_geometry_data;
string S_geometry_data;
string R_offset_map_data;
string S_offset_map_data;

void buildFilePaths(string &argument1, string &argument2){
	//INTERVAL DATA
	switch(COMPRESSION){
		case 0:
			intervalsRfilepath_A = "APRIL/interval_data/uncompressed/"+argument1+"_binary_interval_A_" + to_string(H) + ".dat";
			intervalsSfilepath_A = "APRIL/interval_data/uncompressed/"+argument2+"_binary_interval_A_" + to_string(H) + ".dat";
			intervalsRfilepath_F = "APRIL/interval_data/uncompressed/"+argument1+"_binary_interval_F_" + to_string(H) + ".dat";
			intervalsSfilepath_F = "APRIL/interval_data/uncompressed/"+argument2+"_binary_interval_F_" + to_string(H) + ".dat";			
			break;
		case 1:
			intervalsRfilepath_A = "APRIL/interval_data/compressed/"+argument1+"_binary_interval_A_" + to_string(H) + ".dat";
			intervalsSfilepath_A = "APRIL/interval_data/compressed/"+argument2+"_binary_interval_A_" + to_string(H) + ".dat";
			intervalsRfilepath_F = "APRIL/interval_data/compressed/"+argument1+"_binary_interval_F_" + to_string(H) + ".dat";
			intervalsSfilepath_F = "APRIL/interval_data/compressed/"+argument2+"_binary_interval_F_" + to_string(H) + ".dat";
			break;
	}

	//GEOMETRY DATA
	if(argument1.at(0) == 'T' && argument2.at(0) == 'T'){
		//TIGER JOIN
		R_geometry_data = "./data/" + argument1 + "_fixed_binary.dat";
		S_geometry_data = "./data/" + argument2 + "_fixed_binary.dat";
		R_offset_map_data = "./data/" + argument1 + "_offset_map.dat";
		S_offset_map_data = "./data/" + argument2 + "_offset_map.dat";
	}else if((argument1.at(0) == 'Q' || argument2.at(0) == 'Q') && SELECTION_QUERY){
		//TIGER RANGE QUERY
		R_geometry_data = "./data/" + argument1 + "_fixed_binary.dat";
		S_geometry_data = "./data/" + argument2 + "_fixed_binary.dat";
		R_offset_map_data = "./data/" + argument1 + "_offset_map.dat";
		S_offset_map_data = "./data/" + argument2 + "_offset_map.dat";
	}else if(argument1.at(0) == 'O' && argument1.at(2) == '_' && argument2.at(0) == 'O' && argument2.at(2) == '_'){
		//OSM CONTINENTS
		R_geometry_data = "./data/" + argument1 + "_fixed.dat";
		S_geometry_data = "./data/" + argument2 + "_fixed.dat";
		R_offset_map_data = "./data/" + argument1+ "_offset_map.dat";
		S_offset_map_data = "./data/" + argument2+ "_offset_map.dat";
	}else{
		cout << "Error: No dataset data meta-mapping with specified input files: " << argument1 << " and " << argument2 << endl;
		exit(0);
	}

	// cout << "File paths:" << endl;
	// cout << "\tinterval files: " << endl;
	// cout << "\t" << intervalsRfilepath_A << endl;
	// cout << "\t" << intervalsSfilepath_A << endl;
	// cout << "\t" << intervalsRfilepath_F << endl;
	// cout << "\t" << intervalsSfilepath_F << endl;
	// cout << "\tinput files: " << endl;
	// cout << "\t" << R_geometry_data << endl;
	// cout << "\t" << S_geometry_data << endl;

}


/* geometry files */
string getBinaryGeometryFilename(int flag){
	switch(flag){
		case 0:		//R
			return R_geometry_data;
			break;
		case 1:		//S
			return S_geometry_data;
			break;
	}
}

/* geometry offset map */
string getOffsetMap(int flag){
	switch(flag){
		case 0:		//R
			return R_offset_map_data;
			break;
		case 1:		//S
			return S_offset_map_data;
			break;
	}
}

/* interval files */
string getIntervalALLBinaryFilename(int flag){
	switch(flag){
		case 0:		//R
			return intervalsRfilepath_A;
			break;
		case 1:		//S
			return intervalsSfilepath_A;
			break;
	}
}

string getIntervalFBinaryFilename(int flag){	
	switch(flag){
		case 0:
			return intervalsRfilepath_F;
			break;
		case 1:
			return intervalsSfilepath_F;
			break;
	}
}

//fixed universal coordinates per data set 
void getUniversalCoordinates(int setcode){
	switch(setcode){
		case 0:
			//TIGER DATASET, SET FIXED COORDINATES OF TIGER NORTHA AMERICA low48
			universalMinX = -124.849;
		    universalMinY = 24.5214;
		    universalMaxX = -66.8854;
		    universalMaxY = 49.3844;
			break;
		case 1:
			//OCEANIA
			universalMinX = 112.6;
		    universalMinY = -51.11;
		    universalMaxX = 180.5;
		    universalMaxY = 13.4;
			break;
		case 2:
			//ASIA
			universalMinX = 43.9999;
		    universalMinY = 5.771669;
		    universalMaxX = 145.36601;
		    universalMaxY = 82.50001;
			break;
		case 3:
			//EUROPE
			universalMinX = -10.594201;
		    universalMinY = 34.761799;
		    universalMaxX = 45.893501;
		    universalMaxY = 71.170701;
			break;
		case 4:
			//NORTH AMERICA
			universalMinX = -127.698001;
		    universalMinY = 12.443799;
		    universalMaxX = -54.002399;
		    universalMaxY = 60.686401;
			break;
		case 5:
			//AFRICA
			universalMinX = -18.138101;
		    universalMinY = -34.785201;
		    universalMaxX = 51.24801;
		    universalMaxY = 38.339401;
			break;
		case 6:
			//SOUTH AMERICA
			universalMinX = -87.361101;
		    universalMinY = -56.500601;
		    universalMaxX = -34.78799;
		    universalMaxY = 12.878401;
			break;

		}
}
