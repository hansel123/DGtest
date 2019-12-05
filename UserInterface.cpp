#include "UserInterface.h"

using namespace cv;
using namespace std;

UserInterface::UserInterface(const char* weights) {
    mDetector = make_unique<DGtest>(weights);
}

UserInterface::~UserInterface() {}

void UserInterface::startUI() {
    CallbackData callbackData;
    callbackData.windowName = mWindow;
    Mat img(400, 400, CV_8UC3, Scalar(0, 0, 0));

    callbackData.image = img.clone();
    namedWindow(mWindow, CV_WINDOW_AUTOSIZE);
    setMouseCallback(mWindow, UserInterface::onMouse, &callbackData);
    imshow(callbackData.windowName, callbackData.image);
    int key;
    do {
        key = waitKey();
        switch (key) {
            
        case 'r':
        case 'R':
            mDetector->runInference(callbackData.image);
            break;
        case 'c':
        case 'C':
            callbackData.image = img.clone();
            imshow(callbackData.windowName, callbackData.image);
            break;
        }
    } while (key != 27);

    destroyWindow(mWindow);
}

void UserInterface::onMouse(int event, int x, int y, int, void *data) {
    CallbackData *callbackData = (CallbackData *) data;
    
    switch(event){

    case CV_EVENT_LBUTTONDOWN:
        callbackData->isDrawing = true;
        callbackData->p1.x = x;
        callbackData->p1.y = y;
        callbackData->p2.x = x;
        callbackData->p2.y = y;
        break;

    case CV_EVENT_LBUTTONUP:
        callbackData->p2.x = x;
        callbackData->p2.y = y;
        callbackData->isDrawing = false;
        break;

    case CV_EVENT_MOUSEMOVE:
        if(callbackData->isDrawing) {
            circle(callbackData->image, Point(x, y), 5, Scalar(255, 255, 255), 10, CV_AA);
            imshow(callbackData->windowName, callbackData->image);
        }
        break;
        
    default:
        break;
    }
}
