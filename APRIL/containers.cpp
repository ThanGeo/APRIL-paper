#include "containers.h"

unsigned long refinementCandidatesR = 0;
unsigned long refinementCandidatesS = 0;

unsigned long total_pip_tests = 0;

//argument flags
int INTERMEDIATE_FILTER = 0;
int REFINEMENT = 0;
int CALCULATE_INTERVALS = 0;
int COMPRESSION = 0;
int EXPERIMENTS = 0;
int DIFF_GRANULARITY_FIXED = 0;
int SELECTION_QUERY = 0;
int WITHIN = 0;
int GALLOPING = 0;

uint DESIGNATED_ORDER;
string selection_query_filename;


uint DATA_TYPE = POLYGON_TYPE;


//universal coordinates of the 2 datasets
double universalMinX = std::numeric_limits<double>::max();
double universalMinY = std::numeric_limits<double>::max();
double universalMaxX = -std::numeric_limits<double>::max();
double universalMaxY = -std::numeric_limits<double>::max();

//DECLARE GLOBAL GEOMETRY DATASETS
Dataset geometryDatasetA("A");
Dataset geometryDatasetB("B");

int HILBERT_POWER=16;
uint HILBERT_n = pow(2,HILBERT_POWER);

//the sections vector
DataSpace DATA_SPACE;
uint H = 1;
double GRANULARITY_RATIO_THRESHOLD = 100000;
uint MAXIMUM_CELLS_PER_POLYGON = 1024;
double mbrToPolygonPercentage = 0.5;

int VAR_TYPE;


void setIDtype(){
	if(HILBERT_n > 65536){
		//typedef unsigned long ID;
		//typedef uint64_t CONTAINER;
		VAR_TYPE = 1;
		cout << "N = " << log2(HILBERT_n) << ", ID set to ulong" << endl;
	}else{
		//typedef uint ID;
		//typedef uint32_t CONTAINER;
		VAR_TYPE = 0;
		cout << "N = " << log2(HILBERT_n) << ", ID set to uint" << endl;
	}
}


void printContainer(uint8_t *container, uint &totalBytes){
	cout << "CONTAINER:" << endl;
	for(int i = 0; i<totalBytes; i++){
		printf(""BYTE_TO_BINARY_PATTERN" ",BYTE_TO_BINARY(container[i]));
		if((i+1) % 4 == 0){

			cout << endl;
		}
	}
	cout << endl;
}



// rotate/flip a quadrant appropriately
inline void rot(ID n, ID &x, ID &y, ID rx, ID ry) {
    if (ry == 0) {
        if (rx == 1) {
            x = n-1 - x;
            y = n-1 - y;
        }

        //Swap x and y
        ID t  = x;
        x = y;
        y = t;
    }
}

// convert (x,y) to d
ID xy2d (ID n, ID x, ID y) {
    ID rx, ry, s;
    ID d=0;
    for (s=n/2; s>0; s/=2) {
        rx = (x & s) > 0;
        ry = (y & s) > 0;
        d += s * s * ((3 * rx) ^ ry);
        rot(s, x, y, rx, ry);
    }
    return d;
}

// convert d to (x,y)
void d2xy(ID n, ID d, ID &x, ID &y) {
    ID rx, ry, s, t=d;
    x = y = 0;
    for (s=1; s<n; s*=2) {
        rx = 1 & (t/2);
        ry = 1 & (t ^ rx);
        rot(s, x, y, rx, ry);
        x += s * rx;
        y += s * ry;
        t /= 4;
    }
}

/*
*-------------------------------------------------------
*
*     CLASSES
*       
*
*-------------------------------------------------------
*/
Point::Point(double x, double y){
	this->x = x;
	this->y = y;
}

Point::Point(){}

Point::~Point(){}

bool Point::operator< (const Point &other) const{
    return x < other.x;
}

bool Point::operator== (const Point &other) const{
    return (this->x == other.x && this->y == other.y);
}

double Point::to_angle(Point &o){   
	return atan2(y - o.y, x - o.x);
}


MBR::MBR(double xS, double yS, double xE, double yE){
	pMin = Point(xS, yS);
	pMax = Point(xE, yE);
}

double MBR::getArea(){
	return ((pMax.x - pMin.x) * (pMax.y - pMin.y));
}

