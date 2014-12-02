#include "OpticalFlowManager.h"
#include "Utils.h"

void OpticalFlowManager::updateFlow(Mat prev,Mat next){
    this->features_prev = this->features_next;
    Mat err;
    if(features_prev.size() >0 ){
        if(features_next.size() > 0){
            calcOpticalFlowPyrLK(prev, next,this->features_prev,this->features_next,this->status,err);
        }
    }
}

bool OpticalFlowManager::isHandAlreadyTracked(Rect hand){
    Rect handSup = Rect(hand.x-10,hand.y-10,hand.width+10,hand.height+10);
    int nbPoints = 0;
    int index = -1;
    for(int i = 0 ; i < this->features_next.size() ; i++){
        if(this->features_next.at(i).inside(handSup)){
            nbPoints++;
            index = i;
        }
    }
    if(nbPoints == 1){
        this->features_next[index] = Point2f(hand.x+(hand.width/2),hand.y+(hand.height/2));
    }
    return nbPoints > 0;
}

void OpticalFlowManager::addFeatureToTrack(Point2f p){
    vector<Scalar> colors;
    colors.push_back(Scalar(0,0,255));
    colors.push_back(Scalar(0,255,0));
    colors.push_back(Scalar(0,255,255));
    for(int i = 0 ; i < this->features_next.size() ; i++){
        for(int j=0;j< colors.size();j++){
            if( this->features_color[i] == colors[j]){
                colors.erase(colors.begin()+j);
                break;
            }
        }
    }
    this->features_next.push_back(p);
    //Scalar color =  Utils::getRandomColor();
    this->features_color.push_back(colors[0]);
}

void OpticalFlowManager::addFeatureToTrack(Point2f p,Scalar color){
    this->features_next.push_back(p);
    this->features_color.push_back(color);
}

vector<Point2f> OpticalFlowManager::getFeatureNext(){
    return this->features_next;
}

vector<Point2f> OpticalFlowManager::getFeaturePrevious(){
    return this->features_prev;
}

vector<uchar> OpticalFlowManager::getStatus(){
    return this->status;
}

vector<Scalar> OpticalFlowManager::getFeaturesColor(){
    return this->features_color;
}

void OpticalFlowManager::setFeatureColor(int index,Scalar color){
    this->features_color[index]=color;
}

vector<Point2f> OpticalFlowManager::getPointsInRect(Rect rect){
    vector<Point2f> points;
    Rect rectSup = Rect(rect.x-15,rect.y-15,rect.width+15,rect.height+15);
    for(int i = 0 ; i < this->features_next.size() ; i++){
        if(this->features_next.at(i).inside(rectSup)){
            points.push_back(this->features_next.at(i));
        }
    }
    return points;
}

int OpticalFlowManager::getPointIndex(Point2f p){

    for(int i = 0 ; i < this->features_next.size() ; i++){
        if(this->features_next[i] == p){
            return i;
        }
    }
    return -1;
}

void OpticalFlowManager::removeFeature(int index){
    this->features_next.erase(this->features_next.begin()+index);
    this->features_color.erase(this->features_color.begin()+index);
}