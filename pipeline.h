#ifndef PIPE_H
#define PIPE_H

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <iostream>
#include <limits>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>
#include <queue>
#include <fstream>
#include <future>
#include <unordered_set>
#include <unordered_map>
#include <chrono>
#include "omp.h"
#include <functional>
#include <array>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>

#include "./APRIL/april-main.h"

using namespace std;

int TOTAL_RESULTS = 0;

Dataset rasterIntervalsR;
Dataset rasterIntervalsS;

//geometry filenames
string geometryFileNameR;
string geometryFileNameS;
//offset maps for binary geometries
unordered_map<uint,unsigned long> offsetMapR;
unordered_map<uint,unsigned long> offsetMapS;
//binary geometry files
ifstream finR;
ifstream finS;

string argument1, argument2;

string result_filename = "results_pairs.csv";

double intermediateFilterTime = 0;
double refinementTime = 0;

int postMBRCandidates = 0;
int accepted = 0;
int rejected = 0;
int refinementCandidates = 0;
int acceptedAfterRefinement = 0;

clock_t timer;

int (*join_function_pointer)(Polygon *polA, Polygon *polB);
bool (*refinement_function)(uint &idA, uint &idB, unordered_map<uint,unsigned long> &offsetMapR, unordered_map<uint,unsigned long> &offsetMapS, ifstream &finR, ifstream &finS);


//-----------------------------
//
//          UTILITY
//
//-----------------------------

void resetMetricParameters(){
        intermediateFilterTime = 0;
        refinementTime = 0;
        postMBRCandidates = 0;
        accepted = 0;
        rejected = 0;
        refinementCandidates = 0;
        acceptedAfterRefinement = 0;
        TOTAL_RESULTS = 0;
        refinementCandidatesR = 0;
        refinementCandidatesS = 0;

}

void printSections(){
        cout << "DATA SPACE (" << DATA_SPACE.xMin << "," << DATA_SPACE.yMin << "),(" << DATA_SPACE.xMax << "," << DATA_SPACE.yMax << ") " << DATA_SPACE.sections.size() << " sections: " << endl;

        for(auto &it : DATA_SPACE.sections){
                cout << "       Section " << it.sectionID << " (" << it.x << "," << it.y << "): " << endl;
                cout << "               interest area: (" << it.interestxMin << "," << it.interestyMin << "),(" << it.interestxMax << "," << it.interestyMax << ")" << endl;
                cout << "               rasterin area: (" << it.rasterxMin << "," << it.rasteryMin << "),(" << it.rasterxMax << "," << it.rasteryMax << ")" << endl;
        }
}

void saveResultPair(uint &idA, uint &idB){
        ofstream fout(result_filename, fstream::out | ios_base::binary | fstream::app);

        fout << idA << " " << idB << endl; 

        fout.close();
}

//-----------------------------
//
//          INITIALIZE
//
//-----------------------------

