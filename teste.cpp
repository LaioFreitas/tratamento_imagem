#include <iostream>
#include <opencv2/highgui.hpp>
#include <opencv2/opencv.hpp>

int main() {
  cv::Mat image = cv::imread(
      "/home/laio/Documents/programacao/tratamento_imagem/images.jpg");

  if (image.empty()) {
    std::cout << "erro ai caba" << std::endl;
    return -1;
  }

  cv::namedWindow("Minha janela", cv::WINDOW_AUTOSIZE);
  cv::imshow("minha janela", image);
  cv::waitKey(0);
  return 0;
}
