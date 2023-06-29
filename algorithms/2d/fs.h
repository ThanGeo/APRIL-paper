#include "../../containers/relation.h"
#include "../../pipeline.h"

inline bool CompareByYStart(const Record& lhs, const Record& rhs);


namespace fs_2d{ 
    
    inline int myQuotient(int numer, int denom) {
        return numer/denom;
    };


    inline int myRemainder(int numer, int denom) {
        return numer%denom;
    };
    
    
    inline int findReferenceCell1(double x, double y, double cellExtent, int numCellsPerDimension) {
        int xInt,yInt;

        xInt = (x + EPS)/cellExtent;
        yInt = (y + EPS)/cellExtent;

        return (yInt * numCellsPerDimension + xInt);
    };

    vector<pair<uint,uint>> mbr_filter_output_pairs;
   

    
    namespace single{        

        namespace sort{
            void SortXStartOneArray(Relation &pR, Relation &pS, size_t pRA_size , size_t pSA_size , size_t pRB_size, size_t pSB_size){

                std::sort(pR.begin(), pR.begin() + pRA_size);
                std::sort(pS.begin(), pS.begin() + pSA_size);

                std::sort(pR.begin() + pRA_size, pR.begin() + pRB_size);
                std::sort(pS.begin() + pSA_size, pS.begin() + pSB_size);    

            };

            void SortYStartOneArray(Relation &pR, Relation &pS, size_t pRA_size , size_t &pSA_size , size_t pRB_size, size_t pSB_size,  size_t pRC_size, size_t pSC_size){

                std::sort(pR.begin(), pR.begin() + pRA_size, CompareByYStart);
                std::sort(pS.begin(), pS.begin() + pSA_size, CompareByYStart);

                std::sort(pR.begin() + pRB_size, pR.begin() + pRC_size, CompareByYStart);
                std::sort(pS.begin() + pSB_size, pS.begin() + pSC_size, CompareByYStart);
            };
        };
        
        
        namespace sweepX{
   
            inline unsigned long long InternalLoop_Rolled_CNT_X_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->xEnd >= pivot->xStart))
                {
                    if ((rec->yStart > pivot->yEnd) || (rec->yEnd < pivot->yStart))
                    {
                        pivot++;
                        continue;
                    }
                    result++;
                    pivot++;
                }