MBR::MBR(){};

MBR::~MBR(){};

void MBR::set(double xS, double yS, double xE, double yE){
	pMin = Point(xS, yS);
	pMax = Point(xE, yE);
}


Cell::Cell(){};

//generates top right automatically
Cell::Cell(double bottomLeftX, double bottomLeftY, int classificationID, uint hilbertID){
	bottomLeft.x = bottomLeftX;
	bottomLeft.y = bottomLeftY;
	topRight.x = bottomLeftX + 1;
	topRight.y = bottomLeftY + 1;
	this->classificationID = classificationID;
	this->hilbertID = hilbertID;
}

Cell::Cell(double bottomLeftX, double bottomLeftY, double topRightX, double topRightY){
	bottomLeft.x = bottomLeftX;
	bottomLeft.y = bottomLeftY;
	topRight.x = topRightX;
	topRight.y = topRightY;
}

Cell::Cell(double bottomLeftX, double bottomLeftY, double topRightX, double topRightY, int classificationID){
	bottomLeft.x = bottomLeftX;
	bottomLeft.y = bottomLeftY;
	topRight.x = topRightX;
	topRight.y = topRightY;
	this->classificationID = classificationID;
}


CellPackage::CellPackage(){};

CellPackage::~CellPackage(){};

CellPackage::CellPackage(int cellType){
	this->cellType = cellType;
}

void CellPackage::setType(int cellType){
	this->cellType = cellType;
}


void CellPackage::addID(ID cellID){
	hilbertCellIDs.emplace_back(cellID);
}

Interval::Interval(ID s, ID e){
	start = s;
	end = e;
}

Interval::Interval(){}

bool Interval::operator <(const Interval& other){
	return start < other.start;
}


Polygon::Polygon(uint &recID){
	this->recID = recID;
	minHilbertX = std::numeric_limits<uint>::max();
	minHilbertY = std::numeric_limits<uint>::max();
	maxHilbertX = -std::numeric_limits<uint>::max();
	maxHilbertY = -std::numeric_limits<uint>::max();

	fullCellPackage.setType(FULL_COLOR);
	partialCellPackage.setType(PARTIAL_COLOR);
	partialCellPackage.cellCount = 0;
	fullCellPackage.cellCount = 0;
}

Polygon::Polygon(){};
Polygon::~Polygon(){};

void Polygon::addPoint(Point &p){
	vertices.push_back(p);
}

Dataset::Dataset(){};

Dataset::Dataset(string &filename){
	this->filename = filename;
}

Dataset::Dataset(string letterID){
	this->letterID = letterID;
}

Dataset::Dataset(string argument, string letterID){
	this->argument = argument;
	this->letterID = letterID;
}

Polygon* Dataset::getPolygonByID(uint &recID){
	auto it = polygons.find(recID);
	if(it != polygons.end()){
		return &(it->second);
	}
	return NULL;
}

Polygon* Dataset::getPolygonByIDAndSection(uint &sectionID, uint &recID){
	//cout << "retrieving polygon " << recID << " from section " << sectionID << endl;

	auto secIT = sectionObjectsMap.find(sectionID);
	if(secIT != sectionObjectsMap.end()){
		
		auto polIT = secIT->second.find(recID);
		if(polIT != secIT->second.end()){
			return &(polIT->second);
		}

		return NULL;

	}
	return NULL;
}


Section::Section(){
	objectCountR = 0;
	objectCountS = 0;
};


Section::Section(int &x, int &y){
	this->x = x;
	this->y = y;
	sectionID = y * H + x;
	objectCountR = 0;
	objectCountS = 0;

	interestxMin = DATA_SPACE.xMin + (x * (DATA_SPACE.xExtent / H));	
	interestxMax = DATA_SPACE.xMin + ((x+1) * (DATA_SPACE.xExtent / H));
	interestyMin = DATA_SPACE.yMin + (y * (DATA_SPACE.yExtent / H));	
	interestyMax = DATA_SPACE.yMin + ((y+1) * (DATA_SPACE.yExtent / H));

	rasterxMin = interestxMin;
	rasteryMin = interestyMin;
	rasterxMax = interestxMax;
	rasteryMax = interestyMax;

	xExtent = (rasterxMax - rasterxMin);
	yExtent = (rasteryMax - rasteryMin);
	area = (xExtent * yExtent);

	normInterestxMin = (interestxMin - DATA_SPACE.xMin) / DATA_SPACE.maxExtent;
	normInterestyMin = (interestyMin - DATA_SPACE.yMin) / DATA_SPACE.maxExtent;
	normInterestxMax = (interestxMax - DATA_SPACE.xMin) / DATA_SPACE.maxExtent;
	normInterestyMax = (interestyMax - DATA_SPACE.yMin) / DATA_SPACE.maxExtent;
}

