#pragma once
#include "annmodule.h"

/**
 *  Class to run the inference
 */
class DGtest
{
public:
    DGtest(const char* weights);
    ~DGtest();

    /**
     *  Run the inference
     */
    int runInference(cv::Mat &image); 

private:

    /**
     *  Weights file name
     */
    // const char* mWeights;

    /**
     *  Context that will be used for the inference
     */
    vx_context mContext;
    
    /**
     *  Graph that will be used for the inference
     */
    vx_graph mGraph;

    /**
     *  Graph that will be used for the inference
     */
    vx_tensor mInputTensor;
    
    /**
     *  Graph that will be used for the inference
     */
    vx_tensor mOutputTensor;

};