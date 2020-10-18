#include <iostream>
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <future>
#include <mutex>
#include "images.h"

Images::Images( string _srcPath, const char* _dstPathC ) {
    srcPath = _srcPath ;
    dstPathC = _dstPathC ;

    stringstream dstPathStream ;
    dstPathStream << dstPathC ;
    dstPathStream >> dstPath ;

}

void Images::loadAllImages() {

    for ( const auto& file : std::filesystem::directory_iterator( srcPath ) ) {
	Image image ;
	image.filePath = file.path() ;
	image.fileName = imageName( image.filePath ) ;
	allImages.push_back( image  ) ;
    }

    mkdir( dstPathC, 0777 ) ;

}

void Images::getGrayFlag( bool flag ) {
    grayFlag = flag ;
}

void Images::setScale( float _scale ) {
    scale = _scale ;
}

void Images::setResultType( string _resultType ) {
    resultType = _resultType ;
}

void Images::process( Image&& image ) { 

    image.loadImage() ;
    image.setImageDim() ;
//    images->loadGrayImage() ;

    unique_lock<mutex> lck1( mtx ) ;
        cout << endl << "Processing image: " << image.fileName << endl ;
        cout << "Image  dimention: " << image.cols <<" X " << image.rows << endl ;
    lck1.unlock() ;
    
    image.calcNewColRow( scale ) ;

    image.doImageResize() ;

    image.doSegmentation( resultType ) ;

   
    unique_lock<mutex> lck( mtx ) ;
//    lock_guard<mutex> lck( mtx ) ;
	string name = dstPath + "/" + image.fileName ;
	finalImages.push_back( image.combined ) ;
	finalPaths.push_back( name ) ;
    lck.unlock() ;

}

void Images::displayImages( vector<Mat>&& finalImages ) {
    
    for ( Mat& img : finalImages ) {
	imshow( " ", img ) ;
	waitKey( 1500 ) ;
    }

}

void Images::save( Mat finalImage, string dstPath ) {
/*
    double alpha = 0.5; 
    double beta = 0.5 ;
    addWeighted( resizeImage, alpha, filteredImage, beta, 0.0, combined ) ;
*/
    //    lock_guard<mutex> lck( mtx ) ;
    imwrite( dstPath, finalImage ) ;
}

