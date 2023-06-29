#include "../../containers/relation.h"

namespace ditt_2d{
    
    int myQuotient(double numer, double denom) {
        return int(numer/denom + EPS);
    };


    double myRemainder(double numer, double denom, int q) {
        double rem = double(numer - q*denom);

        return ((abs(rem) < EPS) ? 0: rem);
    };


    int getCellId(int x, int y, int numCellsPerDimension) {
        return (y * numCellsPerDimension + x);
    };


    int findReferenceCell(double x, double y, double cellExtent, int numCellsPerDimension) {
        int xInt,yInt;

        xInt = (x + EPS)/cellExtent;
        yInt = (y + EPS)/cellExtent;


        return (yInt * numCellsPerDimension + xInt);
    };
    
    namespace single{
        namespace partition{
            // Partition on x-axis, sweep on x-axis
            void PartitionUniform(const Relation& R, const Relation& S, Relation *pR, Relation *pS, size_t *pR_size, size_t * pS_size, int runNumPartitionsPerRelation)
            {
                int runNumPartitions = runNumPartitionsPerRelation*runNumPartitionsPerRelation;
                Coord partitionExtent = 1.0/runNumPartitionsPerRelation;
   
                double xStartCell, yStartCell, xEndCell, yEndCell;
                int firstCell, lastCell;
   
                for (size_t i = 0; i < R.size(); i++){
                    auto &r = R[i];
                    // Determine cell for (rec.xStart, rec.yStart)
                    xStartCell = myQuotient(r.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(r.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);


                    if (r.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }
                    else {
                        xEndCell = myQuotient(r.xEnd + EPS, partitionExtent);
                    }


                    if (r.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }
                    else {
                        yEndCell = myQuotient(r.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        pR_size[firstCell]++;

                    }
                    else {
                        pR_size[firstCell]++;

                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);
                                pR_size[cellNumber]++;
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);

                                pR_size[cellNumber]++;
                            }
                        }
                    }
                }

