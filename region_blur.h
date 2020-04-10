#pragma once
#ifndef REGION_BLUR_042020
#define REGION_BLUR_042020

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

struct RegionBlurParams
{
  int region_w;
  int region_h;
  int kernel_w;
  int kernel_h;
  double  sigma_x;
  double  sigma_y;
};

class RegionBlur
{
  public:
    RegionBlur() = delete;
    RegionBlur(const RegionBlurParams& params);
    
    void setMousePose(int x, int y);
    void setImageSize(int width, int height);
    bool process(cv::Mat& img) const;

  private:
    bool mouse_moved;
    void updateRegion(); // update region to blur

    //Gaussian Blur Parameters
    cv::Size kernel_size; // gaussian blur kernel size
    cv::Point2d sigma_xy; // covariance values

    //Blur parameters
    cv::Size region_size; // blur size
    cv::Size image_size; // image size to do checks
    cv::Rect boundary; // calculated boundary for image
    cv::Rect current_region; // current region to blur inside image
    cv::Point current_mouse_position; // current mouse position
    cv::Mat kernel_x; // precalculated gaussian kernel along x
    cv::Mat kernel_y; // precalculated gaussian kernel along y
};

#endif //REGION_BLUR_042020