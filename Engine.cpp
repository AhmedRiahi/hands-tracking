#include "Engine.h"
#include <ctime>
#include "Utils.h"



Engine::Engine(){
    this->skinFilter = new SkinFilter();
    this->ofm = new OpticalFlowManager();
    this->isVideo = false;
    this->isDynamic = false;
    this->xw = new XMLWriter();
}

void Engine::start(){
    int minArea = 5000;
    int maxArea = 100000;
    bool occultationDetected = false;
    bool newObjectDetected = false;
    int newObjectIndex = -1;
    vector<Scalar> oldColors;

    clock_t firstTime = clock();
    //this->cam.open("/Users/ahmedriahi/Documents/C++_workspace/PFA/inputs/Seq-herbivore-qui-lion-non-cheval-mouton-oui.mov");
    //this->cam.open("/Users/ahmedriahi/Documents/C++_workspace/PFA/inputs/vid5.mov");
    if(this->isVideo){
        this->cam.open(this->fileName);
    }else{
        this->cam.open(0);
    }


    for(int i = 0 ; i< 6;i++){
        cam >> this->background;
    }
    
    this->background.copyTo(this->previousImage);
    cvtColor(this->previousImage, this->previousImage,CV_BGR2GRAY);
    
    vector<Rect> faces = this->skinFilter->faceDetect(this->background);
    if(faces.size()>0){
        minArea = (faces.at(0).width/2)*(faces.at(0).width/2);
        //maxArea = (faces.at(0).width+20)*(faces.at(0).height+20);
    }
    //imshow("background",this->background);
    //this->skinFilter->faceBasedFiltering(this->background);
    cout << "min max "<<minArea << " " << maxArea<<endl;
    
    this->xw->file << "<timeline>";
    this->xw->file << "<image_size><width>" << this->background.cols << "</width><height>" << this->background.rows << "</height></image_size>" <<endl;
    this->xw->file << "<records>" << endl;
    Mat dessin = Mat::zeros( this->background.size(), CV_8UC3 );
    this->handRecognizer = new HandRecognizer(minArea,maxArea);
    
    Mat drawFrame;
    
    while(cam.read(this->currentImage)){
        //cam >> this->currentImage;
        for(int i=0;i<faces.size();i++){
            Mat face=this->currentImage(Rect(faces[i].x+(faces[i].width*.05),faces[i].y+(faces[i].height*.05),faces[i].width-(faces[i].width*0.05),faces[i].height-(faces[i].height*0.05)));
            //face = Mat::zeros(face.rows, face.cols, CV_8UC3);
        }
        //this->currentImage = imread("/Users/ahmedriahi/Documents/C++_workspace/PFA/inputs/img/me20.png");
        Mat contourDraw = Mat::zeros( this->currentImage.size(), CV_8UC3 );
       
        Mat tmp,flowImage;
        this->currentImage.copyTo(drawFrame);
        this->currentImage.copyTo(tmp);
        this->currentImage.copyTo(flowImage);
        //tmp = this->skinFilter->removeFaces(tmp);
        
        tmp = this->removeBackground(tmp, this->background);
        
        //imshow("Background",this->background);
        cvtColor(tmp, tmp, CV_BGR2YCrCb);
        
        /*for(int i=0;i<tmp.rows;i++){
            for(int j=0;j<tmp.cols;j++){
                tmp.data[tmp.channels()*(tmp.cols*i + j) + 0] = 0;
            }
        }*/
        
        this->skinFilter->enhanceImage(tmp);
        if(this->isDynamic){
            this->skinFilter->faceBasedFiltering(tmp);
        }

        tmp = this->skinFilter->defaultFiltering(tmp);
        imshow("filtered",tmp);
        
        
        //tmp = this->skinFilter->faceBasedFiltering(this->currentImage);
        vector< vector<Point> > contours = this->handRecognizer->findHandContour(tmp);
        cvtColor(flowImage, flowImage, CV_BGR2GRAY);
        this->ofm->updateFlow(this->previousImage, flowImage);
        
        for(int i=0;i<this->ofm->getFeatureNext().size();i++){
            bool online = false;
            for(int c=0;c<contours.size();c++){
                Rect hand = boundingRect(contours[c]);
                if( this->ofm->getFeatureNext()[i].inside(hand) ){
                    online = true;
                }
            }
            if( ! online){
                this->ofm->removeFeature(i);
            }
        }
        
        if(occultationDetected && newObjectDetected){
            for(int c=0;c<contours.size();c++){
                Rect hand = boundingRect(contours[c]);
                vector<Point2f> points;
                vector<int> indexes;
                for(int i=0;i<this->ofm->getFeatureNext().size();i++){
                    if( this->ofm->getFeatureNext()[i].inside(hand) ){
                        points.push_back(this->ofm->getFeatureNext()[i]);
                        indexes.push_back(i);
                    }
                }
                if(points.size()>1){
                    double dist1 = Utils::euclideanDist(points[0],Point2f(hand.x+(hand.width/2),hand.y+(hand.height/2)));
                    double dist2 = Utils::euclideanDist(points[1],Point2f(hand.x+(hand.width/2),hand.y+(hand.height/2)));
                    if(dist1<dist2){
                        this->ofm->setFeatureColor(newObjectIndex,this->ofm->getFeaturesColor()[indexes[1]]);
                        this->ofm->removeFeature(indexes[1]);
                    }else{
                        this->ofm->setFeatureColor(newObjectIndex,this->ofm->getFeaturesColor()[indexes[0]]);
                        this->ofm->removeFeature(indexes[0]);
                    }
                }
            }
        }
        
        occultationDetected = false;
        newObjectDetected = false;
        
        for(int c=0;c<contours.size();c++){
            
            this->xw->file << "<hand>" << endl;
            clock_t begin = clock();
            this->xw->file << "<time>" << (begin-firstTime)/(CLOCKS_PER_SEC/1000) << "</time>"<<endl;
            approxPolyDP(contours[c], contours[c], 15,true);
            vector< vector<Point> > fingers = this->handRecognizer->findFingers(contours[c]);
            
            if(this->ofm->getFeaturePrevious().size()>0 && this->ofm->getFeatureNext().size()>0){
                if(this->ofm->getStatus()[0]){
                    if( Utils::euclideanDist(this->ofm->getFeaturePrevious()[0], this->ofm->getFeaturePrevious()[0]) < 1){
                        line(dessin, this->ofm->getFeaturePrevious()[0],this->ofm->getFeatureNext()[0], Scalar(0,255,0),15);
                    }else{
                        dessin = Mat::zeros( this->background.size(), CV_8UC3 );
                    }
                }
            }
            if(fingers.size() >=4){
                dessin = Mat::zeros( this->background.size(), CV_8UC3 );
            }
           
            
            for(int i=0;i<fingers.size();i++){
                line(drawFrame,fingers.at(i).at(0),fingers.at(i).at(1),Scalar(12,122,222));
                this->xw->file << "<finger><start><x>" << fingers.at(i).at(0).x<<"</x><y>"<< fingers.at(i).at(0).y<<"</y></start><end><x>"<<fingers.at(i).at(1).x << "</x><y>" << fingers.at(i).at(1).y <<"</y></end></finger>"<< endl;
            }
            
            
            Point2f palmCenter = this->handRecognizer->getPalmCenter();
            if( !this->ofm->isHandAlreadyTracked(this->handRecognizer->getHandRectangle()) ){
                this->ofm->addFeatureToTrack(palmCenter);
                newObjectDetected = true;
                int index  = this->ofm->getPointIndex(palmCenter);
                newObjectIndex = index;
            }
            
            int index  = this->ofm->getPointIndex(palmCenter);
            Scalar palmColor = this->ofm->getFeaturesColor()[index];

            
            for(int i=0; i<this->ofm->getFeaturePrevious().size(); i++){
                if(this->ofm->getStatus()[i]){
                    line(drawFrame,this->ofm->getFeaturePrevious()[i],this->ofm->getFeatureNext()[i],this->ofm->getFeaturesColor()[i],8.0);
                }
            }
            
            this->previousImage = flowImage;
            this->xw->file <<"<palm><x>" <<palmCenter.x <<"</x><y>"<<palmCenter.y<<"</y></palm>"<< endl;
            this->xw->file << "</hand>" << endl;
            
            Mat hand = Mat(contours[0]);
            vector<Point2f> points = this->ofm->getPointsInRect(this->handRecognizer->getHandRectangle());
            
            if( points.size() > 1 ){
                occultationDetected = true;
                Mat subImg = this->currentImage(this->handRecognizer->getHandRectangle());
                Mat subImgDraw = drawFrame(this->handRecognizer->getHandRectangle());
                Mat gray;
                cvtColor(subImg, gray, CV_BGR2GRAY);
                vector< vector<Point> > subContours,temp;
                vector<Vec4i> hierarchy;
                blur( gray, gray, Size(3,3) );
                Canny( gray, gray, 50, 100, 3 );
                findContours(gray, subContours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
                for(int k=0;k<subContours.size();k++){
                    drawContours(subImgDraw, subContours,k, Scalar(0,255,0));
                }
                subImgDraw = subImg.clone();
                imshow("gray", gray);
                drawContours(drawFrame, contours,c, Scalar(255,0,0),3);
            }else{
                if(points.size() == 1){
                    int index  = this->ofm->getPointIndex(points[0]);
                    Scalar color = this->ofm->getFeaturesColor()[index];
                    drawContours(drawFrame, contours,c,color,3);
                }
            }
            vector< vector<Point> > temp;
            temp.push_back(this->handRecognizer->getConvexHull());
            drawContours(contourDraw,temp,0, Scalar(255,0,0));
        }
        
        imshow("Dessin",dessin);
        imshow("Hand",drawFrame);
        int c = cvWaitKey(80);
        if((char)c==27 ) break;
    }
    destroyAllWindows();
    this->xw->file << "</records>" << endl;
    this->xw->file << "</timeline>";
}



Mat Engine::removeBackground(Mat img, Mat bg){
    for(int i=0;i<img.rows;i++){
        for(int j=0;j<img.cols;j++){
            bool deletePixel=true;
            for(int k=0;k<3;k++){
                if( img.at<Vec3b>(i,j)[k] - bg.at<Vec3b>(i,j)[k] < -15 || img.at<Vec3b>(i,j)[k] - bg.at<Vec3b>(i,j)[k] > 15  ){
                    deletePixel = false;
                }
            }
            if( deletePixel ){
                for(int k=0;k<3;k++){
                    img.at<Vec3b>(i,j)[k] = 0;
                }
            }
        }
    }
    return img;
}

void Engine::setVideo(string video){
    this->isVideo=true;
    this->fileName = video;
}

void Engine::setCam(){
    this->isVideo=false;
}

void Engine::isDynamicSeg(bool val){
    this->isDynamic = val;
}

void Engine::setSergmentationValues(int minY, int maxY, int minCR, int maxCR, int minCB, int maxCB){
    this->skinFilter->setSergmentationValues(minY,maxY,minCR,maxCR,minCB,maxCB);
}
