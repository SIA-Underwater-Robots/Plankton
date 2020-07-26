// Copyright (c) 2016 The UUV Simulator Authors.
// All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __UUV_DVL_ROS_PLUGIN_HH__
#define __UUV_DVL_ROS_PLUGIN_HH__

#include <gazebo/gazebo.hh>

#include <rclcpp/rclcpp.hpp>
#include <message_filters/subscriber.h>
#include <message_filters/time_synchronizer.h>
#include <sensor_msgs/msg/range.hpp>

#include <geometry_msgs/msg/twist_with_covariance_stamped.hpp>
#include <geometry_msgs/msg/pose_stamped.hpp>

#include <uuv_sensor_ros_plugins/ROSBaseModelPlugin.h>
#include <uuv_sensor_ros_plugins_msgs/msg/dvl.hpp>
#include <uuv_sensor_ros_plugins_msgs/msg/dvl_beam.hpp>


#include <tf2_ros/transform_listener.h>

#include <vector>

//#include "SensorDvl.pb.h"

#define ALTITUDE_OUT_OF_RANGE -1.0
namespace gazebo
{
  /// TODO: Modify computation of velocity using the beams
  class DVLROSPlugin : public ROSBaseModelPlugin
  {
    /// \brief Class constructor
    public: DVLROSPlugin();

    /// \brief Class destructor
    public: virtual ~DVLROSPlugin();

    /// \brief Load the plugin
    public: virtual void Load(physics::ModelPtr _model, sdf::ElementPtr _sdf);

    /// \brief Update sensor measurement
    protected: virtual bool OnUpdate(const common::UpdateInfo& _info);

    /// \brief Get beam Range message update
    protected: void OnBeamCallback(const sensor_msgs::msg::Range::SharedPtr _range0,
      const sensor_msgs::msg::Range::SharedPtr _range1,
      const sensor_msgs::msg::Range::SharedPtr _range2,
      const sensor_msgs::msg::Range::SharedPtr _range3);

    /// \brief Updates the poses of each beam wrt the DVL frame
    protected: bool UpdateBeamTransforms();

    protected: bool beamTransformsInitialized;

    /// \brief Measured altitude in meters
    protected: double altitude;

    /// \brief ROS DVL message
    protected: uuv_sensor_ros_plugins_msgs::msg::DVL dvlROSMsg;

    protected: std::vector<uuv_sensor_ros_plugins_msgs::msg::DVLBeam> dvlBeamMsgs;

    /// \brief ROS publisher for twist data.
    protected: rclcpp::Publisher<geometry_msgs::msg::TwistWithCovarianceStamped>::SharedPtr twistPub;

    /// \brief Store pose message since many attributes do not change (cov.).
    protected: geometry_msgs::msg::TwistWithCovarianceStamped twistROSMsg;

    /// \brief List of beam links
    protected: std::vector<std::string> beamsLinkNames;

    /// \brief List of beam topics
    protected: std::vector<std::string> beamTopics;

    /// \brief List of poses of each beam wrt to the DVL frame
    protected: std::vector<ignition::math::Pose3d> beamPoses;

    protected: boost::shared_ptr<message_filters::TimeSynchronizer<
      sensor_msgs::msg::Range, sensor_msgs::msg::Range, sensor_msgs::msg::Range, sensor_msgs::msg::Range>>
      syncBeamMessages;

    protected: std::shared_ptr<message_filters::Subscriber<
      sensor_msgs::msg::Range>> beamSub0;

    protected: std::shared_ptr<message_filters::Subscriber<
      sensor_msgs::msg::Range>> beamSub1;

    protected: std::shared_ptr<message_filters::Subscriber<
      sensor_msgs::msg::Range>> beamSub2;

    protected: std::shared_ptr<message_filters::Subscriber<
      sensor_msgs::msg::Range>> beamSub3;

    protected: std::unique_ptr<tf2_ros::TransformListener> myTransformListener;
    std::unique_ptr<tf2_ros::Buffer> myBuffer;

    /// \brief publisher for transporting measurement messages.
    protected: rclcpp::Publisher<uuv_sensor_ros_plugins_msgs::msg::DVL>::SharedPtr rosSensorOutputPub;
    
  };
}

#endif // __UUV_DVL_ROS_PLUGIN_HH__