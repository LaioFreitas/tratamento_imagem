#ifndef PROCESSADOR_IMAGEM_H
#define PROCESSADOR_IMAGEM_H

#include <opencv2/core/types.hpp>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>
#include <string>

class ProcessadorImagem {
private:
  cv::Mat image_proc;
  std::string image_path;

  float lastGamma;
  std::vector<uchar> LUTGammaCorrection;

  uchar lastBrightness;
  std::vector<uchar> LUTBrightness;

public:
  cv::Mat image;
  ProcessadorImagem();
  explicit ProcessadorImagem(const std::string &path);
  ~ProcessadorImagem();

  bool loadImage(const std::string &path, int mode = cv::IMREAD_GRAYSCALE);
  bool saveImage(const std::string &target_path) const;
  bool showImage(const std::string &window_name) const;

  void negative();
  void gammaCorrection(float gamma);
  void thresHolding(uchar threshold);
  void brightnessAdj(uchar brightness);
  void simpleScale(int sx, int sy);
  void scale(int sx, int sy);
  void rotation(int angle);
  void rotation(int angle, cv::Point2d rotationPoint = cv::Point2d(0, 0));
  void mediaFilter(const std::vector<int> kernel);
  void medianFilter(const int kernel);
  void histogramEqualization();
  // TODO: definir os outros metodos necessarios
  // TODO: algoritimo da transformada de furrie
  // TODO: algoritimo da transformada rapida de furrie (FFT)

  bool isEmpty() const;
  cv::Mat getImage() const;
};

#endif
