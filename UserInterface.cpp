#include <string>
#include "UserInterface.h"
#define CVUI_IMPLEMENTATION
#include "cvui.h"

using namespace cv;
using namespace std;

UserInterface::UserInterface(const char* weights) {
    mDetector = make_unique<DGtest>(weights);
}

UserInterface::~UserInterface() {}

void UserInterface::startUI() {
    CallbackData callbackData;
    callbackData.windowName = mWindow;
    cvui::init(mProgressWindow);
    namedWindow(mWindow, CV_WINDOW_AUTOSIZE);
    moveWindow(mWindow, 700, 500);
    moveWindow(mProgressWindow, 1000, 500);
    
    Mat img(300, 300, CV_8UC3, Scalar(0, 0, 0));
    Mat progressImage(300, 250, CV_8UC3, Scalar(255, 255, 255));
    // cvui::text(progressImage, 20, 100, "Top1", 0.5);
    // cvui::text(progressImage, 20, 150, "Top2", 0.5);
    // cvui::text(progressImage, 20, 200, "Top3", 0.5);
    
    callbackData.image = img.clone();
    Mat cloneImg = progressImage.clone();
    setMouseCallback(mWindow, UserInterface::onMouse, &callbackData);
    
    imshow(callbackData.windowName, callbackData.image);
    
    int key;

    do {
        key = waitKey(20);
        //key = waitKey();
        cvui::text(cloneImg, 75, 30, "Result", 1, 0x000000);
        if (cvui::button(cloneImg, 30, 250, 70, 25, "Clear")) {
            cloneImg = progressImage.clone();
            callbackData.image = img.clone();
            imshow(mWindow, callbackData.image);
        }
        
        if (cvui::button(cloneImg, 140, 250, 70, 25, "Run")) {
            mDetector->runInference(callbackData.image);
            cvui::text(cloneImg, 80, 100, to_string(mDetector->getResult()), 5, 0x0000ff);
        }

        // Update cvui internal stuff
        cvui::update();
        // Show window content
        cvui::imshow(mProgressWindow, cloneImg);
        
    } while (key != 27);

    destroyAllWindows();
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
