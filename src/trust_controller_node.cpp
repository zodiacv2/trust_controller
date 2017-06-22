/*
trust_controller:
takes 'Twist' information from "joystick", navigation stack "move_base"
takes '' information from interface to determine level of autonomy
sends 'Twist' information to "cmd_vel", interface(?)
*/

//includes:
#include "ros/ros.h"
#include "geometry_msgs/Twist.h"

//globals
ros::Publisher cmd_pub;
ros::Publisher ui_pub;//potentially unnecessary

class referee{
  public:
    static geometry_msgs::Twist safetyCheck(geometry_msgs::Twist test){
      if (test.linear.x > speedLimit){test.linear.x = speedLimit;}
      if (test.angular.z > turnLimit){test.angular.z = turnLimit;}
    return test;}
  private:
//speed limit subject to change
    static const float speedLimit = 4;
    static const float turnLimit  = 50;
};

//callbacks
void nsCallback(geometry_msgs::Twist autoTwist){
  cmd_pub.publish(autoTwist);
}
void joyCallback(geometry_msgs::Twist joyTwist){
  joyTwist = referee::safetyCheck(joyTwist);
  cmd_pub.publish(joyTwist);
}

int main(int argc, char **argv){
  ros::init(argc, argv,"trust_controller");
  ros::NodeHandle n;
  ros::Subscriber ns_sub = n.subscribe("/auto_vel",1000, nsCallback);
  ros::Subscriber joy_sub= n.subscribe("/joy_vel", 1000,joyCallback);
  cmd_pub = n.advertise<geometry_msgs::Twist>("cmd_vel",1000);
//PLEASE CHANGE PUBLISHED TOPIC NAME AND DELETE ME <------------
//  ui_pub  = n.advertise<geometry_msgs::Twist>("TBD",1000);//potentially unnecessary
  ros::spin();
return 0;}

