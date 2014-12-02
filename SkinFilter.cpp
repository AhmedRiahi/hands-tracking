#include "SkinFilter.h"
#include "NofaceException.h"
#include "Utils.h"

SkinFilter::SkinFilter(){
    /*this->Ymin = 0;
    this->Ymax = 179;
    this->CRmin = 133;
    this->CRmax = 173;
    this->CBmin = 77;
    this->CBmax = 127;*/

    this->Ymin = 80;
    this->Ymax = 255;
    this->CRmin = 120;
    this->CRmax = 157;
    this->CBmin = 106;
    this->CBmax = 128;
}

Mat SkinFilter::enhanceImage(Mat img){
    GaussianBlur(img,img,Size(5,5), 0, 0);
	medianBlur(img,img,3);
    return img;
}

vector<Rect> SkinFilter::faceDetect(Mat img){
    CascadeClassifier classifier;
    vector< Rect > faces;
    classifier.load("/Users/ahmedriahi/Documents/C++_workspace/PFA/haarcascades/haarcascade_frontalface_alt.xml");
    classifier.detectMultiScale(img.clone(), faces);
    return faces;
}

Mat SkinFilter::defaultFiltering(Mat img){    
    //inRange(img,Scalar(0, 132, 102),Scalar(179, 173, 140),img);
    //inRange(img,Scalar(0, 132, 77),Scalar(179, 173, 127),img);
    //inRange(img,Scalar(80, 120, 106),Scalar(255, 157, 128),img);
    inRange(img,Scalar(this->Ymin,this->CRmin,this->CBmin),Scalar(this->Ymax,this->CRmax,this->CBmax),img);
    return img;
}

void SkinFilter::faceBasedFiltering(Mat imgRGB){
    vector<Rect> faces = this->faceDetect(imgRGB);
    Mat imgYCrCb;
    cvtColor(imgRGB, imgYCrCb,CV_RGB2YCrCb);
    if(faces.size() > 0){
        Point p1(faces[0].x+faces[0].width*0.25,faces[0].y+faces[0].height*0.25);
        Point p2(faces[0].x+faces[0].width*0.75,faces[0].y+faces[0].height*0.75);
        Mat face = Mat(imgYCrCb,Rect(p1,p2));
        imshow("face",face);
        vector<int> min=Utils::getMinPixelValues(face);
        vector<int> max=Utils::getMaxPixelValues(face);
        cout << min[0] << " "<< min[1] << " "<< min[2] << " max " << max[0]<< " " << max[1] <<" " << max[2] << endl;
        this->Ymin = min[0];
        this->Ymax = max[0];
        this->CRmin = min[1];
        this->CRmax = max[1];
        this->CBmin = min[2];
        this->CBmax = max[2];
    }
}

Mat SkinFilter::removeFaces(Mat img){
    vector<Rect> faces = this->faceDetect(img);
    for(int i=0;i<faces.size();i++){
        Mat face=img(Rect(faces[i].x+(faces[i].width*.10),faces[i].y+(faces[i].height*.10),faces[i].width-(faces[i].width*0.20),faces[i].height-(faces[i].height*0.20)));
        face = Mat::zeros(face.rows, face.cols, CV_8UC3);
    }
    return img;
}

void SkinFilter::setSergmentationValues(int minY, int maxY, int minCR, int maxCR, int minCB, int maxCB){
    this->Ymin = minY;
    this->Ymax = maxY;
    this->CRmin = minCR;
    this->CRmax = maxCR;
    this->CBmin = minCB;
    this->CBmax = maxCB;
}


