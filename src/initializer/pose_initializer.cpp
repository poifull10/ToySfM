#include "pose_initializer.h"

#include <Eigen/Core>
#include <Eigen/Geometry>
#include <opencv2/core/eigen.hpp>
#include <opencv2/opencv.hpp>

#include "../matcher/image_matcher.h"

namespace tsfm
{
Pose PoseInitializer::operator()(const Image& img1, const Image& img2,
                                 const CameraModel& cm) const
{
  const auto img1_ptr = std::make_shared<Image>(img1);
  const auto img2_ptr = std::make_shared<Image>(img2);
  ImageMatcher im(img1_ptr, img2_ptr);
  im.extractFeatures();
  const auto matched = im.match();

  assert(matched.size() >= 5);

  std::vector<cv::Point2f> points1, points2;

  for (const auto [v1, v2] : matched)
  {
    points1.emplace_back(v1[0], v1[1]);
    points2.emplace_back(v2[0], v2[1]);
  }

  const cv::Mat K = cm.K();
  double apertureW, apertureH, fovx, fovy, focalLength, aspectRatio;
  cv::Point2d principalPoint;
  cv::calibrationMatrixValues(K, img1.image().size(), apertureW, apertureH,
                              fovx, fovy, focalLength, principalPoint,
                              aspectRatio);
  const cv::Mat E = cv::findEssentialMat(points1, points2, K);
  cv::Mat R, t;
  cv::recoverPose(E, points1, points2, R, t, focalLength);
  Eigen::Matrix<double, 3, 3> eigenR;
  eigenR(0, 0) = R.at<double>(0, 0);
  eigenR(1, 0) = R.at<double>(1, 0);
  eigenR(2, 0) = R.at<double>(2, 0);
  eigenR(0, 1) = R.at<double>(0, 1);
  eigenR(1, 1) = R.at<double>(1, 1);
  eigenR(2, 1) = R.at<double>(2, 1);
  eigenR(0, 2) = R.at<double>(0, 2);
  eigenR(1, 2) = R.at<double>(1, 2);
  eigenR(2, 2) = R.at<double>(2, 2);
  Eigen::Quaterniond quat(eigenR);

  return Pose({t.at<double>(0, 0), t.at<double>(1, 0), t.at<double>(2, 0)},
              {quat.x(), quat.y(), quat.z(), quat.w()});
}
} // namespace tsfm