#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>

int main() {
  // setenv("QT_QPA_PLATFORM", "xcb", 1);
  cv::Mat image = cv::imread(
      "../../DIP3E_Original_Images_CH01/Fig0101(1921 digital image).tif",
      cv::IMREAD_GRAYSCALE);

  if (image.empty()) {
    std::cout << "erro ai caba" << std::endl;
    return -1;
  }
  std::cout << "rows: " << image.rows << "\n"
            << "colluns: " << image.cols << std::endl;
  //
  // for (int i = 0; i < image.rows; i++) {
  //   uchar *pixel = image.ptr<uchar>(i);
  //   for (int j = 0; j < image.cols; j++) {
  //     pixel[j] = 255 - pixel[j];
  //   }
  // }
  cv::Mat image_neg;
  cv::Mat image_concat;
  cv::bitwise_not(image, image_neg);
  // cv::Mat negativo = 255 - image;

  cv::namedWindow("Minha janela", cv::WINDOW_NORMAL);
  cv::namedWindow("Segunda janela", cv::WINDOW_NORMAL);
  cv::hconcat(image, image_neg, image_concat);

  cv::moveWindow("Minha janela", 100, 100);
  cv::moveWindow("Segunda janela", 100 + image.cols, 100);
  cv::resizeWindow("Minha janela", 200, 150);
  cv::resizeWindow("Segunda janela", 200, 150);
  cv::imshow("Minha janela", image);
  cv::imshow("Segunda janela", image_neg);
  while (cv::waitKey(30) != 'q')
    ;
  cv::destroyAllWindows();
  return 0;
}
