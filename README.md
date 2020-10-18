# Concurrent image segmentation with OpenCV 
![alice-donovan-rouse-tMHAmxLyzvA-unsplash](https://user-images.githubusercontent.com/72900895/96362139-0eaefd80-10e0-11eb-9435-2f866cd6fa19.jpg)

# Introduction
In this project, a concurrent opject oriented program has been developed to conduct image segmentation for a given set of images using OpenCV. For compilation, cmake and make has been used to compile the source and also to link it with OpenCV. Source code contains two main classes, one class to process single image while that other inherences the other class public methods and attributes to process all images. For concurrency, std::async is used to create one task per image. In order to save images for writing to the dick or displaying, unique_lock is used to push_back new images to the vector of final images. Users can provide 5 command line arguments to set path to the input images, path to output images, type of output images for display, scaling factor for inlet images and flag to conduct concurrent or not. Speedup of 3 has been observed for running the code to process 26 images using a 4-core machine.

# Project files
`main.cpp` is the top driver of the program. The main function in this file gets command line arguments and alternatively sets the default values for the five entries from user. In addition, the main class `images` intancited, concurreny loops create tasks to read images, conduct segementaion, save them to the disk.  

`image.h` and `image.cpp` these 2 head and source files declare and define `image` class. This class stores all information needed to process one image, that includes getting image location, calcuationg image dimentio, resizing image, segmenting image and pushing it the output vector.

`images.h` and `images.cpp` contain decleration and definition of the main driver clas called `images`. This inherates all public methods and attributes of `image` and creates a vector of `image` class to process all images. Each entity of the vector will be one concurrent task. Also, output vector `finalImages` get created and moved to `display` method. 

# How to run and command line arguments
The project's executable is called imageProcess and needs 5 arguments from command line:
 
_path_to_input_directory_: Full or relative path to input image folder. Default value of "../inputImages" is set in `main.cpp`

_path_to_output_directory_: Full or relative path to segmented output image folder. Default value of "../outputImages" is set in `main.cpp`

_result_type_: This is set either segmented output images to be saved and showed side by side (combined option) the original image or just the segmented images are only showed. Default value of "combined" is set in `main.cpp`. Only 2 options of "combined" or "segmented" will be processed.

_scale_factor_: This is option to scale down the images before conduction segmentation. Default value is 1.0 and is getting set in `main.cpp`. A value of 1.0 of bigger will accepted.
 
_if_concurrent_: This is to ask if the program to run concurrently or sequencially. Default value is "yes" in `main.cpp`. Only "yes" and "no" are acceptable.
 
If no argument is provided from command line default values will be used. However, if one or more defaults need to be changed all five values need to be provided. Here is an example how to run the code concurrent with scale factor of 2:

{`imageProcess "../inputImages" "../outputImages" combined 2 yes`}
 
To run sequencially with scale factor 1, the command will be:

imageProcess "../inputImages" "../outputImages" combined 1 no
  
# Build instruction

## Setup OpenCV 4.1 in Ubuntu:
1. Download OpenCV, cd to opencv
2. mkdir build
3. cd build
4. export OpenCV_DIR=...path_to.../opencv/build
5. sudo apt-get install libgtk2.0-dev pkg-
6. sudo apt install libcanberra-gtk-module libcanberra-gtk3-module
7. cmake ..
8. make

## Compiling the project (imageProcess)
1. mkdir build
2. cd build
3. export OpenCV_DIR=...path_to.../opencv/build
4. cmake ..
make