void initialize(string &arg1, string &arg2){
        string info_message = "";
        argument1 = arg1;
        argument2 = arg2;

        //build file paths
        buildFilePaths(arg1, arg2);

        geometryFileNameR = getBinaryGeometryFilename(0);
        geometryFileNameS = getBinaryGeometryFilename(1);
        offsetMapR = loadOffsetMap(0);
        offsetMapS = loadOffsetMap(1);
        finR.open(geometryFileNameR, fstream::in | ios_base::binary);
        finS.open(geometryFileNameS, fstream::in | ios_base::binary);

        if(!finR){
                cout << "Error opening: " << geometryFileNameR << endl;
                exit(-1);
        }
        if(!finS){
                cout << "Error opening: " << geometryFileNameS << endl;
                exit(-1);
        }

        //set universal coordinates
        // cout << "Setting universal min/max..." << endl;

        if(SELECTION_QUERY && argument1.at(0) != 'T' && argument2.at(0) != 'T'){
                cout << "Error: Range query implemented only for TIGER datasets." << endl;
                exit(1);
        }else{
                //JOIN 
                if((argument1.at(0) == 'T' && argument2.at(0) == 'T') || SELECTION_QUERY){
                        getUniversalCoordinates(0);
                }else{
                        string continent = argument1.substr(argument1.find("_") + 1);
                        if(continent == "Oceania"){
                                getUniversalCoordinates(1);
                        }else if(continent == "Asia"){
                                getUniversalCoordinates(2);
                        }else if(continent == "Europe"){
                                getUniversalCoordinates(3);
                        }else if(continent == "NorthAmerica"){
                                getUniversalCoordinates(4);                        
                        }else if(continent == "Africa"){
                                getUniversalCoordinates(5);                        
                        }else if(continent == "SouthAmerica"){
                                getUniversalCoordinates(6);                        
                        }
                }
        }


        //resize the data space and create its sections
        DATA_SPACE.resize();
        //pass it to the data space object
        DATA_SPACE.setUniversalCoordinates();

        // cout << fixed << setprecision(6) << "Done: " << universalMinX << " " << universalMinY << "," << universalMaxX << " " << universalMaxY << endl;
        //return to begining
        finR.seekg(0, ios::beg);
        finS.seekg(0, ios::beg);

        // ofstream fout(result_filename, fstream::out | ios_base::binary);
        // fout.close();

        //create the data space sections
        for(int i=0; i<H; i++){
                for(int j=0; j<H; j++){
                        Section sec(i,j);
                        //cout << "created section " << sec.sectionID << " with interest area: " << sec.interestxMin << "," << sec.interestyMin << " and " << sec.interestxMax << "," << sec.interestyMax << endl; 
                        DATA_SPACE.sections.at(sec.sectionID) = sec;
                }
        }

        info_message += "-Datasets (R & S): \t\t" + argument1 + " & " + argument2 + "\n";
        info_message += "-Partitions: \t\t\t" + to_string(H) + "\n";

        //set the join function (based on whether it has to join compressed APRIL intervals)        
        if(COMPRESSION == 0){
                info_message += "-Compression: \t\t\tno\n";
                //UNCOMPRESSED APRIL
                if(DATA_TYPE == POLYGON_TYPE){
                        info_message += "-Data: \t\t\t\t\tpolygon-polygon\n";
                        if(WITHIN){
                                info_message += "-Query: \t\t\t\twithin join\n";
                                //POLYGON-POLYGON WITHIN JOIN
                                join_function_pointer = &joinPolygonsWithin_uncompressed;
                                refinement_function = &refinementWithinWithIDs;
                        }else{
                                if(SELECTION_QUERY){
                                        info_message += "-Query: \t\t\t\tselection\n";
                                }else{
                                        info_message += "-Query: \t\t\t\tintersection join\n";
                                }
                                //POLYGON-POLYGON INTERSECTION JOIN
                                if(DIFF_GRANULARITY_FIXED){
                                        //DIFFERENT GRANULARITY
                                        join_function_pointer = &joinPolygons_uncompressed_different_granularities;
                                        info_message += "-Granularity: \t\t\t16-" + to_string(DESIGNATED_ORDER) + "\n";
                                }else{
                                        //SAME GRANULARITY
                                        
                                        if(GALLOPING){
                                                //with galloping
                                                join_function_pointer = &joinPolygons_uncompressed_galloping;
                                                info_message += "-Galloping: \t\t\tyes\n";
                                        }else{
                                                //no galloping
                                                join_function_pointer = &joinPolygons_uncompressed;
                                                info_message += "-Galloping: \t\t\tno\n";
                                        }
                                        info_message += "-Granularity: \t\t\t16-16\n";
                                }                                
                                refinement_function = &refinementWithIDs;
                        }


                }else{
                        info_message += "-Data: \t\t\t\t\tpolygon-linestring\n";
                        info_message += "-Query: \t\t\tintersection join\n";                        
                        info_message += "-Granularity: \t\t\t16-16\n";
                        //POLYGON-LINESTRING INTERSECTION JOIN
                        join_function_pointer = &joinPolygons_uncompressed_linestrings;
                        refinement_function = &refinementWithIDsLinestring;
                }

        }else{
                info_message += "-Compression: \t\t\tyes\n";
                //COMPRESSED APRIL
                if(DATA_TYPE == POLYGON_TYPE){
                        info_message += "-Data: \t\t\t\t\tpolygon-polygon\n";
                        if(WITHIN){
                                info_message += "-Query: \t\t\t\twithin join\n";
                                //POLYGON-POLYGON WITHIN JOIN
                                join_function_pointer = &joinPolygonsWithin_compressed;
                                refinement_function = &refinementWithinWithIDs;
                        }else{
                                if(SELECTION_QUERY){
                                        info_message += "-Query: \t\t\t\tselection\n";
                                }else{
                                        info_message += "-Query: \t\t\t\tintersection join\n";
                                }
                                //POLYGON-POLYGON INTERSECTION JOIN
                                if(DIFF_GRANULARITY_FIXED){
                                        //DIFFERENT GRANULARITY
                                        join_function_pointer = &joinPolygons_compressed_different_granularities;
                                        info_message += "-Granularity: \t\t\t16-" + to_string(DESIGNATED_ORDER) + "\n";
                                }else{
                                        //SAME GRANULARITY
                                        join_function_pointer = &joinPolygons_compressed;
                                        info_message += "-Granularity: \t\t\t16-16\n";
                                }
                                refinement_function = &refinementWithIDs;                                
                        }
                }else{
                        info_message += "-Data: \t\t\t\t\tpolygon-linestring\n";
                        info_message += "-Query: \t\t\t\tintersection join\n";      
                        info_message += "-Granularity: \t\t\t16-16\n";
                        //POLYGON-LINESTRING INTERSECTION JOIN
                        join_function_pointer = &joinPolygons_compressed_linestrings;
                        refinement_function = &refinementWithIDsLinestring;
                }        
        }
        
        //set ID type
        setIDtype();

        //reset result file
        ofstream fout(result_filename, fstream::out | ios_base::binary);
        fout.close();

        //check if APRIL dirs exists
        DIR* dirUncompressed = opendir("APRIL/interval_data/uncompressed/");
        if(dirUncompressed) {
                /* Directory exists. */
                closedir(dirUncompressed);
        }else if(ENOENT == errno) {
                /* Directory does not exist. */
                mkdir("APRIL/interval_data/uncompressed/", 0700);
        }else{
                /* opendir() failed for some other reason. */
                cout << "Init error: Cannot open directory 'APRIL/interval_data/uncompressed/'" << endl;
                exit(-1);
        }
        DIR* dirCompressed = opendir("APRIL/interval_data/compressed/");
        if(dirCompressed) {
                /* Directory exists. */
                closedir(dirCompressed);
        }else if(ENOENT == errno) {
                /* Directory does not exist. */
                mkdir("APRIL/interval_data/compressed/", 0700);
        }else{
                /* opendir() failed for some other reason. */
                cout << "Init error: Cannot open directory 'APRIL/interval_data/compressed/'" << endl;
                exit(-1);
        }



        cout << "***************************************************" << endl;
        cout << "*************** APRIL configuration: **************" << endl;
        cout << info_message << "***************************************************" << endl;
        cout << "Pipeline: \n" << "MBR-join";
        if(INTERMEDIATE_FILTER){
                cout << " -> Intermediate Filter";
        }
        if(REFINEMENT){
                cout << " -> Refinement Stage";
        }
        cout << " -> Result" << endl;
        cout << "***************************************************" << endl << endl;

}

