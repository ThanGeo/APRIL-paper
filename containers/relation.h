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


#ifndef _RELATION_H_
#define _RELATION_H_

#include "../def.h"



class Record
{
public:
    RecordId id;
    
    // MBR
	Coord xStart, yStart; // bottom-left corner
    Coord xEnd, yEnd;     // top-right corner

	Record();
	Record(RecordId id, Coord xStart, Coord yStart, Coord xEnd, Coord yEnd);
	bool operator < (const Record& rhs) const;
	bool operator >= (const Record& rhs) const;
	void print() const;
	void print(char c) const;
	~Record();	

};




class Relation : public vector<Record>
{
public:
	//size_t numRecords;
	bool planeSweep;
    Coord minX, maxX, minY, maxY;
    //double avgXExtent, avgYExtent;

	Relation();
	void load(string filename);
	void sortByXStart();
    void sortByYStart();
	void print(char c);
    void normalize(Coord minX, Coord maxX, Coord minY, Coord maxY, Coord maxExtent);
    void computeAvgExtents1d();
    ~Relation();
};
typedef Relation::const_iterator RelationIterator;



class ABrec{
	public:
		RecordId id;
		Coord yStart, xStart, xEnd;   

		ABrec();
		ABrec(RecordId id, Coord xStart, Coord yStart, Coord xEnd);
		~ABrec();
};


class Crec{
	public:
		RecordId id;
		Coord yStart, xEnd;

		Crec();
		Crec(RecordId id, Coord yStart, Coord xEnd);
		~Crec();

};

class Drec{
	public:
		RecordId id;
		Coord xEnd, yEnd;

		Drec();
		Drec(RecordId id, Coord xEnd, Coord yEnd);
		~Drec();
};

class YENDrec{
	public:
		RecordId id;
		Coord yEnd;


		YENDrec();
		YENDrec(RecordId id, Coord yEnd);
		~YENDrec();

};

class OneDStorage{
	public:
		RecordId id;
		Coord xEnd, yStart,yEnd;

		OneDStorage();
		OneDStorage(RecordId id, Coord xEnd, Coord yStart, Coord yEnd);
		~OneDStorage();

};

inline bool CompareByYStart(const Record& lhs, const Record& rhs);

//inline bool CompareByYStart2(const OneDStorage& ld, const OneDStorage& rd);


// struct myclass {
// 	bool  operator() (const Record& i, const Record& j) { return (i.yStart<j.yStart);}
// } myobject;



typedef Relation Group;
typedef Group::const_iterator GroupIterator;
#endif //_RELATION_H_
