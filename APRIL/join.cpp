#include "join.h"


/*
*-------------------------------------------------------
*
*     COMMON
*       
*
*-------------------------------------------------------
*/

void nextinterval(CONTAINER *ar, uint *offset, ID *st, ID *end)
{
	
	(*st) = *(ID *)(ar+(*offset));
    (*offset)+=1;
    (*end) = *(ID *)(ar+(*offset));
    (*offset)+=1;

}

int compareIntervalsOfDifferentGranularity(vector<ID> &ar1, uint &numintervals1, vector<ID> &ar2, uint &numintervals2, uint &powerA, uint &powerB){
	//they may not have any intervals of this type
	if(numintervals1 == 0 || numintervals2 == 0){
		return 0;
	}
	
	// ID st1,st2,end1,end2;
	uint cur1=0;
	uint cur2=0;

	auto st1 = ar1.begin();
	auto end1 = ar1.begin() + 1;
	cur1++;

	auto st2 = ar2.begin();
	auto end2 = ar2.begin() + 1;
	cur2++;
	
	if(powerA < powerB){
		//A lower granularity than B so: scale B down
	    int granularityDiff = powerB - powerA;
	    int bitsToShift = granularityDiff*2;
	    
	    //shift
	    uint startA = *st1;
		uint endA = *end1;
		uint startB = (*st2) >> bitsToShift;
		uint endB = ((*end2)-1) >> bitsToShift;

	    do {			
			if (startA<=startB)
			{
				if (endA-1 >= startB) // overlap, endA>=startB if intervals are [s,e] and endA>startB if intervals are [s,e)
				{
					//they overlap, return 1
					return 1;
				}	
				else
				{
					st1 += 2;
					end1 += 2;
					cur1++;

					startA = *st1;
					endA = *end1;
				}
			}
			else // startB<startA
			{
				if (endB >= startA) // overlap, endB>=startA if intervals are [s,e] and endB>startA if intervals are [s,e)
				{

					//they overlap, return 1
					return 1;
				}
				else
				{
					st2 += 2;
					end2 += 2;
					cur2++;
				    //shift
	    			startB = (*st2) >> bitsToShift;
	    			endB = ((*end2)-1) >> bitsToShift;
				}
			}
		} while(cur1<=numintervals1 && cur2<=numintervals2);
		
		//no overlap, return 0
		return 0;
	}else{
		//B lower granularity than A so: scale A down
	    int granularityDiff = powerA - powerB;
	    int bitsToShift = granularityDiff*2;

	    //shift
	    uint startA = (*st1) >> bitsToShift;
		uint endA = ((*end1) - 1) >> bitsToShift;
		uint startB = *st2;
		uint endB = *end2;

	    do {
			if (startA<=startB)
			{
				if (endA>=startB) // overlap, endA>=startB if intervals are [s,e] and endA>startB if intervals are [s,e)
				{
					//they overlap, return 1
					return 1;
				}	
				else
				{
					st1 += 2;
					end1 += 2;
					cur1++;

				    //shift
				    startA = (*st1) >> bitsToShift;
				    endA = ((*end1) - 1) >> bitsToShift;
				}
			}
			else // startB<startA
			{
				if (endB-1 >= startA) // overlap, endB>=startA if intervals are [s,e] and endB>startA if intervals are [s,e)
				{

					//they overlap, return 1
					return 1;
				}
				else
				{
					st2 += 2;
					end2 += 2;
					cur2++;

					startB = *st2;
					endB = *end2;
				}
			}
		} while(cur1<=numintervals1 && cur2<=numintervals2);
		
		//no overlap, return 0
		return 0;

	}	
}

