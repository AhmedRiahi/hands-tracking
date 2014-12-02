#include "HandRecognizer.h"


HandRecognizer::HandRecognizer(int minHandArea,int maxHandArea){
    this->minHandArea = minHandArea;
    this->maxHandArea = maxHandArea;
}

vector< vector<Point> > HandRecognizer::findHandContour(Mat img){
    vector< vector<Point> > contours,temp;
    findContours(img, contours, this->hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
    for(int i=0;i<contours.size();i++){
        if(fabs(contourArea(contours[i]))<this->maxHandArea && fabs(contourArea(contours[i]))>this->minHandArea){
            temp.push_back(contours[i]);
        }
    }
    return temp;
}

vector< vector<Point> > HandRecognizer::findFingers(vector<Point> contour){
    this->fingers.clear();
    this->handRectangle = boundingRect(contour);
    convexHull( Mat(contour), hullI, false );
    convexHull( Mat(contour), hull, false );
    if(hullI.size() > 3){
        convexityDefects(contour,hullI,defects);
    }
    
    
    int nb_fingers=0;
    for(int j=0;j<defects.size();j++){
        if(defects[j][3]/256 >= this->handRectangle.height/10){
            vector<Point> vecTmp;
            vecTmp.push_back(Point(contour[defects[j][0]]));
            vecTmp.push_back(Point(contour[defects[j][2]]));
            nb_fingers++;
            this->fingers.push_back(vecTmp);
        }
        //cout << this->fingers.size() << endl;
    }
    return fingers;
}


Point HandRecognizer::getPalmCenter(){
    vector<Point> palmPoints;
    for(int i=0;i<this->fingers.size();i++){
        palmPoints.push_back(this->fingers.at(i).at(1));
    }
    if(palmPoints.size()>2){
        Moments mu =  moments(palmPoints);
        return Point2f( mu.m10/mu.m00 , mu.m01/mu.m00 );
    }else{
        return Point(this->handRectangle.x+(this->handRectangle.width/2),this->handRectangle.y+(this->handRectangle.height/2));
    }
}



vector<Point> HandRecognizer::getConvexHull(){
    return this->hull;
}

vector<Vec4i> HandRecognizer::getHierarchy(){
    return this->hierarchy;
}

Rect HandRecognizer::getHandRectangle(){
    return this->handRectangle;
}

