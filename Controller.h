#ifndef CONTROLLER_H
#define CONTROLLER_H

class Controller {
   
    public:
    
    static const int nInputs = 9;
    static const int nOutputs = 2;
    
    Controller();
    Controller(double **w);
    ~Controller();
    double* fwd(double *inputs);
    double** getWeights();
    
    private:
    
    double** weights;
};

#endif