int compareIntervalsWithin(vector<ID> &ar1, uint &numintervals1, vector<ID> &ar2, uint &numintervals2){
	//they may not have any intervals of this type
	if(numintervals1 == 0 || numintervals2 == 0){
		return 0;
	}
	
	// ID st1,st2,end1,end2;
	uint cur1=0;
	uint cur2=0;
	
	auto st1 = ar1.begin();
	auto end1 = ar1.begin() + 1;
    cur1++;
    
	auto st2 = ar2.begin();
	auto end2 = ar2.begin() + 1;
    cur2++;

    bool intervalRcontained = false;

    do {
    	//check if it is contained completely
    	if(*st1 >= *st2 && *end1 <= *end2){
    		intervalRcontained = true;
    	}

		if (*end1<=*end2)
		{
			if(!intervalRcontained){
				//we are skipping this interval because it was never contained, so return false (not within)
				return 0;
			}
			st1 += 2;
			end1 += 2;
			cur1++;
			intervalRcontained = false;
		}
		else 
		{
			st2 += 2;
			end2 += 2;
			cur2++;
		}
	} while(cur1<=numintervals1 && cur2<=numintervals2);
			
	//if we didnt check all of the R intervals
	if(cur1 <= numintervals1){	
		return 0;
	}
	//all intervals R were contained
	return 1;
}


int compareIntervals(vector<ID> &ar1, uint &numintervals1, vector<ID> &ar2, uint &numintervals2){
	//they may not have any intervals of this type
	if(numintervals1 == 0 || numintervals2 == 0){
		return 0;
	}
	
	// ID st1,st2,end1,end2;
	uint cur1=0;
	uint cur2=0;
	
	auto st1 = ar1.begin();
	auto end1 = ar1.begin() + 1;
    cur1++;
    
	auto st2 = ar2.begin();
	auto end2 = ar2.begin() + 1;
    cur2++;

    do {
		if (*st1<=*st2)
		{
			if (*end1>*st2) // overlap, end1>=st2 if intervals are [s,e] and end1>st2 if intervals are [s,e)
			{
				//they overlap, return 1
				return 1;
			}	
			else
			{
				st1 += 2;
				end1 += 2;
				cur1++;
			}
		}
		else // st2<st1
		{
			if (*end2>*st1) // overlap, end2>=st1 if intervals are [s,e] and end2>st1 if intervals are [s,e)
			{

				//they overlap, return 1
				return 1;
			}
			else
			{
				st2 += 2;
				end2 += 2;
				cur2++;
			}
		}
	} while(cur1<=numintervals1 && cur2<=numintervals2);
	
	//no overlap, return 0
	return 0;
}


bool intervalsOverlap(ID &st1, ID &end1, ID &st2, ID &end2){
	// cout << "	Comparing intervals [" << st1 << "," << end1 << ") and [" << st2 << "," << end2 << ")" << endl; 
	if(st1 >= end2){
		return false;
	}
	if(st2 >= end1){
		return false;
	}
	return true;
}

int binarySearchInterval(vector<ID> &arr, int l, int r, ID &st, ID &end)
{
    while (l+1 <= r) {
        int mleft = (l + (r - l) / 2);
        int mright = mleft+1;

        if(mleft % 2 == 1){
        	if(mright+1 < r){
        		mleft++;
        		mright++;
        	}else{
        		mleft--;
        		mright--;
        	}
        }

		// cout << "Indexes [" << l << "," << r << "]" << endl;
		// cout << "Middle [" << mleft << "," << mright << "]" << endl; 
 
        //check if interval at mid overlaps with interval [st,end)
        if (intervalsOverlap(arr[mleft],arr[mright],st,end)){
        	//overlap
            return 1;
        }
 
        // If x greater, ignore left half
        if (arr[mright] < st){
            l = mright + 1;
        }
 
        // If x is smaller, ignore right half
        else{
            r = mleft - 1;
        }
    }
 
    // If we reach here, then element was not present
    return 0;
}