//-----------------------------
//
//          ENABLERS
//
//-----------------------------

void enableIntermediateFilter(string &argument1, string &argument2){        
        rasterIntervalsR.argument = argument1;
        rasterIntervalsR.letterID = "A";
        rasterIntervalsS.argument = argument2;
        rasterIntervalsS.letterID = "B";

        cout << "Loading datasets' APRIL... " << endl;
        loadApproximations(rasterIntervalsR, argument1, 0);
        loadApproximations(rasterIntervalsS, argument2, 1);
        cout << "Finished." << endl;
}


void initiateRasterIntervalsCreation(string &argument1, string &argument2){
        // initializeRasterizationOnOpenGL();
        createApproximations(argument1, 0);
        createApproximations(argument2, 1);
}

//-----------------------------
//
//          CONNECTORS
//
//-----------------------------

void forwardCandidatePair(uint idA, uint idB){
        bool refFlag = false;
        int result=2;
        postMBRCandidates++;
        if(INTERMEDIATE_FILTER){
                timer = clock();
                //find common sections
                vector<uint> commonSections = DATA_SPACE.getCommonSectionsIDOfObjects(idA, idB);
                //check in each common section
                for(auto &secID : commonSections){


                        // if(idA == 112853 && idB == 1872570){
                        // }
                        result = (*join_function_pointer)(rasterIntervalsR.getPolygonByIDAndSection(secID, idA), rasterIntervalsS.getPolygonByIDAndSection(secID, idB));

                        // if(idB == 1872570){
                        //         cout << idA << "," << idB << " result " << result << endl;
                        // }

                        //if hit
                        if(result == 1){
                                //accepted                        
                                TOTAL_RESULTS++;
                                accepted++;
                                // saveResultPair(idA, idB);
                                intermediateFilterTime += (clock() - timer) / (double) CLOCKS_PER_SEC;
                                return;
                        }else if(result == 2){
                                //mark for refinement
                                refFlag = true;
                        }
                }
                //if it isn't marked for refinement, reject
                if(!refFlag){
                        //else rejected     
                        rejected++;
                        intermediateFilterTime += (clock() - timer) / (double) CLOCKS_PER_SEC;
                        return;
                }                           
                intermediateFilterTime += (clock() - timer) / (double) CLOCKS_PER_SEC;
        }

        refinementCandidates++;  
        if(REFINEMENT){
                //needs refinement
                timer = clock();
                if((*refinement_function)(idA, idB, offsetMapR, offsetMapS, finR, finS)){
                        TOTAL_RESULTS++;
                        acceptedAfterRefinement++;
                        // saveResultPair(idA, idB);
                }
                refinementTime += (clock() - timer) / (double) CLOCKS_PER_SEC;
        }
}

#endif
