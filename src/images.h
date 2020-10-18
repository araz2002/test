#ifndef IMAGES_H
#define IMAGES_H

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include <mutex>
#include "image.h"

using namespace std;

class Images : public Image {
    private:
	string srcPath ;
	const char* dstPathC ;
	mutex mtx ;
    public:
	string dstPath ;
	string resultType ;
	vector<Image> allImages ;
	float scale ;
	bool grayFlag ;
	vector<string> fileNames ;
	vector<Mat> finalImages ;
	vector<string> finalPaths ;
	Images( string _srcPath, const char* _dstPathC ) ;
	void loadAllImages() ; 
	void displayImages() ; 
	void displayImages( vector<Mat>&& finalImages ) ;
	void getGrayFlag( bool flag ) ;
	void setScale( float _scale ) ;
	void setResultType( string _resultType ) ;
	void process( Image&& image ) ; 
	void save( Mat image, string dstPath ) ;

} ;


#endif
