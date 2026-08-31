#ifndef PROCESSADOR_IMAGEM_H
#define PROCESSADOR_IMAGEM_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <string>

class ProcessadorImagem {
private:
  cv::Mat image;
  std::string image_path;

public:
  ProcessadorImagem();
  explicit ProcessadorImagem(const std::string &path);
  ~ProcessadorImagem();

  bool loadImage(const std::string &path, int mode = cv::IMREAD_GRAYSCALE);
  bool saveImage(const std::string &target_path) const;
  bool showImage(const std::string &window_name) const;

  void negative();
  // TODO: definir os outros metodos necessarios

  bool isEmpty() const;
};

#endif
