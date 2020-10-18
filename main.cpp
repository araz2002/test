#include <iostream>
#include <thread>
#include <string>
#include <memory>
#include <future>
#include<algorithm>

#include "images.h"

int main( int argc, char** argv ) {

    chrono::high_resolution_clock::time_point t1 = chrono::high_resolution_clock::now() ;

    const char*		imagesDstPath	= "../outputImages" ;
    float		scale 		= 1 ;
    string		imagesSrcPath	= "../inputImages" ;
    string		resultType	= "combined" ;
    string		concurrent	= "yes" ;
    
    cout << "--------------------------------------------------------------" << endl ;
    if ( argc == 1 ) {

	cout << "Usage : imageProcess path_to_input_directory path_to_output_directory result_type"
	" scale_factor if_concurrent " << endl ;
	cout << "Default for path_to_input_directory is \"../inputImages\" " << endl ;;
	cout << "Default for path_to_output_directory is \"../outputImages\" " << endl ;
	cout << "Default for result_type is combined; options: combined, segmented " ;
	cout << "Default for scale_factor is 1.0; value: float >= 1  " ;
	cout << "Default for if_concurrent is yes; options: yes, no" ;
	cout << "--------------------------------------------------------------" << endl ;

    } else if ( argc == 6 ) {

	stringstream	runningStream1 ;
	runningStream1	<< argv[1] ;
	runningStream1	>> imagesSrcPath ;

	imagesDstPath	= argv[2] ;

	stringstream	runningStream2 ;
	runningStream2	<< argv[3] ;
	runningStream2	>> resultType ;
	if ( resultType != "combined" && resultType != "segmented" ) {
	    cout << " Wrong command line argument for result_type: " << resultType << endl ;
	    cout << "--------------------------------------------------------------" << endl ;
	    return 0 ;
	}

	stringstream	runningStream3 ;
	string		scl ;
	runningStream3	<< argv[4] ;
	runningStream3	>> scl ;
	scale		= stof( scl ) ;
	if ( scale < 1 ) {
	    cout << " Wrong command line argument for :scale_factor " << scale << endl ;
	    cout << "--------------------------------------------------------------" << endl ;
	    return 0 ;
	}

	stringstream	runningStream4 ;
	runningStream4	<< argv[5] ;
	runningStream4	>> concurrent ;

	if ( concurrent != "yes" && concurrent != "no" ) {
	    cout << " Wrong command line argument for if_concurrent: " << resultType << endl ;
	    cout << "--------------------------------------------------------------" << endl ;
	    return 0 ;
	}

    } else {

	cout << "--------------------------------------------------------------" << endl ;
	cout << "Usage : imageProcess path_to_input_directory path_to_output_directory result_type"
	" scale_factor if_concurrent " << endl ;
	cout << "Default for path_to_input_directory is \"../inputImages\" " << endl ;;
	cout << "Default for path_to_output_directory is \"../outputImages\" " << endl ;
	cout << "Default for result_type is combined; options: combined, segmented " ;
	cout << "Default for if_concurrent is yes; options: yes, no" ;
	cout << "Please enter all 5 arguments or no argument to use defaults" << endl ;
	cout << "--------------------------------------------------------------" << endl ;
	return 0 ;

    }

    shared_ptr<Images> images( new Images( imagesSrcPath, imagesDstPath ) ) ; 

    vector<future<void>> futures ;

    images->loadAllImages() ;

    images->setScale( scale ) ;

    images->setResultType( resultType ) ;

    if ( concurrent == "yes" ) {
	for ( int i = 0 ; i < images->allImages.size() ; i++ ) {
	    futures.emplace_back(	async( launch::async, 
					&Images::process, 
					images, 
					move(images->allImages[i]) ) ) ;
	}
	for_each( futures.begin(), futures.end(), [] ( future<void> &ftr ) {
	    ftr.wait() ; } ) ;

	/* for ( auto& f : futures )
	    f.wait() ; */

    } else {
	for ( int i = 0 ; i < images->allImages.size() ; i++ ) {
	    images->process(		move(images->allImages[i]) ) ; 
	}
    }

    if ( concurrent == "yes" ) { 
	vector<thread> threads ;
	for ( int i = 0 ; i < images->allImages.size() ; i++ ) {
	    threads.emplace_back( thread(	&Images::save, 
				    		images, 
						images->finalImages[i], 
						images->finalPaths[i] ) 	) ;
	}
	for_each( threads.begin(), threads.end(), []( thread &t ) {
	    t.join() ; } ) ;

	// for ( auto& t : threads )
	//    t.join() ; 


    } else {
	for ( int i = 0 ; i < images->allImages.size() ; i++ ) {
	    images->save( 			images->finalImages[i], 
			    			images->finalPaths[i] 		) ;
	}
    }


    cout << "--------------------------------------------------------------" << endl ;
    chrono::high_resolution_clock::time_point t2 = chrono::high_resolution_clock::now() ;
    auto duration1 =  chrono::duration_cast<chrono::seconds>( t2 - t1 ).count() ;
    cout << "Segmention and save time:  " << duration1 <<" seconds" << endl ;
    cout << "--------------------------------------------------------------" << endl ;

    images->displayImages( move( images->finalImages ) ) ;

    cout << "--------------------------------------------------------------" << endl ;
    chrono::high_resolution_clock::time_point t3 = chrono::high_resolution_clock::now() ;
    auto duration2 =  chrono::duration_cast<chrono::seconds>( t3 - t2 ).count() ;
    cout << "Display time " << duration2 <<" seconds" << endl ;
    cout << "--------------------------------------------------------------" << endl ;
    auto duration3 =  chrono::duration_cast<chrono::seconds>( t3 - t1 ).count() ;
    cout << "Total execution time " << duration3 <<" seconds" << endl ;
    cout << "--------------------------------------------------------------" << endl ;


    return 0 ;
}

