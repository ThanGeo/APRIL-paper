#include "join_geometry_refinement.h"

Polygon loadPolygonGeometry(uint &recID, unordered_map<uint,unsigned long> &offsetMap, ifstream &fin){
	Polygon pol(recID);
	int readID;
	int vertexCount, polygonCount;
	double x,y;

	double polxMin = numeric_limits<int>::max();
	double polyMin = numeric_limits<int>::max();
	double polxMax = -numeric_limits<int>::max();
	double polyMax = -numeric_limits<int>::max();

	//search the map for the specific polygon offset
	unordered_map<uint,unsigned long>::const_iterator got = offsetMap.find(recID);
	if(got != offsetMap.end()){
		//set read offset
		fin.seekg(got->second-fin.tellg(), fin.cur);		
		//read rec ID
		fin.read((char*) &readID, sizeof(int));
		//read vertex count
		fin.read((char*) &vertexCount, sizeof(int));

		pol.vertices.reserve(vertexCount);

		for(int i=0; i<vertexCount; i++){
			fin.read((char*) &x, sizeof(double));
			fin.read((char*) &y, sizeof(double));

			pol.vertices.emplace_back(x,y);

			//save polygon's min/max (for mbr)
			polxMin = min(polxMin, x);
			polyMin = min(polyMin, y);
			polxMax = max(polxMax, x);
			polyMax = max(polyMax, y);
		}

		//add MBR to polygon
		pol.mbr.pMin.x = polxMin;
		pol.mbr.pMin.y = polyMin;
		pol.mbr.pMax.x = polxMax;
		pol.mbr.pMax.y = polyMax;
	}

	return pol;
}

polygon loadPolygonGeometryBOOST(uint &recID, unordered_map<uint,unsigned long> &offsetMap, ifstream &fin){
	polygon pol;
	int readID;
	int vertexCount, polygonCount;
	double x,y;

	//search the map for the specific polygon offset
	unordered_map<uint,unsigned long>::const_iterator got = offsetMap.find(recID);
	if(got != offsetMap.end()){ 
		//set read offset
		fin.seekg(got->second-fin.tellg(), fin.cur);		
		//read rec ID
		fin.read((char*) &readID, sizeof(int));
		//read vertex count
		fin.read((char*) &vertexCount, sizeof(int));
		for(int i=0; i<vertexCount; i++){
			fin.read((char*) &x, sizeof(double));
			fin.read((char*) &y, sizeof(double));

			pol.outer().push_back(point_xy(x,y));
		}
	}

	boost::geometry::correct(pol);
	return pol;
}

linestring loadLinestringGeometryBOOST(uint &recID, unordered_map<uint,unsigned long> &offsetMap, ifstream &fin){
	linestring ls;
	int readID;
	int vertexCount;
	double x,y;

	//search the map for the specific polygon offset
	unordered_map<uint,unsigned long>::const_iterator got = offsetMap.find(recID);
	if(got != offsetMap.end()){ 
		//set read offset
		fin.seekg(got->second-fin.tellg(), fin.cur);
		//read rec ID
		fin.read((char*) &readID, sizeof(int));
		//read vertex count
		fin.read((char*) &vertexCount, sizeof(int));
		for(int i=0; i<vertexCount; i++){
			fin.read((char*) &x, sizeof(double));
			fin.read((char*) &y, sizeof(double));
			ls.emplace_back(x,y);
		}
	}
	return ls;
}


MBR getCMBR(Polygon &polA, Polygon &polB){
	MBR cmbr;
	cmbr.pMin.x = max(polA.mbr.pMin.x, polB.mbr.pMin.x);
	cmbr.pMin.y = max(polA.mbr.pMin.y, polB.mbr.pMin.y);
	cmbr.pMax.x = min(polA.mbr.pMax.x, polB.mbr.pMax.x);
	cmbr.pMax.y = min(polA.mbr.pMax.y, polB.mbr.pMax.y);
	return cmbr;
}

bool refinementWithIDsLinestring(uint &idA, uint &idB, unordered_map<uint,unsigned long> &offsetMapR, unordered_map<uint,unsigned long> &offsetMapS, ifstream &finR, ifstream &finS){
	/*  BOOST GEOMETRY REFINEMENT */
	polygon boostPolygonR = loadPolygonGeometryBOOST(idA, offsetMapR, finR);
	linestring boostLinestringS = loadLinestringGeometryBOOST(idB, offsetMapS, finS);
	refinementCandidatesR += boost::geometry::num_points(boostPolygonR);
	refinementCandidatesS += boost::geometry::num_points(boostLinestringS);

    return boost::geometry::intersects(boostPolygonR, boostLinestringS);
   
}

bool refinementWithIDs(uint &idA, uint &idB, unordered_map<uint,unsigned long> &offsetMapR, unordered_map<uint,unsigned long> &offsetMapS, ifstream &finR, ifstream &finS){
	/*  BOOST GEOMETRY REFINEMENT */
	polygon boostPolygonR = loadPolygonGeometryBOOST(idA, offsetMapR, finR);
	polygon boostPolygonS = loadPolygonGeometryBOOST(idB, offsetMapS, finS);
	refinementCandidatesR += boost::geometry::num_points(boostPolygonR);
	refinementCandidatesS += boost::geometry::num_points(boostPolygonS);

    return boost::geometry::intersects(boostPolygonR, boostPolygonS);
   
}

bool refinementWithinWithIDs(uint &idA, uint &idB, unordered_map<uint,unsigned long> &offsetMapR, unordered_map<uint,unsigned long> &offsetMapS, ifstream &finR, ifstream &finS){
	/*  BOOST GEOMETRY REFINEMENT */
	polygon boostPolygonR = loadPolygonGeometryBOOST(idA, offsetMapR, finR);
	polygon boostPolygonS = loadPolygonGeometryBOOST(idB, offsetMapS, finS);
	refinementCandidatesR += boost::geometry::num_points(boostPolygonR);
	refinementCandidatesS += boost::geometry::num_points(boostPolygonS);

    return boost::geometry::covered_by(boostPolygonR, boostPolygonS);
   
}