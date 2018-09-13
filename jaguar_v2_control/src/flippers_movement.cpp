#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/Float64.h"
#include "joint_state_controller/joint_state_controller.h"
#include "control_msgs/JointControllerState.h"
#include "geometry_msgs/Twist.h"

//control_msgs::JointControllerState left_flipper;

std_msgs::Float64 left_flipper_msg;
std_msgs::Float64 right_flipper_msg;

float flippers_velocity = 0.0;

void left_flipper_lecture(const control_msgs::JointControllerStateConstPtr& msg)
{
  float left_flipper = msg->process_value;
  ROS_INFO("Left flipper is in position: %.2f", left_flipper);
}

void right_flipper_lecture(const control_msgs::JointControllerStateConstPtr& msg)
{
  float right_flipper = msg->process_value;
  ROS_INFO("Right flipper is in position: %.2f", right_flipper);
}

void flippers_teleop_callback(const geometry_msgs::Twist::ConstPtr& vel)
{
  float velocity = vel->linear.x;
  flippers_velocity = velocity;

}


int main(int argc, char **argv)
{
  ros::init(argc, argv, "flippers_movement");
  ros::NodeHandle nh;

  ros::Publisher flipper_left_movement = nh.advertise<std_msgs::Float64>("/jaguar_v2/joint_left_flipper_controller/command", 1);
  ros::Publisher flipper_right_movement = nh.advertise<std_msgs::Float64>("/jaguar_v2/joint_right_flipper_controller/command", 1);

  ros::Subscriber flipper_left_position = nh.subscribe("/jaguar_v2/joint_left_flipper_controller/state", 1, left_flipper_lecture);
  ros::Subscriber flipper_right_position = nh.subscribe("/jaguar_v2/joint_right_flipper_controller/state", 1, right_flipper_lecture);

  ros::Subscriber cmd_vel_keyboard = nh.subscribe("/cmd_vel",1, flippers_teleop_callback);


  ros::Rate loop_rate(10);
  while (ros::ok())
  {
    left_flipper_msg.data = 10*flippers_velocity;
    right_flipper_msg.data = 10*-flippers_velocity;

    flipper_left_movement.publish(left_flipper_msg);
    flipper_right_movement.publish(right_flipper_msg);

    ros::spinOnce();

    loop_rate.sleep();
  }

  return 0;
}
