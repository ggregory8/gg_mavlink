#include "ros/ros.h"
#include "std_msgs/String.h"
#include "mavlink_ros/Mavlink.h"    // This is the definition for the format of the ROS message to the mavlink_ros node

#include <iostream>
#include <string>
#include <sstream>

using namespace std;    // instead of writing std::string etc.

// Desclare ROS publisher and subscribers here
ros::Publisher mavlink_pub
//ros::Subscriber mavlink_sub;
//ros::Publisher mavlink_pub;
std::string frame_id("fcu");  // mavlink_ros_serial had this declared globally
  

void quad_arm(void)
{
// Steps
// 1. Encode packet
  std_msgs::Header header;
  header.stamp = ros::Time::now();
  header.seq = imu_raw.time_usec / 1000;
  header.frame_id = frame_id;

  mavlink_ros::Mavlink rosmavlink_msg;
  
  mavlink_message_t msg;

  //mavlink_set_quad_swarm_roll_pitch_yaw_thrust_t sp;
  mavlink_command_long_t mypacket;
  
  mypacket.target_system = 250; ///< System which should execute the command
  mypacket.target_component = 250; ///< Component which should execute the command, 0 for all components
  mypacket.confirmation = 0; ///< 0: First transmission of this command. 1-255: Confirmation transmissions (e.g. for kill command)
  mypacket.command = MAV_CMD_COMPONENT_ARM_DISARM;    // ID=400
  mypacket.param1 = 1;   // 1 = Arm, 0 = Disarm
  mypacket.param2 = 0;   // Not used. Do we need to define these?
  mypacket.param3 = 0;   // Not used. Do we need to define these?
  mypacket.param4 = 0;   // Not used. Do we need to define these?
  mypacket.param5 = 0;   // Not used. Do we need to define these?
  mypacket.param6 = 0;   // Not used. Do we need to define these?
  mypacket.param7 = 0;   // Not used. Do we need to define these?

  //static inline uint16_t mavlink_msg_command_long_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_command_long_t* command_long)
  //MAV_COMP_ID_SYSTEM_CONTROL = 250
  mavlink_msg_command_long_encode(200, 150, msg, mypacket)

  // Do we need this header??
/*
  std_msgs::Header header;
  header.stamp = ros::Time::now();
  header.seq = imu_raw.time_usec / 1000;
  header.frame_id = frame_id;

  imu_msg->header = header;

          //OR 

  imu_msg->header.frame_id = frame_id;
  imu_msg->header.seq = imu_raw.time_usec / 1000;
  imu_msg->header.stamp = ros::Time::now();
*/

// this is the structure of rosmavlink_msg
//Header header
//uint8 len\n\
//uint8 seq\n\
//uint8 sysid\n\
//uint8 compid\n\
//uint8 msgid\n\
//bool fromlcm\n\
//uint64[] payload64\n\
  rosmavlink_msg.header = header;
  rosmavlink_msg.len = 5;             // What is this???
  rosmavlink_msg.seq = count;
  rosmavlink_msg.sysid = 200;
  rosmavlink_msg.compid = 150;
  rosmavlink_msg.msgid = 76 // COMMAND_LONG #76
  rosmavlink_msg.payload64 = msg.payload64;   // Copy the payload portion of the message I have just encoded to the ros mavlink message
  
  // Maybe I'll need to use this??
  //Copy payload from mavlink_msg (from ROS) to the new "real" mavlink message
  copy(mavlink_ros_msg.payload64.begin(), mavlink_ros_msg.payload64.end(), msg.payload64);

  mavlink_pub.publish(rosmavlink_msg);

//////////////////////////////////////////

/*
  typedef struct __mavlink_message {
  uint16_t checksum; /// sent at end of packet
  uint8_t magic;   ///< protocol magic marker
  uint8_t len;     ///< Length of payload
  uint8_t seq;     ///< Sequence of packet
  uint8_t sysid;   ///< ID of message sender system/aircraft
  uint8_t compid;  ///< ID of the message sender component
  uint8_t msgid;   ///< ID of message in payload
  uint64_t payload64[(MAVLINK_MAX_PAYLOAD_LEN+MAVLINK_NUM_CHECKSUM_BYTES+7)/8];
} mavlink_message_t;
*/
/////////////////////////////////////////
// From mavlink_ros_serial.cpp
//Copy payload from mavlink_msg (from ROS) to the new "real" mavlink message
//  copy(mavlink_ros_msg.payload64.begin(), mavlink_ros_msg.payload64.end(), msg.payload64);

//  mavlink_finalize_message_chan(&msg, mavlink_ros_msg.sysid, mavlink_ros_msg.compid, MAVLINK_COMM_0,
//                                mavlink_ros_msg.len, mavlink_crcs[msg.msgid]);

  /**
   * Send mavlink_message to UART
   */
//  if (verbose)
//    ROS_INFO("Sent Mavlink from ROS to UART, Message-ID: [%i]", mavlink_ros_msg.msgid);

  // Send message over serial port
//  static uint8_t buffer[MAVLINK_MAX_PACKET_LEN];
//  int messageLength = mavlink_msg_to_send_buffer(buffer, &msg);
//  if (debug)
//    printf("Writing %d bytes\n", messageLength);
//  int written = write(fd, (char*)buffer, messageLength);
 // tcflush(fd, TCOFLUSH);
  

/////////////////////////////////////////
// From mavlink_msg_command_long
  // MESSAGE COMMAND_LONG PACKING

//#define MAVLINK_MSG_ID_COMMAND_LONG 76

//typedef struct __mavlink_command_long_t
//{
// float param1; ///< Parameter 1, as defined by MAV_CMD enum.
// float param2; ///< Parameter 2, as defined by MAV_CMD enum.
// float param3; ///< Parameter 3, as defined by MAV_CMD enum.
// float param4; ///< Parameter 4, as defined by MAV_CMD enum.
// float param5; ///< Parameter 5, as defined by MAV_CMD enum.
// float param6; ///< Parameter 6, as defined by MAV_CMD enum.
// float param7; ///< Parameter 7, as defined by MAV_CMD enum.
// uint16_t command; ///< Command ID, as defined by MAV_CMD enum.
// uint8_t target_system; ///< System which should execute the command
// uint8_t target_component; ///< Component which should execute the command, 0 for all components
// uint8_t confirmation; ///< 0: First transmission of this command. 1-255: Confirmation transmissions (e.g. for kill command)
//} mavlink_command_long_t;
  __/**
 * @brief Encode a command_long struct into a message
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param command_long C-struct to read the message contents from
 */
//static inline uint16_t mavlink_msg_command_long_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_command_long_t* command_long)
//{
//  return mavlink_msg_command_long_pack(system_id, component_id, msg, command_long->target_system, command_long->target_component, command_long->command, command_long->confirmation, command_long->param1, command_long->param2, command_long->param3, command_long->param4, command_long->param5, command_long->param6, command_long->param7);
//}

/**
 * @brief Send a command_long message
 * @param chan MAVLink channel to send the message
 *
 * @param target_system System which should execute the command
 * @param target_component Component which should execute the command, 0 for all components
 * @param command Command ID, as defined by MAV_CMD enum.
 * @param confirmation 0: First transmission of this command. 1-255: Confirmation transmissions (e.g. for kill command)
 * @param param1 Parameter 1, as defined by MAV_CMD enum.
 * @param param2 Parameter 2, as defined by MAV_CMD enum.
 * @param param3 Parameter 3, as defined by MAV_CMD enum.
 * @param param4 Parameter 4, as defined by MAV_CMD enum.
 * @param param5 Parameter 5, as defined by MAV_CMD enum.
 * @param param6 Parameter 6, as defined by MAV_CMD enum.
 * @param param7 Parameter 7, as defined by MAV_CMD enum.
 */
//static inline void mavlink_msg_command_long_send(mavlink_channel_t chan, uint8_t target_system, uint8_t target_component, uint16_t command, uint8_t confirmation, float param1, float param2, float param3, float param4, float param5, float param6, float param7)
//{
//#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
//  char buf[33];
//  _mav_put_float(buf, 0, param1);
//  _mav_put_float(buf, 4, param2);
//  _mav_put_float(buf, 8, param3);
// _mav_put_float(buf, 12, param4);
//  _mav_put_float(buf, 16, param5);
//  _mav_put_float(buf, 20, param6);
//  _mav_put_float(buf, 24, param7);
//  _mav_put_uint16_t(buf, 28, command);
//  _mav_put_uint8_t(buf, 30, target_system);
//  _mav_put_uint8_t(buf, 31, target_component);
//  _mav_put_uint8_t(buf, 32, confirmation);

//  _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COMMAND_LONG, buf, 33, 152);
//#else
//  mavlink_command_long_t packet;
//  packet.param1 = param1;
//  packet.param2 = param2;
//  packet.param3 = param3;
//  packet.param4 = param4;
//  packet.param5 = param5;
//  packet.param6 = param6;
//  packet.param7 = param7;
//  packet.command = command;
//  packet.target_system = target_system;
//  packet.target_component = target_component;
//  packet.confirmation = confirmation;

//  _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_COMMAND_LONG, (const char *)&packet, 33, 152);

}


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
  mavlink_pub = n.advertise<mavlink_ros::Mavlink>("/mavlink/to", 1000);
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
    mavlink_pub.publish(rosmavlink_msg);

    ros::spinOnce();

    loop_rate.sleep();
    ++count;
  }


  return 0;
}
