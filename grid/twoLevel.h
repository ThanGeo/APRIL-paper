#ifndef TWOLEVEL_H
#define	TWOLEVEL_H

namespace twoLevel{ 
    
    int myRemainder(int numer, int denom) {
        return numer%denom;
    };
    
    double myRemainder2(double numer, double denom, int q) {
        double rem = double(numer - q*denom);

        return ((abs(rem) < EPS) ? 0: rem);
    };


    int myQuotient(double numer, double denom) {
        return int(numer/denom + EPS);
    };
    

    int findReferenceCell(double x, double y, double cellExtent, int numCellsPerDimension) {
        int xInt,yInt;

        xInt = (x + EPS)/cellExtent;
        yInt = (y + EPS)/cellExtent;

        return (yInt * numCellsPerDimension + xInt);
    };

    int getCellId(int x, int y, int numCellsPerDimension) {
        return (y * numCellsPerDimension + x);
    };
    
    namespace window
    {
    
        /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        inline unsigned long long InternalLoop_Range_Corners(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        {             
            unsigned long long result = 0;    
            auto pivot = firstFS;
            while ((pivot < lastFS))
            {
                if (S.yStart > pivot->yEnd || S.yEnd < pivot->yStart || S.xStart > pivot->xEnd || S.xEnd < pivot->xStart)
                {
                    pivot ++;
                    continue;
                }
                
                //result += pivot->id ^ S.id;
                result ++;
                pivot ++;
            }

            return result;
        }

        inline unsigned long long Range_Corners(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;
            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_Corners(r,S,lastR);

            return result;
        };


        inline unsigned long long InternalLoop_Range_Corners_A(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        {           
            unsigned long long result = 0;      
            auto pivot = firstFS;
            while ((pivot < lastFS))
            {
                if ( S.yEnd < pivot->yStart || S.xEnd < pivot->xStart)
                {
                    pivot ++;
                    continue;
                }
                
                //result += pivot->id ^ S.id;
                result ++;
                pivot ++;
            }

            return result;
        }

        inline unsigned long long Range_Corners_A(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;
            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_Corners_A(r,S,lastR);

            return result;
        };

        inline unsigned long long InternalLoop_Range_Corners_AB(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        {      
            unsigned long long result = 0;           
            auto pivot = firstFS;
            while ((pivot < lastFS))
            {
                if (S.yStart > pivot->yEnd || S.xEnd < pivot->xStart)
                {
                    pivot ++;
                    continue;
                }
                
                //result += pivot->id ^ S.id;
                result ++;
                pivot ++;
            }

            return result;
        }

        inline unsigned long long Range_Corners_AB(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;
            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_Corners_AB(r,S,lastR);

            return result;
        };


        inline unsigned long long InternalLoop_Range_Corners_AC(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        {         
            unsigned long long result = 0;        
            auto pivot = firstFS;
            while ((pivot < lastFS))
            {
                if( S.yEnd < pivot->yStart || S.xStart > pivot->xEnd )
                {
                    pivot ++;
                    continue;
                }
                
                //result += pivot->id ^ S.id;
                result ++;

                pivot ++;
            }

            return result;
        }

        inline unsigned long long Range_Corners_AC(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;
            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_Corners_AC(r,S,lastR);

            return result;
        };


        inline unsigned long long InternalLoop_Range_Corners_ABCD(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        {      
            unsigned long long result = 0;           
            auto pivot = firstFS;
            while ((pivot < lastFS))
            {
                if ( S.xStart > pivot->xEnd || S.yStart > pivot->yEnd)
                {
                    pivot ++;
                    continue;
                }
                
                //result += pivot->id ^ S.id;
                result ++;

                pivot ++;
            }

            return result;
        }

        inline unsigned long long Range_Corners_ABCD(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;
            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_Corners_ABCD(r,S,lastR);

            return result;
        };




        inline unsigned long long InternalLoop_Range_B_Class(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        {     
            unsigned long long result = 0;            
            auto pivot = firstFS;
            while ((pivot < lastFS))
            {    
                if ( S.yEnd < pivot->yStart || S.yStart > pivot->yEnd )
                {
                    pivot++;
                    continue;
                }
                
                //result += pivot->id ^ S.id;
                result ++;

                pivot++;
            }

            return result;
        }

        inline unsigned long long Range_B_Class(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;

            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_B_Class(r,S,lastR);

            return result;
        };


        inline unsigned long long InternalLoop_Range_Border_A_Horizontally(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        {   
            unsigned long long result = 0;              
            auto pivot = firstFS;

            while ((pivot < lastFS))
            {    
                if ( S.yEnd < pivot->yStart)
                {
                    pivot++;
                    continue;
                }
                
                //result += pivot->id ^ S.id;
                result ++;
                
                pivot++;
            }

            return result;
        }

        inline unsigned long long Range_Border_A_Horizontally(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;

            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_Border_A_Horizontally(r,S,lastR);

            return result;
        };


        inline unsigned long long InternalLoop_Range_Borders_AB(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        {    
            unsigned long long result = 0;             
            auto pivot = firstFS;

            while ((pivot < lastFS))
            {    
                if ( S.yStart > pivot->yEnd )
                {
                    pivot++;
                    continue;
                }
                //result += pivot->id ^ S.id;
                result ++;
                
                pivot++;
            }

            return result;
        }

        inline unsigned long long Range_Borders_AB(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;
            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_Borders_AB(r,S,lastR);

            return result;
        };


        inline unsigned long long InternalLoop_Range_C_Class(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        { 
            unsigned long long result = 0;
            auto pivot = firstFS;

            while ((pivot < lastFS))
            {
                if ( S.xStart > pivot->xEnd || S.xEnd < pivot->xStart )
                {
                    pivot++;
                    continue;
                }
                //result += pivot->id ^ S.id;
                result ++;
                
                pivot++;
            }

            return result;
        }

        inline unsigned long long Range_C_Class(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;
            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_C_Class(r,S,lastR);

            return result;
        };


        inline unsigned long long InternalLoop_Range_Border_A_Vertically(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        { 
            unsigned long long result = 0;
            auto pivot = firstFS;

            while ((pivot < lastFS))
            {
                if ( S.xEnd < pivot->xStart )
                {
                    pivot++;
                    continue;
                }
                //result += pivot->id ^ S.id;
                result ++;
                
                pivot++;
            }

            return result;
        }

        inline unsigned long long Range_Border_A_Vertically(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;
            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_Border_A_Vertically(r,S,lastR);

            return result;
        };


        inline unsigned long long InternalLoop_Range_Borders_AC(RelationIterator firstFS,Record &S, RelationIterator lastFS)
        { 
            unsigned long long result = 0;
            auto pivot = firstFS;

            while ((pivot < lastFS))
            {
                if ( S.xStart > pivot->xEnd )
                {
                    pivot++;
                    continue;
                }
                //result += pivot->id ^ S.id;
                result ++;
                
                pivot++;
            }

            return result;
        }

         inline unsigned long long Range_Borders_AC(Relation &pR, Record &S, size_t startR, size_t endR)
        {
            unsigned long long result = 0;
            auto r = pR.begin() + startR;
            auto lastR = pR.begin() + endR;

            result = InternalLoop_Range_Borders_AC(r,S,lastR);

            return result;
        };

    
    }

}

#endif

