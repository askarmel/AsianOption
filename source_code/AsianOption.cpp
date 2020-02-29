#ifndef ASIAN_OPTION_CPP 
#define ASIAN_OPTION_CPP

/////////////GOOD//////////////////////////

#include "AsianOption.h"
#include <numeric> // Necessary for std : : accumulate 
#include <cmath> // For log/exp functions
#include <iostream>
using namespace std;

/************************************************************/
/*   For direct access to the price function                                     */
/************************************************************/ 


// ===================== // AsianOption // =====================
AsianOption::AsianOption(PayOff* payOff):payOff( payOff) {}

// ===================== // AsianOptionArithmetic // =====================
AsianOptionArithmetic::AsianOptionArithmetic(PayOff*  payOff):AsianOption( payOff ) {}

// Arithmetic mean pay−off prices

double AsianOptionArithmetic::payOffFixed(const vector<double>& assetPrices)  {
    
    unsigned timeStep = assetPrices.size () ;
    double  sum = accumulate(assetPrices.begin(), assetPrices.end(), 0); 
    
    double arithMean = sum / static_cast<double>(timeStep);
    //call the compute function of PayOff class with the A(T,0)  as strike - fixed float
    return (payOff->computeFixed(arithMean)) ;
}

double AsianOptionArithmetic::payOffFloated(const std::vector<double>& assetPrices) const {
    unsigned timeStep = assetPrices.size () ;
    double sum = accumulate(assetPrices.begin(), assetPrices.end(), 0); 
    double arithMean = sum / static_cast<double>(timeStep);

  //call the operator () of PayOff class with the A(T,0)  as strike - fixed float
    return (payOff->computeFloated(arithMean,assetPrices[timeStep-1]) );
}

//function norminv
double normalCFD(double value)
{
   return 0.5 * erfc(-value * M_SQRT1_2);
}


// Geometric mean pay−off price - fixed
double asian_geometric_fixed(double strike, double sigma, double S0, double r, double T)  { 
    
    double sigma_a = sigma/sqrt(3);
    double b  = 0.5*(r - sigma* sigma /6); 
    
     
    double d1=( log(S0/strike)+(b-sigma_a*sigma_a/2)*T)/sigma_a/sqrt(T);
    double d2=d1-sigma_a*sqrt(T); 
    
    double price=exp( (b-r)*T)*S0*normalCFD(d1) - strike*exp(-r*T)*normalCFD(d2);

    return price; 

}



// Geometric mean pay−off price - floated
double asian_geometric_floated(double sigma, double S0, double r, double T)  { 
    
    double mu  = r * sigma* sigma /2; 
     
    double d1=(mu*T*T/2) / sigma / sqrt(T*T*T/3);
    double d2=d1-sigma*sqrt(T*T*T/3); 
    
    double price=S0 * normalCFD(d1) - S0*exp( (-mu*T/2)+(sigma*sigma)*T/6) *normalCFD(d2);


    return price; 

}

#endif