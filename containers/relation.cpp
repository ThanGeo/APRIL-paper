/******************************************************************************
 * Project:  ijoin
 * Purpose:  Compute interval overlap joins
 * Author:   Panagiotis Bouros, pbour@github.io
 ******************************************************************************
 * Copyright (c) 2017, Panagiotis Bouros
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 ******************************************************************************/


#include "relation.h"


/*inline bool CompareByYStart2(const OneDStorage& ld, const OneDStorage& rd)
{
    return (ld.yStart < rd.yStart);
}*/


inline bool CompareByYStart(const Record& lhs, const Record& rhs)
{
    return (lhs.yStart < rhs.yStart);
}



Record::Record()
{
}


Record::Record(RecordId id, Coord xStart, Coord yStart, Coord xEnd, Coord yEnd)
{
    this->id = id;

    // MBR
    this->xStart = xStart;
    this->xEnd   = xEnd;
    this->yStart = yStart;
    this->yEnd   = yEnd;
}


bool Record::operator < (const Record& rhs) const
{
    return this->xStart < rhs.xStart;
}

bool Record::operator >= (const Record& rhs) const
{
    return !((*this) < rhs);
}


void Record::print(char c) const
{
    cout <<"[" << c << this->id << ": ("  << this->xStart << "," << this->yStart << ") -> ("  << this->xEnd << "," << this->yEnd << ")]" << endl;
}

   
Record::~Record()
{
}



Relation::Relation()
{
    this->minX = std::numeric_limits<Coord>::max();
    this->maxX = std::numeric_limits<Coord>::min() -500;
    this->minY = std::numeric_limits<Coord>::max();
    this->maxY = std::numeric_limits<Coord>::min();
    this->planeSweep = true;
}


void Relation::load(string filename)
{
    int polygonCount;
    int vertexCount;
    int recID;
    Coord x,y;

    ifstream file( filename, fstream::in | ios_base::binary);
    if(!file)
    {
        cerr << "Cannot open the File : " << filename << endl;
        exit(1);
    }

    //first read the total polygon count
    file.read((char*) &polygonCount, sizeof(int));
    //read polygons
    for(int j=0; j<polygonCount; j++){

        Coord minXmbr, minYmbr, maxXmbr, maxYmbr;
        minXmbr = std::numeric_limits<Coord>::max();
        maxXmbr = -std::numeric_limits<Coord>::max();
        minYmbr = std::numeric_limits<Coord>::max();
        maxYmbr = -std::numeric_limits<Coord>::max();

        //read/write the polygon id
        file.read((char*) &recID, sizeof(int)); 

        //read the vertex count
        file.read((char*) &vertexCount, sizeof(int));

        for(int i=0; i<vertexCount; i++){
            file.read((char*) &x, sizeof(double));
            file.read((char*) &y, sizeof(double));

            this->minX = std::min(this->minX, x);
            this->maxX = std::max(this->maxX, x);
            this->minY = std::min(this->minY, y);
            this->maxY = std::max(this->maxY, y);

            minXmbr = std::min(minXmbr, x);
            maxXmbr = std::max(maxXmbr, x);
            minYmbr = std::min(minYmbr, y);
            maxYmbr = std::max(maxYmbr, y);
        }
        //cout << "loaded pol " << recID << " with mbr: " << minXmbr << " " << minYmbr << "," << maxXmbr << " " << maxYmbr << endl;

        this->emplace_back(recID, minXmbr, minYmbr, maxXmbr, maxYmbr);
    }

    file.close();
    
}


void Relation::sortByXStart()
{
    sort(this->begin(), this->end());
}


void Relation::sortByYStart()
{
    sort(this->begin(), this->end(), CompareByYStart);
}


void Relation::print(char c)
{
    for (const Record& rec : (*this)){
        rec.print(c);
    }
}

void Relation::normalize(Coord minX, Coord maxX, Coord minY, Coord maxY, Coord maxExtent) {
    for (Record& rec : (*this))
    {
        rec.xStart = Coord(rec.xStart - minX) / maxExtent;
        rec.xEnd   = Coord(rec.xEnd   - minX) / maxExtent;
        rec.yStart = Coord(rec.yStart - minY) / maxExtent;
        rec.yEnd   = Coord(rec.yEnd   - minY) / maxExtent;
    }
    
    
}


void Relation::computeAvgExtents1d() {
    double sumX = 0, sumY = 0;

    for (Record& rec : (*this)) {
        sumX += rec.xEnd-rec.xStart;
        sumY += rec.yEnd-rec.yStart;
    }
    // this->avgXExtent = (double)sumX/this->numRecords;
    // this->avgYExtent = (double)sumY/this->numRecords;
}


/*void Relation::computeAvgExtents2d() {
    double sumX = 0, sumY = 0;

    for (Record& rec : (*this)) {
        sumX += rec.xEnd-rec.xStart;
        sumY += rec.yEnd-rec.yStart;
    }
    this->avgXExtent = (double)sumX/this->numRecords;
    this->avgYExtent = (double)sumY/this->numRecords;
}*/


Relation::~Relation()
{
}

//////////////////////////////////two level plus///////////////////////////


ABrec::ABrec(){

}

ABrec::ABrec(RecordId id, Coord xStart, Coord yStart, Coord xEnd){
    this->id = id;
    this->xStart = xStart;
    this->yStart = yStart;
    this->xEnd = xEnd;

}

ABrec::~ABrec(){

}


Crec::Crec(){

}

Crec::Crec(RecordId id, Coord yStart, Coord xEnd){
    this->id = id;
    this->yStart = yStart;
    this->xEnd = xEnd;
}

Crec::~Crec(){

}

Drec::Drec(){

}

Drec::Drec(RecordId id, Coord xEnd, Coord yEnd){
    this->id = id;
    this->xEnd = xEnd;
    this->yEnd = yEnd;
}

Drec::~Drec(){

}


YENDrec::YENDrec(){

}

YENDrec::YENDrec(RecordId id, Coord yEnd){
    this->id = id;
    this->yEnd = yEnd;
}

YENDrec::~YENDrec(){

}


OneDStorage::OneDStorage(){

}

OneDStorage::OneDStorage(RecordId id, Coord xEnd, Coord yStart, Coord yEnd){
    this->id = id;
    this->xEnd = xEnd;
    this->yStart = yStart;
    this->yEnd = yEnd;
}

OneDStorage::~OneDStorage(){

}

