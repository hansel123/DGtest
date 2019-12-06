#pragma once

#include <opencv2/opencv.hpp>
#include "DGtest.h"

struct CallbackData {
    cv::String windowName;
    cv::Mat image;
    bool isDrawing = false;
    cv::Point p1, p2;
};

class UserInterface
{
public:

    UserInterface(const char* weights);
    ~UserInterface();

    void startUI();
    static void onMouse(int event, int x, int y, int, void*);
 
private:
    /**
     *  Main window name for user interface
     */
    cv::String mWindow = "Palette";

    /**
     *  Progress window name for user interface
     */
    cv::String mProgressWindow = "Progress window";

    /**
     *  Digit detector
     */
    std::unique_ptr<DGtest> mDetector;
};