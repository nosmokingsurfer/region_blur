#include <region_blur.h>
#include <iostream>

#include <vector>

RegionBlur::RegionBlur()
  : region_size(10, 10)
  , kernel_size(11, 11)
  , sigma_xy(0.0, 0.0)
{
}

bool RegionBlur::init(const cv::FileNode& node)
{
  if (!node.empty())
  {
    if (!node["kernel_size"].empty())
    {
      node["kernel_size"] >> this->kernel_size;

      //kernel must be only odd size
      if (this->kernel_size.width % 2 == 0)
      {
        std::cout << "Wrong kernel size : " << this->kernel_size << std::endl;
        return false;
      }

      if (this->kernel_size.height % 2 == 0)
      {
        std::cout << "Wrong kernel size : " << this->kernel_size << std::endl;
        return false;
      }

      std::cout << "kernel_size = " << this->kernel_size << std::endl;
    }
    if (!node["sigma_xy"].empty())
    {
      node["sigma_xy"] >> this->sigma_xy;
      std::cout << "sigma_xy = " << this->sigma_xy << std::endl;
    }
    if (!node["region_size"].empty())
    {
      node["region_size"] >> this->region_size;
      std::cout << "region_size = " << this->region_size << std::endl;
    }
    return true;
  }
  return false;
}

void RegionBlur::updateRegion(const cv::Point& mouse)
{
  current_region.x = current_mouse_position.x - region_size.width / 2;
  current_region.y = current_mouse_position.y - region_size.height / 2;
  current_region.width = this->region_size.width;
  current_region.height = this->region_size.height;
}


bool RegionBlur::init(const std::string& filename)
{
  cv::FileStorage fs(filename, cv::FileStorage::READ);
  if (fs.isOpened())
  {
    return this->init(fs.root());
  }
  else
  {
    return false;
  }
}

void RegionBlur::setMousePose(int x, int y)
{
  this->current_mouse_position = cv::Point(x, y);
}

void RegionBlur::process(cv::Mat& img)
{
  if (img.empty())
    return;


  if (boundary.contains(current_mouse_position))
  {
    updateRegion(current_mouse_position);

  }
  else
  {
    if (current_mouse_position.x < region_size.width / 2)
      current_mouse_position.x = region_size.width / 2;
    if (current_mouse_position.x > image_size.width - region_size.width / 2)
      current_mouse_position.x = image_size.width - region_size.width / 2;
    if (current_mouse_position.y < region_size.height / 2)
      current_mouse_position.y = region_size.height / 2;
    if (current_mouse_position.y > image_size.height - region_size.height / 2)
      current_mouse_position.y = image_size.height - region_size.height / 2;

    updateRegion(current_mouse_position);
  }

  cv::GaussianBlur(img(current_region), img(current_region), kernel_size, sigma_xy.x, sigma_xy.y);
  cv::rectangle(img, current_region, cv::Scalar(100, 200, 100), 2);
}

bool RegionBlur::setImageSize(int width, int height)
{
  this->image_size = cv::Size(width, height);

  this->boundary = cv::Rect(cv::Point(this->region_size.width/2, this->region_size.height/2),
                            cv::Point(this->image_size.width - this->region_size.width/2,
                                      this->image_size.height - this->region_size.height/2));
  return true;
}
