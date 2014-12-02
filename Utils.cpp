#include "Utils.h"

vector<int> Utils::getMinPixelValues(Mat img){
    vector<int> mins(3);
    mins[0]=mins[1]=mins[2]=255;
    MatIterator_<Vec3b> it, end;
    for( it = img.begin<Vec3b>(), end = img.end<Vec3b>(); it != end; ++it){
        if((*it)[1] > 30 && (*it)[2] > 30){
            if(mins[0]>(*it)[0]){
                mins[0]=(*it)[0];
            }
            if(mins[1]>(*it)[1]){
                mins[1]=(*it)[1];
            }
            if(mins[2]>(*it)[2]){
                mins[2]=(*it)[2];
            }
        }
    }
    return mins;
}

vector<int> Utils::getMaxPixelValues(Mat img){
    vector<int> max(3);
    max[0]=max[1]=max[2]=0;
    MatIterator_<Vec3b> it, end;
    for( it = img.begin<Vec3b>(), end = img.end<Vec3b>(); it != end; ++it){
        if(max[0]<(*it)[0]){
            max[0]=(*it)[0];
        }
        if(max[1]<(*it)[1]){
            max[1]=(*it)[1];
        }
        if(max[2]<(*it)[2]){
            max[2]=(*it)[2];
        }
    }
    return max;
}

Scalar Utils::getRandomColor(){
    vector<Scalar> colors;
    colors.push_back(Scalar(0,0,255));
    colors.push_back(Scalar(0,255,0));
    colors.push_back(Scalar(0,255,255));
    int index = (int)(rand()*colors.size()/RAND_MAX);
    return colors[index];
}

double Utils::euclideanDist(Point2f p, Point2f q) {
    Point diff = p - q;
    return cv::sqrt(diff.x*diff.x + diff.y*diff.y);
}