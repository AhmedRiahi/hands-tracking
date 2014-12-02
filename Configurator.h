
#ifndef __PFA__Config__
#define __PFA__Config__

#include <iostream>
#include <opencv/cv.h>

using namespace cv;

class Configurator{
public:
    void start();
    void setWindowSetting();
    Mat GetThresholdedImage(Mat);

    int lowerY=0;
    int lowerCR=133;
    int lowerCB=77;

    int upperY=179;
    int upperCR=173;
    int upperCB=127;
};

#endif /* defined(__PFA__Config__) */
