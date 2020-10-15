#include <iostream>
#include "images.h"


namespace fs = std::filesystem ;

void Images::loadAllImages() {
    for ( const auto& file : fs::directory_iterator( dirPath ) ) {
	std::cout << file.path() << std::endl;
	filePaths.push_back( file.path() ) ;
    }
}

void Images::getGrayFlag( bool flag ) {
    grayFlag = flag ;
}

void Images::getScale( int _scale ) {
    scale = _scale ;
}

void Images::process( shared_ptr<Images> images, string _file, string displayType ) {
    images->setImagePath( _file ) ;
    images->loadImage() ;
    images->loadGrayImage() ;
    images->setImageDim() ;
    cout << "Image dimention is " << images->imageRows() <<"X" <<images->imageCols() << endl ;

    images->setScale( images->scale ) ;

    cout << "Resize Image dimention is " << images->resizeImageRows() <<"X" <<images->resizeImageCols() << endl ;

    images->doImageResize() ;
    images->pass2DFilter() ;

    images->display( displayType ) ;
}

