#ifndef IMAGE_H
#define IMAGE_H

#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video/background_segm.hpp"
#include "opencv2/video/tracking.hpp"

using namespace cv;
using namespace std;

class Image {
    private:
    public :
	Mat srcImage ;
	Mat grayImage ;
	Mat resizeImage ;
	Mat filteredImage ;
	Mat combined ;
	string filePath ;
	string fileName ;
	int rows2 ;
	int cols2 ;	
	int rows ;
	int cols ;	
	void setImagePath( string _filePath ) ;
	void calcNewColRow( float _scale ) ;
	void setImageDim() ;
	void loadImage() ; 
	void loadGrayImage() ;
	int imageCols() ;
	int imageRows() ;
	int resizeImageCols() ;
	int resizeImageRows() ;
	string imageName( string _filePath ) ;
	void doImageResize() ;
	void doSegmentation( string resType ) ;
} ;


#endif
