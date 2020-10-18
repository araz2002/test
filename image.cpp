#include <iostream>
#include <memory>
#include "image.h"


void Image::setImagePath( string _filePath ) {
    filePath = _filePath ;
}
void Image::loadImage() {
    srcImage = imread( filePath, IMREAD_COLOR ) ;
    if ( srcImage.empty() ) {
        cout << "Could not open or find the image with path \n" << filePath << endl;
    }
}

string Image::imageName( string _filePath ) {
    stringstream nameStream( _filePath ) ; 
    string name ;
    while( nameStream.good() ) {
	getline( nameStream, name, '/') ; 
    }
    return name ;
}

void Image::loadGrayImage() {
    grayImage = imread( filePath, IMREAD_GRAYSCALE ) ;
    if ( srcImage.empty() ) {
        cout << "Could not open or find the image with path \n" << filePath << endl;
    }
}

void Image::setImageDim() {
    cols = srcImage.cols ;
    rows = srcImage.rows ;
}

int Image::imageCols() {
    return cols ;
}

int Image::imageRows() {
    return rows ;
}

int Image::resizeImageCols() {
    return cols2 ;
}

int Image::resizeImageRows() {
    return rows2 ;
}

void Image::calcNewColRow( float _scale ) {
    cols2 = (int) ( cols / _scale ) ;
    rows2 = (int) ( rows / _scale ) ;
}

void Image::doImageResize() {
    resize( srcImage, resizeImage, Size( cols2, rows2 ) ) ;

}


void Image::doSegmentation( string resType ) {

/* make background to black
    for ( int i = 0; i < srcImage.rows; i++ ) {
        for ( int j = 0; j < srcImage.cols; j++ ) {
            if ( srcImage.at<Vec3b>(i, j) == Vec3b(255,255,255) )
            {
                srcImage.at<Vec3b>(i, j)[0] = 0;
                srcImage.at<Vec3b>(i, j)[1] = 0;
                srcImage.at<Vec3b>(i, j)[2] = 0;
            }
        }
    }
*/

// Create a kernel that we will use to sharpen our image
    Mat kernel = (Mat_<float>(3,3) <<
                  1,  1, 1,
                  1, -8, 1,
                  1,  1, 1); // an approximation of second derivative, a quite strong kernel
    // do the laplacian filtering as it is
    // well, we need to convert everything in something more deeper then CV_8U
    // because the kernel has some negative values,
    // and we can expect in general to have a Laplacian image with negative values
    // BUT a 8bits unsigned int (the one we are working with) can contain values from 0 to 255
    // so the possible negative number will be truncated
    //
    Mat sharp ;
    Mat imgLaplacian ;
    Mat hsv_image ;

    medianBlur( resizeImage, resizeImage, 3 ) ;
    //cvtColor( srcImage, hsv_image, cv::COLOR_BGR2HSV);
    cvtColor( resizeImage, filteredImage, cv::COLOR_BGR2HSV);

    /*
    inRange( hsv_image, cv::Scalar(0, 100, 100), cv::Scalar(10, 255, 255), filteredImage ) ;

    filter2D( srcImage, imgLaplacian, CV_32F, kernel ) ;

    srcImage.convertTo( sharp, CV_32F ) ;

    filter2D( resizeImage, imgLaplacian, CV_32F, kernel ) ;

    resizeImage.convertTo( sharp, CV_32F ) ;

    filteredImage = sharp - imgLaplacian ;
    */

    // convert back to 8bits gray scale
    filteredImage.convertTo( filteredImage, CV_8UC3 ) ;
    imgLaplacian.convertTo( imgLaplacian, CV_8UC3 ) ;

    // Create binary image from source image
    Mat bw;
    cvtColor( filteredImage, bw, COLOR_BGR2GRAY);
    threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
//    threshold(bw, bw, 4, 25, THRESH_BINARY | THRESH_OTSU);

    Mat dist;
    // Perform the distance transform algorithm
    distanceTransform(bw, dist, DIST_L2, 3);
    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 1.0, NORM_MINMAX);
    
    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    threshold(dist, dist, 0.02, 1.0, THRESH_BINARY);

    // Dilate a bit the dist image
    Mat kernel1 = Mat::ones(3, 3, CV_8U);
    dilate(dist, dist, kernel1);


    // Create the CV_8U version of the distance image
    // It is needed for findContours()
    Mat dist_8u;
    dist.convertTo(dist_8u, CV_8U);

    // Find total markers
    vector<vector<Point> > contours;
    findContours(dist_8u, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    // Create the marker image for the watershed algorithm
    Mat markers = Mat::zeros(dist.size(), CV_32S);

    // Draw the foreground markers
    for ( size_t i = 0 ; i < contours.size() ; i++ ) {
        drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);
    }
    // Draw the background marker
    circle(markers, Point(5,5), 3, Scalar(255), -1);

    // Perform the watershed algorithm
    watershed( filteredImage, markers ) ;


    Mat mark;
    markers.convertTo(mark, CV_8U);
    bitwise_not(mark, mark);
    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 256);
        int g = theRNG().uniform(0, 256);
        int r = theRNG().uniform(0, 256);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Create the result image
    Mat final = Mat::zeros(markers.size(), CV_8UC3);
    
    for (int i = 0; i < markers.rows; i++) {
        for (int j = 0; j < markers.cols; j++) {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
                final.at<Vec3b>(i,j) = colors[index-1];
        }
    }

    if ( resType == "combined" ) {

	combined = final ;

	Mat combined2 ;
	hconcat( resizeImage, final, combined2 ) ;
	//resize( combined2, combined, Size(960, 540) ) ;
	resize( combined2, combined, Size(1560, 940) ) ;

    } else 

	resize( final, combined, Size(960, 940) ) ;
}
