#ifndef PAYOFF_H
#define PAYOFF_H

///////////////GOOD///////////////////

#include <algorithm> // needed for the std ::max  function

class PayOff { 
    
protected : 
        double _E; //Strike

    public :

    PayOff() ; 
    virtual ~PayOff() {}; 
    
    // virtual pure compute function  - fixed strike
    virtual double computeFixed(const double& mean) const = 0; 
    
       // virtual pure compute function - floated strike
    virtual double computeFloated(const double& mean, const double& S) const = 0; 
        
};

class PayOffCall : public PayOff { 

    public :
        PayOffCall ( const double& E) ; 
        virtual ~PayOffCall () {};
        
        // surcharge of the compute function - fixed strike
        virtual double computeFixed(const double& mean) const; 
          // surcharge of the compute function - floated strike
        virtual double computeFloated(const double& mean, const double& S) const; 
        
    

};


#endif // PAYOFF_H
