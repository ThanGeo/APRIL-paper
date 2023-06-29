#include "../../containers/relation.h"
#include <iostream>     // std::cout, std::fixed
#include <iomanip>  

namespace ditt_2d{

    ///////////////////////////////////////
    // PROCESSING_DITTRICH_DEDUPLICATION //
    ///////////////////////////////////////

    
    vector<pair<uint,uint>> mbr_filter_output_pairs;
   


    inline unsigned long long InternalLoop_Rolled_CNT_X(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS, int pid, int runNumPartitionsPerRelation, Coord partitionExtent/*, unsigned long long &count, unsigned long long &count2*/)
    {
        unsigned long long result = 0;//, count = 0;
        auto pivot = firstFS;

        while ((pivot < lastFS) && (rec->xEnd >= pivot->xStart))
        {
            if ((rec->yStart > pivot->yEnd) || (rec->yEnd < pivot->yStart)) {
                pivot++;
                continue;
            }

            auto x = max(rec->xStart, pivot->xStart);
            auto y = min(rec->yEnd, pivot->yEnd);
            auto pid_ref = findReferenceCell1(x, y, partitionExtent, runNumPartitionsPerRelation);

            if (pid_ref == pid)
            {
                    result++;
            }
            pivot++;
        }

        return result;
    };



    inline unsigned long long InternalLoop_Rolled_CNT_Y(RelationIterator rec, RelationIterator firstFS, RelationIterator lastFS, int pid, int runNumPartitionsPerRelation, Coord partitionExtent/*, unsigned long long &count, unsigned long long &count2*/, int flag)
    {
        unsigned long long result = 0;//, count = 0;
        auto pivot = firstFS;
        while ((pivot < lastFS) && (rec->yEnd >= pivot->yStart))
        {
            if ((rec->xStart > pivot->xEnd) || (rec->xEnd < pivot->xStart)) {
                pivot++;
                continue;
            }

            auto x = max(rec->xStart, pivot->xStart);
            auto y = min(rec->yEnd, pivot->yEnd);
            auto pid_ref = findReferenceCell1(x, y, partitionExtent, runNumPartitionsPerRelation);

            if (pid_ref == pid)
            {
                result++;
                if(flag == 0){
                    //rec = A
                     cout << rec->id << " " << pivot->id << endl;
                     mbr_filter_output_pairs.emplace_back(rec->id, pivot->id);
                }else{
                     cout << pivot->id << " " << rec->id << endl;
                     mbr_filter_output_pairs.emplace_back(pivot->id, rec->id);
                }
               
                //cout<<pivot->id<<endl;
            }
            pivot++;
        }

        return result;
    };


    inline unsigned long long Sweep_Rolled_CNT_X(Relation &R, Relation &S, int pid, int runNumPartitionsPerRelation, Coord partitionExtent/*, unsigned long long &count, unsigned long long &count2*/)
    {
        unsigned long long result = 0;
        auto r = R.begin();
        auto s = S.begin();
        auto lastR = R.end();
        auto lastS = S.end();

        while ((r < lastR) && (s < lastS))
        {
            if (*r < *s)
            {
                // Run internal loop.
                result += InternalLoop_Rolled_CNT_X(r, s, lastS, pid, runNumPartitionsPerRelation, partitionExtent/*, count, count2*/);
                r++;
            }
            else
            {
                // Run internal loop.
                result += InternalLoop_Rolled_CNT_X(s, r, lastR, pid, runNumPartitionsPerRelation, partitionExtent/*, count, count2*/);
                s++;
            }
        }

        return result;
    };




    inline unsigned long long Sweep_Rolled_CNT_Y(Relation &R, Relation &S, int pid, int runNumPartitionsPerRelation, Coord partitionExtent)
    {
        unsigned long long result = 0;
        auto r = R.begin();
        auto s = S.begin();
        auto lastR = R.end();
        auto lastS = S.end();

        while ((r < lastR) && (s < lastS))
        {
            if (r->yStart < s->yStart)
            {
                // Run internal loop.
                result += InternalLoop_Rolled_CNT_Y(r, s, lastS, pid, runNumPartitionsPerRelation, partitionExtent, 0);
                r++;
            }
            else
            {
                // Run internal loop.
                result += InternalLoop_Rolled_CNT_Y(s, r, lastR, pid, runNumPartitionsPerRelation, partitionExtent, 1);
                s++;
            }
        }

        return result;
    };

    ////////////////////////////////
    // Single-threaded processing //
    ////////////////////////////////
    
    namespace single {
        
        
       
