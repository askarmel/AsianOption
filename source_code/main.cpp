#include <iostream>
#include <fstream>
#include "PayOff.h"
#include "AsianOption.h"
#include "PathGeneration.h"
#include <boost/accumulators/accumulators.hpp>
#include <boost/accumulators/statistics.hpp>
#include <boost/range/algorithm.hpp>
#include <ostream>

using namespace std;

using namespace boost;
using namespace boost::accumulators;


int main(int argc , char  **argv) {
    
    // parameter list
    unsigned nbSim  ; // Number of simulated asset paths 
    unsigned timeStep = 250; // Number of asset sample intervals 
    double S0 = 100.0; // Asset Price t=0
    double E = 100.0; // Strike price
    double r = 0.05;  // Risk−free rate 
    double v = 0.2;  // Volatility of the underlying 
    double T = 1.0; // 1 year

    //Reinitialize the seed for random variable generation
    srand(time(NULL));
            
     ///////////////////////////////////////////////////////////////////////////////
    //Arithmetic average Asian Option Price (MC simulations)
    ////////////////////////////////////////////////////////////////////////////////

    // Vector of asset prices - will contain prices with Milstein correction
    vector<double> assetPrices(timeStep , S0) ; 
    // Vector of asset - will contain prices with method Euler-M
    vector<double> assetPricesEuler(timeStep , S0) ; 
    
    // Create the PayOff object 
    PayOff* payOff= new PayOffCall(E);
    
    // Create the AsianOption objects
    AsianOptionArithmetic asianA(payOff) ;

       
    //open and write a file 
    //ofstream myFile; 
    //myFile.open("simulationOptionPrice.txt");
    
    // boost accumulator to compute mean and variance on vector<double> variables (payoff and option prices)
    accumulator_set<double, stats<tag::sum , tag::variance, tag::mean > > acc_payoffFix;
    accumulator_set<double, stats<tag::sum , tag::variance, tag::mean > > acc_optionpriceFix;
    accumulator_set<double, stats<tag::sum , tag::variance, tag::mean > > acc_payoffFloat;
    accumulator_set<double, stats<tag::sum , tag::variance, tag::mean > > acc_optionpriceFloat;
    accumulator_set<double, stats<tag::sum , tag::variance, tag::mean > > acc_payoffFixEuler;
    accumulator_set<double, stats<tag::sum , tag::variance, tag::mean > > acc_optionpriceFixEuler;
    accumulator_set<double, stats<tag::sum , tag::variance, tag::mean > > acc_payoffFloatEuler;
    accumulator_set<double, stats<tag::sum , tag::variance, tag::mean > > acc_optionpriceFloatEuler;
  
    // dummy variable for counting the main loop iteration - will be used for vector of option prices
    int j = 0;
    
    // Each loop executes MC simulations nbSim times - to compare results for 10 to 10^6 simulations 
    for(nbSim = 10 ; nbSim <= 1000000 ; nbSim*=10) {
         // Vector of payoffs (non discounted)
        vector<double> payoffFix(nbSim,0.0) ;
        vector<double> payoffFloat(nbSim,0.0) ;
        vector<double> payoffFixEuler(nbSim,0.0) ;
        vector<double> payoffFloatEuler(nbSim,0.0) ;
    
        // Vector of option prices
        vector<double> optionPriceFix(nbSim,0.0) ;
        vector<double> optionPriceFloat(nbSim,0.0) ;
        vector<double> optionPriceFixEuler(nbSim,0.0) ;
        vector<double> optionPriceFloatEuler(nbSim,0.0) ;

        //Monte Carlo simulation
        for (int i=0; i<nbSim; i++) {
        
            // simulation of asset prices with 2 methods
            calcPathAssetPrices(assetPricesEuler,r,v,T);
            calcPathAssetPricesMilstein(assetPrices , r, v, T);
            
            // compute payoff prices for both fixed and floated strikes - with Milstein correction
            payoffFix[i]= asianA.payOffFixed(assetPrices) ;
            payoffFloat[i]=asianA.payOffFloated(assetPrices) ;
           
            // compute payoff prices for both fixed and floated strikes - with E-M method
            payoffFixEuler[i]= asianA.payOffFixed(assetPricesEuler) ;
            payoffFloatEuler[i]=asianA.payOffFloated(assetPricesEuler) ;
        
        }
        
        //assign accumulator variable for payoffs
        acc_payoffFix= for_each(payoffFix, acc_payoffFix);
        acc_payoffFloat= for_each(payoffFloat, acc_payoffFloat);
        acc_payoffFixEuler= for_each(payoffFixEuler, acc_payoffFixEuler);
        acc_payoffFloatEuler= for_each(payoffFloatEuler, acc_payoffFloatEuler);
        
          
        // discount mean of payoffs
        optionPriceFix[j] = (mean(acc_payoffFix)) *exp(-r*T);
        optionPriceFloat[j] = (mean(acc_payoffFloat)) *exp(-r*T);
        
        optionPriceFixEuler[j] = (mean(acc_payoffFixEuler)) *exp(-r*T);
        optionPriceFloatEuler[j] = (mean(acc_payoffFloatEuler)) *exp(-r*T);
        
        // MC error 
        long double errorFix = sqrt(variance(acc_payoffFix)*exp(-2*r*T)/nbSim)  ; 
        long double errorFloat = sqrt(variance(acc_payoffFloat)*exp(-2*r*T)/nbSim)  ; 
        long double errorFixEuler = sqrt(variance(acc_payoffFixEuler)*exp(-2*r*T)/nbSim)  ; 
        long double errorFloatEuler = sqrt(variance(acc_payoffFloatEuler)*exp(-2*r*T)/nbSim)  ; 
        
        //myFile << j << ";" << optionPriceFix[j] <<";"<< errorFix <<";";
        //myFile << optionPriceFixEuler[j] <<";"<< errorFixEuler <<";";
        //myFile << optionPriceFloat[j] <<";"<< errorFloat <<";";
        //myFile << optionPriceFloatEuler[j] <<";"<< errorFloatEuler <<";" <<endl;

        cout << "number of simulations : "<<nbSim <<endl;
        cout<< "price asian arithmetic fixed :"<<optionPriceFix[j]<<endl;
        cout<< "error asian arithmetic fixed  :"<<errorFix<<endl;
        cout << "confidence interval 95% : ["<< optionPriceFix[j]-1.96*errorFix<<" ; "<< optionPriceFix[j]+1.96*errorFix<<" ]"<< endl;
        cout<< "price asian arithmetic fixed Euler:"<<optionPriceFixEuler[j]<<endl;
        cout<< "error asian arithmetic fixed Euler :"<<errorFixEuler<<endl;
        cout << "confidence interval 95% : ["<< optionPriceFixEuler[j]-1.96*errorFixEuler<<" ; "<< optionPriceFixEuler[j]+1.96*errorFixEuler<<" ]"<< endl;
        cout<< "price asian arithmetic floated :"<<optionPriceFloat[j]<<endl;
        cout<< "error asian arithmetic floated  :"<<errorFloat<<endl;
        cout << "confidence interval 95% : ["<< optionPriceFloat[j]-1.96*errorFloat<<" ; "<< optionPriceFloat[j]+1.96*errorFloat<<" ]"<< endl;
        cout<< "price asian arithmetic floated Euler :"<<optionPriceFloatEuler[j]<<endl;
        cout<< "error asian arithmetic floated Euler :"<<errorFloatEuler<<endl;
        cout << "confidence interval 95% : ["<< optionPriceFloatEuler[j]-1.96*errorFloatEuler<<" ; "<< optionPriceFloatEuler[j]+1.96*errorFloatEuler<<" ]"<< endl;
     
        j++;
        
}
   

   // myFile.close();
    
   // delete  PayOff objects
    delete payOff;

     ////////////////////////////////////////////////////////////////////////////////////////////////////// 
    //Geometric average Asian Option Price (closed form solutions - exact prices)
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    double priceAsianGFix =  asian_geometric_fixed(E,v,S0,r,T);
    double priceAsianGFloat =  asian_geometric_floated(v,S0,r,T);
        
    /* PUT IN COMMENTS WHEN LOOPING ON NB_SIMULATIONS
    // Finally we output the parameters and prices
    cout << "Number of Paths: " << nbSim << endl;
    cout << "Number of Ints:  "<< timeStep << endl;
    cout << "Underlying: "<< S0<< endl;
    cout << "Strike (fixed) : " << E << endl;
    cout << "Risk−Free Rate : " <<r<< endl;
    cout << "Volatility : " <<v<< endl;
    cout << "Maturity: "<< T<< endl;
    cout << "Arithmetic average Asian Price  (Fixed Strike) : "<< priceAsianAFix << endl ;
    cout << "Arithmetic average Asian Price  (Floated Strike) : "<< priceAsianAFloat << endl ;
     * */
    cout << "Geometric average Asian Price (Fixed Strike) : "<< priceAsianGFix<< endl ;
    cout << "Geometric average Asian Price (Floated Strike) : "<< priceAsianGFloat<< endl ;
       
    return 0 ; 

}
