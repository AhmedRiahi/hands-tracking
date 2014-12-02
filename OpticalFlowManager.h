#ifndef __PFA__OpticalFlowManager__
#define __PFA__OpticalFlowManager__

#include "opencv/cv.h"
#include <iostream>

using namespace std;
using namespace cv;


class OpticalFlowManager{
    
public:
    void updateFlow(Mat,Mat);
    void addFeatureToTrack(Point2f);
    void addFeatureToTrack(Point2f,Scalar);
    void removeFeature(int index);
    vector<Point2f> getFeaturePrevious();
    vector<Point2f> getFeatureNext();
    vector<uchar> getStatus();
    bool isHandAlreadyTracked(Rect);
    vector<Point2f> getPointsInRect(Rect);
    vector<Scalar> getFeaturesColor();
    void setFeatureColor(int,Scalar);
    int getPointIndex(Point2f);
    
private:
    vector<Point2f> features_prev;
    vector<Point2f> features_next;
    vector<Scalar> features_color;
    vector<uchar> status;
};

#endif /* defined(__PFA__OpticalFlowManager__) */
