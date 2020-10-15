#ifndef IMAGES_H
#define IMAGES_H

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include "image.h"

using namespace std;

class Images : public Image {
    private:
	string dirPath ;
    public:
	float scale ;
	bool grayFlag ;
	vector<string> filePaths ;
	Images( string _dirPath ) {
	    dirPath = _dirPath ;
	}
	void loadAllImages() ; 
	void getGrayFlag( bool flag ) ;
	void getScale( int _scale ) ;
	void process( shared_ptr<Images> images, string _file, string _displayType ) ;

} ;


#endif
