#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video/tracking.hpp"
#include <iostream>

using namespace cv;
using namespace std;

class Image {
    //private:
    public :
	Mat srcImage ;
	Mat grayImage ;
	Mat resizeImage ;
	Mat filteredImage ;
	string filePath ;
	int rows ;
	int cols ;	
	int rows2 ;
	int cols2 ;	
    //public:
	void setImagePath( string _filePath ) ;
	void loadImage() ; 
	void loadGrayImage() ;
	void setImageDim() ;
	int imageCols() ;
	int imageRows() ;
	int resizeImageCols() ;
	int resizeImageRows() ;
	void doImageResize() ;
	void pass2DFilter() ;
	void display( string _type ) ;
	void setScale( int _scale ) ;
} ;


#endif
