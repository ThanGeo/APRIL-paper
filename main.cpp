#include "def.h"
#include "getopt.h"
#include "./containers/relation.h"
#include "./algorithms/2d/ditt.h"
#include "./algorithms/2d/fs.h"
#include "./algorithms/2d/nls.h"
#include "./partitioning/2d/ditt.h"
#include "./partitioning/2d/fs.h"

#include "pipeline.h"

void toLowercase(char *buf){
    auto i = 0;
    while (buf[i]){
        buf[i] = tolower(buf[i]);
        i++;
    }
}

void saveFormattedStats(string argument1, string argument2, unsigned long long &result, double totalTime, double MBRFTime, double IFtime, double REFTime){
    string result_filename = "experiment_data/"+argument1+"-"+argument2+"-"+to_string(HILBERT_POWER)+"_"+to_string(H)+".txt";
    ofstream fout(result_filename, fstream::out | ios_base::binary);
    
    fout << accepted / (double) postMBRCandidates << ",";
    fout << rejected / (double) postMBRCandidates << ",";
    fout << rejected / (double) postMBRCandidates << ",";
    fout << MBRFTime << ",";
    fout << IFtime << ",";
    fout << REFTime << "\n";
    
    fout.close();
}

void saveStats(unsigned long long &result, double totalTime, double MBRFTime, double IFtime, double REFTime){
    //print total results
    cout << "***************************************************" << endl;
    cout << "Total time: \t\t\t\t\t\t" << totalTime << " sec." << endl;
    cout << fixed << setprecision(6) << "\t-MBR-join time: \t\t\t\t" << MBRFTime << " sec." << endl;    
    cout << fixed << setprecision(6) << "\t-Intermediate Filter time: \t\t" << IFtime << " sec." << endl;
    cout << fixed << setprecision(6) << "\t-Refinement time: \t\t\t\t" << REFTime << " sec." << endl; 

    cout << "***************************************************" << endl;

    cout << "Intermediate Filter Effectiveness: " << endl;
    cout << fixed << setprecision(2) << "\t-Accepted: \t\t\t\t" << accepted / (double) postMBRCandidates * 100 << "%" << endl;
    cout << fixed << setprecision(2) << "\t-Rejected: \t\t\t\t" << rejected / (double) postMBRCandidates * 100 << "%" << endl;
    cout << fixed << setprecision(2) << "\t-Inconclusive: \t\t\t" << refinementCandidates / (double) postMBRCandidates * 100 << "%" << endl;

    cout << "***************************************************" << endl;
    
    cout << "Candidate Analysis: " << endl;
    cout << "\t-Candidates after MBR Filter: \t" << postMBRCandidates << endl;
    
    if(REFINEMENT){
        cout << "\t-Candidates after Refinement: \t" << refinementCandidates << endl;
        cout << fixed << setprecision(1) << "\t\t--Average # of vertices of ref. candidates in " << argument1 << ": \t" << refinementCandidatesR / (double) (refinementCandidates) << " vertices." << endl;
        cout << fixed << setprecision(1) << "\t\t--Average # of vertices of ref. candidates in " << argument2 << ": \t" << refinementCandidatesS / (double) (refinementCandidates) << " vertices." << endl;
        // cout << "   Accepted AFTER refinement (as a percentage of the refinement candidates): \t" << acceptedAfterRefinement / (double) refinementCandidates * 100 << "%" << endl;
    }

    cout << "***************************************************" << endl;
    
    if(INTERMEDIATE_FILTER == 0){
        //print the post MBR filter result
        cout << "Query Result: "<< acceptedAfterRefinement << " pairs." << endl;
    }else{
        //print the end of pipeline result
        cout << "Query Result: "<< TOTAL_RESULTS << " pairs." << endl;
    }
    cout << "***************************************************" << endl;
}

