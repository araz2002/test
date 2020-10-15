#include <iostream>
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

void Image::setScale( int _scale ) {
    cols2 = (int) ( cols / _scale ) ;
    rows2 = (int) ( rows / _scale ) ;
}

void Image::doImageResize() {
    resize( srcImage, resizeImage, Size( cols, rows2 ) ) ;

}

void Image::display( string _type ) {
    if ( _type == "original" ) {
	imshow("Original loaded image ", srcImage ) ;
	waitKey( 2000 ) ;
    } else if ( _type == "resized" ) {
	imshow("Resized image ", resizeImage ) ;
	waitKey( 2000 ) ;
    } else if ( _type == "gray" ) {
	imshow("Gray loaded image ", grayImage ) ;
	waitKey( 2000 ) ;
    } else if ( _type == "filtered" ) {
	double alpha = 0.5; 
	double beta = 0.5 ;
	Mat combined ;
	Mat combined2 ;
	addWeighted( resizeImage, alpha, filteredImage, beta, 0.0, combined ) ;
	hconcat( resizeImage, filteredImage, combined ) ;
	resize(combined, combined2, Size(960, 540) ) ;
	imshow("Gray loaded image ", combined2 ) ;
	//imshow("Gray loaded image ", filteredImage ) ;
	waitKey( 1000 ) ;
    }
}

void Image::pass2DFilter() {
/*
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

    filter2D( srcImage, imgLaplacian, CV_32F, kernel ) ;

    srcImage.convertTo( sharp, CV_32F ) ;

/*
    filter2D( resizeImage, imgLaplacian, CV_32F, kernel ) ;

    resizeImage.convertTo( sharp, CV_32F ) ;
*/
    filteredImage = sharp - imgLaplacian ;

    // convert back to 8bits gray scale
    filteredImage.convertTo( filteredImage, CV_8UC3 ) ;
    imgLaplacian.convertTo( imgLaplacian, CV_8UC3 ) ;

    // Create binary image from source image
    Mat bw;
    cvtColor( filteredImage, bw, COLOR_BGR2GRAY);
    //threshold(bw, bw, 40, 255, THRESH_BINARY | THRESH_OTSU);
    threshold(bw, bw, 4, 25, THRESH_BINARY | THRESH_OTSU);

    Mat dist;
    // Perform the distance transform algorithm
    distanceTransform(bw, dist, DIST_L2, 3);
    // Normalize the distance image for range = {0.0, 1.0}
    // so we can visualize and threshold it
    normalize(dist, dist, 0, 1.0, NORM_MINMAX);
//    imshow("Distance Transform Image", dist);
    
    // Threshold to obtain the peaks
    // This will be the markers for the foreground objects
    //threshold(dist, dist, 0.4, 1.0, THRESH_BINARY);
    threshold(dist, dist, 0.02, 1.0, THRESH_BINARY);
    // Dilate a bit the dist image
    Mat kernel1 = Mat::ones(3, 3, CV_8U);
    dilate(dist, dist, kernel1);

    //imshow("Markers", dist);
    //filteredImage = dist ;
    //return ;

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
    for (size_t i = 0; i < contours.size(); i++)
    {
        drawContours(markers, contours, static_cast<int>(i), Scalar(static_cast<int>(i)+1), -1);
    }
    // Draw the background marker
    circle(markers, Point(5,5), 3, Scalar(255), -1);

    //imshow("Markers", markers*10000);
    //return;
    // Perform the watershed algorithm
    watershed( filteredImage, markers ) ;


    Mat mark;
    markers.convertTo(mark, CV_8U);
    bitwise_not(mark, mark);
    //    imshow("Markers_v2", mark); // uncomment this if you want to see how the mark
    // image looks like at that point
    // Generate random colors
    vector<Vec3b> colors;
    for (size_t i = 0; i < contours.size(); i++)
    {
        int b = theRNG().uniform(0, 25);
        int g = theRNG().uniform(0, 25);
        int r = theRNG().uniform(0, 256);
        colors.push_back(Vec3b((uchar)b, (uchar)g, (uchar)r));
    }
    // Create the result image
    Mat dst = Mat::zeros(markers.size(), CV_8UC3);
    imshow("Final Result", dst);
    // Fill labeled objects with random colors
    //filteredImage = dst ;
    //return ;
    
    for (int i = 0; i < markers.rows; i++)
    {
        for (int j = 0; j < markers.cols; j++)
        {
            int index = markers.at<int>(i,j);
            if (index > 0 && index <= static_cast<int>(contours.size()))
            {
                dst.at<Vec3b>(i,j) = colors[index-1];
            }
        }
    }
    // Visualize the final image
//    imshow("Final Result", dst);
    filteredImage = dst ;
    
}
