#include <region_blur.h>
#include <iostream>

#include <vector>

using namespace std;

RegionBlur::RegionBlur(const RegionBlurParams& params)
{
  //grabbing parameters
  kernel_size.width = params.kernel_w;
  kernel_size.height = params.kernel_h;

  region_size.width = params.region_w;
  region_size.height = params.region_h;

  sigma_xy.x = params.sigma_x;
  sigma_xy.y = params.sigma_y;

  //initializing kernels for gaussian blur. Will allow to skip calculation on every cycle
  kernel_x = cv::getGaussianKernel(kernel_size.width, sigma_xy.x);
  kernel_y = cv::getGaussianKernel(kernel_size.height, sigma_xy.y);

  mouse_moved = false;
}

void RegionBlur::updateRegion()
{
  //check if 
  if(!boundary.contains(current_mouse_position))
  {
    if (current_mouse_position.x < region_size.width / 2)
      current_mouse_position.x = region_size.width / 2;
    if (current_mouse_position.x > image_size.width - region_size.width / 2)
      current_mouse_position.x = image_size.width - region_size.width / 2;
    if (current_mouse_position.y < region_size.height / 2)
      current_mouse_position.y = region_size.height / 2;
    if (current_mouse_position.y > image_size.height - region_size.height / 2)
      current_mouse_position.y = image_size.height - region_size.height / 2;
  }

  //updating state of region to be blurred
  current_region.x = current_mouse_position.x - region_size.width / 2;
  current_region.y = current_mouse_position.y - region_size.height / 2;
  current_region.width = region_size.width;
  current_region.height = region_size.height;
}

void RegionBlur::setMousePose(int x, int y)
{
  current_mouse_position = cv::Point(x, y);

  mouse_moved = true;

  updateRegion();
}

bool RegionBlur::process(cv::Mat& img) const
{
  if (img.empty() || (img.size() != this->image_size))
  {
    cerr << "Image empty or wrong size : " << img.size() << endl;
    return false;
  }

  //auto start_time = std::chrono::system_clock::now();

    //cv::gaussianBlur was faster than sepFilter2D
  if (mouse_moved)
  {
    cv::GaussianBlur(img(current_region), img(current_region), kernel_size, sigma_xy.x, sigma_xy.y);

    //cv::sepFilter2D(img(current_region), img(current_region), CV_8U, kernel_x, kernel_y);

    //auto finish_time = std::chrono::system_clock::now();
    //cout << chrono::duration_cast<chrono::milliseconds>(finish_time - start_time).count() << endl;

    //green rectangle for debug and visualization purposes
    //cv::rectangle(img, current_region, cv::Scalar(100, 200, 100), 2);
  }

  return true;
}

void RegionBlur::setImageSize(int width, int height)
{
  this->image_size = cv::Size(width, height);

  this->boundary = cv::Rect(cv::Point(this->region_size.width/2, this->region_size.height/2),
                            cv::Point(this->image_size.width - this->region_size.width/2,
                                      this->image_size.height - this->region_size.height/2));
}
