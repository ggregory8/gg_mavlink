#include "ros/ros.h"
#include "std_msgs/String.h"
#include "mavlink_ros/Mavlink.h"    // This is the definition for the format of the ROS message to the mavlink_ros node

#include <iostream>
#include <string>
#include <sstream>

using namespace std;    // instead of writing std::string etc.

/**
 * This tutorial demonstrates simple sending of messages over the ROS system.
 */
int main(int argc, char **argv)
{
  /**
   * The ros::init() function needs to see argc and argv so that it can perform
   * any ROS arguments and name remapping that were provided at the command line. For programmatic
   * remappings you can use a different version of init() which takes remappings
   * directly, but for most command-line programs, passing argc and argv is the easiest
   * way to do it.  The third argument to init() is the name of the node.
   *
   * You must call one of the versions of ros::init() before using any other
   * part of the ROS system.
   */
  ros::init(argc, argv, "mavlink_send");
  
  // Reference code from mavlink_ros_serial.cpp
  //ros::init(argc, argv, "mavlink_ros_serial");
  // SETUP ROS
  //ros::NodeHandle mavlink_nh("/mavlink"); // always root namespace since I assume it's somewhat "broadcast"
  //mavlink_sub = mavlink_nh.subscribe("to", 1000, mavlinkCallback);
  //mavlink_pub = mavlink_nh.advertise<mavlink_ros::Mavlink>("from", 1000);


  /**
   * NodeHandle is the main access point to communications with the ROS system.
   * The first NodeHandle constructed will fully initialize this node, and the last
   * NodeHandle destructed will close down the node.
   */
  ros::NodeHandle n;

  /**
   * The advertise() function is how you tell ROS that you want to
   * publish on a given topic name. This invokes a call to the ROS
   * master node, which keeps a registry of who is publishing and who
   * is subscribing. After this advertise() call is made, the master
   * node will notify anyone who is trying to subscribe to this topic name,
   * and they will in turn negotiate a peer-to-peer connection with this
   * node.  advertise() returns a Publisher object which allows you to
   * publish messages on that topic through a call to publish().  Once
   * all copies of the returned Publisher object are destroyed, the topic
   * will be automatically unadvertised.
   *
   * The second parameter to advertise() is the size of the message queue
   * used for publishing messages.  If messages are published more quickly
   * than we can send them, the number here specifies how many messages to
   * buffer up before throwing some away.
   */
 
  //ros::Publisher chatter_pub = n.advertise<std_msgs::String>("chatter", 1000);
  ros::Publisher chatter_pub = n.advertise<mavlink_ros::Mavlink>("/mavlink/to", 1000);
  //ros::Subscriber mavlink_sub = n.subscribe("from", 1000, mavlinkCallback);
  

  //mavlink_pub = mavlink_nh.advertise<mavlink_ros::Mavlink>("from", 1000);

  ros::Rate loop_rate(1);       // in Hz

  ROS_INFO("Mavlink_send started V2. Going into loop at 1Hz");

  /**
   * A count of how many messages we have sent. This is used to create
   * a unique string for each message.
   */
  int count = 0;
  while (ros::ok())
  {
    /**
     * This is a message object. You stuff it with data, and then publish it.
     */
    std_msgs::String msg;
    mavlink_ros::Mavlink rosmavlink_msg;

    string cmd_inputs = "";
    char cmd_inputc = {0};

    while(true)
    {
      cout << "Please enter a command char: ";
      getline(cin, cmd_inputs);

      if (cmd_inputs.length() == 1){
        if (cmd_inputs[0] == 'a'){
          cmd_inputc = cmd_inputs[0];
          break;
        }
      }
      cout << "Invalid char please try again" << endl;
    }
    cout << "You entered: " << cmd_inputc << endl << endl;




    // mavlink_ros message structure
    // note this is different to the real mavlink message to the UAV

//uint8 len\n\
//uint8 seq\n\
//uint8 sysid\n\
//uint8 compid\n\
//uint8 msgid\n\
//bool fromlcm\n\
//uint64[] payload64\n\
  
    rosmavlink_msg.len = 5;
    rosmavlink_msg.seq = count;
    rosmavlink_msg.sysid = 200;
    rosmavlink_msg.compid = 150;
    rosmavlink_msg.msgid = 42;

    for (int i = 0; i < rosmavlink_msg.len / 8; i++)
    {
      rosmavlink_msg.payload64[i] = i;
    }

    std::stringstream ss;
    ss << "v1 Sending Mavink Message " << count;
    msg.data = ss.str();

    ROS_INFO("%s", msg.data.c_str());

    
    /**
     * The publish() function is how you send messages. The parameter
     * is the message object. The type of this object must agree with the type
     * given as a template parameter to the advertise<>() call, as was done
     * in the constructor above.
     */
    //chatter_pub.publish(msg);
    chatter_pub.publish(rosmavlink_msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
