#include "../../containers/relation.h"

//inline bool CompareByYStart(const Record& lhs, const Record& rhs);


namespace fs_2d{


    int findReferenceCell(double x, double y, double cellExtent, int numCellsPerDimension) {
        int xInt,yInt;

        xInt = (x + EPS)/cellExtent;
        yInt = (y + EPS)/cellExtent;

        return (yInt * numCellsPerDimension + xInt);
    };
    
    int myQuotient2(double numer, double denom) {
        return int(numer/denom + EPS);
    };


    double myRemainder2(double numer, double denom, int q) {
        double rem = double(numer - q*denom);

        return ((abs(rem) < EPS) ? 0: rem);
    };


    int getCellId2(int x, int y, int numCellsPerDimension) {
        return (y * numCellsPerDimension + x);
    };


    int findReferenceCell2(double x, double y, double cellExtent, int numCellsPerDimension) {
        int xInt,yInt;

        xInt = (x + EPS)/cellExtent;
        yInt = (y + EPS)/cellExtent;

        return (yInt * numCellsPerDimension + xInt);
    };
    
    namespace single{
        namespace partition{
            namespace oneArray{
                
                void Partition_One_Array(const Relation& R, const Relation& S, Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, int runNumPartitionsPerRelation)
                {
                    int runNumPartitions = runNumPartitionsPerRelation*runNumPartitionsPerRelation;
                    Coord partitionExtent = 1.0/runNumPartitionsPerRelation;
   
                    double xStartCell, yStartCell, xEndCell, yEndCell;
                    int firstCell, lastCell;

                    for (size_t i = 0; i < R.size(); i++){
                        auto &r = R[i];

                        // Determine cell for (rec.xStart, rec.yStart)
                        // Determine cell for (rec.xStart, rec.yStart)
                        xStartCell = myQuotient2(r.xStart + EPS, partitionExtent);
                        yStartCell = myQuotient2(r.yStart + EPS, partitionExtent);
                        firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);


                        if (r.xEnd + EPS >= 1) {
                            xEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            xEndCell = myQuotient2(r.xEnd + EPS, partitionExtent);
                        }

                        if (r.yEnd + EPS >= 1) {
                            yEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            yEndCell = myQuotient2(r.yEnd + EPS, partitionExtent);
                        }

                        lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                        int x = 0, y = 0;

                        // Put record in cells.
                        if (firstCell == lastCell) {
                            pRA_size[firstCell]++;
                        }
                        else {
                            pRA_size[firstCell]++;
                            int cellNumber;
                            for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                                if ( i != xStartCell){
                                    cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);
                                    pRC_size[cellNumber]++;
                                }
                                for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                    cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                                    if( i == xStartCell){
                                        pRB_size[cellNumber]++;
                                    }
                                    else{
                                        pRD_size[cellNumber]++;
                                    }
                                }
                            }
                        }
                    }

                    for (size_t i = 0; i < S.size(); i++){
                        auto &s = S[i];

                        // Determine cell for (rec.xStart, rec.yStart)
                        // Determine cell for (rec.xStart, rec.yStart)
                        xStartCell = myQuotient2(s.xStart + EPS, partitionExtent);
                        yStartCell = myQuotient2(s.yStart + EPS, partitionExtent);
                        firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);

                        if (s.xEnd + EPS >= 1) {
                            xEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            xEndCell = myQuotient2(s.xEnd + EPS, partitionExtent);
                        }

                        if (s.yEnd + EPS >= 1) {
                            yEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            yEndCell = myQuotient2(s.yEnd + EPS, partitionExtent);
                        }

                        lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                        int x = 0, y = 0;

