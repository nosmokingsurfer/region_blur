#include <opencv2/videoio.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#include <iostream>

using namespace cv;

void myBlur(int event, int x, int y, int flags, void* params)
{
  cv::Size reg(100, 100);
  if (event == cv::EVENT_MOUSEMOVE)
  {
    Mat& temp = *(Mat*)params;
    
    if (cv::Rect(cv::Point(reg.width/2, reg.height/2), cv::Size(temp.cols - reg.width/2, temp.rows - reg.height/2)).contains(cv::Point(x, y)))
    {
      cv::Rect region(Point(x - reg.width/2, y - reg.height / 2), cv::Point(x + reg.width / 2, y + reg.height/2));

      cv::GaussianBlur(temp(region), temp(region), cv::Size(51, 51), 0, 0);
      //temp(region).setTo(0);
      cv::rectangle(temp, region, cv::Scalar(100, 200, 100), 1);
    }
  }
}


int main()
{
  //auto video = cv::VideoCapture("test.avi");
  auto video = cv::VideoCapture("C:/Users/nosmokingsurfer/source/repos/skytrack/IMG_3676.MOV");
  if (video.isOpened())
  {

    cv::Mat tmp;
    cv::namedWindow("blur_app");
    
    cv::setMouseCallback("blur_app", myBlur, &tmp);
    
    while (video.grab())
    {
      video.retrieve(tmp);
      
      //cv::GaussianBlur(tmp, tmp, cv::Size(51, 51),0,0);
      
      int key = cv::waitKey(20);
      if (key == 27)
        return 1;

      cv::imshow("blur_app", tmp);
    }
  }
  return 1;
}