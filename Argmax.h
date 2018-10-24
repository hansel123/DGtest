#pragma once

#include <vector>
#include <fstream>

/**
 *  Utility class for reading the tensor file and argmax it against the label file
 */
class Argmax
{
public:
    Argmax(const char* fileName, const std::string labelName, const std::string tagName);
    ~Argmax();

    /**
     *  Caculate the index number with the maximum probability
     */
    void setIndex (const std::vector<float> &vec);

    /**
     *  Get the current index
     */
    int getIndex ();

    /**
     *  Prints out the result
     */
    void printResult(const std::vector<std::string> &list, const std::vector<std::string> &tag);

    /**
     *  Converts the text file to the vector
     */
    std::vector<std::string> txtToVector(std::ifstream &textFile);

    /**
     *  Run the argmax
     */
    void run();

private:

    /**
     *  The index of the maximum probability will be stored
     */
    int mIndex;

    /**
     *  The count of a current image
     */
    static int mCount;

    /**
     *  File stream to open and read the tensor object
     */
    FILE *mFile;

    /**
     *  File stream to open and read the label text file
     */
    std::ifstream mLabel;
    
    /**
     *  File stream to open and read the tag text file
     */
    std::ifstream mTag;
};