                //#pragma omp for reduction(+ : pS_size[:runNumPartitions])
                for (size_t i = 0; i < S.size(); i++){
                    auto &s = S[i];
                    // Determine cell for (rec.xStart, rec.yStart)
                    xStartCell = myQuotient(s.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(s.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);

                    if (s.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }
                    else {
                        xEndCell = myQuotient(s.xEnd + EPS, partitionExtent);
                    }

                    if (s.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }
                    else {
                        yEndCell = myQuotient(s.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        pS_size[firstCell]++;
                    }
                    else {
                        pS_size[firstCell]++;

                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);
                                pS_size[cellNumber]++;
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);
                                pS_size[cellNumber]++;
                            }
                        }
                    }           
                }

                for (int i = 0; i < runNumPartitions; i++){
                    //if ( i == 472537 || i == 474538 || i == 458541|| i == 588521 || i == 628507 || i == 448550 || i == 458542 || i == 586521|| i == 538339 || i == 474537){
                    //if ( i == 578318 || i == 578319 || i == 604414|| i == 540341 || i == 522403 || i == 616588 || i == 536514 || i == 498485|| i == 590314 || i == 616374){
                    //    cout<<"i = " << i <<"\tpR = "<< pR_size[i] << "\tpS = "<< pS_size[i]<<endl;
                        //exit(0);
                    //}
                    pR[i].resize(pR_size[i]);
                    pS[i].resize(pS_size[i]);
                }
                //exit(0);

                // for (int i = 0; i < runNumPartitions; i++){
                //     pR_size[i] = 0;
                //     pS_size[i] = 0;
                // }


                for (size_t i = 0; i < R.size(); i++){
                    auto &r = R[i];

                    xStartCell = myQuotient(r.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(r.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);

                    if (r.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }

                    else {
                        xEndCell = myQuotient(r.xEnd + EPS, partitionExtent);
                    }

                    if (r.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }

                    else {
                        yEndCell = myQuotient(r.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        //pR[firstCell][pR_size[firstCell]] = r;

                        pR_size[firstCell]--;

                        pR[firstCell][pR_size[firstCell]].id = r.id;
                        pR[firstCell][pR_size[firstCell]].xStart = r.xStart;
                        pR[firstCell][pR_size[firstCell]].yStart = r.yStart;
                        pR[firstCell][pR_size[firstCell]].xEnd = r.xEnd;
                        pR[firstCell][pR_size[firstCell]].yEnd = r.yEnd;

                        //pR_size[firstCell]++;

                    }
                    else {

                        //pR[firstCell][pR_size[firstCell]] = r;

                        pR_size[firstCell]--;

                        pR[firstCell][pR_size[firstCell]].id = r.id;
                        pR[firstCell][pR_size[firstCell]].xStart = r.xStart;
                        pR[firstCell][pR_size[firstCell]].yStart = r.yStart;
                        pR[firstCell][pR_size[firstCell]].xEnd = r.xEnd;
                        pR[firstCell][pR_size[firstCell]].yEnd = r.yEnd;

                        //pR_size[firstCell]++;

                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);
                                
                                //pR[cellNumber][pR_size[cellNumber]] = r;
                                pR_size[cellNumber]--;

                                pR[cellNumber][pR_size[cellNumber]].id = r.id;
                                pR[cellNumber][pR_size[cellNumber]].xStart = r.xStart;
                                pR[cellNumber][pR_size[cellNumber]].yStart = r.yStart;
                                pR[cellNumber][pR_size[cellNumber]].xEnd = r.xEnd;
                                pR[cellNumber][pR_size[cellNumber]].yEnd = r.yEnd;

                                //pR_size[cellNumber]++;
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);
                                
                                //pR[cellNumber][pR_size[cellNumber]] = r;
                                pR_size[cellNumber]--;

                                pR[cellNumber][pR_size[cellNumber]].id = r.id;
                                pR[cellNumber][pR_size[cellNumber]].xStart = r.xStart;
                                pR[cellNumber][pR_size[cellNumber]].yStart = r.yStart;
                                pR[cellNumber][pR_size[cellNumber]].xEnd = r.xEnd;
                                pR[cellNumber][pR_size[cellNumber]].yEnd = r.yEnd;

                                //pR_size[cellNumber]++;
                            }
                        }
                    }        
                }

                for (size_t i = 0; i < S.size(); i++){
                    auto &s = S[i];

                    xStartCell = myQuotient(s.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(s.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);

                    if (s.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }

                    else {
                        xEndCell = myQuotient(s.xEnd + EPS, partitionExtent);
                    }

                    if (s.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }

                    else {
                        yEndCell = myQuotient(s.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        //pS[firstCell][pS_size[firstCell]] = s;
                        pS_size[firstCell]--;

                        pS[firstCell][pS_size[firstCell]].id = s.id;
                        pS[firstCell][pS_size[firstCell]].xStart = s.xStart;
                        pS[firstCell][pS_size[firstCell]].yStart = s.yStart;
                        pS[firstCell][pS_size[firstCell]].xEnd = s.xEnd;
                        pS[firstCell][pS_size[firstCell]].yEnd = s.yEnd;

                        //pS_size[firstCell]++;

                    }
                    else {

                        //pS[firstCell][pS_size[firstCell]] = s;

                        pS_size[firstCell]--;

                        pS[firstCell][pS_size[firstCell]].id = s.id;
                        pS[firstCell][pS_size[firstCell]].xStart = s.xStart;
                        pS[firstCell][pS_size[firstCell]].yStart = s.yStart;
                        pS[firstCell][pS_size[firstCell]].xEnd = s.xEnd;
                        pS[firstCell][pS_size[firstCell]].yEnd = s.yEnd;

                        //pS_size[firstCell]++;

                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);
                                
                                //pS[cellNumber][pS_size[cellNumber]] = s;

                                pS_size[cellNumber]--;

                                pS[cellNumber][pS_size[cellNumber]].id = s.id;
                                pS[cellNumber][pS_size[cellNumber]].xStart = s.xStart;
                                pS[cellNumber][pS_size[cellNumber]].yStart = s.yStart;
                                pS[cellNumber][pS_size[cellNumber]].xEnd = s.xEnd;
                                pS[cellNumber][pS_size[cellNumber]].yEnd = s.yEnd;

                                //pS_size[cellNumber]++;
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);
                                //pS[cellNumber][pS_size[cellNumber]] = s;

                                pS_size[cellNumber]--;

                                pS[cellNumber][pS_size[cellNumber]].id = s.id;
                                pS[cellNumber][pS_size[cellNumber]].xStart = s.xStart;
                                pS[cellNumber][pS_size[cellNumber]].yStart = s.yStart;
                                pS[cellNumber][pS_size[cellNumber]].xEnd = s.xEnd;
                                pS[cellNumber][pS_size[cellNumber]].yEnd = s.yEnd;

                                //pS_size[cellNumber]++;
                            }
                        }
                    }
                }         
            }




            /*void PartitionUniform(const Relation& R, const Relation& S, Relation *pR, Relation *pS, size_t *pR_size, size_t *pS_size, int runNumPartitionsPerRelation)
            {
                int runNumPartitions = runNumPartitionsPerRelation*runNumPartitionsPerRelation;
                Coord partitionExtent = 1.0/runNumPartitionsPerRelation;


                double xStartCell, yStartCell, xEndCell, yEndCell;
                int firstCell, lastCell;
   
                for (size_t i = 0; i < R.size(); i++){
                    auto &r = R[i];
                    // Determine cell for (rec.xStart, rec.yStart)
                    xStartCell = myQuotient(r.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(r.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);


                    if (r.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }
                    else {
                        xEndCell = myQuotient(r.xEnd + EPS, partitionExtent);
                    }


                    if (r.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }
                    else {
                        yEndCell = myQuotient(r.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        pR_size[firstCell]++;

                    }
                    else {
                        pR_size[firstCell]++;

                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);
                                pR_size[cellNumber]++;
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);

                                pR_size[cellNumber]++;
                            }
                        }
                    }
                }

                //#pragma omp for reduction(+ : pS_size[:runNumPartitions])
                for (size_t i = 0; i < S.size(); i++){
                    auto &s = S[i];
                    // Determine cell for (rec.xStart, rec.yStart)
                    xStartCell = myQuotient(s.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(s.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);

                    if (s.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }
                    else {
                        xEndCell = myQuotient(s.xEnd + EPS, partitionExtent);
                    }

                    if (s.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }
                    else {
                        yEndCell = myQuotient(s.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        pS_size[firstCell]++;
                    }
                    else {
                        pS_size[firstCell]++;

                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);
                                pS_size[cellNumber]++;
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);
                                pS_size[cellNumber]++;
                            }
                        }
                    }           
                }
                //cout<<"111111"<<endl;
                for (int i = 0; i < runNumPartitions; i++){
                    
                    pR[i].reserve(pR_size[i]);
                    pS[i].reserve(pS_size[i]);
                }

                //cout<<"222222"<<endl;

                //memset(pR_size, 0, runNumPartitions*sizeof(size_t));
                //memset(pS_size, 0, runNumPartitions*sizeof(size_t));


                for (size_t i = 0; i < R.size(); i++){
                    auto &r = R[i];

                    xStartCell = myQuotient(r.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(r.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);

                    if (r.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }

                    else {
                        xEndCell = myQuotient(r.xEnd + EPS, partitionExtent);
                    }

                    if (r.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }

                    else {
                        yEndCell = myQuotient(r.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        //pR[firstCell][pR_size[firstCell]] = r;
                        //pR_size[firstCell]++;
                        pR[firstCell].push_back(r);


                    }
                    else {

                        // pR[firstCell][pR_size[firstCell]] = r;
                        // pR_size[firstCell]++;

                        pR[firstCell].push_back(r);


                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);
                                
                                // pR[cellNumber][pR_size[cellNumber]] = r;
                                // pR_size[cellNumber]++;

                                pR[cellNumber].push_back(r);
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);
                                
                                // pR[cellNumber][pR_size[cellNumber]] = r;
                                // pR_size[cellNumber]++;

                                pR[cellNumber].push_back(r);
                            }
                        }
                    }        
                }

                for (size_t i = 0; i < S.size(); i++){
                    auto &s = S[i];

                    xStartCell = myQuotient(s.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(s.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);

                    if (s.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }

                    else {
                        xEndCell = myQuotient(s.xEnd + EPS, partitionExtent);
                    }

                    if (s.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }

                    else {
                        yEndCell = myQuotient(s.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        // pS[firstCell][pS_size[firstCell]] = s;
                        // pS_size[firstCell]++;
                        pS[firstCell].push_back(s);

                    }
                    else {

                        // pS[firstCell][pS_size[firstCell]] = s;
                        // pS_size[firstCell]++;
                        pS[firstCell].push_back(s);

                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);
                                
                                // pS[cellNumber][pS_size[cellNumber]] = s;
                                // pS_size[cellNumber]++;
                                pS[cellNumber].push_back(s);
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);
                                // pS[cellNumber][pS_size[cellNumber]] = s;
                                // pS_size[cellNumber]++;
                                pS[cellNumber].push_back(s);
                            }
                        }
                    }
                }
        
            }*/


            void PartitionUniform(const Relation& R, Relation *pR, size_t *pR_size, int runNumPartitionsPerRelation)
            {
                int runNumPartitions = runNumPartitionsPerRelation*runNumPartitionsPerRelation;
                Coord partitionExtent = 1.0/runNumPartitionsPerRelation;

                double xStartCell, yStartCell, xEndCell, yEndCell;
                int firstCell, lastCell;
   
                for (size_t i = 0; i < R.size(); i++){
                    auto &r = R[i];
                    // Determine cell for (rec.xStart, rec.yStart)
                    xStartCell = myQuotient(r.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(r.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);

                    if (r.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }
                    else {
                        xEndCell = myQuotient(r.xEnd + EPS, partitionExtent);
                    }


                    if (r.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }
                    else {
                        yEndCell = myQuotient(r.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        pR_size[firstCell]++;

                    }
                    else {
                        pR_size[firstCell]++;

                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);
                                pR_size[cellNumber]++;
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);

                                pR_size[cellNumber]++;
                            }
                        }
                    }
                }

                for (int i = 0; i < runNumPartitions; i++){
                    pR[i].resize(pR_size[i]);
                }


                for (size_t i = 0; i < R.size(); i++){
                    auto &r = R[i];

                    xStartCell = myQuotient(r.xStart + EPS , partitionExtent);
                    yStartCell = myQuotient(r.yStart  + EPS, partitionExtent);
                    firstCell = getCellId(xStartCell, yStartCell,runNumPartitionsPerRelation);

                    if (r.xEnd + EPS >= 1) {
                        xEndCell = runNumPartitionsPerRelation - 1;
                    }

                    else {
                        xEndCell = myQuotient(r.xEnd + EPS, partitionExtent);
                    }

                    if (r.yEnd + EPS >= 1) {
                        yEndCell = runNumPartitionsPerRelation-1;
                    }

                    else {
                        yEndCell = myQuotient(r.yEnd + EPS, partitionExtent);
                    }

                    lastCell = getCellId(xEndCell, yEndCell,runNumPartitionsPerRelation);

                    // Put record in cells.
                    if (firstCell == lastCell) {
                        //pR[firstCell].push_back(r);
                        
                        pR_size[firstCell]--;

                        pR[firstCell][pR_size[firstCell]].id = r.id;
                        pR[firstCell][pR_size[firstCell]].xStart = r.xStart;
                        pR[firstCell][pR_size[firstCell]].yStart = r.yStart;
                        pR[firstCell][pR_size[firstCell]].xEnd = r.xEnd;
                        pR[firstCell][pR_size[firstCell]].yEnd = r.yEnd;


                    }
                    else {

                       //pR[firstCell].push_back(r);
                        pR_size[firstCell]--;

                        pR[firstCell][pR_size[firstCell]].id = r.id;
                        pR[firstCell][pR_size[firstCell]].xStart = r.xStart;
                        pR[firstCell][pR_size[firstCell]].yStart = r.yStart;
                        pR[firstCell][pR_size[firstCell]].xEnd = r.xEnd;
                        pR[firstCell][pR_size[firstCell]].yEnd = r.yEnd;

                        int cellNumber;
                        for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                            if ( i != xStartCell){
                                cellNumber = getCellId(i, yStartCell, runNumPartitionsPerRelation);

                                pR_size[cellNumber]--;

                                pR[cellNumber][pR_size[cellNumber]].id = r.id;
                                pR[cellNumber][pR_size[cellNumber]].xStart = r.xStart;
                                pR[cellNumber][pR_size[cellNumber]].yStart = r.yStart;
                                pR[cellNumber][pR_size[cellNumber]].xEnd = r.xEnd;
                                pR[cellNumber][pR_size[cellNumber]].yEnd = r.yEnd;
                                
                                //pR[cellNumber].push_back(r);
                            }
                            for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                cellNumber = getCellId(i, j, runNumPartitionsPerRelation);
                                
                                pR_size[cellNumber]--;

                                pR[cellNumber][pR_size[cellNumber]].id = r.id;
                                pR[cellNumber][pR_size[cellNumber]].xStart = r.xStart;
                                pR[cellNumber][pR_size[cellNumber]].yStart = r.yStart;
                                pR[cellNumber][pR_size[cellNumber]].xEnd = r.xEnd;
                                pR[cellNumber][pR_size[cellNumber]].yEnd = r.yEnd;


                                //pR[cellNumber].push_back(r);
                            }
                        }
                    }        
                }
            }        
        };


        struct myclass {
            bool operator() (Record &i, Record &j) { return (i.yStart < j.yStart);}
        } myobject;

        
        namespace sort{
            void SortXStart2d(Relation *pR, Relation *pS, int runNumPartitions){

                for (int i = 0; i < runNumPartitions; i++){
                    pR[i].sortByXStart();
                    pS[i].sortByXStart();
                }                
            };


            void SortYStart2d(Relation *pR, Relation *pS,  int runNumPartitions){

                for (int i = 0; i < runNumPartitions; i++){
//                     pR[i].sortByYStart();
//                     pS[i].sortByYStart();
                    std::sort(pR[i].begin(), pR[i].end(), myobject);
                    std::sort(pS[i].begin(), pS[i].end(), myobject);
                }
            }           
        };
    }
}
