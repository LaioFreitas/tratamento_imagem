#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <processadorImagem.hpp>

ProcessadorImagem::ProcessadorImagem() : image_path("") {}

ProcessadorImagem::ProcessadorImagem(const std::string &path) {
  loadImage(path);
}

ProcessadorImagem::~ProcessadorImagem() { image.release(); }

bool ProcessadorImagem::loadImage(const std::string &path, int mode) {
  image = cv::imread(path, mode);

  if (isEmpty()) {
    std::cout << "ERRO: nao foi possivel carreger a imagem" << std::endl;
    return false;
  }

  return true;
}

bool ProcessadorImagem::saveImage(const std::string &target_path) const {
  if (isEmpty()) {
    std::cout << "ERROR: nao foi possivel salvar" << std::endl;
    return false;
  }
  return cv::imwrite(target_path, image);
}

bool ProcessadorImagem::isEmpty() const { return image.empty(); }

void ProcessadorImagem::negative() {
  // pixel a pixel
  // for (int i = 0; i < image.rows; i++) {
  //   uchar *pixel = image.ptr<uchar>(i);
  //   for (int j = 0; j < image.cols; j++) {
  //     pixel[j] = 255 - pixel[j];
  //   }
  // }
  cv::Mat image_neg;
  cv::bitwise_not(image, image_neg);
  image = image_neg;
}
