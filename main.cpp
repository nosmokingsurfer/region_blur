#include <iostream>
#include <opencv2/videoio.hpp>
#include <opencv2/highgui/highgui.hpp>

#include <chrono>

#include <region_blur.h>

void onMouse(int event, int x, int y, int flags, void* params)
{
  RegionBlur& blur = *(RegionBlur*)params;
  blur.setMousePose(x, y);
}


int main()
{
  //opening config file. YAML file - cv:::FileStorage to read/write.
  cv::FileStorage fs("../config.ini", cv::FileStorage::READ);

  cv::VideoCapture video;
  RegionBlur region_blur;

  if (fs.isOpened())
  {
    if ((!fs["input_file"].empty()) && (fs["input_file"].isString()))
    {
      std::cout << "Trying to open video: " << fs["input_file"].string() << std::endl;
      video = cv::VideoCapture(fs["input_file"].string());
      if (!video.isOpened())
      {
        std::cout << "Unable to open video" << std::endl;
        return -1;
      }
    }
  }
  else
  {
    std::cout << "Unable to open config file" << std::endl;
    return -1; //config file is not opened
  }
  
  if (!region_blur.init(fs.root()))
  {
    std::cout << "Unable to initialize RegionBlur instance" << std::endl;
    return -1;
  }

  fs.release();

  //processing video
  if (video.isOpened())
  {

    //auto t_start = std::chrono::system_clock::now();
    
    region_blur.setImageSize((int)video.get(cv::CAP_PROP_FRAME_WIDTH), (int)video.get(cv::CAP_PROP_FRAME_HEIGHT));

    cv::namedWindow("blur_app", cv::WINDOW_FREERATIO);

    cv::setMouseCallback("blur_app", onMouse, &region_blur);

    cv::Mat img;

    while (video.grab())
    {
      video.retrieve(img);
      
      region_blur.process(img);

      int key = cv::waitKey(3);
      if (key == 27)
        return 1;

      cv::imshow("blur_app", img);
    }

    //auto t_finish = std::chrono::system_clock::now();
    //std::cout << "Duration = " << std::chrono::duration_cast<std::chrono::milliseconds>(t_finish - t_start).count() << " milliseconds" << std::endl;
  }
  else
  {
    std::cout << "Video is not opened" << std::endl;
  }

  return 1;
}