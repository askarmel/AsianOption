#ifndef ASIAN_OPTION_H 
#define ASIAN_OPTION_H


///////////////////GOOD ////////////////////

#include <vector> 
#include "PayOff.h"
#include <cmath>

double normalCFD(double value);

double asian_geometric_fixed(double strike, double sigma, double S0, double r, double T)  ;

double asian_geometric_floated(double sigma, double S0, double r, double T)  ;


//Abstract class - can be derived in call or put
class AsianOption { 
    protected :
        PayOff* payOff ; // Pay−off class (in this instance call or put)
    
    public :
        AsianOption(PayOff* payOff ) ; 
        virtual ~AsianOption () {};

        // Pure virtual pay−off operator (this will determine arithmetic or geometric ) for fixed strike
        virtual double payOffFixed(const std::vector<double>&  assetPrices)  = 0;
        // Pure virtual pay−off operator (this will determine arithmetic or geometric ) for floated strike
        virtual double payOffFloated(const std::vector<double>& assetPrices) const = 0;

};

/**********************************************/
/* Arithmetic average option class                         */
/**********************************************/

class AsianOptionArithmetic : public AsianOption { 
    public :
           AsianOptionArithmetic(PayOff* payOff); 
           virtual ~AsianOptionArithmetic () {};

            // Override the pure virtual function to produce arithmetic Asian Options for fixed strike
            virtual double payOffFixed(const std::vector<double>& assetPrices)  ;
            
            // Override the pure virtual function to produce arithmetic Asian Options for floated strike
            virtual double payOffFloated(const std::vector<double>& assetPrices) const ;

};

#endif