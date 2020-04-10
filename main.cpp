#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <chrono>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <region_blur.h>



void onMouse(int event, int x, int y, int flags, void* params)
{
  //if mouse moves - update mouse cursor position
  if (event == cv::EVENT_MOUSEMOVE)
  {
    RegionBlur& blur = *(RegionBlur*)params;
    blur.setMousePose(x, y);
  }
}

int main()
{
  //reading config.ini file
  boost::property_tree::ptree pt;
  boost::property_tree::ini_parser::read_ini("../config.ini", pt);

  //opening video file
  cv::VideoCapture video;
  RegionBlurParams blur_params;
  if (!pt.empty())
  {
    std::string video_file = pt.get<std::string>("input.input_file");
    if (!video_file.empty())
    {
      
      std::cout << "Trying to open video: " << video_file << std::endl;
      video = cv::VideoCapture(video_file);
      if (!video.isOpened())
      {
        std::cerr << "Unable to open video" << std::endl;
        return -1;
      }
      std::cout << "Video openned." << std::endl;
    }
    
    blur_params.video_w = (int)video.get(cv::CAP_PROP_FRAME_WIDTH);
    blur_params.video_h = (int)video.get(cv::CAP_PROP_FRAME_HEIGHT);
    
    blur_params.kernel_w = pt.get<int>("region_blur.kernel_w");
    blur_params.kernel_h = pt.get<int>("region_blur.kernel_h");
    blur_params.region_w = pt.get<int>("region_blur.region_w");
    blur_params.region_h = pt.get<int>("region_blur.region_h");
    blur_params.anchor_x = pt.get<int>("region_blur.anchor_x");
    blur_params.anchor_y = pt.get<int>("region_blur.anchor_y");
    blur_params.borderType = pt.get<int>("region_blur.borderType");
    
  }
  else
  {
    std::cerr << "Properties are empty" << std::endl;
    return -1; //config file was not opened
  }
  
  //initializing RegionBlur instance
  RegionBlur region_blur(blur_params);

  cv::namedWindow("blur_app", cv::WINDOW_FREERATIO);

  cv::setMouseCallback("blur_app", onMouse, &region_blur);

  cv::Mat img;
  //processing video
  while (video.grab())
  {
    video.retrieve(img);
    region_blur.process(img);
    
    //press ESC button to exit
    int key = cv::waitKey(3);
    if (key == 27)
      return 0;

    cv::imshow("blur_app", img);
  }
  
  return 0;
}