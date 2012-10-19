#include "Controller.h"

Controller::Controller(){
        
    weights = new double*[nInputs];
    for (int i = 0; i < nInputs; i++) {
        weights[i] = new double[nOutputs];
        for (int j = 0; j < nOutputs; j++) {
            weights[i][j] = 0;
        }
    }
}

Controller::Controller(double** w){
        
    weights = new double*[nInputs];
    for (int i = 0; i < nInputs; i++) {
        weights[i] = new double[nOutputs];
        for (int j = 0; j < nOutputs; j++) {
            weights[i][j] = w[i][j];
        }
    }
}

Controller::~Controller(){
    
    for (int i = 0; i < nInputs; i++) {
        delete [] weights[i];
    }
    delete [] weights;
}

double* Controller::fwd(double *inputs){
    
    double* outputs = new double[2];
    outputs[0] = 0;
    outputs[1] = 0;
    for (int i = 0; i < nInputs; i++) {
        for (int j = 0; j < nOutputs; j++) {
            outputs[j] = weights[i][j] * inputs[i];
        }
    }
    return outputs;
}

double** Controller::getWeights(){
    return weights;
}