int compareIntervalsWithGaloping(vector<ID> &longar, uint &numintervalslong, vector<ID> &shortar, uint &numintervalsshort){
	//they may not have any intervals of this type
	if(numintervalslong == 0 || numintervalsshort == 0){
		return 0;
	}

	// cout << "GALLOPING: long " << longar.size() << " , short: " << shortar.size() << endl;

	ID st,end;
	for(auto it = shortar.begin(); it != shortar.end(); it+=2){
		st = *(it);
		end = *(it+1);

		// if the first interval overlaps, return hit
		if ( intervalsOverlap(st, end, *(longar.begin()), *(longar.begin()+1)) )
			return 1;

		// Find range for binary search by repeated doubling
		int i = 1;
		while (i*2 < longar.size() && longar[(i*2) + 1] <= end)
			i = i*2;

		// cout << "interval: [" << st << "," << end << ")" << endl;
		// cout << "i = " << i << endl;
		// cout << "will perform binary search in pos range " << (i/2)*2 << "," << min((i + 1)*2, (int)longar.size()) << endl; 
		
		//  Call binary search for the found range.
		if(binarySearchInterval(longar, (i/2)*2, min((i + 1)*2, (int)longar.size()), st, end)){
			//overlap
			// cout << "   hit!" << endl;
			return 1;
		}
		// cout << "no hit..." << endl;
	}
	//no overlap, return 0
	// cout << "do not overlap..." << endl;
	// exit(0);
	return 0;

}


int compareIntervalsWithCells(vector<ID> &ar1, uint &numintervals, vector<ID> &ar2, uint &numcells){
	//they may not have any intervals of this type
	if(numintervals == 0 || numcells == 0){
		return 0;
	}
	
	// ID st,end;
	uint cur1=0;
	uint cur2=0;	

	auto st = ar1.begin();
	auto end = ar1.begin() + 1;
    cur1++;
    
	auto c = ar2.begin();
    cur2++;

    do {
		if (*st<=*c)
		{
			if (*end>*c) // overlap, end>=c if intervals are [s,e] and end>c if intervals are [s,e)
			{
				//they overlap, return 1
				return 1;
			}	
			else
			{
				st += 2;
				end += 2;
				cur1++;
			}
		}
		else // c<st
		{
			c++;
			cur2++;
		}
	} while(cur1<=numintervals && cur2<=numcells);
	
	//no overlap, return 0
	return 0;
}

/*
*-------------------------------------------------------
*
*     INTERMEDIATE FILTER JOIN POLYGONS
*       
*
*-------------------------------------------------------
*/

//join two compressed APRIL approximations
int joinPolygons_compressed(Polygon *polA, Polygon *polB){
	//check ALL - ALL
	if(vbyte_join_compressed_sorted32(&polA->compressedALLarray[0], polA->numIntervalsALL, &polB->compressedALLarray[0], polB->numIntervalsALL) == 0){
		//guaranteed not hit
		return 0;
	}

	//check ALL - F (if any F in B)
	if(polB->F){
		if(vbyte_join_compressed_sorted32(&polA->compressedALLarray[0], polA->numIntervalsALL, &polB->compressedFarray[0], polB->numIntervalsF)){
			//hit
			return 1;
		}
	}

	//check F - ALL (if any F in A)
	if(polA->F){
		if(vbyte_join_compressed_sorted32(&polA->compressedFarray[0], polA->numIntervalsF, &polB->compressedALLarray[0], polB->numIntervalsALL)){
			//hit
			return 1;
		}
	}
	//The weak have not been checked
	return 2; //send to refinement
}

//join two uncompressed APRIL approximations
int joinPolygons_uncompressed(Polygon *polA, Polygon *polB){
	//check ALL - ALL
	if(compareIntervals(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedALL, polB->numIntervalsALL) == 0){
		//guaranteed not hit
		return 0;
	}

	//check ALL - F (if any F in B)
	if(polB->F){
		if(compareIntervals(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedF, polB->numIntervalsF)){
			//hit
			return 1;
		}
	}

	//check F - ALL (if any F in A)
	if(polA->F){
		if(compareIntervals(polA->uncompressedF, polA->numIntervalsF, polB->uncompressedALL, polB->numIntervalsALL)){
			//hit
			return 1;
		}
	}

	//The weak have not been checked
	return 2; //send to refinement
}

