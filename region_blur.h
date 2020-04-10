#pragma once
#ifndef REGION_BLUR_042020
#define REGION_BLUR_042020

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

struct RegionBlurParams
{
  //video frame size
  int video_w;
  int video_h;

  //blur params
  //https://docs.opencv.org/4.2.0/d4/d86/group__imgproc__filter.html#ga8c45db9afe636703801b0b2e440fce37
  int region_w;
  int region_h;
  int kernel_w;
  int kernel_h;
  int anchor_x;
  int anchor_y;
  int borderType;
};

class RegionBlur
{
  public:
    RegionBlur() = delete;
    RegionBlur(const RegionBlurParams& params);
    void setMousePose(int x, int y); //set new position of mouse cursor
    bool process(cv::Mat& img) const; // process image

  private:
    bool mouse_moved; // if mouse ever moved on window
    void updateRegion(); // update region to blur

    //Blur Parameters
    cv::Size kernel_size; // blur kernel size
    cv::Point anchor; // kernel anchor position
    cv::BorderTypes bordertype; // type of border

    //Blur parameters
    cv::Size region_size; // blur size
    cv::Size image_size; // image size to do checks
    cv::Rect boundary; // boundary for image rectangle
    cv::Rect current_region; // current region to blur inside image
    cv::Point current_mouse_position; // current mouse position
};

#endif //REGION_BLUR_042020