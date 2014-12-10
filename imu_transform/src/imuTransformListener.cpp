#include <ros/ros.h>
#include <geometry_msgs/PointStamped.h>
#include <tf/transform_listener.h>

void transformVector(const tf::TransformListener& listener){
  //we'll create a point in the base_laser frame that we'd like to transform to the base_link frame
  geometry_msgs::Vector3Stamped imu_vector;
  imu_vector.header.frame_id = "base_imu";

  //we'll just use the most recent transform available for our simple example
  imu_vector.header.stamp = ros::Time();

  //just an arbitrary point in space
  imu_vector.vector.x = 1.0;
  imu_vector.vector.y = 0.2;
  imu_vector.vector.z = 0.0;

  try{
    geometry_msgs::Vector3Stamped base_point;
    listener.transformPoint("base_link", imu_vector, base_point);

    ROS_INFO("base_imu: (%.2f, %.2f. %.2f) -----> base_link: (%.2f, %.2f, %.2f) at time %.2f",
        imu_vector.vector.x, imu_vector.vector.y, imu_vector.vector.z,
        base_point.vector.x, base_point.vector.y, base_point.vector.z, base_point.header.stamp.toSec());
  }
  catch(tf::TransformException& ex){
    ROS_ERROR("Received an exception trying to transform a point from \"base_imu\" to \"base_link\": %s", ex.what());
  }
}

int main(int argc, char** argv){
  ros::init(argc, argv, "robot_tf_listener");
  ros::NodeHandle n;

  tf::TransformListener listener(ros::Duration(10));

  //we'll transform a point once every second
  ros::Timer timer = n.createTimer(ros::Duration(1.0), boost::bind(&transformVector, boost::ref(listener)));

  ros::spin();

}
