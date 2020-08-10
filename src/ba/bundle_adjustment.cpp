#include "bundle_adjustment.h"

#include "../initializer/pose_initializer.h"

namespace tsfm
{

void BundleAdjustment::addFrame(std::shared_ptr<Frame> frame)
{
  frames_.push_back(frame);
}

void BundleAdjustment::addCameraModel(std::shared_ptr<CameraModel> cm)
{
  cm_ = cm;
}

void BundleAdjustment::initialization()
{
  if (frames_.empty())
  {
    return;
  }

  frames_.front()->setPose(Pose());
  if (frames_.size() == 1)
  {
    return;
  }

  PoseInitializer pi;
  for (size_t i = 1; i < frames_.size(); i++)
  {
    // TODO: validate pose
    const auto estimatedPose =
      pi(frames_[i - 1]->image(), frames_[i]->image(), *cm_);
    frames_[i]->setPose(estimatedPose);
  }
}

double BundleAdjustment::optimize(size_t N)
{
  return 0.;
}

} // namespace tsfm
