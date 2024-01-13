#include <iostream>
#include <cstdlib>
#include <ippi.h>
#include <ipps.h>
#include <ippcore.h>
#include <ippcc.h>
#include <jpeglib.h>
#include <fstream>
#include <opencv2/opencv.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/X.h>

using namespace std;
// main() is where program execution begins.

void saveRGBToImageFile(const std::string& filename, char* dstImage, int width, int height) {
    cv::Mat rgbImage(height, width, CV_8UC3, dstImage);

    // Save the image as a file
    cv::imwrite(filename, rgbImage);
}

void saveRGBAToImageFile(const std::string& filename, char* dstImage, int width, int height) {
    cv::Mat rgbImage(height, width, CV_8UC4, dstImage);

    // Save the image as a file
    cv::imwrite(filename, rgbImage);
}

void saveGrayToImageFile(const std::string& filename, char* dstImage, int width, int height) {
    // Create an OpenCV Mat object with YUV422 data
    cv::Mat GrayImage(height, width, CV_8UC1, dstImage);

    // Save the image as a file
    cv::imwrite(filename, GrayImage);
}

int main(int argc, char ** argv) {
    Display * display;
    Window root;
    int width = 0;
    int height = 0;
    XWindowAttributes gwa;

    display = XOpenDisplay(NULL);
    if (display == NULL) {
        std::cerr << "No display can be aquired" << std::endl;
        exit(1);
    }
    root = DefaultRootWindow(display);

    XGetWindowAttributes(display, root, &gwa);
    width = gwa.width;
    height = gwa.height;

    XImage * ximage = XGetImage(
        display,
        root,
        0,
        0,
        width,
        height,
        AllPlanes,
        ZPixmap
    );

    IppiSize srcSize = {width, height};
    char* input = ximage->data;
    int srcStep = width * sizeof(Ipp8u) * 4;

    int outputStep = width * sizeof(Ipp8u) * 3;
    std::vector<char> output(width * height * 3);

    ippiCopy_8u_AC4C3R(reinterpret_cast<Ipp8u*> (input), srcStep, reinterpret_cast<Ipp8u*> (output.data()), outputStep, srcSize);

    // saveRGBToImageFile("rgb.jpg", output.data(), width, height);

    int output_GrayStep = width * sizeof(Ipp8u) * 1;
    std::vector<char> output_Gray(width * height * 1);

    IppStatus status = ippiRGBToGray_8u_C3C1R(reinterpret_cast<Ipp8u*> (output.data()), outputStep,
        reinterpret_cast<Ipp8u*> (output_Gray.data()), output_GrayStep, srcSize
    );

    saveGrayToImageFile("gray.jpg", output_Gray.data(), width, height);
    
    std::vector<char> output_grayToRGB(width * height * 4);
    int output_grayToRGBStep = width * sizeof(Ipp8u) * 4;
    status = ippiGrayToRGB_8u_C1C4R(reinterpret_cast<Ipp8u*> (output_Gray.data()), output_GrayStep,
        reinterpret_cast<Ipp8u*> (output_grayToRGB.data()), output_grayToRGBStep, srcSize, 0xFF
    );

    if(status != ippStsNoErr) {
        std::cerr << "ippiRGBToGray_8u_C3C1R err" << '\n';
        XDestroyImage(ximage);
        XCloseDisplay(display);
        return false;
    }
    
    saveRGBAToImageFile("rgba.jpg", output_grayToRGB.data(), width, height);

    // Cleanup X11 resources
    XDestroyImage(ximage);
    XCloseDisplay(display);

    return 0;
}
