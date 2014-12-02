#ifndef __PFA__Engine__
#define __PFA__Engine__

#include <iostream>
#include <opencv/cv.h>
#include "opencv2/highgui/highgui.hpp"
#include "SkinFilter.h"
#include "HandRecognizer.h"
#include "OpticalFlowManager.h"
#include "xmlwriter.h"

class Engine{
public:
    Engine();
    void start();
    Mat removeBackground(Mat,Mat);
    void setVideo(string);
    void setCam();
    void isDynamicSeg(bool);
    void setSergmentationValues(int,int,int,int,int,int);
    
private:
    SkinFilter * skinFilter;
    HandRecognizer * handRecognizer;
    OpticalFlowManager * ofm;
    VideoCapture cam;
    Mat currentImage,background,previousImage;
    bool isVideo;
    bool isDynamic;
    string fileName;
    XMLWriter *xw;
};

#endif /* defined(__PFA__Engine__) */