//join two uncompressed APRIL approximations
int joinPolygons_uncompressed_galloping(Polygon *polA, Polygon *polB){
	//check ALL - ALL
	// cout << "Polygons " << polA->recID << " and " << polB->recID << endl;

	if(polA->numIntervalsALL * 1000 <= polB->numIntervalsALL){
		//galloping at list B
		if(compareIntervalsWithGaloping(polB->uncompressedALL, polB->numIntervalsALL, polA->uncompressedALL, polA->numIntervalsALL) == 0){
			//guaranteed not hit
			return 0;
		}
	}else if(polB->numIntervalsALL * 1000 <= polA->numIntervalsALL){
		//galloping at list A
		if(compareIntervalsWithGaloping(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedALL, polB->numIntervalsALL) == 0){
			//guaranteed not hit
			return 0;
		}
	}else{
		//no galloping
		if(compareIntervals(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedALL, polB->numIntervalsALL) == 0){
			//guaranteed not hit
			return 0;
		}
	}

	//check ALL - F (if any F in B)
	if(polB->F){
		// cout << "ALL-F: " << polA->numIntervalsALL << " and " << polB->numIntervalsF << endl;
		if(polB->numIntervalsF * 1000 <= polA->numIntervalsALL){
			//galloping at list A all
			if(compareIntervalsWithGaloping(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedF, polB->numIntervalsF)){
				//hit
				// cout << "ALL-F galop ALL hit" << endl;
				return 1;
			}
		}else if(polA->numIntervalsALL * 1000 <= polB->numIntervalsF){
			if(compareIntervalsWithGaloping(polB->uncompressedF, polB->numIntervalsF, polA->uncompressedALL, polA->numIntervalsALL)){
				//hit
				// cout << "ALL-F galop F hit" << endl;
				return 1;
			}
		}else{
			//no galloping
			if(compareIntervals(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedF, polB->numIntervalsF)){
				//hit
				return 1;
			}
		}
	}

	//check F - ALL (if any F in A)
	if(polA->F){
		// cout << "F-ALL: " << polA->numIntervalsF << " and " << polB->numIntervalsALL << endl;
		if(polA->numIntervalsF * 1000 <= polB->numIntervalsALL){
			if(compareIntervalsWithGaloping(polA->uncompressedF, polA->numIntervalsF, polB->uncompressedALL, polB->numIntervalsALL)){
				//hit
				// cout << "F-ALL galop F hit" << endl;
				return 1;
			}
		}else if(polB->numIntervalsALL * 1000 <= polA->numIntervalsF){
			if(compareIntervalsWithGaloping(polB->uncompressedALL, polB->numIntervalsALL, polA->uncompressedF, polA->numIntervalsF)){
				//hit
				// cout << "F-ALL galop ALL hit" << endl;
				return 1;
			}
		}else{
			//no galloping
			if(compareIntervals(polA->uncompressedF, polA->numIntervalsF, polB->uncompressedALL, polB->numIntervalsALL)){
				//hit
				return 1;
			}
		}
		
	}

	// exit(0);
	//The weak have not been checked
	return 2; //send to refinement
}

/*
*-------------------------------------------------------
*
*     INTERMEDIATE FILTER JOIN POLYGON-LINESTRING
*       
*
*-------------------------------------------------------
*/

//join two uncompressed APRIL approximations
int joinPolygons_uncompressed_linestrings(Polygon *polA, Polygon *polB){
	//check ALL - ALL
	if(compareIntervalsWithCells(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedALL, polB->numIntervalsALL) == 0){
		//guaranteed not hit
		return 0;
	}

	//check F - ALL (if any F in A)
	if(polA->F){
		if(compareIntervalsWithCells(polA->uncompressedF, polA->numIntervalsF, polB->uncompressedALL, polB->numIntervalsALL)){
			//hit
			return 1;
		}
	}

	//The weak have not been checked
	return 2; //send to refinement
}

