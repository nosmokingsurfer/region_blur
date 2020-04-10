#include <region_blur.h>
#include <iostream>

#include <vector>

using namespace std;

bool operator<(const cv::Size& lhs, const cv::Size& rhs)
{
  return ((lhs.width < rhs.width) && (lhs.height < rhs.height));
}

RegionBlur::RegionBlur(const RegionBlurParams& params)
  : mouse_moved(false)
{
  image_size = cv::Size(params.video_w, params.video_h);
  cout << "Video frame size = " << image_size << endl;

  //grabbing parameters
  kernel_size.width = params.kernel_w;
  kernel_size.height = params.kernel_h;
  cout << "Blur kernel size = " << kernel_size << endl;


  region_size.width = params.region_w;
  region_size.height = params.region_h;
  cout << "Blur window size = " << region_size << endl;

  anchor = cv::Point(params.anchor_x, params.anchor_y);
  cout << "Anchor point of kernel = " << anchor << endl;

  bordertype = static_cast<cv::BorderTypes>(params.borderType);
  cout << "Blur border type = " << bordertype << endl;

  boundary = cv::Rect(0, 0, image_size.width, image_size.height);
}

void RegionBlur::updateRegion()
{
  //updating state of region to be blurred
  current_region.x = current_mouse_position.x - region_size.width / 2;
  current_region.y = current_mouse_position.y - region_size.height / 2;
  current_region.width = region_size.width;
  current_region.height = region_size.height;

  //cutting off everything outside the image
  current_region &= boundary;
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
    
  if (mouse_moved)
  {
    cv::blur(img(current_region), img(current_region), kernel_size, anchor, bordertype);
  }

  return true;
}

