# Concurrent image segmentation with OpenCV 
![alice-donovan-rouse-tMHAmxLyzvA-unsplash](https://user-images.githubusercontent.com/72900895/96362139-0eaefd80-10e0-11eb-9435-2f866cd6fa19.jpg)

# Introduction
In this project, a concurrent opject oriented program has been developed to conduct image segmentation for a given set of images using OpenCV. For compilation, cmake and make has been used to compile the source and also to link it with OpenCV. Source code contains two main classes, one class to process single image while that other inherences the other class public methods and attributes to process all images. For concurrency, std::async is used to create one task per image. In order to save images for writing to the dick or displaying, unique_lock is used to push_back new images to the vector of final images. Users can provide 5 command line arguments to set path to the input images, path to output images, type of output images for display, scaling factor for inlet images and flag to conduct concurrent or not. Speedup of 3 has been observed for running the code to process 26 images using a 4-core machine.

# Project files

# How to run and command line arguments

## Build instruction

#  Ubuntu - setup OpenCV 4.1:
1. Download OpenCV, cd to opencv
2. mkdir build
3. cd build
4. export OpenCV_DIR=...path_to.../opencv/build
5. sudo apt-get install libgtk2.0-dev pkg-config
6. sudo apt install libcanberra-gtk-module libcanberra-gtk3-module
7. cmake ..
8. make

# Compile imageProcess
1. mkdir build
2. cd build
3. export OpenCV_DIR=...path_to.../opencv/build
4. cmake ..
make