        inline unsigned long long ForwardScanBased_PlaneSweep_CNT_X_Rolled_DynamicScheduling(Relation *pR, Relation *pS, int runNumPartitionsPerRelation, Coord partitionExtent)
        {
            unsigned long long result = 0;
            int runNumPartitions = runNumPartitionsPerRelation * runNumPartitionsPerRelation;

            for (int pid = 0; pid < runNumPartitions; pid++)
            {
                if ((pR[pid].size() > 0) && (pS[pid].size() > 0))
                    result += ditt_2d::Sweep_Rolled_CNT_X(pR[pid], pS[pid], pid, runNumPartitionsPerRelation, partitionExtent);
            }

            return result;
        };



        inline unsigned long long ForwardScanBased_PlaneSweep_CNT_Y_Rolled_DynamicScheduling(Relation *pR, Relation *pS, int runNumPartitionsPerRelation, Coord partitionExtent)
        {
            unsigned long long result = 0;
            int runNumPartitions = runNumPartitionsPerRelation * runNumPartitionsPerRelation;

            for (int pid = 0; pid < runNumPartitions; pid++)
            {
                if ((pR[pid].size() > 0) && (pS[pid].size() > 0)){
                    result += ditt_2d::Sweep_Rolled_CNT_Y(pR[pid], pS[pid], pid, runNumPartitionsPerRelation, partitionExtent);
                }
            }
            return result;
        };


        inline unsigned long long ForwardScanBased_PlaneSweep_CNT_X_Rolled_DynamicScheduling(Relation *pR, Relation *pS, int runNumPartitionsPerRelation, Coord partitionExtent, double* tileTime)
        {
            unsigned long long result = 0;
            int runNumPartitions = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
            Timer tim;

            for (int pid = 0; pid < runNumPartitions; pid++)
            {
                if ((pR[pid].size() > 0) && (pS[pid].size() > 0)){
                    tim.start();
                    result += ditt_2d::Sweep_Rolled_CNT_X(pR[pid], pS[pid], pid, runNumPartitionsPerRelation, partitionExtent);
                    tileTime[pid] += tim.stop();
                }
            }

            return result;
        };



        inline unsigned long long ForwardScanBased_PlaneSweep_CNT_Y_Rolled_DynamicScheduling(Relation *pR, Relation *pS, int runNumPartitionsPerRelation, Coord partitionExtent, double* tileTime){
            unsigned long long result = 0;
            int runNumPartitions = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
            Timer tim;

            for (int pid = 0; pid < runNumPartitions; pid++){
                if ((pR[pid].size() > 0) && (pS[pid].size() > 0)){
                    // if( pid == 472537){
                    //     cout<<"pR size = "<< pR[pid].size()<<endl;
                    //     cout<<"pS size = "<< pS[pid].size()<<endl;
                    //     exit(0);
                    // }
                    
                    tim.start();
                    result += ditt_2d::Sweep_Rolled_CNT_Y(pR[pid], pS[pid], pid, runNumPartitionsPerRelation, partitionExtent);
                    tileTime[pid] += tim.stop();
                }
            }
            return result;
        };


        unsigned long long ForwardScanBased_PlaneSweep_CNT(Relation *pR, Relation *pS, bool runPlaneSweepOnX, int runNumPartitionsPerRelation)
        {

            Coord partitionExtent = 1.0/runNumPartitionsPerRelation;

            if (runPlaneSweepOnX)
            {
                return ditt_2d::single::ForwardScanBased_PlaneSweep_CNT_X_Rolled_DynamicScheduling(pR, pS, runNumPartitionsPerRelation, partitionExtent);
            }
            else
            {
                return ditt_2d::single::ForwardScanBased_PlaneSweep_CNT_Y_Rolled_DynamicScheduling(pR, pS, runNumPartitionsPerRelation, partitionExtent);
            }
        };


        unsigned long long ForwardScanBased_PlaneSweep_CNT(Relation *pR, Relation *pS, bool runPlaneSweepOnX, int runNumPartitionsPerRelation, double* tileTime)
        {

            Coord partitionExtent = 1.0/runNumPartitionsPerRelation;

            if (runPlaneSweepOnX)
            {
                return ditt_2d::single::ForwardScanBased_PlaneSweep_CNT_X_Rolled_DynamicScheduling(pR, pS, runNumPartitionsPerRelation, partitionExtent, tileTime);
            }
            else
            {
                return ditt_2d::single::ForwardScanBased_PlaneSweep_CNT_Y_Rolled_DynamicScheduling(pR, pS, runNumPartitionsPerRelation, partitionExtent, tileTime);
            }
        };
    };
    
};
