
#ifndef PAY_OFF_CPP 
#define PAY_OFF_CPP

/////////////////GOOD////////////////////

#include "PayOff.h"

/*************************************/
/*  Class PayOff - abstract                     */
/*************************************/ 

// Default constructor for abstract class
PayOff::PayOff ( ) {}

/***************************************************************/
/*  Class PayOffCall                        -   with operator() to be called     */
/*   For direct access to the payoff value                                             */
/***************************************************************/ 

// Constructor of a Call with single strike parameter
PayOffCall::PayOffCall(const double&  E) { 
    _E =  E; 
}
    

// Surcharge computeFixed() method for a Call with fixed Strike A(T,0) - E
double PayOffCall::computeFixed(const double& mean) const {
    return std::max((mean - _E) ,0.0);
}

// Surcharge computeFloated () method for a Call with fixed Strike A(T,0) - E
double PayOffCall::computeFloated(const double& mean, const double& S) const {
    return std::max((S - mean) ,0.0);
}

#endif