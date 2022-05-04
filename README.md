# Automatic-Gamma-Correction-Multithreaded
Split image into parts to calculate its gamma and correct the image using C++ multithreading

In this assignment, we calculate gamma automatically using parallel threads (if gamma is not given as an option on the command line.) Only the image file name should be given on the command line. 

## Calculating and applying Gamma
1. For each pixel, calculate its luminance or luma L = (3*R + B+ 4*G) / 8, where R, G, and B are the Red, Green, and Blue channel values. Don't use an arithmtic average of the three channels as it would result in color biasing of the image.
2. Calculate the average luma of the image L<sub>avg</sub> = (&Sigma;<sub>n</sub> L)/n, where n is the total number of pixels in the image. This L<sub>avg</sub> needs to be calculated in parallel and synchronized threads as we did in the Vector Avrage to acclerate its computations (i.e., break the image into parts). 
3. Calculate the image Gamma = -0.3 / [log<sub>10</sub>(L<sub>avg</sub>/255)]. Be careful with L<sub>avg</sub> = 0 or 255!
4. Apply the a gamma correction to the image using the Gamma value from step 3.

## Timing your code
You code should have a timer that starts just before the gamma calucation and should stop right after applying the gamma correction. Your code should then print the time difference between start and stop values. Use the high_resolution_clock method:
```cpp
auto start = high_resolution_clock::now();
// gamma calculation and correction here...
auto stop = high_resolution_clock::now();
auto duration = duration_cast<microseconds>(stop - start);
cout << "Time taken for gamma correction: " << duration.count() / 1000000.0 << " seconds" << endl;
```
## Getting started
You can use the code from this repo that does automatic gamma calculation and correction using a single thread. 

## Deliverables
- Your code should compile with no errors or warnings.
- Your code should print the gamma value calculated for the input image. 
- Your code should print the time taken for the gamma calcuation and correction (see above).
- To get full credit, your speedup compared to the single thread should be greater than three on the RPi 4 with four cores. 
