#pragma once
#ifndef REGION_BLUR_042020
#define REGION_BLUR_042020

#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

class RegionBlur
{
  public:
    RegionBlur();
    bool init(const std::string& filename);
    bool init(const cv::FileNode& node);
    void updateRegion(const cv::Point& mouse);
    void setMousePose(int x, int y);
    bool setImageSize(int width, int height);
    void process(cv::Mat& img);

  private:
    cv::Size region_size;
    cv::Size kernel_size;
    cv::Point2d sigma_xy;
    cv::Size image_size;
    cv::Rect boundary;
    cv::Rect current_region;
    cv::Point current_mouse_position;
};

#endif //REGION_BLUR_042020