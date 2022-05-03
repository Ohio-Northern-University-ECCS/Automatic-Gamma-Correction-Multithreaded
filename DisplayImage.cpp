#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <math.h>
#include <time.h>
#include <chrono>

using namespace std::chrono;
using namespace std;
using namespace cv;

// This function gets a copy of the object pointer new_image. Since the we
// are only changing the data of new_image, and not the pointer itself,
// we can pass it argument by copy. 
void gamma_correct (Mat image, Mat new_image, float gamma) {
    
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            for( int c = 0; c < image.channels(); c++ ) {
                new_image.at<Vec3b> (y,x)[c] =
                  saturate_cast<uchar> (pow (image.at<Vec3b> (y,x)[c] / 255.0, gamma) * 255);
            }
        }
    }
    return;
}

float calcGamma (Mat image) {
    unsigned int luma_sum = 0;
    for( int y = 0; y < image.rows; y++ ) {
        for( int x = 0; x < image.cols; x++ ) {
            unsigned int R = image.at<Vec3b> (y,x)[0];
            unsigned int G = image.at<Vec3b> (y,x)[1];
            unsigned int B = image.at<Vec3b> (y,x)[2];
            // Fast luma calculation
            unsigned int luma = (R+R+R+B+G+G+G+G) >> 3;
            luma_sum += luma;
        }
    }
    float lumaAvg = ((float)luma_sum / (image.rows * image.cols));
    cout << "Luma avg = " << lumaAvg << endl;
    float gamma = -0.3 / log10 (lumaAvg/255.0);
    cout << "Calculated Gamma = " << gamma << endl;
    return gamma;
}

int main (int argc, char** argv) {
    
    if (argc < 2) {
        printf("usage: DisplayImage <Image_Path> <gamma_value>\n");
        return -1;
    }
    
    cout << "Image = " << argv[1] << endl;
    Mat image;
    image = imread (argv[1], 1);

    if (!image.data) {
        cout << "No image data" << endl;
        return -1;
    }
    
    cout << "Image height = " << image.rows << " pixels" << endl;
    cout << "Image width  = " << image.cols << " pixels" << endl;
    
    // start timer
    auto start = high_resolution_clock::now();
    
    float gamma = 1;
    if (argc == 3) {
        gamma = stof(argv[2]);
    } else {
        gamma = calcGamma (image);
    }
    
    // Correct the image into the new image.
    Mat new_image = Mat::zeros (image.size(), image.type());
    gamma_correct (image, new_image, gamma);
    
    // Stop timer and report time
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(stop - start);
    cout << "Time taken for gamma correction: " << duration.count() / 1000000.0
         << " seconds" << endl;
    
    cout << "Gamma = " << gamma << endl;
    
    // Display the images in a smaller window than full size to make them fit
    // on screen. 
    resize (new_image, new_image, Size(), 0.25, 0.25, INTER_CUBIC);
    resize (image, image, Size(), 0.25, 0.25, INTER_CUBIC);
    
    cout << "Image height after resize = " << new_image.rows << " pixels" << endl;
    cout << "Image width after resize  = " << new_image.cols << " pixels" << endl; 
    
    namedWindow("Display New Image", WINDOW_AUTOSIZE );
    imshow("Display New Image", new_image);

    namedWindow ("Display Old Image", WINDOW_AUTOSIZE);
    imshow ("Display Old Image", image);

    waitKey (0);
    return 0;
}
