#ifndef __PFA__HandRecognizer__
#define __PFA__HandRecognizer__

#include "opencv/cv.h"
#include <iostream>

using namespace std;
using namespace cv;



class HandRecognizer{
public :
    HandRecognizer(int,int);
    vector< vector<Point> > findHandContour(Mat);
    vector< vector<Point> > findFingers(vector<Point>);
    Point getPalmCenter();
    vector<Point> getConvexHull();
    vector<Vec4i> getHierarchy();
    Rect getHandRectangle();
    
    
private:
    vector<Vec4i> hierarchy;
    vector< vector<Point> > fingers;
    vector<Point> hull;
    vector<int> hullI;
    vector<Vec4i> defects;
    Rect handRectangle;
    double minHandArea,maxHandArea;
    
    
};

#endif /* defined(__PFA__HandRecognizer__) */
