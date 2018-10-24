#include "DGtest.h"
#include <string>
#include "Argmax.h"
#include <iostream>

int main(int argc, const char ** argv)
{
   // check command-line usage
    if(argc < 6) {
        printf(
            "\n"
            "Usage: DGtest <weights.bin> <input-tensor> <output-tensor> <labels.txt> <batch-size> \n"
            "\n"
            "   <input-data-file>: is filename to initialize tensor\n"
            "\n"
            "   <output-tensor>: is filename to write out the tensor"
            "\n"
        );
        return -1;
    }
    // create and initialize input tensor data
    const char* weights = argv[1];
    std::string inputFile = argv[2];
    std::string outputFile = argv[3];
    std::string labelFile = argv[4];
    std::string tagFile = argv[6];
    int batchSize = atoi(argv[5]);
    DGtest dgtest(weights, inputFile, outputFile, batchSize);
    dgtest.runInference();

    Argmax argmax(outputFile.c_str(), labelFile, tagFile);
    argmax.run();

    return 0;
}