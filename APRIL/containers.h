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
#include <unordered_map>
#include <inttypes.h>


#ifdef __APPLE__

        typedef unsigned long int ulong;
        typedef unsigned short int ushort;
        typedef unsigned int uint;
#endif
        
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/assign.hpp>
#include <boost/foreach.hpp>
#include <boost/assign.hpp>

typedef boost::geometry::model::d2::point_xy<double> point_xy;
typedef boost::geometry::model::linestring<point_xy> linestring;

typedef boost::geometry::model::polygon<boost::geometry::model::d2::point_xy<double> > polygon;


using namespace std;

extern unsigned long total_pip_tests;

//argument flags
extern int INTERMEDIATE_FILTER;
extern int REFINEMENT;
extern int CALCULATE_INTERVALS;
extern int COMPRESSION;
extern int EXPERIMENTS;
extern int DIFF_GRANULARITY_FIXED;
extern int SELECTION_QUERY;
extern int WITHIN;
extern int GALLOPING;

extern string selection_query_filename;
extern uint DESIGNATED_ORDER;


//CONSTANTS 
#define EMPTY_COLOR 0
#define PARTIAL_COLOR 1
#define FULL_COLOR 2
#define UNCERTAIN_COLOR 3
#define FULL_CHECKED 4

#define POLYGON_TYPE 0
#define LINESTRING_TYPE 1
extern uint DATA_TYPE;

extern int HILBERT_POWER;
extern uint HILBERT_n;

// #define E 1e-08	//error margin
#define DECIMAL_POINTS_PRECISION 6

extern uint H;

#if HILBERT_n > 65536
	typedef unsigned long ID;
	typedef uint64_t CONTAINER;
#else
	typedef uint ID;
	typedef uint32_t CONTAINER;
#endif

//GLOBALS 
extern double GRANULARITY_RATIO_THRESHOLD;
extern uint MAXIMUM_CELLS_PER_POLYGON;
extern double mbrToPolygonPercentage;

extern int VAR_TYPE;	//0 = uint, 1 = unsigned long

extern unsigned long refinementCandidatesR;
extern unsigned long refinementCandidatesS;

//universal coordinates of the 2 datasets
extern double universalMinX,universalMinY,universalMaxX,universalMaxY;


#define BYTE_TO_BINARY_PATTERN "%c%c%c%c%c%c%c%c"
#define BYTE_TO_BINARY(byte)  \
  (byte & 0x80 ? '1' : '0'), \
  (byte & 0x40 ? '1' : '0'), \
  (byte & 0x20 ? '1' : '0'), \
  (byte & 0x10 ? '1' : '0'), \
  (byte & 0x08 ? '1' : '0'), \
  (byte & 0x04 ? '1' : '0'), \
  (byte & 0x02 ? '1' : '0'), \
  (byte & 0x01 ? '1' : '0') 

/*
*-------------------------------------------------------
*
*     FUNCTIONS
*       
*
*-------------------------------------------------------
*/

/* HILBERT FUNCTIONS */
// rotate/flip a quadrant appropriately
extern void rot(ID n, ID &x, ID &y, ID rx, ID ry);
// convert (x,y) to d
extern ID xy2d (ID n, ID x, ID y);
// convert d to (x,y)
extern void d2xy(ID n, ID d, ID &x, ID &y);
/* ---------------- */


extern void setIDtype();
extern void printContainer(uint8_t *container, uint &totalBytes);

/*
*-------------------------------------------------------
*
*     CLASSES
*       
*
*-------------------------------------------------------
*/

class Point{
public:
	double x, y;
	Point(double x, double y);
	Point();
	~Point();

	bool operator< (const Point &other) const;

    bool operator== (const Point &other) const;
	double to_angle(Point &o);
};

class MBR{
public:
	Point pMin;
	Point pMax;
	MBR(double xS, double yS, double xE, double yE);
	void set(double xS, double yS, double xE, double yE);
	MBR();
	~MBR();
	double getArea();
};

class Cell{
public:
	uint hilbertID;
	int classificationID;	//type 1 = partial, type 2 = full
	Point bottomLeft;
	Point topRight;

	Cell();

	//generates top right automatically
	Cell(double bottomLeftX, double bottomLeftY, int classificationID, uint hilbertID);

