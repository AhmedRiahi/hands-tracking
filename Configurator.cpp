#include "Configurator.h"
#include "opencv/cv.h"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>



using namespace std;
using namespace cv;


Mat Configurator::GetThresholdedImage(Mat imgYcrCb){

    Mat imgThresh;
    inRange(imgYcrCb, Scalar(lowerY,lowerCR,lowerCB), Scalar(upperY,upperCR,upperCB), imgThresh);

    return imgThresh;
}


void Configurator::setWindowSetting(){
    namedWindow("Hands",WINDOW_NORMAL);

    createTrackbar("LowerY", "Hands", &lowerY, 255, NULL);
    createTrackbar("UpperY", "Hands", &upperY, 255, NULL);

    createTrackbar("LowerCR", "Hands", &lowerCR, 255, NULL);
    createTrackbar("UpperCR", "Hands", &upperCR, 255, NULL);

    createTrackbar("LowerCB", "Hands", &lowerCB, 255, NULL);
    createTrackbar("UpperCB", "Hands", &upperCB, 255, NULL);
}

void Configurator::start(){

    setWindowSetting();
    VideoCapture cam;
    cam.open(0);

    Mat frame ;//= imread("/Users/ahmedriahi/Documents/C++_workspace/PFA/inputs/Seq-herbivore-qui-lion-non-cheval-mouton-oui.mov");
    Mat out;
    bool pause = false;
    while(1){
        if(!cam.read(frame)){
            cam.open("/Users/ahmedriahi/Documents/C++_workspace/PFA/inputs/vid5.mov");
            cam >> frame;
        }

        cvtColor(frame, out, CV_BGR2YCrCb);

        GaussianBlur(out,out,Size(5, 5), 0, 0);
        medianBlur(out,out,3);

        Mat imgThresh=GetThresholdedImage(out.clone());
        imshow("Hands", frame);
        imshow("Video", imgThresh);
        int c = cvWaitKey(80);
        if((char)c==27 ) break;
        else{
            if((char)c==  112){
                pause = true;
            }
        }
        while(pause){
            Mat imgThresh=GetThresholdedImage(out.clone());
            imshow("Video", imgThresh);
            c = cvWaitKey(80);
            if((char)c==  112){
                pause = false;
            }
        }
        cout <<"lower " <<lowerY << " "<< lowerCR<< " " << lowerCB << endl;
        cout <<"upper " <<upperY << " "<< upperCR<< " " << upperCB << endl;
    }
    destroyWindow("Video");
    destroyWindow("Hands");
}



