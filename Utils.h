#ifndef __PFA__Utils__
#define __PFA__Utils__

#include <iostream>
#include "opencv/cv.h"

using namespace std;
using namespace cv;

class Utils {
    public :
    static vector<int> getMinPixelValues(Mat);
    static vector<int> getMaxPixelValues(Mat);
    static Scalar getRandomColor();
    static double euclideanDist(Point2f,Point2f);
};

#endif /* defined(__PFA__Utils__) */
