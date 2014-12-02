#ifndef __PFA__SkinFilter__
#define __PFA__SkinFilter__

#include <iostream>
#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;



class SkinFilter{
    
public :
    SkinFilter();
    Mat enhanceImage(Mat);
    void faceBasedFiltering(Mat);
    Mat defaultFiltering(Mat);
    vector< Rect > faceDetect(Mat);
    Mat removeFaces(Mat);
    void setSergmentationValues(int,int,int,int,int,int);
    
private :
    int Ymin;
    int Ymax;
    int CRmin;
    int CRmax;
    int CBmin;
    int CBmax;
    
};
#endif /* defined(__PFA__SkinFilter__) */
