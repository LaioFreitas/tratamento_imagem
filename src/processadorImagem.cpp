#include <cmath>
#include <iostream>
// #include <opencv2/core.hpp>
#include <opencv2/core/hal/interface.h>
#include <opencv2/core/types.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgcodecs.hpp>
#include <ostream>
#include <processadorImagem.hpp>

ProcessadorImagem::ProcessadorImagem()
    : image_path(""), LUTBrightness(256), LUTGammaCorrection(256),
      image(800, 600, CV_8UC1, cv::Scalar(0)),
      image_proc(800, 600, CV_8UC1, cv::Scalar(0)) {}

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
  return cv::imwrite(target_path, image_proc);
}

void ProcessadorImagem::negative() {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }
  // pixel a pixel
  // for (int i = 0; i < image.rows; i++) {
  //   uchar *pixel = image.ptr<uchar>(i);
  //   for (int j = 0; j < image.cols; j++) {
  //     pixel[j] = 255 - pixel[j];
  //   }
  // }
  cv::Mat image_neg;
  cv::bitwise_not(image, image_neg);
  image_proc = image_neg;
}

void ProcessadorImagem::gammaCorrection(float gamma) {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }
  // lookup table
  if (lastGamma != gamma) {
    for (int i = 0; i < 256; i++) {
      float result = std::pow(i / 255, gamma) * 255;
      if (result > 255)
        result = 255;
      if (result < 0)
        result = 0;
      LUTGammaCorrection[i] = result;
    }
  }
  for (int y = 0; y < image.rows; y++) {
    uchar *pixel = image.ptr<uchar>(y);
    uchar *pixel_proc = image_proc.ptr<uchar>(y);
    for (int x = 0; x < image.cols; y++) {
      uchar valor_pixel = pixel[x];
      pixel_proc[x] = LUTGammaCorrection[valor_pixel];
    }
  }
}

void ProcessadorImagem::thresHolding(uchar threshold) {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }

  for (int y = 0; y < image.rows; y++) {
    uchar *pixel = image.ptr<uchar>(y);
    uchar *pixel_proc = image_proc.ptr<uchar>(y);
    for (int x = 0; x < image.cols; y++) {
      (pixel[x] > threshold) ? pixel_proc[x] = 255 : pixel_proc[x] = 0;
    }
  }
}

void ProcessadorImagem::brightnessAdj(uchar brightness) {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }

  if (brightness != lastBrightness) {
    lastBrightness = brightness;
    for (int i = 0; i < 256; i++) {
      int result = i + brightness;
      if (brightness > 255)
        result = 250;
      else if (brightness < 0)
        result = 0;

      LUTBrightness[i] = static_cast<uchar>(result);
    }

    for (int y = 0; y < image.rows; y++) {
      uchar *pixel = image.ptr<uchar>(y);
      uchar *pixel_proc = image_proc.ptr<uchar>(y);
      for (int x = 0; x < image.cols; x++) {
        uchar pixel_value = pixel[x];
        pixel_proc[x] = LUTBrightness[pixel[x]];
      }
    }
  }
}

void ProcessadorImagem::simpleScale(int sx, int sy) {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }
  image_proc = cv::Mat::zeros(sy * image.rows, sx * image.cols, CV_8UC1);

  for (int y = 0; y < image_proc.rows; y++) {
    uchar *pixel_proc = image_proc.ptr<uchar>(y);
    uchar *pixel = image.ptr<uchar>(y / sy);
    for (int x = 0; x < image_proc.cols; x++) {
      pixel_proc[x] = pixel[x / sx];
    }
  }
}

void ProcessadorImagem::scale(int sx, int sy) {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }
  image_proc = cv::Mat::zeros(sy * image.rows, sx * image.cols, CV_8UC1);

  for (int i = 0; i < image_proc.rows; i++) {
    uchar *pixel_proc = image_proc.ptr<uchar>(i);
    for (int j = 0; j < image_proc.cols; j++) {

      float y = i / static_cast<float>(sy);
      float x = j / static_cast<float>(sx);

      int yp = static_cast<int>(y);
      int xp = static_cast<int>(x);

      float py = y - yp;
      float px = x - xp;

      float color1 = (1 - py) * image.ptr<uchar>(yp)[xp] +
                     py * image.ptr<uchar>(yp + 1)[xp];
      float color2 = (1 - py) * image.ptr<uchar>(yp)[xp + 1] +
                     py * image.ptr<uchar>(yp + 1)[xp + 1];

      float colorFinal = (1 - px) * color1 + px * color2;
      pixel_proc[j] = static_cast<uchar>(colorFinal);
    }
  }
}