                        // Put record in cells.
                        if (firstCell == lastCell) {
                            pSA_size[firstCell]++;
                        }
                        else {
                            pSA_size[firstCell]++;
                            int cellNumber;
                            for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                                if ( i != xStartCell){
                                    cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);
                                    pSC_size[cellNumber]++;
                                }
                                for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                    cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                                    if( i == xStartCell){
                                        pSB_size[cellNumber]++;
                                    }
                                    else{
                                        pSD_size[cellNumber]++;
                                    }
                                }
                            }
                        }
                    }     



                    int counter = 0;
                    for (int i = 0; i < runNumPartitions; i++){
                        counter = pRA_size[i] + pRB_size[i] + pRC_size[i] + pRD_size[i] ;
                        pR[i].resize(counter);  

                        pRB_size[i] = pRA_size[i] + pRB_size[i];
                        pRC_size[i] = counter - pRD_size[i];
                        pRD_size[i] = counter;

                        counter = pSA_size[i] + pSB_size[i] + pSC_size[i] + pSD_size[i] ;       
                        pS[i].resize(counter);       

                        pSB_size[i] = pSA_size[i] + pSB_size[i];
                        pSC_size[i] = counter - pSD_size[i];
                        pSD_size[i] = counter;

                    }

                    for (size_t i = 0; i < R.size(); i++){
                        auto &r = R[i];

                        xStartCell = myQuotient2(r.xStart + EPS, partitionExtent);
                        yStartCell = myQuotient2(r.yStart + EPS, partitionExtent);
                        firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);

                        if (r.xEnd + EPS >= 1) {
                            xEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            xEndCell = myQuotient2(r.xEnd + EPS, partitionExtent);
                        }

                        if (r.yEnd + EPS >= 1) {
                            yEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            yEndCell = myQuotient2(r.yEnd + EPS, partitionExtent);
                        }
                        lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                        int x = 0 , y = 0;

                        // Put record in cells.
                        if (firstCell == lastCell) {

                            //pR[firstCell][pRA_size[firstCell]] = r;

                            pRA_size[firstCell] = pRA_size[firstCell] - 1;

                            pR[firstCell][pRA_size[firstCell]].id = r.id;
                            pR[firstCell][pRA_size[firstCell]].xStart = r.xStart;
                            pR[firstCell][pRA_size[firstCell]].yStart = r.yStart;
                            pR[firstCell][pRA_size[firstCell]].xEnd = r.xEnd;
                            pR[firstCell][pRA_size[firstCell]].yEnd = r.yEnd;

                            //pRA_size[firstCell] = pRA_size[firstCell] + 1;

                            

                        }
                        else {

                            pRA_size[firstCell] = pRA_size[firstCell] - 1;

                            //pR[firstCell][pRA_size[firstCell]] = r;
                            pR[firstCell][pRA_size[firstCell]].id = r.id;
                            pR[firstCell][pRA_size[firstCell]].xStart = r.xStart;
                            pR[firstCell][pRA_size[firstCell]].yStart = r.yStart;
                            pR[firstCell][pRA_size[firstCell]].xEnd = r.xEnd;
                            pR[firstCell][pRA_size[firstCell]].yEnd = r.yEnd;

                            //pRA_size[firstCell] = pRA_size[firstCell] + 1;
                            

                            int cellNumber;
                            for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                                if ( i != xStartCell){
                                    cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);

                                    //pR[cellNumber][pRC_size[cellNumber]] = r;

                                    pRC_size[cellNumber] = pRC_size[cellNumber] - 1;

                                    pR[cellNumber][pRC_size[cellNumber]].id = r.id;
                                    pR[cellNumber][pRC_size[cellNumber]].xStart = r.xStart;
                                    pR[cellNumber][pRC_size[cellNumber]].yStart = r.yStart;
                                    pR[cellNumber][pRC_size[cellNumber]].xEnd = r.xEnd;
                                    pR[cellNumber][pRC_size[cellNumber]].yEnd = r.yEnd;

                                    //pRC_size[cellNumber] = pRC_size[cellNumber] + 1;

                                    

                                }
                                for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                    cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                                    if( i == xStartCell){

                                        pRB_size[cellNumber] = pRB_size[cellNumber] - 1 ;

                                        //pR[cellNumber][pRB_size[cellNumber]] = r;

                                        pR[cellNumber][pRB_size[cellNumber]].id = r.id;
                                        pR[cellNumber][pRB_size[cellNumber]].xStart = r.xStart;
                                        pR[cellNumber][pRB_size[cellNumber]].yStart = r.yStart;
                                        pR[cellNumber][pRB_size[cellNumber]].xEnd = r.xEnd;
                                        pR[cellNumber][pRB_size[cellNumber]].yEnd = r.yEnd;

                                        //pRB_size[cellNumber] = pRB_size[cellNumber] + 1 ;

                                        

                                    }
                                    else{

                                        //pR[cellNumber][pRD_size[cellNumber]] = r;

                                        pRD_size[cellNumber] = pRD_size[cellNumber] - 1 ;

                                        pR[cellNumber][pRD_size[cellNumber]].id = r.id;
                                        pR[cellNumber][pRD_size[cellNumber]].xStart = r.xStart;
                                        pR[cellNumber][pRD_size[cellNumber]].yStart = r.yStart;
                                        pR[cellNumber][pRD_size[cellNumber]].xEnd = r.xEnd;
                                        pR[cellNumber][pRD_size[cellNumber]].yEnd = r.yEnd;

                                        //pRD_size[cellNumber] = pRD_size[cellNumber] + 1 ;

                                        
                                    }
                                }
                            }
                        }
                    }

                    for (size_t i = 0; i < S.size(); i++){
                        auto &s = S[i];

                        xStartCell = myQuotient2(s.xStart + EPS, partitionExtent);
                        yStartCell = myQuotient2(s.yStart + EPS, partitionExtent);
                        firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);

                        if (s.xEnd + EPS >= 1) {
                            xEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            xEndCell = myQuotient2(s.xEnd + EPS, partitionExtent);
                        }

                        if (s.yEnd + EPS >= 1) {
                            yEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            yEndCell = myQuotient2(s.yEnd + EPS, partitionExtent);
                        }
                        lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                        int x = 0 , y = 0;

                        // Put record in cells.
                        if (firstCell == lastCell) {

                            //pS[firstCell][pSA_size[firstCell]] = s;

                            pSA_size[firstCell] = pSA_size[firstCell] - 1;

                            pS[firstCell][pSA_size[firstCell]].id = s.id;
                            pS[firstCell][pSA_size[firstCell]].xStart = s.xStart;
                            pS[firstCell][pSA_size[firstCell]].yStart = s.yStart;
                            pS[firstCell][pSA_size[firstCell]].xEnd = s.xEnd;
                            pS[firstCell][pSA_size[firstCell]].yEnd = s.yEnd;
                        }
                        else {

                            pSA_size[firstCell] = pSA_size[firstCell] - 1;

                            pS[firstCell][pSA_size[firstCell]].id = s.id;
                            pS[firstCell][pSA_size[firstCell]].xStart = s.xStart;
                            pS[firstCell][pSA_size[firstCell]].yStart = s.yStart;
                            pS[firstCell][pSA_size[firstCell]].xEnd = s.xEnd;
                            pS[firstCell][pSA_size[firstCell]].yEnd = s.yEnd;
                        
                            int cellNumber;
                            for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                                if ( i != xStartCell){
                                    cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);

                                    //pS[cellNumber][pSC_size[cellNumber]] = s;
                                    pSC_size[cellNumber] = pSC_size[cellNumber] - 1;

                                    pS[cellNumber][pSC_size[cellNumber]].id = s.id;
                                    pS[cellNumber][pSC_size[cellNumber]].xStart = s.xStart;
                                    pS[cellNumber][pSC_size[cellNumber]].yStart = s.yStart;
                                    pS[cellNumber][pSC_size[cellNumber]].xEnd = s.xEnd;
                                    pS[cellNumber][pSC_size[cellNumber]].yEnd = s.yEnd;
                                }
                                for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                    cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                                    if( i == xStartCell){

                                        //pS[cellNumber][pSB_size[cellNumber]] = s;

                                        pSB_size[cellNumber] = pSB_size[cellNumber] - 1 ;

                                        pS[cellNumber][pSB_size[cellNumber]].id = s.id;
                                        pS[cellNumber][pSB_size[cellNumber]].xStart = s.xStart;
                                        pS[cellNumber][pSB_size[cellNumber]].yStart = s.yStart;
                                        pS[cellNumber][pSB_size[cellNumber]].xEnd = s.xEnd;
                                        pS[cellNumber][pSB_size[cellNumber]].yEnd = s.yEnd;
                                    }
                                    else{

                                        pSD_size[cellNumber] = pSD_size[cellNumber] - 1 ;

                                        //pS[cellNumber][pSD_size[cellNumber]] = s;

                                        pS[cellNumber][pSD_size[cellNumber]].id = s.id;
                                        pS[cellNumber][pSD_size[cellNumber]].xStart = s.xStart;
                                        pS[cellNumber][pSD_size[cellNumber]].yStart = s.yStart;
                                        pS[cellNumber][pSD_size[cellNumber]].xEnd = s.xEnd;
                                        pS[cellNumber][pSD_size[cellNumber]].yEnd = s.yEnd;

                                        //pSD_size[cellNumber] = pSD_size[cellNumber] + 1 ;

                                        
                                    }
                                }
                            }
                        }          
                    }                    
                };

                void Partition_One_Array_Dec(const Relation& R, const Relation& S, Relation *pR, Relation *pS, vector<Drec> *pRDdec, vector<Drec> *pSDdec, size_t * pRA_size, size_t * pSA_size, size_t * pRB_size, size_t * pSB_size, size_t * pRC_size, size_t * pSC_size, size_t * pRD_size, size_t * pSD_size, int runNumPartitionsPerRelation)
                {
                    //cout<<"decomposition partition"<<endl;
                    int runNumPartitions = runNumPartitionsPerRelation*runNumPartitionsPerRelation;
                    Coord partitionExtent = 1.0/runNumPartitionsPerRelation;
   
                    double xStartCell, yStartCell, xEndCell, yEndCell;
                    int firstCell, lastCell;

                    for (size_t i = 0; i < R.size(); i++){
                        auto &r = R[i];

                        xStartCell = myQuotient2(r.xStart + EPS, partitionExtent);
                        yStartCell = myQuotient2(r.yStart + EPS, partitionExtent);
                        firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);

                        if (r.xEnd + EPS >= 1) {
                            xEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            xEndCell = myQuotient2(r.xEnd + EPS, partitionExtent);
                        }

                        if (r.yEnd + EPS >= 1) {
                            yEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            yEndCell = myQuotient2(r.yEnd + EPS, partitionExtent);
                        }

                        lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                        int x = 0, y = 0;

                        // Put record in cells.
                        if (firstCell == lastCell) {
                            pRA_size[firstCell]++;
                        }
                        else {
                            pRA_size[firstCell]++;
                            int cellNumber;
                            for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                                if ( i != xStartCell){
                                    cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);
                                    pRC_size[cellNumber]++;
                                }
                                for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                    cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                                    if( i == xStartCell){
                                        pRB_size[cellNumber]++;
                                    }
                                    else{
                                        pRD_size[cellNumber]++;
                                    }
                                }
                            }
                        }
                    }


                    for (size_t i = 0; i < S.size(); i++){
                        auto &s = S[i];

                        // Determine cell for (rec.xStart, rec.yStart)
                        // Determine cell for (rec.xStart, rec.yStart)
                        xStartCell = myQuotient2(s.xStart + EPS, partitionExtent);
                        yStartCell = myQuotient2(s.yStart + EPS, partitionExtent);
                        firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);

                        if (s.xEnd + EPS >= 1) {
                            xEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            xEndCell = myQuotient2(s.xEnd + EPS, partitionExtent);
                        }

                        if (s.yEnd + EPS >= 1) {
                            yEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            yEndCell = myQuotient2(s.yEnd + EPS, partitionExtent);
                        }

                        lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                        int x = 0, y = 0;

                        // Put record in cells.
                        if (firstCell == lastCell) {
                            pSA_size[firstCell]++;
                        }
                        else {
                            pSA_size[firstCell]++;
                            int cellNumber;
                            for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                                if ( i != xStartCell){
                                    cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);
                                    pSC_size[cellNumber]++;
                                }
                                for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                    cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                                    if( i == xStartCell){
                                        pSB_size[cellNumber]++;
                                    }
                                    else{
                                        pSD_size[cellNumber]++;
                                    }
                                }
                            }
                        }
                    } 

                    int counter = 0;
                    for (int i = 0; i < runNumPartitions; i++){
    
                        counter = pRA_size[i] + pRB_size[i] + pRC_size[i];
                        pR[i].resize(counter);                     
                        pRDdec[i].resize(pRD_size[i]);
                        pRB_size[i] = pRA_size[i] + pRB_size[i];
                        pRC_size[i] = counter;

                        counter = pSA_size[i] + pSB_size[i] + pSC_size[i];
                        pS[i].resize(counter);                      
                        pSDdec[i].resize(pSD_size[i]);
                        pSB_size[i] = pSA_size[i] + pSB_size[i];
                        pSC_size[i] = counter;
                    }



                    for (size_t i = 0; i < R.size(); i++){
                        auto &r = R[i];

                        xStartCell = myQuotient2(r.xStart + EPS, partitionExtent);
                        yStartCell = myQuotient2(r.yStart + EPS, partitionExtent);
                        firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);

                        if (r.xEnd + EPS >= 1) {
                            xEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            xEndCell = myQuotient2(r.xEnd + EPS, partitionExtent);
                        }

                        if (r.yEnd + EPS >= 1) {
                            yEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            yEndCell = myQuotient2(r.yEnd + EPS, partitionExtent);
                        }
                        lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                        int x = 0 , y = 0;

                        // Put record in cells.
                        if (firstCell == lastCell) {

                            pRA_size[firstCell] = pRA_size[firstCell] - 1;

                            pR[firstCell][pRA_size[firstCell]].id = r.id;
                            pR[firstCell][pRA_size[firstCell]].xStart = r.xStart;
                            pR[firstCell][pRA_size[firstCell]].yStart = r.yStart;
                            pR[firstCell][pRA_size[firstCell]].xEnd = r.xEnd;
                            pR[firstCell][pRA_size[firstCell]].yEnd = r.yEnd;
                        
                        }
                        else {

                            //pR[firstCell][pRA_size[firstCell]] = r;
                            pRA_size[firstCell] = pRA_size[firstCell] - 1;

                            pR[firstCell][pRA_size[firstCell]].id = r.id;
                            pR[firstCell][pRA_size[firstCell]].xStart = r.xStart;
                            pR[firstCell][pRA_size[firstCell]].yStart = r.yStart;
                            pR[firstCell][pRA_size[firstCell]].xEnd = r.xEnd;
                            pR[firstCell][pRA_size[firstCell]].yEnd = r.yEnd;


                            //pRA_size[firstCell] = pRA_size[firstCell] + 1;

                            int cellNumber;
                            for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                                if ( i != xStartCell){
                                    cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);

                                    //pR[cellNumber][pR_size[cellNumber]] = r;
                                    
                                    pRC_size[cellNumber] = pRC_size[cellNumber] - 1;

                                    pR[cellNumber][pRC_size[cellNumber]].id = r.id;
                                    pR[cellNumber][pRC_size[cellNumber]].xStart = r.xStart;
                                    pR[cellNumber][pRC_size[cellNumber]].yStart = r.yStart;
                                    pR[cellNumber][pRC_size[cellNumber]].xEnd = r.xEnd;
                                    pR[cellNumber][pRC_size[cellNumber]].yEnd = r.yEnd;
                                    
                                    //pRC_size[cellNumber] = pRC_size[cellNumber] + 1;

                                }
                                for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                    cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                                    if( i == xStartCell){
                                        
                                        //pR[cellNumber][pRB_size[cellNumber]] = r;

                                        pRB_size[cellNumber] = pRB_size[cellNumber] - 1 ;

                                        pR[cellNumber][pRB_size[cellNumber]].id = r.id;
                                        pR[cellNumber][pRB_size[cellNumber]].xStart = r.xStart;
                                        pR[cellNumber][pRB_size[cellNumber]].yStart = r.yStart;
                                        pR[cellNumber][pRB_size[cellNumber]].xEnd = r.xEnd;
                                        pR[cellNumber][pRB_size[cellNumber]].yEnd = r.yEnd;
                                        

                                        //pRB_size[cellNumber] = pRB_size[cellNumber] + 1 ;

                                    }
                                    else{

                                        pRD_size[cellNumber] = pRD_size[cellNumber] - 1;
                                        pRDdec[cellNumber][pRD_size[cellNumber]].id = r.id;
                                        pRDdec[cellNumber][pRD_size[cellNumber]].xEnd = r.xEnd;
                                        pRDdec[cellNumber][pRD_size[cellNumber]].yEnd = r.yEnd;                                    
                                        //pRD_size[cellNumber] = pRD_size[cellNumber] + 1;
                                    }
                                }
                            }
                        }
                    }

                    for (size_t i = 0; i < S.size(); i++){
                        auto &s = S[i];

                        xStartCell = myQuotient2(s.xStart + EPS, partitionExtent);
                        yStartCell = myQuotient2(s.yStart + EPS, partitionExtent);
                        firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);

                        if (s.xEnd + EPS >= 1) {
                            xEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            xEndCell = myQuotient2(s.xEnd + EPS, partitionExtent);
                        }

                        if (s.yEnd + EPS >= 1) {
                            yEndCell = runNumPartitionsPerRelation - 1;
                        }
                        else {
                            yEndCell = myQuotient2(s.yEnd + EPS, partitionExtent);
                        }
                        lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                        int x = 0 , y = 0;

                        // Put record in cells.
                        if (firstCell == lastCell) {
                            pSA_size[firstCell] = pSA_size[firstCell] - 1;

                            pS[firstCell][pSA_size[firstCell]].id = s.id;
                            pS[firstCell][pSA_size[firstCell]].xStart = s.xStart;
                            pS[firstCell][pSA_size[firstCell]].yStart = s.yStart;
                            pS[firstCell][pSA_size[firstCell]].xEnd = s.xEnd;
                            pS[firstCell][pSA_size[firstCell]].yEnd = s.yEnd;
                        }
                        else {

                            //pS[firstCell][pSA_size[firstCell]] = s;

                            pSA_size[firstCell] = pSA_size[firstCell] - 1;

                            pS[firstCell][pSA_size[firstCell]].id = s.id;
                            pS[firstCell][pSA_size[firstCell]].xStart = s.xStart;
                            pS[firstCell][pSA_size[firstCell]].yStart = s.yStart;
                            pS[firstCell][pSA_size[firstCell]].xEnd = s.xEnd;
                            pS[firstCell][pSA_size[firstCell]].yEnd = s.yEnd;

                            //pSA_size[firstCell] = pSA_size[firstCell] + 1;

                            int cellNumber;
                            for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                                if ( i != xStartCell){
                                    cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);

                                    //pS[cellNumber][pS_size[cellNumber]] = s;

                                    pSC_size[cellNumber] = pSC_size[cellNumber] - 1;

                                    pS[cellNumber][pSC_size[cellNumber]].id = s.id;
                                    pS[cellNumber][pSC_size[cellNumber]].xStart = s.xStart;
                                    pS[cellNumber][pSC_size[cellNumber]].yStart = s.yStart;
                                    pS[cellNumber][pSC_size[cellNumber]].xEnd = s.xEnd;
                                    pS[cellNumber][pSC_size[cellNumber]].yEnd = s.yEnd;


                                    //pS_size[cellNumber] = pS_size[cellNumber] + 1;

                                }
                                for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                                    cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                                    if( i == xStartCell){
                                        //pS[cellNumber][pSB_size[cellNumber]] = s;      


                                        pSB_size[cellNumber] = pSB_size[cellNumber] - 1 ;

                                        pS[cellNumber][pSB_size[cellNumber]].id = s.id;
                                        pS[cellNumber][pSB_size[cellNumber]].xStart = s.xStart;
                                        pS[cellNumber][pSB_size[cellNumber]].yStart = s.yStart;
                                        pS[cellNumber][pSB_size[cellNumber]].xEnd = s.xEnd;
                                        pS[cellNumber][pSB_size[cellNumber]].yEnd = s.yEnd;

                                        //pSB_size[cellNumber] = pSB_size[cellNumber] + 1 ;

                                    }
                                    else{

                                        pSD_size[cellNumber] = pSD_size[cellNumber] - 1;

                                        pSDdec[cellNumber][pSD_size[cellNumber]].id = s.id;
                                        pSDdec[cellNumber][pSD_size[cellNumber]].xEnd = s.xEnd;
                                        pSDdec[cellNumber][pSD_size[cellNumber]].yEnd = s.yEnd;                                    
                                        //pSD_size[cellNumber] = pSD_size[cellNumber] + 1;
                                    }
                                }
                            }
                        }          
                    }

                  
                }

            };
        };
        namespace sort{

            struct myclass {
                bool operator() (Record &i, Record &j) { return (i.yStart < j.yStart);}
            } myobject;
            
            
            namespace oneArray{
                void SortXStartOneArray(Relation *pR, Relation *pS, size_t *pRA_size , size_t *pSA_size , size_t *pRB_size, size_t *pSB_size,  int runNumPartitions){
     
                    for (int i = 0; i < runNumPartitions; i++){
                        std::sort(pR[i].begin(), pR[i].begin() + pRA_size[i]);
                        std::sort(pS[i].begin(), pS[i].begin() + pSA_size[i]);

                        std::sort(pR[i].begin() + pRA_size[i], pR[i].begin() + pRB_size[i]);
                        std::sort(pS[i].begin() + pSA_size[i], pS[i].begin() + pSB_size[i]);  
                    }
                };

                
  ////////////////counters are in the end////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////              
                void SortYStartOneArray(Relation *pR, Relation *pS, size_t *pRB_size, size_t *pSB_size,  size_t *pRC_size, size_t *pSC_size,size_t *pRD_size, size_t *pSD_size, int runNumPartitions){
                    for (int i = 0; i < runNumPartitions; i++){
                        auto pRStart = pR[i].begin();
                        std::sort(pRStart, pRStart + pRB_size[i], myobject);
                        std::sort(pRStart + pRC_size[i], pRStart + pRD_size[i], myobject);
                        
                        auto pSStart = pS[i].begin();

                        std::sort(pSStart, pSStart + pSB_size[i], myobject);
                        std::sort(pSStart + pSC_size[i], pSStart + pSD_size[i], myobject);
                    }         
                    
                };
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

                void SortYStartOneArray2(Relation *pR, Relation *pS, size_t *pRB_size, size_t *pSB_size,  size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size ,int runNumPartitions){
                    for (int i = 0; i < runNumPartitions; i++){
                        auto pRStart = pR[i].begin();
//                        std::sort(pRStart, pRStart + pRA_size[i], CompareByYStart);
//                        std::sort(pRStart + pRB_size[i], pRStart + pRC_size[i], CompareByYStart);
                        std::sort(pRStart, pRStart + pRB_size[i], myobject);
                        std::sort(pRStart + pRC_size[i], pRStart + pRD_size[i], myobject);
                        
                        auto pSStart = pS[i].begin();
//                        std::sort(pSStart, pSStart + pSA_size[i], CompareByYStart);
//                        std::sort(pSStart + pSB_size[i], pSStart + pSC_size[i], CompareByYStart);
                        std::sort(pSStart, pSStart + pSB_size[i], myobject);
                        std::sort(pSStart + pSC_size[i], pSStart + pSD_size[i], myobject);
                    }         
                    
                };

            };
            namespace decomposition{
                
                void SortXStartOneArray(Relation *pR, Relation *pS, size_t *pRA_size , size_t *pSA_size , size_t *pRB_size, size_t *pSB_size,  int runNumPartitions){

                };
                
                void SortYStartOneArray(Relation *pR, Relation *pS, size_t *pRB_size , size_t *pSB_size , size_t *pRC_size, size_t *pSC_size, int runNumPartitions){
    
                    //cout<<"decomposition sort"<<endl;
                    for (int i = 0; i < runNumPartitions; i++){
                        std::sort(pR[i].begin(), pR[i].begin() + pRB_size[i], myobject);
                        std::sort(pS[i].begin(), pS[i].begin() + pSB_size[i], myobject);

                        std::sort(pR[i].begin() + pRC_size[i], pR[i].end(), myobject);
                        std::sort(pS[i].begin() + pSC_size[i], pS[i].end(), myobject);
                    }
                    
                };

                void copyDec(Relation *pR, Relation *pS,  vector<ABrec>* pRABdec, vector<ABrec>* pSABdec, vector<Crec> *pRCdec, vector<Crec> *pSCdec, vector<Coord>* pRYEND, vector<Coord>* pSYEND,size_t * pRB_size, size_t * pSB_size, size_t * pRC_size, size_t * pSC_size, int runNumPartitions ){

                    for ( int i  = 0 ; i < runNumPartitions ; i ++){

                        pRABdec[i].resize(pRC_size[i]);
                        pRCdec[i].resize(pR[i].size() - pRC_size[i]);
                        pRYEND[i].resize(pR[i].size());

                        pSABdec[i].resize(pSC_size[i]);
                        pSCdec[i].resize(pS[i].size() - pSC_size[i]);
                        pSYEND[i].resize(pS[i].size());
                    }

                    for ( int i  = 0 ; i < runNumPartitions ; i ++){

                        for ( int j = 0 ; j < pRC_size[i]; j++){
                            auto &r = pR[i][j];

                            pRABdec[i][j].id = r.id;
                            pRABdec[i][j].yStart = r.yStart;
                            pRABdec[i][j].xStart = r.xStart;
                            pRABdec[i][j].xEnd = r.xEnd;

                            pRYEND[i][j] = r.yEnd;
                        }

                        int counter = 0;
                        for ( int j = pRC_size[i]; j < pR[i].size(); j++ ){
                            auto &r = pR[i][j];               

                            pRCdec[i][counter].id = r.id;
                            pRCdec[i][counter].yStart = r.yStart;
                            pRCdec[i][counter].xEnd = r.xEnd;

                            pRYEND[i][j] = r.yEnd;
                            counter ++;
                        }

                        for ( int j = 0 ; j < pSC_size[i]; j++){
                            auto &s = pS[i][j];

                            pSABdec[i][j].id = s.id;
                            pSABdec[i][j].yStart = s.yStart;
                            pSABdec[i][j].xStart = s.xStart;
                            pSABdec[i][j].xEnd = s.xEnd;

                            pSYEND[i][j] = s.yEnd;
                        }

                        counter = 0 ;
                        for ( int j = pSC_size[i]; j < pS[i].size(); j++ ){
                            auto &s = pS[i][j];

                            pSCdec[i][counter].id = s.id;
                            pSCdec[i][counter].yStart = s.yStart;
                            pSCdec[i][counter].xEnd = s.xEnd;

                            pSYEND[i][j] = s.yEnd;
                            counter ++;
                        }
                    }


                };
            }
        };
        

        void PartitionTwoDimensional(const Relation& R, const Relation& S, Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, bool runPlaneSweepOnX, int runNumPartitionsPerRelation)
        {
            //cout<<"single Partitionnnnnnn" <<endl;
            fs_2d::single::partition::oneArray::Partition_One_Array(R, S, pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartitionsPerRelation);
        };

        void PartitionTwoDimensionalDec (const Relation& R, const Relation& S, Relation *pR, Relation *pS, vector<ABrec>* pRABdec, vector<ABrec>* pSABdec,  vector<Crec> *pRCdec, vector<Crec> *pSCdec, vector<Drec> *pRDdec, vector<Drec> *pSDdec, vector<Coord>* pRYEND, vector<Coord>* pSYEND, size_t * pRA_size, size_t * pSA_size, size_t * pRB_size, size_t * pSB_size, size_t * pRC_size, size_t * pSC_size, size_t * pRD_size, size_t * pSD_size, bool runPlaneSweepOnX, int runNumPartitionsPerRelation){
            //cout<<"single Partitionnnnnnn" <<endl;
            fs_2d::single::partition::oneArray::Partition_One_Array_Dec( R, S, pR, pS, pRDdec, pSDdec, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartitionsPerRelation);

        };

        void PartitionUniform(const Relation& R, Relation *pR, size_t *pRA_size, size_t *pRB_size, size_t *pRC_size, size_t *pRD_size, int runNumPartitionsPerRelation)
        {
            int runNumPartitions = runNumPartitionsPerRelation*runNumPartitionsPerRelation;
            Coord partitionExtent = 1.0/runNumPartitionsPerRelation;

            //cout<<"runNumPartitions = " << runNumPartitions<<endl;

            double xStartCell, yStartCell, xEndCell, yEndCell;
            int firstCell, lastCell;
            //Timer tim;
           // double timepR = 0, timeDecomp = 0;
           
            for (size_t i = 0; i < R.size(); i++){
                auto &r = R[i];

                // Determine cell for (rec.xStart, rec.yStart)
                xStartCell = myQuotient2(r.xStart + EPS, partitionExtent);
                yStartCell = myQuotient2(r.yStart + EPS, partitionExtent);
                firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);

                if (r.xEnd + EPS >= 1) {
                    xEndCell = runNumPartitionsPerRelation - 1;
                }
                else {
                    xEndCell = myQuotient2(r.xEnd + EPS, partitionExtent);
                }

                if (r.yEnd + EPS >= 1) {
                    yEndCell = runNumPartitionsPerRelation - 1;
                }
                else {
                    yEndCell = myQuotient2(r.yEnd + EPS, partitionExtent);
                }

                lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                int x = 0, y = 0;

                // Put record in cells.
                if (firstCell == lastCell) {
                    pRA_size[firstCell]++;
                }
                else {
                    pRA_size[firstCell]++;

                    int cellNumber;
                    for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                        if ( i != xStartCell){
                            cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);
                            pRC_size[cellNumber]++;
                        }
                        for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                            cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                            if( i == xStartCell){
                                pRB_size[cellNumber]++;
                            }
                            else{
                                pRD_size[cellNumber]++;
                            }
                        }
                    }
                }
            }

            int counter = 0;
            for (int i = 0; i < runNumPartitions; i++){
                counter = pRA_size[i] + pRB_size[i] + pRC_size[i] + pRD_size[i] ;
                pR[i].resize(counter);

                //cout<<"i = " << i << "\tpRA_size[i] = " << pRA_size[i] <<"\tpRB_size[i] = " << pRB_size[i] << "\tpRC_size[i] = " << pRC_size[i] <<"\tpRD_size[i] = " << pRD_size[i]<<endl;

                pRB_size[i] = pRA_size[i] + pRB_size[i];
                pRC_size[i] = counter - pRD_size[i];
                pRD_size[i] = counter;

                //cout<<"i = " << i << "\tpRA_size[i] = " << pRA_size[i] <<"\tpRB_size[i] = " << pRB_size[i] << "\tpRC_size[i] = " << pRC_size[i] <<"\tpRD_size[i] = " << pRD_size[i]<<endl;
            }
        

            for (size_t i = 0; i < R.size(); i++){
                auto &r = R[i];

                xStartCell = myQuotient2(r.xStart + EPS, partitionExtent);
                yStartCell = myQuotient2(r.yStart + EPS, partitionExtent);
                firstCell = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);

                if (r.xEnd + EPS >= 1) {
                    xEndCell = runNumPartitionsPerRelation - 1;
                }
                else {
                    xEndCell = myQuotient2(r.xEnd + EPS, partitionExtent);
                }

                if (r.yEnd + EPS >= 1) {
                    yEndCell = runNumPartitionsPerRelation - 1;
                }
                else {
                    yEndCell = myQuotient2(r.yEnd + EPS, partitionExtent);
                }
                lastCell = getCellId2(xEndCell, yEndCell, runNumPartitionsPerRelation);

                int x = 0 , y = 0;

                // Put record in cells.
                if (firstCell == lastCell) {

                    pRA_size[firstCell] = pRA_size[firstCell] - 1;

                    pR[firstCell][pRA_size[firstCell]].id = r.id;
                    pR[firstCell][pRA_size[firstCell]].xStart = r.xStart;
                    pR[firstCell][pRA_size[firstCell]].yStart = r.yStart;
                    pR[firstCell][pRA_size[firstCell]].xEnd = r.xEnd;
                    pR[firstCell][pRA_size[firstCell]].yEnd = r.yEnd;
                    
                }
                else {

                    pRA_size[firstCell] = pRA_size[firstCell] - 1;

                    //pR[firstCell][pRA_size[firstCell]] = r;
                    pR[firstCell][pRA_size[firstCell]].id = r.id;
                    pR[firstCell][pRA_size[firstCell]].xStart = r.xStart;
                    pR[firstCell][pRA_size[firstCell]].yStart = r.yStart;
                    pR[firstCell][pRA_size[firstCell]].xEnd = r.xEnd;
                    pR[firstCell][pRA_size[firstCell]].yEnd = r.yEnd;

                    int cellNumber;
                    for ( int i = xStartCell ; i <= xEndCell ; i++ ){
                        if ( i != xStartCell){
                            cellNumber = getCellId2(i, yStartCell, runNumPartitionsPerRelation);

                            pRC_size[cellNumber] = pRC_size[cellNumber] - 1;

                            pR[cellNumber][pRC_size[cellNumber]].id = r.id;
                            pR[cellNumber][pRC_size[cellNumber]].xStart = r.xStart;
                            pR[cellNumber][pRC_size[cellNumber]].yStart = r.yStart;
                            pR[cellNumber][pRC_size[cellNumber]].xEnd = r.xEnd;
                            pR[cellNumber][pRC_size[cellNumber]].yEnd = r.yEnd;

                        }
                        for ( int j = yStartCell + 1 ; j <= yEndCell ; j ++ ){
                            cellNumber = getCellId2(i, j, runNumPartitionsPerRelation);
                            if( i == xStartCell){

                                pRB_size[cellNumber] = pRB_size[cellNumber] - 1 ;

                                //pR[cellNumber][pRB_size[cellNumber]] = r;

                                pR[cellNumber][pRB_size[cellNumber]].id = r.id;
                                pR[cellNumber][pRB_size[cellNumber]].xStart = r.xStart;
                                pR[cellNumber][pRB_size[cellNumber]].yStart = r.yStart;
                                pR[cellNumber][pRB_size[cellNumber]].xEnd = r.xEnd;
                                pR[cellNumber][pRB_size[cellNumber]].yEnd = r.yEnd;

                            }
                            else{

                                pRD_size[cellNumber] = pRD_size[cellNumber] - 1 ;

                                pR[cellNumber][pRD_size[cellNumber]].id = r.id;
                                pR[cellNumber][pRD_size[cellNumber]].xStart = r.xStart;
                                pR[cellNumber][pRD_size[cellNumber]].yStart = r.yStart;
                                pR[cellNumber][pRD_size[cellNumber]].xEnd = r.xEnd;
                                pR[cellNumber][pRD_size[cellNumber]].yEnd = r.yEnd;
                            }
                        }
                    }
                }
            }
        };

        void PartitionUniform(const Relation& R, Relation *pR, size_t *pR_size, int runNumPartitionsPerRelation)
        {
            int runNumPartitions = runNumPartitionsPerRelation*runNumPartitionsPerRelation;
            Coord partitionExtent = 1.0/runNumPartitionsPerRelation;

            double xStartCell, yStartCell, xEndCell, yEndCell;
            int firstCell, lastCell;

            for (size_t i = 0; i < R.size(); i++){
                Coord xCenter,yCenter;
                Coord xStartCell, yStartCell;
                auto &r = R[i];

                xCenter = (r.xStart + r.xEnd)/2;
                yCenter = (r.yStart + r.yEnd)/2;

                xStartCell = myQuotient2(xCenter + EPS, partitionExtent);
                yStartCell = myQuotient2(yCenter + EPS, partitionExtent);

                //cout<<"xstartCell = " << xStartCell << "\t ystartCell = " << yStartCell<<endl;
                
                int tileID = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);
                pR_size[tileID]++;
               
            }

            int counter = 0;
            for (int i = 0; i < runNumPartitions; i++){
                pR[i].resize(pR_size[i]);
                //pR[i].numRecords = pR_size[i];
            }

            // for (int i = 0; i < runNumPartitions; i++){
            //     pR_size[i] = 0;           
            // }


            for (size_t i = 0; i < R.size(); i++){
                Coord xCenter,yCenter;
                Coord xStartCell, yStartCell;
                auto &r = R[i];

                xCenter = (r.xStart + r.xEnd)/2;
                yCenter = (r.yStart + r.yEnd)/2;

                xStartCell = myQuotient2(xCenter + EPS, partitionExtent);
                yStartCell = myQuotient2(yCenter + EPS, partitionExtent);

                //cout<<"xstartCell = " << xStartCell << "\t ystartCell = " << yStartCell<<endl;
                int tileID = getCellId2(xStartCell, yStartCell, runNumPartitionsPerRelation);
                
                pR_size[tileID] = pR_size[tileID] - 1;
                pR[tileID][pR_size[tileID]] = r; 
                // pR_size[tileID] = pR_size[tileID] + 1;
            }
        };

    
    };

    
}