	Cell(double bottomLeftX, double bottomLeftY, double topRightX, double topRightY);
	Cell(double bottomLeftX, double bottomLeftY, double topRightX, double topRightY, int classificationID);
};


class Interval{
public:
	ID start;
	ID end;

	Interval(ID s, ID e);

	Interval();
	// inside your class
	bool operator <(const Interval& other);

};

class CellPackage
{
public:
	int cellType;
	vector<ID> hilbertCellIDs;
	uint cellCount;
	vector<Interval> intervals;

	CellPackage();
	~CellPackage();

	CellPackage(int cellType);

	void setType(int cellType);

	void addID(ID cellID);
	
};



class Polygon{
public:	
	//original mbr
	MBR mbr;

	//only these are needed for RI join, the rest are cleared
	uint recID;
	vector<Point> vertices;
	polygon boostPolygon;
	linestring boostLinestring;

	//Ν	
	uint orderN;

	//raster intervals
	uint numBytesALL;
	uint numBytesF;

	uint numIntervalsALL;
	uint numIntervalsF;

	//compression
	vector<uint8_t> compressedALL;
	vector<uint8_t> compressedF;
	uint8_t *compressedALLarray;
	uint8_t *compressedFarray;

	vector<ID> uncompressedALL;
	vector<ID> uncompressedF;

	//new rasterization 
	uint minHilbertX, minHilbertY, maxHilbertX, maxHilbertY;
	uint bufferWidth, bufferHeight;

	CellPackage partialCellPackage, fullCellPackage;

	bool F = false;

	Polygon(uint &recID);

	Polygon();
	~Polygon();

	void addPoint(Point &p);
};

class Dataset{
public:
	string filename;
	unordered_map<uint,Polygon> polygons;

	//key = section id, value = map of polygons
	unordered_map<uint, unordered_map<uint,Polygon>> sectionObjectsMap;
	
	Point pMin, pMax;
	string letterID;

	//for example T1, T2 etc...
	string argument;

	Dataset();

	Dataset(string &filename);

	Dataset(string letterID);

	Dataset(string argument, string letterID);
	Polygon* getPolygonByID(uint &recID);
	Polygon* getPolygonByIDAndSection(uint &sectionID, uint &recID);

};



class Section{
public:
	uint sectionID;
	//x and y axis position indexes
	uint x,y;

	//objects that intersect this MBR will be assigned to this area
	double interestxMin, interestyMin, interestxMax, interestyMax;
	double normInterestxMin, normInterestyMin, normInterestxMax, normInterestyMax;
	//this MBR defines the rasterization area (widened interest area to include intersecting polygons completely)
	double rasterxMin, rasteryMin, rasterxMax, rasteryMax;
	double normRasterxMin, normRasteryMin, normRasterxMax, normRasteryMax;

	double area;

	double xExtent, yExtent;

	//2^16 granularity cell area of this section in actual coordinates
	double cellArea;

	uint objectCountR, objectCountS;

	Section();
	~Section();

	Section(int &x, int &y);

	//if the parameters passed exceed the interest area, expand the raster area
	void expandRasterArea(double xMin, double yMin, double xMax, double yMax);

	double getArea();
};

class DataSpace{
public:
	double xMin, yMin, xMax, yMax;
	double xExtent, yExtent;
	double maxExtent;
	vector<Section> sections;
	uint totalObjects;
	//key = rec ID, value = vector of sections the rec intersects
	unordered_map<uint,vector<uint>> objectMapR;
	unordered_map<uint,vector<uint>> objectMapS;

	DataSpace();
	~DataSpace();

	void setUniversalCoordinates();

	void allocateAppropriateSectionsOfMBR(double xMin, double yMin, double xMax, double yMax, int flag);

	vector<Section> getSectionsOfMBR(double &xMin, double &yMin, double &xMax, double &yMax);

	vector<uint> getCommonSectionsIDOfObjects(uint &idA, uint &idB);

	//clears and resizes all sections in data space (based on defined H per dimension)
	void resize();
};

//DECLARE GLOBAL GEOMETRY DATASETS
extern Dataset geometryDatasetA;
extern Dataset geometryDatasetB;

extern DataSpace DATA_SPACE;