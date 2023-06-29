#include "intervalization.h"

uint32_t container[1000000];
uint8_t output[10000000];

void printIntervals(vector<ID> &intervals){
	cout << "INTERVALS: " << endl;
	for(auto it = intervals.begin(); it <= intervals.end()-1; it+=2){
		cout << *it << "," << *(it + 1) << endl;
	}
	cout << endl;
}

void printIntervalsArray(ID* arr, uint &size){

	for(int i=0; i<size-1; i+=2){
		cout << arr[i] << "," << arr[i+1] << endl;
	}
	cout << endl;
}


/*


	CELL-BASED INTERVALIZATION


*/


void createIntervalList(Polygon &pol, CellPackage &cp, int COLOR){
	if(cp.hilbertCellIDs.size() == 0){
		switch(COLOR){
			case PARTIAL_COLOR:
				pol.numIntervalsALL = 0;
				break;			
			case FULL_COLOR:
				pol.numIntervalsF = 0;
				break;
		}
		return;
	}

	//sort the IDs 
	sort(cp.hilbertCellIDs.begin(), cp.hilbertCellIDs.end());

	//INTERVALIZE
	vector<ID> vec;
	vec.reserve(cp.hilbertCellIDs.size());
	ID prev = *cp.hilbertCellIDs.begin();
	ID start = prev;
	vec.emplace_back(start);

	for(auto it = cp.hilbertCellIDs.begin() + 1; it != cp.hilbertCellIDs.end(); it++){
		if(*it > prev + 1){
			vec.emplace_back(prev + 1);
			start = *it;
			vec.emplace_back(start);
		}
		prev = *it;
	}
	//last element
	vec.emplace_back(prev + 1);

	if(COMPRESSION){
		//store into an array for compression
		copy(vec.begin(), vec.end(), container);
		//perform the compression
		switch(COLOR){
			case PARTIAL_COLOR:
				pol.numIntervalsALL = vec.size() / 2;
				pol.numBytesALL = vbyte_compress_sorted32((uint32_t*)&container[0], &output[0], 0, vec.size());
				pol.compressedALL.insert(pol.compressedALL.begin(), &output[0], output + pol.numBytesALL);
				break;
			case FULL_COLOR:
				pol.numIntervalsF = vec.size() / 2;
				pol.numBytesF = vbyte_compress_sorted32((uint32_t*)&container[0], &output[0], 0, vec.size());
				pol.compressedF.insert(pol.compressedF.begin(), &output[0], output + pol.numBytesF);
				break;
		}
	}else{
		//store uncompressed
		switch(COLOR){
			case PARTIAL_COLOR:
				pol.numIntervalsALL = vec.size() / 2;
				pol.numBytesALL = vec.size();
				pol.uncompressedALL = vec;
				break;
			case FULL_COLOR:
				pol.numIntervalsF = vec.size() / 2;
				pol.numBytesF = vec.size();
				pol.uncompressedF = vec;				
				break;
		}
	}
}


/*


	2-GRID INTERVALIZATION


*/

