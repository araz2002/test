#include <iostream>
#include <thread>
#include <string>
#include <iostream>
#include <memory>
#include "images.h"

int main() {
//    string imagesSrcPath ;
    string displayType = "filtered" ;
//    cout << "Please enter full path to images directory: " ;
//    cin >> imagesSrcPath ;
    //string imagesSrcPath = "/home/araz/forthProject/capstone/fall" ;
    string imagesSrcPath = "/home/araz/Downloads/forth" ;

    std::shared_ptr<Images> images( new Images(imagesSrcPath) ) ; 
    images->loadAllImages() ;
    images->getScale( 1 ) ;


    for ( auto& file : images->filePaths ) {
    images->process( images, file, displayType ) ;
    }

//    vector<thread> threads ;
//    for ( auto& file : images->filePaths ) {
//	threads.emplace_back(thread(&Images::process, images, images, file, displayType )) ;
//	Images::process( images, file, displayType ) ;
//    }

//    for ( auto& t : threads )
//    t.join() ;

    return 0 ;
}
/*

     // better results during the use of Distance Transform
    for ( int i = 0; i < LoadedImage.rows; i++ ) {
        for ( int j = 0; j < LoadedImage.cols; j++ ) {
            if ( LoadedImage.at<Vec3b>(i, j) == Vec3b(255,255,255) )
            {
                LoadedImage.at<Vec3b>(i, j)[0] = 0;
                LoadedImage.at<Vec3b>(i, j)[1] = 0;
                LoadedImage.at<Vec3b>(i, j)[2] = 0;
            }
        }
    }

*/
