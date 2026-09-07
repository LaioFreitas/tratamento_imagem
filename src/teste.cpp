// #include <opencv2/core.hpp>
// #include <opencv2/core/mat.hpp>
// #include <opencv2/core/types.hpp>
// #include <opencv2/highgui.hpp>
// #include <opencv2/imgcodecs.hpp>
// #include <opencv2/opencv.hpp>
#include <processadorImagem.hpp>

int main() {
  // setenv("QT_QPA_PLATFORM", "xcb", 1);
  ProcessadorImagem *processor = new ProcessadorImagem(
      "/home/laio/Documents/faculdade/pdi/DIP3E_Original_Images_CH01/"
      "Fig0107(e)(cygnusloop-Xray).tif");

  // processor->negative();
  // processor->simpleScale(5, 3);
  // cv::Mat image_scale = processor->getImage();
  processor->medianFilter(3);

  cv::namedWindow("Minha janela", cv::WINDOW_NORMAL);
  cv::namedWindow("Segunda janela", cv::WINDOW_NORMAL);
  // cv::namedWindow("Segunda janela", cv::WINDOW_NORMAL); cv::hconcat(image,
  // image_neg, image_concat); cv::moveWindow("Minha janela", 100, 100);
  // cv::moveWindow("Segunda janela", 100 + image.cols, 100);
  // cv::resizeWindow("Minha janela", 200, 150);
  // cv::resizeWindow("Segunda janela", 200, 150);
  cv::imshow("Minha janela", processor->getImage());

  cv::imshow("Segunda janela", processor->image);

  while (cv::waitKey(30) != 'q')
    ;
  cv::destroyAllWindows();
  delete processor;
  return 0;
}