void createIntervalListMixed(Polygon &pol, CellPackage &cp, vector<Interval> &intervals, int COLOR){
	if(cp.hilbertCellIDs.size() == 0){
		switch(COLOR){
			case PARTIAL_COLOR:
				pol.numIntervalsALL = 0;
				break;			
			case FULL_COLOR:
				pol.numIntervalsF = 0;
				break;
		}
		return;
	}
	//sort the IDs 
	sort(cp.hilbertCellIDs.begin(), cp.hilbertCellIDs.end());

	//INTERVALIZE into intervals of the [start,end) format
	vector<ID> vec;
	vec.reserve(cp.hilbertCellIDs.size());
	ID prev, start;

	//iterate both cell and interval lists
	auto it = cp.hilbertCellIDs.begin();
	auto intervalIT = intervals.begin();

	if(*cp.hilbertCellIDs.begin() < intervalIT->start){
		prev = *it;
		it++;
	}else{
		prev = intervalIT->start;
	}
	start = prev;
	vec.emplace_back(start);
	
	//loop all cells and existing intervals and merge consecutive ones into new intervals
	while(it != cp.hilbertCellIDs.end() || intervalIT != intervals.end()){		
		if(intervalIT != intervals.end()){
			if(it != cp.hilbertCellIDs.end()){
				if(*it < intervalIT->start){
					if(*it > prev + 1){
						vec.emplace_back(prev + 1);
						start = *it;
						vec.emplace_back(start);

					}
					prev = *it;
					it++;
				}else{
					if(intervalIT->start > prev+1){
						vec.emplace_back(prev + 1);
						start = intervalIT->start;
						vec.emplace_back(start);
					}
					prev = intervalIT->end;
					intervalIT++;
				}			
			}else{
				if(intervalIT->start > prev+1){
					vec.emplace_back(prev + 1);
					start = intervalIT->start;
					vec.emplace_back(start);
				}
				prev = intervalIT->end;
				intervalIT++;
			}			
		}else{
			if(*it > prev + 1){
				vec.emplace_back(prev + 1);
				start = *it;
				vec.emplace_back(start);

			}
			prev = *it;
			it++;
		}
	}
	//last element
	vec.emplace_back(prev + 1);


	if(COMPRESSION){
		//perform the compression and store
		//store into an array for compression
		copy(vec.begin(), vec.end(), container);
		switch(COLOR){
			case PARTIAL_COLOR:		//ALL
				pol.numIntervalsALL = vec.size() / 2;
				pol.numBytesALL = vbyte_compress_sorted32((uint32_t*)&container[0], &output[0], 0, vec.size());
				pol.compressedALL.insert(pol.compressedALL.begin(), &output[0], output + pol.numBytesALL);
				break;
			case FULL_COLOR:
				pol.numIntervalsF = vec.size() / 2;
				pol.numBytesF = vbyte_compress_sorted32((uint32_t*)&container[0], &output[0], 0, vec.size());
				pol.compressedF.insert(pol.compressedF.begin(), &output[0], output + pol.numBytesF);
				break;
		}		
	}else{
		//store uncompressed
		switch(COLOR){
			case PARTIAL_COLOR:		//ALL
				pol.numIntervalsALL = vec.size() / 2;
				pol.numBytesALL = vec.size();
				pol.uncompressedALL = vec;
				break;
			case FULL_COLOR:
				pol.numIntervalsF = vec.size() / 2;
				pol.numBytesF = vec.size();
				pol.uncompressedF = vec;				
				break;
		}

	}
}

void compressLinestringCells(Polygon &pol){
	//store into an array for compression
	copy(pol.uncompressedALL.begin(), pol.uncompressedALL.end(), container);
	//compress
	pol.numBytesALL = vbyte_compress_sorted32((uint32_t*)&container[0], &output[0], 0, pol.uncompressedALL.size());
	pol.compressedALL.insert(pol.compressedALL.begin(), &output[0], output + pol.numBytesALL);
}


void intervalizeMixed(Polygon &pol){
	//sort full intervals list
	sort(pol.fullCellPackage.intervals.begin(), pol.fullCellPackage.intervals.end());
	//first the FULL
	createIntervalListMixed(pol, pol.fullCellPackage, pol.fullCellPackage.intervals, FULL_COLOR);
	//add full cells to the partial list
	//then ALL
	pol.partialCellPackage.hilbertCellIDs.insert(end(pol.partialCellPackage.hilbertCellIDs), begin(pol.fullCellPackage.hilbertCellIDs), end(pol.fullCellPackage.hilbertCellIDs));
	createIntervalListMixed(pol, pol.partialCellPackage, pol.fullCellPackage.intervals, PARTIAL_COLOR);	
}

/*


	GATEWAY FUNCTION


*/


void intervalize(Polygon &pol){
	if(pol.fullCellPackage.intervals.size() > 0){
    	//there are existing intervals besides cells
		intervalizeMixed(pol);
    }else{
    	//only full cells
    	createIntervalList(pol, pol.fullCellPackage, FULL_COLOR);
		//add full cells to partial list
		pol.partialCellPackage.hilbertCellIDs.insert(end(pol.partialCellPackage.hilbertCellIDs), begin(pol.fullCellPackage.hilbertCellIDs), end(pol.fullCellPackage.hilbertCellIDs));
		createIntervalList(pol, pol.partialCellPackage, PARTIAL_COLOR);
    }

}