#ifndef PATH_GENERATION_H 
#define PATH_GENERATION_H

//////////////////GOOD/////////////////////

/****************************************/
/* Generation of asset price path                 */
/****************************************/

#include <vector>  //For the type of  the vairable that will contain the paths

#include <cmath>  // For random Gaussian generation via Box-Muller method
#include <iostream>
double gaussianBoxMuller () {
    
    double x = 0.0;
    double y = 0.0;
    double euclidSq = 0.0;
    
    do {
        x = 2.0 * rand() / static_cast<double>(RAND_MAX)-1; 
        y = 2.0 * rand() / static_cast<double>(RAND_MAX)-1;
        euclidSq = x*x + y*y;
    } while (euclidSq >= 1.0);
    
    return x*sqrt(-2*log(euclidSq)/euclidSq); 
}

// Generation of a vector of GBM stock price path

void calcPathAssetPrices(std::vector<double>& assetPrices , // Asset path
                                                const double& r, // Risk free interest rate 
                                                const double& v, // Volatility of underlying  
                                                const double& T) { // Expiry

    double dt = T/static_cast<double>(assetPrices.size());
    for (unsigned long i=1; i < assetPrices.size(); i++)
    {
       double gaussBM = gaussianBoxMuller () ;
        assetPrices[i] = assetPrices[i-1] * (1+r*dt + v*gaussBM*sqrt(dt)); 
   
    }  
}

void calcPathAssetPricesMilstein(std::vector<double>& assetPrices , // Asset path
                                                const double& r, // Risk free interest rate 
                                                const double& v, // Volatility of underlying  
                                                const double& T) { // Expiry

    double dt = T/static_cast<double>(assetPrices.size());
    for (unsigned long i=1; i < assetPrices.size(); i++)
    {
        double gaussBM = gaussianBoxMuller () ;
        assetPrices[i] = assetPrices[i-1] * exp( (r-0.5*v*v) * dt + v*gaussBM*sqrt(dt)); 
    }  
}

#endif