int main(int argc, char **argv)
{
    char c;
    int runNumPartitionsPerRelation = -1, runProcessingMethod = -1, runNumPartitions = -1, NUM_ITERATIONS = -1;
    bool runPlaneSweepOnX;
    Timer tim;
    double timeSorting = 0, timeIndexingOrPartitioning = 0, timeJoining = 0, timeCopying = 0;
    Relation R, S, *pR, *pS;
    Relation *pRA, *pSA, *pRB, *pSB, *pRC, *pSC, *pRD, *pSD;
    size_t *pRA_size, *pSA_size, *pRB_size, *pSB_size, *pRC_size, *pSC_size, *pRD_size, *pSD_size;
    size_t *pR_size ,*pS_size;
    vector<ABrec> *pRABdec , *pSABdec;
    vector<Crec> *pRCdec, *pSCdec;
    vector<Drec> *pRDdec, *pSDdec;
    vector<Coord> *pRYEND, *pSYEND;     

    clock_t start;

    unsigned long long result = 0;

    runPlaneSweepOnX = false;

    bool FLAGJOINITER  = false, FLAGSORTITER = false, FLAGPARTITER = false, FLAGCOP = false;
    double timeJoinIter = 0.0, timeSortIter = 0.0, timePartIter = 0.0, timeCopyingIter = 0.0;

    //get arguments
    string argument1(argv[argc-2]);
    string argument2(argv[argc-1]);

    

    while ((c = getopt(argc, argv, "sn:fezh:gwld:cqp:?")) != -1)
    {
        switch (c)
        {
            case 'p':
                runNumPartitionsPerRelation = atoi(optarg);
                break;   
            case 'n':
                // cout << "Selected " << atoi(optarg) << " partitions" << endl;
                H = atoi(optarg);
                break;  
            case 'c':
                CALCULATE_INTERVALS = 1;                
                break;
            case 'f':
                INTERMEDIATE_FILTER = 1;                
                break;
            case 'q':
                REFINEMENT = 1;
                break;
            case 'z':
                // cout << "Compressed APRIL version." << endl;
                COMPRESSION = 1;
                break;
            case 'e':
                EXPERIMENTS = 1;
                break;
            case 'd':
                DESIGNATED_ORDER = atoi(optarg);
                // cout << "Order for designated datasets (T3NA, O6_Oceania): " << DESIGNATED_ORDER << endl;
                DIFF_GRANULARITY_FIXED = 1;
                break;
            case 'l':
                // cout << "Selected linestring for data type of set " << argument2 << endl;
                DATA_TYPE = LINESTRING_TYPE;
                break;
            case 's':
                // cout << "Selected range query." << endl;
                SELECTION_QUERY = 1;
                break;
            case 'w':
                // cout << "Selected within join." << endl;
                WITHIN = 1;
                break;
            case 'g':
                GALLOPING = 1;
                break;
            case 'h':
                HILBERT_POWER = atoi(optarg);
                HILBERT_n = pow(2,HILBERT_POWER);
                break;
            default:
                break;
        }
    }

    if(SELECTION_QUERY && WITHIN){
        cout << "Error: You can't select both range query and within join as query type. " << endl;
        exit(1);
    }

    if(DATA_TYPE != POLYGON_TYPE && (SELECTION_QUERY || WITHIN)){
        cout << "Error: linestring support has been implemented only for intersection joins at the moment." << endl;
        exit(1);
    }

    if(DATA_TYPE != POLYGON_TYPE && COMPRESSION){
        cout << "Error: compression option for linestring data has not been implemented yet." << endl;
        exit(1);
    }

    if(WITHIN && COMPRESSION){
        cout << "Error: compression option for within joins has not been implemented yet." << endl;
        exit(1);
    }


    if(DIFF_GRANULARITY_FIXED && (argument2 != "T3NA" && argument2 != "O6_Oceania")){
        cout << "Error: designated order " << DESIGNATED_ORDER << " not implemented for datasets other than T3NA,O6_Oceania." << endl;
        exit(1);
    }

    if(DIFF_GRANULARITY_FIXED && (DESIGNATED_ORDER < 8 || DESIGNATED_ORDER > 15)){
        cout << "Error: designated order for the rasterization of the " << argument2 << " dataset can only be a value in the [8,15] range (16 is default without the -d command)" << endl;
        exit(1);
    }

    if(SELECTION_QUERY){
        if(argument1 == "Q"){
            argument1 = "QUERIES";
        }else if(argument2 == "Q"){
            argument2 = "QUERIES";
        }else{
            cout << "Error: For range queries, one of the two inputs must be 'Q'." << endl;
            exit(1);
        }
    }

    //initialize
    initialize(argument1, argument2);


    cout << "***************************************************" << endl;
    cout << "Initializing MBR-join... " << endl;
    // Load inputs (creates MBRs from geometry files)
    #pragma omp parallel sections
    {
        #pragma omp section
        {
            R.load(getBinaryGeometryFilename(0));
        }
        #pragma omp section
        {
            S.load(getBinaryGeometryFilename(1));
        }
    }
    cout << "Finished: sizes ";
    cout << R.size() << " (" << argument1 << ") & " << S.size() << " (" << argument2 << ") objects in the datasets." << endl;

    //fix the relation data space to be the same as our Hilbert data space
    R.minX = universalMinX;
    R.minY = universalMinY;
    R.maxX = universalMaxX;
    R.maxY = universalMaxY;

    S.minX = universalMinX;
    S.minY = universalMinY;
    S.maxX = universalMaxX;
    S.maxY = universalMaxY;

    Coord minX = min(R.minX, S.minX);
    Coord maxX = max(R.maxX, S.maxX);
    Coord minY = min(R.minY, S.minY);
    Coord maxY = max(R.maxY, S.maxY);
    Coord diffX = maxX - minX;
    Coord diffY = maxY - minY;
    Coord maxExtend = (diffX<diffY)?diffY:diffX;

    // cout << "R min/max: " << minX << "," << minY << " and " << maxX << "," << maxY << endl;
    // cout << "S min/max: " << minX << "," << minY << " and " << maxX << "," << maxY << endl;

    //normalize for MBR filter
    R.normalize(minX, maxX, minY, maxY, maxExtend);
    S.normalize(minX, maxX, minY, maxY, maxExtend);    

    unsigned long long sizeDitt = 0 , sizeMinijoins = 0 , sizeA = 0 , sizeB = 0 , sizeC = 0 , sizeD = 0;

    runNumPartitions = runNumPartitionsPerRelation * runNumPartitionsPerRelation;

    //PREPARE MBR FILTER
    runNumPartitions = runNumPartitionsPerRelation * runNumPartitionsPerRelation;
    pRA_size = new size_t[runNumPartitions];
    pRB_size = new size_t[runNumPartitions];
    pRC_size = new size_t[runNumPartitions];
    pRD_size = new size_t[runNumPartitions];

    pSA_size = new size_t[runNumPartitions];
    pSB_size = new size_t[runNumPartitions];
    pSC_size = new size_t[runNumPartitions];
    pSD_size = new size_t[runNumPartitions];

    memset(pRA_size, 0, runNumPartitions*sizeof(size_t));
    memset(pSA_size, 0, runNumPartitions*sizeof(size_t));
    memset(pRB_size, 0, runNumPartitions*sizeof(size_t));
    memset(pSB_size, 0, runNumPartitions*sizeof(size_t));
    memset(pRC_size, 0, runNumPartitions*sizeof(size_t));
    memset(pSC_size, 0, runNumPartitions*sizeof(size_t));
    memset(pRD_size, 0, runNumPartitions*sizeof(size_t));
    memset(pSD_size, 0, runNumPartitions*sizeof(size_t));

    pR = new Relation[runNumPartitions];
    pS = new Relation[runNumPartitions];
    
    //MBR FILTER PRE-PROCESSING (partitioning and sorting)
    fs_2d::single::PartitionTwoDimensional(R, S, pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size,runPlaneSweepOnX, runNumPartitionsPerRelation);
    fs_2d::single::sort::oneArray::SortYStartOneArray(pR, pS, pRB_size, pSB_size, pRC_size, pSC_size , pRD_size, pSD_size, runNumPartitions);

    //if specified, create the intervals (partition has to be completed first in order to compute the raster sections)
    if(CALCULATE_INTERVALS == 1){
        initiateRasterIntervalsCreation(argument1, argument2);
    }


    // for(auto &sec : DATA_SPACE.sections){
    //     cout << "section " << sec.sectionID << ": " << endl;
    //     cout << "   interest area: " << sec.interestxMin << "," << sec.interestyMin << " and " << sec.interestxMax << "," << sec.interestyMax << endl;
    //     cout << "   raster area: " << sec.rasterxMin << "," << sec.rasteryMin << " and " << sec.rasterxMax << "," << sec.rasteryMax << endl;
    //     cout << "   object count R: " << sec.objectCountR << endl;
    //     cout << "   object count S: " << sec.objectCountS << endl;
    // }

    //enable the intermediate filter if specified by the user, before begining the evaluation
    if(INTERMEDIATE_FILTER == 1){
        enableIntermediateFilter(argument1, argument2);
    }



    if(EXPERIMENTS){
        //for time measuring, runs multiple iterations
        int totalIterations = 10;
        double MBRF = 0;
        double IF = 0;
        double REF = 0;

        cout << "Evaluating query..." << endl;
        for(int i=0; i < totalIterations; i++){

            resetMetricParameters();

            cout << "\tIteration " << i+1 << "..." << endl;
            //EVALUATION
            start = clock();
            result = fs_2d::single::ForwardScanBased_PlaneSweep_CNT_Less(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runPlaneSweepOnX, runNumPartitionsPerRelation);
            double evaluationTotalTime = (clock() - start) /(double) CLOCKS_PER_SEC;

            MBRF += evaluationTotalTime - intermediateFilterTime - refinementTime;
            IF += intermediateFilterTime;
            REF += refinementTime;
        }
        cout << "Finished!" << endl;
        cout << "***************************************************" << endl << endl;

        //save stats/results
        cout << "********SUMMARY (AVERAGE OF 10 ITERATIONS)*********" << endl;
        cout << "***************************************************" << endl << endl;
        saveStats(result, MBRF/(double)totalIterations + IF/(double)totalIterations + REF/(double)totalIterations, MBRF/(double)totalIterations, IF/(double)totalIterations, REF/(double)totalIterations);
        saveFormattedStats(argument1, argument2, result, MBRF/(double)totalIterations + IF/(double)totalIterations + REF/(double)totalIterations, MBRF/(double)totalIterations, IF/(double)totalIterations, REF/(double)totalIterations);
    }else{

        cout << "Evaluating query..." << endl;
        //EVALUATION
        start = clock();
        result = fs_2d::single::ForwardScanBased_PlaneSweep_CNT_Less(pR, pS, pRA_size, pSA_size, pRB_size, pSB_size, pRC_size, pSC_size, pRD_size, pSD_size, runPlaneSweepOnX, runNumPartitionsPerRelation);
        double evaluationTotalTime = (clock() - start) /(double) CLOCKS_PER_SEC;
        cout << "Finished!" << endl;
        cout << "***************************************************" << endl << endl;

        //save stats/results
        cout << "**********************SUMMARY**********************" << endl;
        saveStats(result, evaluationTotalTime,evaluationTotalTime - intermediateFilterTime - refinementTime, intermediateFilterTime, refinementTime);

    }
 
    return 0;
}