//join two uncompressed APRIL approximations
int joinPolygons_compressed_linestrings(Polygon *polA, Polygon *polB){
	cout << "joinPolygons_compressed_linestrings not implemented yet" << endl;
	exit(0); 
}

/*
*-------------------------------------------------------
*
*     INTERMEDIATE FILTER JOIN OF DIFFERENT GRANULARITIES
*       
*
*-------------------------------------------------------
*/

//join two compressed APRIL approximations of different granularity
int joinPolygons_compressed_different_granularities(Polygon *polA, Polygon *polB){
	if(polA->orderN != polB->orderN){
		//different granularity
		
		//check ALL - ALL
		if(vbyte_join_compressed_different_granularities(&polA->compressedALLarray[0], polA->numIntervalsALL, &polB->compressedALLarray[0], polB->numIntervalsALL, polA->orderN, polB->orderN) == 0){
			//guaranteed not hit
			return 0;
		}


		if(polA->orderN < polB->orderN){
			//check F - ALL (if any F in A)
			if(polA->F){
				if(vbyte_join_compressed_different_granularities(&polA->compressedFarray[0], polA->numIntervalsF, &polB->compressedALLarray[0], polB->numIntervalsALL, polA->orderN, polB->orderN)){
					//hit
					return 1;
				}
			}
		}else{
			//check ALL - F (if any F in B)
			if(polB->F){
				if(vbyte_join_compressed_different_granularities(&polA->compressedALLarray[0], polA->numIntervalsALL, &polB->compressedFarray[0], polB->numIntervalsF, polA->orderN, polB->orderN)){
					//hit
					return 1;
				}
			}
		}

		//The weak have not been checked
		return 2; //send to refinement
	}else{
		//same granularity
		return joinPolygons_compressed(polA, polB);
	}
}


//join two uncompressed APRIL approximations of different granularity
int joinPolygons_uncompressed_different_granularities(Polygon *polA, Polygon *polB){
	if(polA->orderN != polB->orderN){
		//different granularity		

		//check ALL - ALL
		if(compareIntervalsOfDifferentGranularity(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedALL, polB->numIntervalsALL, polA->orderN, polB->orderN) == 0){
			//guaranteed not hit
			return 0;
		}

		if(polA->orderN < polB->orderN){
			//check F - ALL (if any F in A)
			if(polA->F){
				if(compareIntervalsOfDifferentGranularity(polA->uncompressedF, polA->numIntervalsF, polB->uncompressedALL, polB->numIntervalsALL, polA->orderN, polB->orderN)){
					//hit
					return 1;
				}
			}
		}else{
			//check ALL - F (if any F in B)
			if(polB->F){
				if(compareIntervalsOfDifferentGranularity(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedF, polB->numIntervalsF, polA->orderN, polB->orderN)){
					//hit
					return 1;
				}
			}
		}
		//The weak have not been checked
		return 2; //send to refinement
	}else{
		//same granularity
		return joinPolygons_uncompressed(polA, polB);
	}


}

/*
*-------------------------------------------------------
*
*
*     INTERMEDIATE FILTER WITHIN JOIN
*       
*
*-------------------------------------------------------
*/


//join two uncompressed APRIL approximations
int joinPolygonsWithin_uncompressed(Polygon *polA, Polygon *polB){
	//check ALL - ALL
	if(compareIntervals(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedALL, polB->numIntervalsALL) == 0){
		//guaranteed not hit
		return 0;
	}

	//check ALL - F (if any F in B)
	if(polB->F){
		if(compareIntervalsWithin(polA->uncompressedALL, polA->numIntervalsALL, polB->uncompressedF, polB->numIntervalsF)){
			//hit
			return 1;
		}
	}

	//The weak have not been checked
	return 2; //send to refinement
}

//join two compressed APRIL approximations
int joinPolygonsWithin_compressed(Polygon *polA, Polygon *polB){
	cout << "Error: Within query for compressed APRIL not yet implemented" << endl;
}