void ProcessadorImagem::rotation(int angle) {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }

  float ang = angle * M_PI / 180;

  cv::Mat pointsMat = cv::Mat(4, 2, CV_32FC1);
  pointsMat.at<float>(0, 0) = 0.0f;
  pointsMat.at<float>(0, 1) = 0.0f;
  pointsMat.at<float>(1, 0) = 0.0f;
  pointsMat.at<float>(1, 1) = (float)image.cols;
  pointsMat.at<float>(2, 0) = (float)image.rows;
  pointsMat.at<float>(2, 1) = 0.0f;
  pointsMat.at<float>(3, 0) = (float)image.rows;
  pointsMat.at<float>(3, 1) = (float)image.cols;

  cv::Mat rotationMatrix = cv::Mat(2, 2, CV_32FC1);
  rotationMatrix.at<float>(0, 0) = cos(ang);
  rotationMatrix.at<float>(0, 1) = -sin(ang);
  rotationMatrix.at<float>(1, 0) = sin(ang);
  rotationMatrix.at<float>(1, 1) = cos(ang);

  cv::Mat pointsRotated = pointsMat * rotationMatrix;

  for (int i = 0; i < pointsRotated.rows; i++) {
    for (int j = 0; j < pointsRotated.cols; j++) {
      std::cout << pointsRotated.at<float>(i, j) << " ";
    }
    std::cout << std::endl;
  }

  double xmax, xmin, ymax, ymin;
  cv::minMaxLoc(pointsRotated.col(1), &xmin, &xmax);
  cv::minMaxLoc(pointsRotated.col(0), &ymin, &ymax);

  image_proc = cv::Mat(static_cast<int>(std::round(ymax - ymin)),
                       static_cast<int>(std::round(xmax - xmin)), CV_8UC1,
                       cv::Scalar(0));

  for (int i = 0; i < image_proc.rows; i++) {
    uchar *pixel_proc = image_proc.ptr<uchar>(i);
    for (int j = 0; j < image_proc.cols; j++) {

      float x = (j + xmin) * cos(-ang) - (i + ymin) * sin(-ang);
      float y = (j + xmin) * sin(-ang) + (i + ymin) * cos(-ang);
      int xaux = static_cast<int>(x);
      int yaux = static_cast<int>(y);

      if (xaux >= 0 && yaux >= 0 && xaux < image.cols && yaux < image.rows) {
        pixel_proc[j] = image.ptr<uchar>(yaux)[xaux];
      } else {
        pixel_proc[j] = 0;
      }
    }
  }
}

void ProcessadorImagem::rotation(int angle, cv::Point2d rotationPoint) {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }

  float ang = angle * M_PI / 180;

  // std::vector<cv::Point2d> edgePoint;
  // edgePoint.push_back(cv::Point2d(0, 0));
  // edgePoint.push_back(cv::Point2d(0, image.cols));
  // edgePoint.push_back(cv::Point2d(image.rows, 0));
  // edgePoint.push_back(cv::Point2d(image.rows, image.cols));

  cv::Mat pointsMat = cv::Mat(4, 2, CV_32FC1);
  pointsMat.at<float>(0, 0) = 0.0f;
  pointsMat.at<float>(0, 1) = 0.0f;
  pointsMat.at<float>(1, 0) = 0.0f;
  pointsMat.at<float>(1, 1) = (float)image.cols - 1;
  pointsMat.at<float>(2, 0) = (float)image.rows - 1;
  pointsMat.at<float>(2, 1) = 0.0f;
  pointsMat.at<float>(3, 0) = (float)image.rows - 1;
  pointsMat.at<float>(3, 1) = (float)image.cols - 1;

  cv::Mat rotationMatrix = cv::Mat(2, 2, CV_32FC1);
  rotationMatrix.at<float>(0, 0) = cos(ang);
  rotationMatrix.at<float>(0, 1) = -sin(ang);
  rotationMatrix.at<float>(1, 0) = sin(ang);
  rotationMatrix.at<float>(1, 1) = cos(ang);
  // cv::Mat rotationMatrix = cv::Mat(2, 2, CV_32FC1);
  // rotationMatrix.at<float>(0, 0) = cos(ang);
  // rotationMatrix.at<float>(0, 1) = -sin(ang);
  // rotationMatrix.at<float>(1, 0) = sin(ang);
  // rotationMatrix.at<float>(1, 1) = cos(ang);
  for (int i = 0; i < pointsMat.rows; i++) {
    float pointx = pointsMat.ptr<float>(i)[0];
    float pointy = pointsMat.ptr<float>(i)[1];
    pointsMat.ptr<float>(i)[0] -= rotationPoint.x;
    pointsMat.ptr<float>(i)[1] -= rotationPoint.y;
    std::cout << '(' << pointsMat.ptr<float>(i)[0] << ','
              << pointsMat.ptr<float>(i)[1] << ')' << std::endl;
  }
  std::cout << "cordenadas novas da imagem rodada" << std::endl;

  cv::Mat pointsRotated = pointsMat * rotationMatrix;
  for (int i = 0; i < pointsMat.rows; i++) {
    pointsRotated.ptr<float>(i)[0] += rotationPoint.x;
    pointsRotated.ptr<float>(i)[1] += rotationPoint.y;
    std::cout << '(' << pointsRotated.ptr<float>(i)[0] << ','
              << pointsRotated.ptr<float>(i)[1] << ')' << std::endl;
  }

  double xmax, xmin, ymax, ymin;
  cv::minMaxLoc(pointsRotated.col(1), &xmin, &xmax);
  cv::minMaxLoc(pointsRotated.col(0), &ymin, &ymax);

  image_proc = cv::Mat(static_cast<int>(std::round(ymax - ymin)),
                       static_cast<int>(std::round(xmax - xmin)), CV_8UC1,
                       cv::Scalar(0));

  for (int i = 0; i < image_proc.rows; i++) {
    uchar *pixel_proc = image_proc.ptr<uchar>(i);
    for (int j = 0; j < image_proc.cols; j++) {

      float x = (j + xmin - rotationPoint.x) * cos(-ang) -
                (i + ymin - rotationPoint.y) * sin(-ang);
      float y = (j + xmin - rotationPoint.x) * sin(-ang) +
                (i + ymin - rotationPoint.y) * cos(-ang);
      int xaux = static_cast<int>(std::round(x + rotationPoint.x));
      int yaux = static_cast<int>(std::round(y + rotationPoint.y));

      if (xaux >= 0 && yaux >= 0 && xaux < image.cols && yaux < image.rows) {
        pixel_proc[j] = image.ptr<uchar>(yaux)[xaux];
      } else {
        pixel_proc[j] = 0;
      }
    }
  }
}