                return result;
            };
            
            
            inline unsigned long long InternalLoop_Rolled_CNT_V2_X_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->xEnd >= pivot->xStart))
                {
                    if ((rec->yStart > pivot->yEnd) || (rec->yEnd < pivot->yStart))
                    {
                        pivot++;
                        continue;
                    }
                    result++;
                    pivot++;
                }

                return result;
            };
            
            
            inline unsigned long long InternalLoop_Rolled_CNT_V3_1_X_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->xEnd >= pivot->xStart))
                {        
                    if (rec->yStart > pivot->yEnd)
                    {
                        pivot++;
                        continue;
                    }
                    result++;
                    pivot++;
                }

                return result;
            };


            inline unsigned long long InternalLoop_Rolled_CNT_V3_2_X_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->xEnd >= pivot->xStart))
                {       
                    if (pivot->yStart > rec->yEnd)
                    {
                        pivot++;
                        continue;
                    }
                    result++;
                    pivot++;
                }

                return result;
            };

            
            inline unsigned long long InternalLoop_Rolled_CNT_V4_X_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->xEnd >= pivot->xStart))
                {
                    if (rec->yStart > pivot->yEnd)
                    {
                        pivot++;
                        continue;
                    }
                    result++;
                    pivot++;
                }

                return result;
            };
            
            
            inline unsigned long long InternalLoop_Rolled_CNT_V5_X_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->xEnd >= pivot->xStart))
                {
                    if ( rec->yEnd < pivot->yStart)
                    {
                        pivot++;
                        continue;
                    }
                    result++;
                    pivot++;
                }

                return result;
            };
                  
            namespace oneArray{
                
                inline unsigned long long Sweep_Rolled_CNT_X_(Relation &R, Relation &S, size_t startR, size_t endR, size_t startS, size_t endS)
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS;

                    while ((r < lastR) && (s < lastS))
                    {
                        if (*r < *s)
                        {
                            // Run internal loop.
                            result += fs_2d::single::sweepX::InternalLoop_Rolled_CNT_X_(r, s, lastS );
                            r++;
                        }
                        else
                        {
                            // Run internal loop.
                            result += fs_2d::single::sweepX::InternalLoop_Rolled_CNT_X_(s, r, lastR );
                            s++;
                        }
                    }

                    return result;
                };
                

                inline unsigned long long Sweep_Rolled_CNT_V2_X_(Relation &R, Relation &S, size_t startR, size_t endR, size_t startS, size_t endS)
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS; 

                    while ((r < lastR))
                    {
                        result += fs_2d::single::sweepX::InternalLoop_Rolled_CNT_V2_X_(r, s, lastS );
                        r++;
                    }

                    return result;
                };
                

                inline unsigned long long Sweep_Rolled_CNT_V3_X_(Relation &R, Relation &S, size_t startR, size_t endR, size_t startS, size_t endS)
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS;

                    while ((r < lastR) && (s < lastS))
                    {
                        if (*r < *s)
                        {
                            // Run internal loop.
                            result += fs_2d::single::sweepX::InternalLoop_Rolled_CNT_V3_1_X_(r, s, lastS );
                            r++;
                        }
                        else
                        {
                            // Run internal loop.
                            result += fs_2d::single::sweepX::InternalLoop_Rolled_CNT_V3_2_X_(s, r, lastR );
                            s++;
                        }
                    }

                    return result;
                };


                inline unsigned long long Sweep_Rolled_CNT_V4_X_(Relation &R, Relation &S, size_t startR, size_t endR, size_t startS, size_t endS )
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS;

                    while ((r < lastR))
                    { 
                        // Run internal loop.
                        result += fs_2d::single::sweepX::InternalLoop_Rolled_CNT_V4_X_(r, s, lastS );
                        r++;
                    }

                    return result;
                };


                inline unsigned long long Sweep_Rolled_CNT_V5_X_(Relation &R, Relation &S, size_t startR, size_t endR, size_t startS, size_t endS )
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS;

                    while ((r < lastR))
                    {
                        // Run internal loop.
                        result += fs_2d::single::sweepX::InternalLoop_Rolled_CNT_V5_X_(r, s, lastS);
                        r++;
                    }

                    return result;
                };

                inline unsigned long long ForwardScanBased_PlaneSweep_CNT_X(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, int runNumPartition)
                {
                    unsigned long long result = 0;

                    for (int pid = 0; pid < runNumPartition; pid++)
                    {
                        if ( (pRA_size[pid] > 0) && (pSA_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], 0, pRA_size[pid], 0, pSA_size[pid] );
                        }

                        if ( (pRA_size[pid] > 0) && (pSB_size[pid] > pSA_size[pid])){
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], 0, pRA_size[pid], pSA_size[pid], pSB_size[pid]);
                        }

                        if ( (pRA_size[pid] > 0) && (pSC_size[pid] > pSB_size[pid])){
                            result += Sweep_Rolled_CNT_X_( pS[pid], pR[pid], pSB_size[pid], pSC_size[pid], 0, pRA_size[pid]);
                        }

                        if ( (pRA_size[pid] > 0) && (pSD_size[pid] > pSC_size[pid])){
                            result += Sweep_Rolled_CNT_X_(pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], 0, pRA_size[pid]);
                        }

                        if ( (pRB_size[pid] > pRA_size[pid]) && (pSA_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_X_(pS[pid], pR[pid], 0, pSA_size[pid], pRA_size[pid], pRB_size[pid]); 
                        }

                        if ( (pRB_size[pid] > pRA_size[pid]) && (pSC_size[pid] > pSB_size[pid])){
                            result += Sweep_Rolled_CNT_X_(pS[pid], pR[pid], pSB_size[pid], pSC_size[pid], pRA_size[pid], pRB_size[pid]);  

                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSA_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], pRB_size[pid], pRC_size[pid], 0, pSA_size[pid]);   
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSB_size[pid] > pSA_size[pid])){
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], pRB_size[pid], pRC_size[pid], pSA_size[pid], pSB_size[pid]);    
                        }

                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSA_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], 0, pSA_size[pid]);    
                        }
                    }

                    return result;
                };


                inline unsigned long long ForwardScanBased_PlaneSweep_CNT_X(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, int runNumPartition, double* tileTime)
                {
                    unsigned long long result = 0;
                    Timer tim;

                    for (int pid = 0; pid < runNumPartition; pid++)
                    {
                        if ( (pRA_size[pid] > 0) && (pSA_size[pid] > 0)){
                            tim.start();
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], 0, pRA_size[pid], 0, pSA_size[pid] );
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRA_size[pid] > 0) && (pSB_size[pid] > pSA_size[pid])){
                            tim.start();
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], 0, pRA_size[pid], pSA_size[pid], pSB_size[pid]);
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRA_size[pid] > 0) && (pSC_size[pid] > pSB_size[pid])){
                            tim.start();
                            result += Sweep_Rolled_CNT_X_( pS[pid], pR[pid], pSB_size[pid], pSC_size[pid], 0, pRA_size[pid]);
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRA_size[pid] > 0) && (pSD_size[pid] > pSC_size[pid])){
                            tim.start();
                            result += Sweep_Rolled_CNT_X_(pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], 0, pRA_size[pid]);
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRB_size[pid] > pRA_size[pid]) && (pSA_size[pid] > 0)){
                            tim.start();
                            result += Sweep_Rolled_CNT_X_(pS[pid], pR[pid], 0, pSA_size[pid], pRA_size[pid], pRB_size[pid]); 
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRB_size[pid] > pRA_size[pid]) && (pSC_size[pid] > pSB_size[pid])){
                            tim.start();
                            result += Sweep_Rolled_CNT_X_(pS[pid], pR[pid], pSB_size[pid], pSC_size[pid], pRA_size[pid], pRB_size[pid]); 
                            tileTime[pid] += tim.stop(); 

                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSA_size[pid] > 0)){
                            tim.start();
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], pRB_size[pid], pRC_size[pid], 0, pSA_size[pid]);   
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSB_size[pid] > pSA_size[pid])){
                            tim.start();
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], pRB_size[pid], pRC_size[pid], pSA_size[pid], pSB_size[pid]); 
                            tileTime[pid] += tim.stop();   
                        }

                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSA_size[pid] > 0)){
                            tim.start();
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], 0, pSA_size[pid]);
                            tileTime[pid] += tim.stop();    
                        }
                    }

                    return result;
                };

                inline unsigned long long ForwardScanBased_PlaneSweep_CNT_X_Less(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, int runNumPartition)
                {
                    unsigned long long result = 0;

                    for (int pid = 0; pid < runNumPartition; pid++)
                    {
                        if ( (pRA_size[pid] > 0) && (pSA_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_X_(pR[pid], pS[pid], 0, pRA_size[pid], 0, pSA_size[pid] );
                        }

                        if ( (pRA_size[pid] > 0) && (pSB_size[pid] > pSA_size[pid])){
                            result +=Sweep_Rolled_CNT_V3_X_(pR[pid], pS[pid], 0, pRA_size[pid], pSA_size[pid], pSB_size[pid]);
                        }
							
                        if ( (pRA_size[pid] > 0) && (pSC_size[pid] > pSB_size[pid])){
                            result += Sweep_Rolled_CNT_V2_X_( pS[pid], pR[pid], pSB_size[pid], pSC_size[pid], 0, pRA_size[pid]);
                        }

                        if ( (pRA_size[pid] > 0) && (pSD_size[pid] > pSC_size[pid])){
                            result += Sweep_Rolled_CNT_V5_X_(pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], 0, pRA_size[pid]);
                        }

                        if ( (pRB_size[pid] > pRA_size[pid]) && (pSA_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_V3_X_(pS[pid], pR[pid], 0, pSA_size[pid], pRA_size[pid], pRB_size[pid]); 
                        }

                        if ( (pRB_size[pid] > pRA_size[pid]) && (pSC_size[pid] > pSB_size[pid])){
                            result += Sweep_Rolled_CNT_V4_X_(pS[pid], pR[pid], pSB_size[pid], pSC_size[pid], pRA_size[pid], pRB_size[pid]);  

                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSA_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_V2_X_(pR[pid], pS[pid], pRB_size[pid], pRC_size[pid], 0, pSA_size[pid]);   
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSB_size[pid] > pSA_size[pid])){
                            result += Sweep_Rolled_CNT_V4_X_(pR[pid], pS[pid], pRB_size[pid], pRC_size[pid], pSA_size[pid], pSB_size[pid]);    
                        }

                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSA_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_V5_X_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], 0, pSA_size[pid]);    
                        }
                    }

                    return result;
                };

            }
        }
        
        
        namespace sweepY{
            
            inline unsigned long long InternalLoop_Rolled_CNT_Y_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/,int flag)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->yEnd >= pivot->yStart))
                {
                    if ((rec->xStart > pivot->xEnd) || (rec->xEnd < pivot->xStart))
                    {
                        pivot++;
                        continue;
                    }

                    if(flag == 0){
                        //cout << rec->id << " " << pivot->id << endl;
                        //mbr_filter_output_pairs.emplace_back(rec->id, pivot->id);
                        forwardCandidatePair(rec->id, pivot->id);
                    }else{
                        //cout << pivot->id << " " << rec->id << endl;
                        //mbr_filter_output_pairs.emplace_back(pivot->id, rec->id);
                        forwardCandidatePair(pivot->id, rec->id);
                    }
                    result++;
                    pivot++;
                }

                return result;
            };
            
            
            inline unsigned long long InternalLoop_Rolled_CNT_V2_Y_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/, int flag)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->yEnd >= pivot->yStart))
                {
                    if ((rec->xStart > pivot->xEnd) || (rec->xEnd < pivot->xStart))
                    {
                        pivot++;
                        continue;
                    }
                    if(flag == 0){
                        //cout << rec->id << " " << pivot->id << endl;
                        //mbr_filter_output_pairs.emplace_back(rec->id, pivot->id);
                        forwardCandidatePair(rec->id, pivot->id);
                    }else{
                        //cout << pivot->id << " " << rec->id << endl;
                        //mbr_filter_output_pairs.emplace_back(pivot->id, rec->id);
                        forwardCandidatePair(pivot->id, rec->id);
                    }
                    result++;
                    pivot++;
                }

                return result;
            };
            
            
            inline unsigned long long InternalLoop_Rolled_CNT_V3_1_Y_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/, int flag)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->yEnd >= pivot->yStart))
                {        
                    if (rec->xStart > pivot->xEnd)
                    {
                        pivot++;
                        continue;
                    }

                    if(flag == 0){
                        //cout << rec->id << " " << pivot->id << endl;
                        //mbr_filter_output_pairs.emplace_back(rec->id, pivot->id);
                        forwardCandidatePair(rec->id, pivot->id);
                    }else{
                        //cout << pivot->id << " " << rec->id << endl;
                        //mbr_filter_output_pairs.emplace_back(pivot->id, rec->id);
                        forwardCandidatePair(pivot->id, rec->id);
                    }
                    result++;
                    pivot++;
                }

                return result;
            };
            

            inline unsigned long long InternalLoop_Rolled_CNT_V3_2_Y_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/, int flag)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->yEnd >= pivot->yStart))
                {       
                    if (pivot->xStart > rec->xEnd)
                    {
                        pivot++;
                        continue;
                    }
                    if(flag == 0){
                        //cout << rec->id << " " << pivot->id << endl;
                        //mbr_filter_output_pairs.emplace_back(rec->id, pivot->id);
                        forwardCandidatePair(rec->id, pivot->id);
                    }else{
                        //cout << pivot->id << " " << rec->id << endl;
                        //mbr_filter_output_pairs.emplace_back(pivot->id, rec->id);
                        forwardCandidatePair(pivot->id, rec->id);
                    }
                    result++;
                    pivot++;
                }

                return result;
            };
            
            
            inline unsigned long long InternalLoop_Rolled_CNT_V4_Y_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/, int flag)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->yEnd >= pivot->yStart))
                {
                    if (rec->xStart > pivot->xEnd)
                    {
                        pivot++;
                        continue;
                    }
                    if(flag == 0){
                        //cout << rec->id << " " << pivot->id << endl;
                        //mbr_filter_output_pairs.emplace_back(rec->id, pivot->id);
                        forwardCandidatePair(rec->id, pivot->id);
                    }else{
                        //cout << pivot->id << " " << rec->id << endl;
                        //mbr_filter_output_pairs.emplace_back(pivot->id, rec->id);
                        forwardCandidatePair(pivot->id, rec->id);
                    }
                    result++;
                    pivot++;
                }

                return result;
            };
            
            
            inline unsigned long long InternalLoop_Rolled_CNT_V5_Y_(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS/*, unsigned long long &count*/, int flag)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (rec->yEnd >= pivot->yStart))
                {
                    if ( rec->xEnd < pivot->xStart)
                    {
                        pivot++;
                        continue;
                    }
                    if(flag == 0){
                        //cout << rec->id << " " << pivot->id << endl;
                        //mbr_filter_output_pairs.emplace_back(rec->id, pivot->id);
                        forwardCandidatePair(rec->id, pivot->id);
                    }else{
                        //cout << pivot->id << " " << rec->id << endl;
                        //mbr_filter_output_pairs.emplace_back(pivot->id, rec->id);
                        forwardCandidatePair(pivot->id, rec->id);
                    }
                    result++;
                    pivot++;
                }

                return result;
            };         


            inline unsigned long long InternalLoop_Rolled_CNT_Y_Dec(RecordId id, Coord xStart, Coord xEnd, Coord yEnd, vector<ABrec>::const_iterator firstFS, vector<ABrec>::const_iterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;

                auto pivot = firstFS;

                while ((pivot < lastFS) && (yEnd >= pivot->yStart))
                {
                    if ((xStart > pivot->xEnd) || (xEnd < pivot->xStart))
                    {
                        pivot++;
                        continue;
                    }
                    
                    result++;
                    pivot++;
                }

                 

                return result;
            };


            inline unsigned long long InternalLoop_Rolled_CNT_V2_Y_Dec(RecordId id, Coord xStart, Coord xEnd, Coord yEnd, vector<ABrec>::const_iterator firstFS, vector<ABrec>::const_iterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (yEnd >= pivot->yStart))
                {
                    if ((xStart > pivot->xEnd) || (xEnd < pivot->xStart))
                    {
                        pivot++;
                        continue;
                    }
                    result++;
                    pivot++;
                }


                return result;
            };



            inline unsigned long long InternalLoop_Rolled_CNT_V3_1_Y_Dec(RecordId id, Coord xStart, Coord  yEnd, vector<Crec>::const_iterator firstFS, vector<Crec>::const_iterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (yEnd >= pivot->yStart))
                {        
                    if (xStart > pivot->xEnd)
                    {
                        pivot++;
                        continue;
                    }
                    result ++;
                    pivot++;
                }

                return result;
            };


            inline unsigned long long InternalLoop_Rolled_CNT_V3_2_Y_Dec(RecordId id, Coord xEnd, Coord yEnd, vector<ABrec>::const_iterator firstFS, vector<ABrec>::const_iterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (yEnd >= pivot->yStart))
                {       
                    if (pivot->xStart > xEnd)
                    {
                        pivot++;
                        continue;
                    }
                    
                    result++;
                    pivot++;
                }

                return result;
            };


            inline unsigned long long InternalLoop_Rolled_CNT_V4_Y_Dec(RecordId id, Coord xStart, Coord yEnd, vector<Crec>::const_iterator firstFS, vector<Crec>::const_iterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (yEnd >= pivot->yStart))
                {
                    if (xStart > pivot->xEnd)
                    {
                        pivot++;
                        continue;
                    }

                    result++;
                    pivot++;
                }

                return result;
            };

            inline unsigned long long InternalLoop_Rolled_CNT_V5_Y_Dec(RecordId id, Coord yEnd, Coord xEnd, vector<ABrec>::const_iterator firstFS, vector<ABrec>::const_iterator lastFS/*, unsigned long long &count*/)
            {
                unsigned long long result = 0;
                auto pivot = firstFS;

                while ((pivot < lastFS) && (yEnd >= pivot->yStart))
                {
                    if ( xEnd < pivot->xStart)
                    {
                        pivot++;
                        continue;
                    }

                    result ++;
                    
                    pivot++;
                }

                return result;
            };   
            
            namespace oneArray{
                
                inline unsigned long long Sweep_Rolled_CNT_Y_(Relation &R, Relation &S, size_t startR, size_t endR, size_t startS, size_t endS)
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS;

                    while ((r < lastR) && (s < lastS))
                    {
                        if (r->yStart < s->yStart)
                        {
                            // Run internal loop.
                            //cout << "Sweep_Rolled_CNT_Y_ 0" << endl;
                            result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_Y_(r, s, lastS, 0 );
                            r++;
                        }
                        else
                        {
                            // Run internal loop.
                            //cout << "Sweep_Rolled_CNT_Y_ 1" << endl;
                            result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_Y_(s, r, lastR, 1 );
                            s++;
                        }
                    }

                    return result;
                };


                inline unsigned long long Sweep_Rolled_CNT_V2_Y_(Relation &R, Relation &S, size_t startR, size_t endR, size_t startS, size_t endS, int flag)
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS;

                    while ((r < lastR))
                    {
                        //cout << "Sweep_Rolled_CNT_V2_Y_ " << flag << endl;
                        result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V2_Y_(r, s, lastS, flag );
                        r++;
                    }

                    return result;
                };


                inline unsigned long long Sweep_Rolled_CNT_V3_Y_(Relation &R, Relation &S, size_t startR, size_t endR, size_t startS, size_t endS, int flag)
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS;

                    while ((r < lastR) && (s < lastS))
                    {
                        if (r->yStart < s->yStart)
                        {
                            // Run internal loop.
                            //cout << "Sweep_Rolled_CNT_V3_Y_ " << flag << endl;
                            result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V3_1_Y_(r, s, lastS, flag^1 );
                            r++;
                        }
                        else
                        {
                            // Run internal loop.
                            //cout << "Sweep_Rolled_CNT_V3_Y_ " << flag << endl;
                            result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V3_2_Y_(s, r, lastR, flag );
                            s++;
                        }
                    }

                    return result;
                };


                inline unsigned long long Sweep_Rolled_CNT_V4_Y_(Relation &R, Relation &S,size_t startR, size_t endR, size_t startS, size_t endS, int flag)
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS;

                    while ((r < lastR))
                    { 
                        // Run internal loop.
                        //cout << "Sweep_Rolled_CNT_V4_Y_ " << flag << endl;
                        result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V4_Y_(r, s, lastS, flag );
                        r++;
                    }

                    return result;
                };


                inline unsigned long long Sweep_Rolled_CNT_V5_Y_(Relation &R, Relation &S, size_t startR, size_t endR, size_t startS, size_t endS, int flag)
                {
                    unsigned long long result = 0;
                    auto r = R.begin() + startR;
                    auto s = S.begin() + startS;
                    auto lastR = R.begin() + endR;
                    auto lastS = S.begin() + endS;

                    while ((r < lastR))
                    {
                        // Run internal loop.
                        //cout << "Sweep_Rolled_CNT_V5_Y_ " << flag << endl;
                        result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V5_Y_(r, s, lastS, flag);
                        r++;
                    }

                    return result;
                };




                inline unsigned long long Sweep_Rolled_CNT_Y_Dec(vector<ABrec>& pRABdec, vector<ABrec>& pSABdec, vector<Coord>& pRYEND, vector<Coord>& pSYEND, size_t startR, size_t endR, size_t startS, size_t endS)
                {
                    unsigned long long result = 0;


                    auto rAB = pRABdec.begin() + startR;
                    auto sAB = pSABdec.begin() + startS;
                    auto lastRAB = pRABdec.begin() + endR;
                    auto lastSAB = pSABdec.begin() + endS;

                    auto rYEND = pRYEND.begin() + startR;
                    auto sYEND = pSYEND.begin() + startS;
                    auto lastRYEND = pRYEND.begin() + endR;
                    auto lastSYEND = pSYEND.begin() + endS;



                    while ((rAB < lastRAB) && (sAB < lastSAB))
                    {
                        if (rAB->yStart < sAB->yStart)
                        {
                            // Run internal loop.                                          //rYEND->id,rYEND->yEnd, rAB->xStart,rAB->xEnd, sAB, lastSAB 
                            result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_Y_Dec(rAB->id, rAB->xStart,rAB->xEnd, *rYEND, sAB, lastSAB );
                            rAB++;
                            rYEND++;
                        }
                        else
                        {
                            // Run internal loop.                                         //rYEND->id,rYEND->yEnd, rAB->xStart,rAB->xEnd, sAB, lastSAB 
                            result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_Y_Dec(sAB->id, sAB->xStart,sAB->xEnd, *sYEND, rAB, lastRAB );
                            sAB++;
                            sYEND++;
                        }
                    }

                    return result;
                };


                inline unsigned long long Sweep_Rolled_CNT_V2_Y_Dec(vector<ABrec>& pRABdec, vector<ABrec>& pSABdec, vector<Coord>& pYEND, size_t startR, size_t endR, size_t startS, size_t endS)
                {

                    unsigned long long result = 0;
                    auto rAB = pRABdec.begin() + startR;
                    auto sAB = pSABdec.begin() + startS;
                    auto lastRAB = pRABdec.begin() + endR;
                    auto lastSAB = pSABdec.begin() + endS;

                    auto yEND = pYEND.begin() + startR;

                    while ((rAB< lastRAB))
                    {                                                                   //rYEND->id,rYEND->yEnd, rAB->xStart,rAB->xEnd, sAB, lastSAB
                        result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V2_Y_Dec(rAB->id, rAB->xStart,rAB->xEnd, *yEND, sAB, lastSAB );
                        rAB++;
                        yEND++;
                    }

                    return result;

                };


                inline unsigned long long Sweep_Rolled_CNT_V3_Y_Dec(vector<ABrec>& pRABdec, vector<Crec>& pSCdec, vector<Coord>& pRYEND, vector<Coord>& pSYEND, size_t startR, size_t endR, size_t startS)
                {
                    unsigned long long result = 0;
                    auto rAB = pRABdec.begin() + startR;
                    auto sC = pSCdec.begin();
                    auto lastRAB = pRABdec.begin() + endR;
                    auto lastSC = pSCdec.end();

                    auto rYEND = pRYEND.begin() + startR;
                    auto sYEND = pSYEND.begin() + startS;
                    auto lastRYEND = pRYEND.begin() + endR;
                    auto lastSYEND = pSYEND.end();


                    while ((rAB < lastRAB) && (sC < lastSC))
                    {
                        if (rAB->yStart < sC->yStart)
                        {
                            // Run internal loop.
                            result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V3_1_Y_Dec(rAB->id,rAB->xStart, *rYEND, sC, lastSC );
                            rAB++;
                            rYEND++;
                        }
                        else
                        {
                            // Run internal loop.
                            result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V3_2_Y_Dec(sC->id, sC->xEnd, *sYEND, rAB, lastRAB );
                            sC++;
                            sYEND++;
                        }
                    }


                    return result;
                };


                inline unsigned long long Sweep_Rolled_CNT_V4_Y_Dec(vector<ABrec> &pRABdec, vector<Crec> &pSCdec, vector<Coord> &pYEND,size_t startR, size_t endR)
                {
                    unsigned long long result = 0;
                    auto rAB = pRABdec.begin() + startR;
                    auto sC = pSCdec.begin();
                    auto lastRAB = pRABdec.begin() + endR;
                    auto lastSC = pSCdec.end();


                    auto yEND = pYEND.begin() + startR;
                    auto lastYEND = pYEND.begin() + endR;

                    while ((rAB < lastRAB))
                    { 
                        // Run internal loop.                                            //rAB->id, rYEND->yEnd, rAB->xStart, sC, lastSC
                        result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V4_Y_Dec(rAB->id, rAB->xStart, *yEND, sC, lastSC );
                        rAB++;
                        yEND++;
                    }

                    return result;
                };


                inline unsigned long long Sweep_Rolled_CNT_V5_Y_Dec(vector<Drec> &pSDdec, vector<ABrec> &pRABdec, size_t startS, size_t endS)
                {
                    unsigned long long result = 0;
                    auto rD = pSDdec.begin();
                    auto sAB = pRABdec.begin() + startS;
                    auto lastRD = pSDdec.end();
                    auto lastSAB = pRABdec.begin() + endS;

                    while ((rD < lastRD))
                    {
                        // Run internal loop.
                        result += fs_2d::single::sweepY::InternalLoop_Rolled_CNT_V5_Y_Dec(rD->id, rD->yEnd, rD->xEnd, sAB, lastSAB);
                        rD++;
                    }

                    return result;
                };


                inline unsigned long long ForwardScanBased_PlaneSweep_CNT_Y(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, int runNumPartition)
                {
                    unsigned long long result = 0;

                    for (int pid = 0; pid < runNumPartition; pid++)
                    {
                        if ( (pRB_size[pid] > 0) && (pSB_size[pid] > 0)){
                            //tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], 0, pRB_size[pid], 0, pSB_size[pid] );
                            //timeP1 += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pSC_size[pid] > pSB_size[pid])){
                            //tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], 0, pRB_size[pid], pSB_size[pid], pSC_size[pid]);
                            //timeP2 += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pSD_size[pid] > pSC_size[pid])){
                            //tim.start();
                            result += Sweep_Rolled_CNT_Y_( pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], 0, pRB_size[pid]);
                            //timeP3 += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pS[pid].size() > pSD_size[pid])){
                            //tim.start();
                            result += Sweep_Rolled_CNT_Y_(pS[pid], pR[pid], pSD_size[pid], pS[pid].size(), 0, pRB_size[pid]);
                            //timeP4 += tim.stop();
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSB_size[pid] > 0)){
                            //tim.start();
                            result += Sweep_Rolled_CNT_Y_(pS[pid], pR[pid], 0, pSB_size[pid], pRB_size[pid], pRC_size[pid]); 
                            //timeP5 += tim.stop();
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSD_size[pid] > pSC_size[pid])){
                            //tim.start();
                            result += Sweep_Rolled_CNT_Y_(pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], pRB_size[pid], pRC_size[pid]);  
                            //timeP6 += tim.stop();

                        }

                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSB_size[pid] > 0)){
                            //tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], 0, pSB_size[pid]);   
                            //timeP7 += tim.stop();
                        }

                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSC_size[pid] > pSB_size[pid])){
                            //tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], pSB_size[pid], pSC_size[pid]);    
                            //timeP8 += tim.stop();
                        }

                        if ( (pR[pid].size() > pRD_size[pid]) && (pSB_size[pid] > 0)){
                            // tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], pRD_size[pid], pR[pid].size(), 0, pSB_size[pid]);    
                            //timeP9 += tim.stop();
                        }
                    }

                    return result;
                };
            

                inline unsigned long long ForwardScanBased_PlaneSweep_CNT_Y(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, int runNumPartition, double* tileTime)
                {
                    unsigned long long result = 0;
                    Timer tim;

                    for (int pid = 0; pid < runNumPartition; pid++)
                    {

                        if ( (pRB_size[pid] > 0) && (pSB_size[pid] > 0)){
                            tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], 0, pRB_size[pid], 0, pSB_size[pid] );
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pSC_size[pid] > pSB_size[pid])){
                            tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], 0, pRB_size[pid], pSB_size[pid], pSC_size[pid]);
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pSD_size[pid] > pSC_size[pid])){
                            tim.start();
                            result += Sweep_Rolled_CNT_Y_( pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], 0, pRB_size[pid]);
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pS[pid].size() > pSD_size[pid])){
                            tim.start();
                            result += Sweep_Rolled_CNT_Y_(pS[pid], pR[pid], pSD_size[pid], pS[pid].size(), 0, pRB_size[pid]);
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSB_size[pid] > 0)){
                            tim.start();
                            result += Sweep_Rolled_CNT_Y_(pS[pid], pR[pid], 0, pSB_size[pid], pRB_size[pid], pRC_size[pid]); 
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSD_size[pid] > pSC_size[pid])){
                            
                            tim.start();
                            result += Sweep_Rolled_CNT_Y_(pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], pRB_size[pid], pRC_size[pid]);  
                            tileTime[pid] += tim.stop();

                        }

                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSB_size[pid] > 0)){
                            tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], 0, pSB_size[pid]);   
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSC_size[pid] > pSB_size[pid])){
                            tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], pSB_size[pid], pSC_size[pid]);    
                            tileTime[pid] += tim.stop();
                        }

                        if ( (pR[pid].size() > pRD_size[pid]) && (pSB_size[pid] > 0)){
                            tim.start();
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], pRD_size[pid], pR[pid].size(), 0, pSB_size[pid]);    
                            tileTime[pid] += tim.stop();
                        }
                    }

                    return result;
                };

                inline unsigned long long ForwardScanBased_PlaneSweep_CNT_Y2(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, int runNumPartition)
                {
                    unsigned long long result = 0;
                
                    for (int pid = 0; pid < runNumPartition; pid++)
                    {
                        if ( (pRB_size[pid] > 0) && (pSB_size[pid] > 0)){
                            //tim.start();
                            //result += Sweep_Rolled_XOR_Y_(pR[pid], pS[pid], 0, pRB_size[pid], 0, pSB_size[pid] );

                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], 0, pRB_size[pid], 0, pSB_size[pid] );
                            //timeP1 += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pSC_size[pid] > pSB_size[pid])){
                            //tim.start();
                            //result += Sweep_Rolled_XOR_Y_(pR[pid], pS[pid], 0, pRB_size[pid], pSB_size[pid], pSC_size[pid]);

                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], 0, pRB_size[pid], pSB_size[pid], pSC_size[pid]);
                            //timeP2 += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pSD_size[pid] > pSC_size[pid])){
                            //tim.start();
                            //result += Sweep_Rolled_XOR_Y_( pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], 0, pRB_size[pid]);

                            result += Sweep_Rolled_CNT_Y_( pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], 0, pRB_size[pid]);
                            //timeP3 += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pS[pid].size() > pSD_size[pid])){
                            //tim.start();
                            //result += Sweep_Rolled_XOR_Y_(pS[pid], pR[pid], pSD_size[pid], pS[pid].size(), 0, pRB_size[pid]);

                            result += Sweep_Rolled_CNT_Y_(pS[pid], pR[pid], pSD_size[pid], pS[pid].size(), 0, pRB_size[pid]);
                            //timeP4 += tim.stop();
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSB_size[pid] > 0)){
                            //tim.start();
                            //result += Sweep_Rolled_XOR_Y_(pS[pid], pR[pid], 0, pSB_size[pid], pRB_size[pid], pRC_size[pid]); 

                            result += Sweep_Rolled_CNT_Y_(pS[pid], pR[pid], 0, pSB_size[pid], pRB_size[pid], pRC_size[pid]); 
                            //timeP5 += tim.stop();
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSD_size[pid] > pSC_size[pid])){
                            //tim.start();
                            //result += Sweep_Rolled_XOR_Y_(pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], pRB_size[pid], pRC_size[pid]); 

                            result += Sweep_Rolled_CNT_Y_(pS[pid], pR[pid], pSC_size[pid], pSD_size[pid], pRB_size[pid], pRC_size[pid]); 
                            //timeP6 += tim.stop();

                        }

                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSB_size[pid] > 0)){
                            //tim.start();
                            //result += Sweep_Rolled_XOR_Y_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], 0, pSB_size[pid]);   

                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], 0, pSB_size[pid]); 
                            //timeP7 += tim.stop();
                        }

                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSC_size[pid] > pSB_size[pid])){
                            //tim.start();
                            //result += Sweep_Rolled_XOR_Y_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], pSB_size[pid], pSC_size[pid]);

                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], pRC_size[pid], pRD_size[pid], pSB_size[pid], pSC_size[pid]);    
                            //timeP8 += tim.stop();
                        }

                        if ( (pR[pid].size() > pRD_size[pid]) && (pSB_size[pid] > 0)){
                           // tim.start();
                            //result += Sweep_Rolled_XOR_Y_(pR[pid], pS[pid], pRD_size[pid], pR[pid].size(), 0, pSB_size[pid]);  

                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], pRD_size[pid], pR[pid].size(), 0, pSB_size[pid]);   
                            //timeP9 += tim.stop();
                        }
                    }
           
                    return result;
                };


                inline unsigned long long ForwardScanBased_PlaneSweep_CNT_Y_Less(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, int runNumPartition)
                {
                    unsigned long long result = 0;

                    for (int pid = 0; pid < runNumPartition; pid++)
                    {
                        if ( (pRB_size[pid] > 0) && (pSB_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_Y_(pR[pid], pS[pid], 0 , pRB_size[pid], 0, pSB_size[pid]);
                        }

                        if ( (pRB_size[pid] > 0) && (pSC_size[pid] > pSB_size[pid])){
                            result +=Sweep_Rolled_CNT_V2_Y_( pS[pid], pR[pid], pSB_size[pid] , pSC_size[pid], 0, pRB_size[pid], 1);
                        }

                        if ( (pRB_size[pid] > 0) && (pSD_size[pid] > pSC_size[pid])){
                            result += Sweep_Rolled_CNT_V3_Y_( pR[pid], pS[pid], 0, pRB_size[pid], pSC_size[pid], pSD_size[pid], 1);
                        }

                        if ( (pRB_size[pid] > 0) && (pS[pid].size() > pSD_size[pid])){
                            result += Sweep_Rolled_CNT_V5_Y_(pS[pid], pR[pid], pSD_size[pid], pS[pid].size(), 0, pRB_size[pid], 1);
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSB_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_V2_Y_( pR[pid], pS[pid], pRB_size[pid], pRC_size[pid], 0, pSB_size[pid], 0);
                        }
                   
                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSD_size[pid] > pSC_size[pid])){
                            result += Sweep_Rolled_CNT_V4_Y_( pR[pid],pS[pid], pRB_size[pid], pRC_size[pid], pSC_size[pid], pSD_size[pid], 0);
                        }
                    
                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSB_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_V3_Y_( pS[pid],pR[pid], 0, pSB_size[pid], pRC_size[pid], pRD_size[pid], 0);
                        }
                    
                        if ( (pRD_size[pid] > pRC_size[pid]) && (pSC_size[pid] > pSB_size[pid])){
                            result += Sweep_Rolled_CNT_V4_Y_( pS[pid], pR[pid], pSB_size[pid], pSC_size[pid], pRC_size[pid], pRD_size[pid], 1);
                        }
                    
                        if ( (pR[pid].size() > pRD_size[pid]) && (pSB_size[pid] > 0)){
                            result += Sweep_Rolled_CNT_V5_Y_(pR[pid], pS[pid], pRD_size[pid], pR[pid].size(), 0, pSB_size[pid], 0);
                        }
                    }

                    return result;
                };

                inline unsigned long long  ForwardScanBased_PlaneSweep_CNT_Y_Less_Dec(vector<ABrec>* pRABdec, vector<ABrec>* pSABdec,  vector<Crec> *pRCdec, vector<Crec> *pSCdec, vector<Drec> *pRDdec, vector<Drec> *pSDdec, vector<Coord>* pRYEND, vector<Coord>* pSYEND, size_t * pRB_size, size_t * pSB_size, size_t * pRC_size, size_t * pSC_size, int runNumPartition)
                {
                    unsigned long long result = 0; 

                    Timer tim;
                    double timeP1 = 0, timeP2 = 0, timeP3 = 0, timeP4 = 0, timeP5 = 0;

                    for (int pid = 0; pid < runNumPartition; pid++)
                    {
                        if ( (pRB_size[pid] > 0) && (pSB_size[pid] > 0)){
                            //tim.start();
                            result += Sweep_Rolled_CNT_Y_Dec(pRABdec[pid], pSABdec[pid],pRYEND[pid] , pSYEND[pid], 0 , pRB_size[pid], 0, pSB_size[pid]);
                            //timeP1 += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pSC_size[pid] > pSB_size[pid])){
                            //tim.start();
                            result +=Sweep_Rolled_CNT_V2_Y_Dec(pSABdec[pid],pRABdec[pid] ,pSYEND[pid], pSB_size[pid] , pSC_size[pid], 0, pRB_size[pid]);
                            //timeP2 += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pSYEND[pid].size() > pSC_size[pid])){
                            //tim.start();
                            result += Sweep_Rolled_CNT_V3_Y_Dec( pRABdec[pid], pSCdec[pid],pRYEND[pid] , pSYEND[pid], 0, pRB_size[pid],  pSC_size[pid]);
                            //timeP3 += tim.stop();
                        }

                        if ( (pRB_size[pid] > 0) && (pSDdec[pid].size() > 0)){
                            //tim.start();
                            result += Sweep_Rolled_CNT_V5_Y_Dec(pSDdec[pid], pRABdec[pid], 0, pRB_size[pid]);
                            //timeP5 += tim.stop();
                        }

                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSB_size[pid] > 0)){
                            //tim.start();
                            result += Sweep_Rolled_CNT_V2_Y_Dec( pRABdec[pid], pSABdec[pid], pRYEND[pid] ,pRB_size[pid], pRC_size[pid], 0, pSB_size[pid]);
                            //timeP2 += tim.stop();
                        }
                   
                        if ( (pRC_size[pid] > pRB_size[pid]) && (pSCdec[pid].size() > 0)){
                            //tim.start();
                            result += Sweep_Rolled_CNT_V4_Y_Dec( pRABdec[pid],pSCdec[pid], pRYEND[pid], pRB_size[pid], pRC_size[pid]);
                            //timeP4 += tim.stop();
                        }
                    
                        if ( (pRCdec[pid].size() > 0) && (pSB_size[pid] > 0)){
                            //tim.start();
                            result += Sweep_Rolled_CNT_V3_Y_Dec( pSABdec[pid],pRCdec[pid],pSYEND[pid] , pRYEND[pid], 0, pSB_size[pid], pRC_size[pid]);
                            //timeP3 += tim.stop();
                        }
                    
                        if ( (pRCdec[pid].size() > 0) && (pSC_size[pid] > pSB_size[pid])){
                            //tim.start();                        
                            result += Sweep_Rolled_CNT_V4_Y_Dec(pSABdec[pid], pRCdec[pid], pSYEND[pid], pSB_size[pid], pSC_size[pid]);
                            //timeP4 += tim.stop();
                        }
                    
                        if ( (pRDdec[pid].size() > 0) && (pSB_size[pid] > 0)){
                            //tim.start();                        
                            result += Sweep_Rolled_CNT_V5_Y_Dec(pRDdec[pid], pSABdec[pid], 0, pSB_size[pid]);
                            //timeP5 += tim.stop();
                        }
                        
                    }

                    return result;
                };

            }
        }
        
        
        unsigned long long ForwardScanBased_PlaneSweep_CNT(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, bool runPlaneSweepOnX, int runNumPartitionsPerRelation)
        {
            int runNumPartition = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
            if (runPlaneSweepOnX){//x
                return fs_2d::single::sweepX::oneArray::ForwardScanBased_PlaneSweep_CNT_X(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartition);
            }
            else{//y
                return fs_2d::single::sweepY::oneArray::ForwardScanBased_PlaneSweep_CNT_Y(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartition);
            }

        };

        unsigned long long ForwardScanBased_PlaneSweep_CNT(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, bool runPlaneSweepOnX, int runNumPartitionsPerRelation, double* tileTime)
        {
            int runNumPartition = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
            if (runPlaneSweepOnX){//x
                return fs_2d::single::sweepX::oneArray::ForwardScanBased_PlaneSweep_CNT_X(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartition, tileTime);
            }
            else{//y
                return fs_2d::single::sweepY::oneArray::ForwardScanBased_PlaneSweep_CNT_Y(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartition, tileTime);
            }

        };


        unsigned long long ForwardScanBased_PlaneSweep_CNT_Dec(vector<ABrec>* pRABdec, vector<ABrec>* pSABdec,  vector<Crec> *pRCdec, vector<Crec> *pSCdec, vector<Drec> *pRDdec, vector<Drec> *pSDdec, vector<Coord>* pRYEND, vector<Coord>* pSYEND, size_t * pRB_size, size_t * pSB_size, size_t * pRC_size, size_t * pSC_size, bool runPlaneSweepOnX, int runNumPartitionsPerRelation)
        {
            int runNumPartition = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
            if (runPlaneSweepOnX){//x
                //return fs_2d::single::sweepX::oneArray::ForwardScanBased_PlaneSweep_XOR_X_Less(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartition);
            }
            else{//y
                return fs_2d::single::sweepY::oneArray::ForwardScanBased_PlaneSweep_CNT_Y_Less_Dec(pRABdec, pSABdec, pRCdec, pSCdec, pRDdec, pSDdec, pRYEND, pSYEND, pRB_size, pSB_size, pRC_size, pSC_size, runNumPartition);
            }

        };

        unsigned long long ForwardScanBased_PlaneSweep_CNT2(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, bool runPlaneSweepOnX, int runNumPartitionsPerRelation)
        {
            //cout<<"simple"<<endl;
            int runNumPartition = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
            if (runPlaneSweepOnX){//x
                //return fs_2d::single::sweepX::oneArray::ForwardScanBased_PlaneSweep_XOR_X(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartition);
            }
            else{//y
            //cout<<"simple222"<<endl;
                return fs_2d::single::sweepY::oneArray::ForwardScanBased_PlaneSweep_CNT_Y2(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartition);
            }

        };


        unsigned long long ForwardScanBased_PlaneSweep_CNT_Less(Relation *pR, Relation *pS, size_t *pRA_size, size_t *pSA_size, size_t *pRB_size, size_t *pSB_size, size_t *pRC_size, size_t *pSC_size, size_t *pRD_size, size_t *pSD_size, bool runPlaneSweepOnX, int runNumPartitionsPerRelation)
        {
            int runNumPartition = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
            if (runPlaneSweepOnX){//x
                return fs_2d::single::sweepX::oneArray::ForwardScanBased_PlaneSweep_CNT_X_Less(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartition);
            }
            else{//y
                return fs_2d::single::sweepY::oneArray::ForwardScanBased_PlaneSweep_CNT_Y_Less(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runNumPartition);
            }

        };

    }
    
}
