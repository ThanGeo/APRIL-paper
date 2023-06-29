#include "../../containers/relation.h"


namespace nls{

    unsigned long long NestedLoops_Rolled_Ditt(const Relation &R, const Relation &S,  int pid, int runNumPartitionsPerRelation, Coord partitionExtent)
    {
        unsigned long long result = 0;
        auto startR = R.begin();
        auto startS = S.begin();
        auto lastR = R.end();
        auto lastS = S.end();

        for (auto r = startR; r != lastR; r++)
        {
            for (auto s = startS; s != lastS; s++)
            {
                // Verification 
                if ((r->xStart > s->xEnd) || (r->xEnd < s->xStart) || (r->yStart > s->yEnd) || (r->yEnd < s->yStart))
                    continue;

                auto x = max(r->xStart, s->xStart);
                auto y = min(r->yEnd, s->yEnd);
                auto pid_ref = findReferenceCell1(x, y, partitionExtent, runNumPartitionsPerRelation);

                if (pid_ref == pid)
                {
                    //result += r->id ^ s->id;
                    result ++;
                }
            }
        }

        return result;
    }

    //  inline unsigned long long ForwardScanBased_PlaneSweep_CNT_X_Rolled_DynamicScheduling(Relation *pR, Relation *pS, int runNumPartitionsPerRelation, Coord partitionExtent/*, unsigned long long &count, unsigned long long &count2*/)
    //     {
    //         unsigned long long result = 0;
    //         int runNumPartitions = runNumPartitionsPerRelation * runNumPartitionsPerRelation;

    //         for (int pid = 0; pid < runNumPartitions; pid++)
    //         {
    //             if ((pR[pid].size() > 0) && (pS[pid].size() > 0))
    //                 result += ditt_2d::Sweep_Rolled_CNT_X(pR[pid], pS[pid], pid, runNumPartitionsPerRelation, partitionExtent/*, count, count2*/);
    //         }

    //         return result;
    //     };


    unsigned long long Nested_LOOPS_CNT(Relation *pR, Relation *pS, int runNumPartitionsPerRelation)
    {
        unsigned long long result = 0;
        int runNumPartitions = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
        Coord partitionExtent = 1.0/runNumPartitionsPerRelation;

        for (int pid = 0; pid < runNumPartitions; pid++)
        {
            if ((pR[pid].size() > 0) && (pS[pid].size() > 0))
                result += NestedLoops_Rolled_Ditt(pR[pid], pS[pid], pid, runNumPartitionsPerRelation, partitionExtent/*, count, count2*/);
        }

        return result;
    }


    unsigned long long Nested_LOOPS_CNT(Relation *pR, Relation *pS, int runNumPartitionsPerRelation, double* tileTime)
    {
        unsigned long long result = 0;
        int runNumPartitions = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
        Coord partitionExtent = 1.0/runNumPartitionsPerRelation;
        Timer tim;

        for (int pid = 0; pid < runNumPartitions; pid++)
        {
            if ((pR[pid].size() > 0) && (pS[pid].size() > 0)){
                tim.start();
                result += NestedLoops_Rolled_Ditt(pR[pid], pS[pid], pid, runNumPartitionsPerRelation, partitionExtent/*, count, count2*/);
                tileTime[pid] += tim.stop();
            }
        }

        return result;
    }


    // unsigned long long NestedLoops_Rolled(const Relation &R, const Relation &S, size_t sR, size_t eR, size_t sS, size_t eS)
    // {
    //     unsigned long long result = 0;
    //     auto startR = R.begin() + sR;
    //     auto startS = S.begin() + sS;
    //     auto lastR = R.begin() + eR;
    //     auto lastS = S.begin() + eS;

    //     for (auto r = startR; r != lastR; r++)
    //     {
    //         for (auto s = startS; s != lastS; s++)
    //         {
    //             // Verification 
    //             if ((r->xStart > s->xEnd) || (r->xEnd < s->xStart) || (r->yStart > s->yEnd) || (r->yEnd < s->yStart))
    //                 continue;
    //             //result += r->id ^ s->id;
    //             result ++;
    //         }
    //     }

    //     return result;
    // }


}   