void ProcessadorImagem::mediaFilter(std::vector<int> kernel) {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }
  int a = (kernel.at(0) - 1) / 2;
  int b = (kernel.at(1) - 1) / 2;
  int m = kernel.at(0);
  int n = kernel.at(1);
  image_proc = image.clone();
  int radius = m / 2;

  cv::Mat imageAux(image.rows + a * 2, image.cols + b * 2, CV_32FC1,
                   cv::Scalar(0));

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      imageAux.ptr<float>(i + a)[j + b] =
          static_cast<float>(image.ptr<uchar>(i)[j]) / 255;
    }
  }

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      float sumPixel = 0;

      for (int dy = -m / 2; dy <= radius; dy++) {
        for (int dx = -n / 2; dx <= radius; dx++) {
          sumPixel += imageAux.ptr<float>(i + radius + dy)[j + radius + dx];
        }
      }
      image_proc.ptr<uchar>(i)[j] =
          static_cast<uchar>((sumPixel / (m * n)) * 255);
    }
  }
}

void ProcessadorImagem::medianFilter(const int kernel) {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }

  int a = kernel - 1 / 2;
  int radius = kernel / 2;
  image_proc = image.clone();

  cv::Mat imageAux(image.rows + a * 2, image.cols + a * 2, CV_32FC1,
                   cv::Scalar(0));

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      imageAux.ptr<float>(i + a)[j + a] =
          static_cast<float>(image.ptr<uchar>(i)[j]) / 255;
    }
  }
  cv::namedWindow("teste", cv::WINDOW_NORMAL);
  cv::imshow("teste", imageAux);

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      std::vector<float> filter;

      for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
          filter.push_back(
              imageAux.at<float>(i + radius + dy, j + radius + dx));
        }
      }
      std::sort(filter.begin(), filter.end());

      image_proc.ptr<uchar>(i)[j] =
          static_cast<uchar>(filter.at((kernel * kernel) / 2) * 255);
    }
  }
}

void ProcessadorImagem::histogramEqualization() {
  if (isEmpty()) {
    std::cout << "ERROR: nao ha imagem carregada" << std::endl;
    return;
  }

  std::vector<uchar> histogramNomalized(256, 0);

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
      histogramNomalized.at(image.ptr<uchar>(i)[j]) +=
          1 / (image.rows * image.cols);
    }
  }

  std::vector<uchar> tranformationIntensity(256, 0);

  for (int i = 0; i < histogramNomalized.size(); i++) {
    for (int j = 0; j < i; j++) {
      tranformationIntensity.at(i) += histogramNomalized.at(j);
    }
    tranformationIntensity.at(i) =
        std::round((256 - 1) * tranformationIntensity.at(i));
  }

  for (int i = 0; i < image.rows; i++) {
    for (int j = 0; j < image.cols; j++) {
    }
  }
}

cv::Mat ProcessadorImagem::getImage() const { return image_proc; }
bool ProcessadorImagem::isEmpty() const { return image.empty(); }