Section::~Section(){};

void Section::expandRasterArea(double xMin, double yMin, double xMax, double yMax){
	// cout << fixed << setprecision(16) << "aaa " << xMax << endl;
	//revert normalization
	xMin > 0 ? xMin = xMin * DATA_SPACE.maxExtent + DATA_SPACE.xMin : xMin = DATA_SPACE.xMin;
	yMin > 0 ? yMin = yMin * DATA_SPACE.maxExtent + DATA_SPACE.yMin : yMin = DATA_SPACE.yMin;
	xMax < 1 ? xMax = xMax * DATA_SPACE.maxExtent + DATA_SPACE.xMin : xMax = DATA_SPACE.maxExtent + DATA_SPACE.xMin;
	yMax < 1 ? yMax = yMax * DATA_SPACE.maxExtent + DATA_SPACE.yMin : yMax = DATA_SPACE.maxExtent + DATA_SPACE.yMin;

	if(xMin < interestxMin){
		rasterxMin = min(rasterxMin, xMin);
	}
	if(yMin < interestyMin){
		rasteryMin = min(rasteryMin, yMin);
	}
	if(xMax > interestxMax){
		rasterxMax = max(rasterxMax, xMax);
	}
	if(yMax > interestyMax){
		rasteryMax = max(rasteryMax, yMax);
	}

	xExtent = (rasterxMax - rasterxMin);
	yExtent = (rasteryMax - rasteryMin);
	area = (xExtent * yExtent);
	cellArea = ((xExtent / HILBERT_n) * (yExtent / HILBERT_n));
}


double Section::getArea(){
	return area;
}

DataSpace::DataSpace(){
	sections.resize(H*H);
	totalObjects = 0;
}

DataSpace::~DataSpace(){};

void DataSpace::resize(){
	sections.resize(H*H);
	totalObjects = 0;
}

void DataSpace::setUniversalCoordinates(){
	xMin = universalMinX;
	yMin = universalMinY;
	xMax = universalMaxX;
	yMax = universalMaxY;

	xExtent = xMax - xMin;
	yExtent = yMax - yMin;
	maxExtent = max(xExtent, yExtent);
}

void DataSpace::allocateAppropriateSectionsOfMBR(double xMin, double yMin, double xMax, double yMax, int flag){
	for(auto &sec: sections){
		if(xMin > sec.normInterestxMax || yMin > sec.normInterestyMax || xMax < sec.normInterestxMin || yMax < sec.normInterestyMin){
			continue;
		}
		sec.expandRasterArea(xMin, yMin, xMax, yMax);
		if(flag == 0){
			sec.objectCountR += 1;
		}else{
			sec.objectCountS += 1;
		}
		totalObjects += 1;
	}
}

vector<Section> DataSpace::getSectionsOfMBR(double &xMin, double &yMin, double &xMax, double &yMax){
	vector<Section> secs;
	secs.reserve(H*H);
	
	for(auto &sec: DATA_SPACE.sections){
		//if the MBR is out of the section's interest scope then continue
		if(xMin > sec.interestxMax || yMin > sec.interestyMax || xMax < sec.interestxMin || yMax < sec.interestyMin){
			continue;
		}
		//else, keep the section for return
		secs.push_back(sec);
	}
	return secs;
}


vector<uint> DataSpace::getCommonSectionsIDOfObjects(uint &idA, uint &idB){
	auto itR = objectMapR.find(idA);
	auto itS = objectMapS.find(idB);
	vector<uint> commonSectionIDs;
	set_intersection(itR->second.begin(),itR->second.end(),itS->second.begin(),itS->second.end(),back_inserter(commonSectionIDs));
	return commonSectionIDs;
}