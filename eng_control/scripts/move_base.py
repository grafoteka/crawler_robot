#!/usr/bin/env python
import rospy
from gazebo_msgs.srv import GetLinkState
from std_msgs.msg import Float64
from control_msgs.msg import JointControllerState
from geometry_msgs.msg import Twist
from gazebo_msgs.msg import LinkStates
import tf


MAX_SPEED=3
MAX_DELTA_SPEED=MAX_SPEED

# Left flipper is a mirror of right flipper, so angles of flippers are inversed (i,-i)
def callback(data):
    # sudo apt-get install ros-indigo-ros-control ros-indigo-ros-controllers
    # axes=data.axes
    # print(axes)

    avg_speed=MAX_SPEED*data.linear.x
    delta_speed=MAX_DELTA_SPEED*data.angular.z
    right=avg_speed+delta_speed
    left=avg_speed-delta_speed
    #flipper=-0.1
    flipper=0.0
    right=0.0
    left=0.0

    pub = rospy.Publisher('/jaguar_v2/right/command', Float64, queue_size=10)
    pub.publish(right)

    pub = rospy.Publisher('/jaguar_v2/left/command', Float64, queue_size=10)
    pub.publish(left)

    #pub = rospy.Publisher('/jaguar_v2/joint_right_flipper_controller/command', Float64, queue_size=10)
    #pub.publish(flipper)

def callback_transformation(data):
    print(data.name)
    index=data.name.index("jaguar_v2::base_link")
    pose=data.pose[index]

    br = tf.TransformBroadcaster()
    br.sendTransform((pose.position.x, pose.position.y, pose.position.z),
                     #tf.transformations.quaternion_from_euler
                     (pose.orientation.x, pose.orientation.y, pose.orientation.z, pose.orientation.w),
                     rospy.Time.now(),
                     "base_link",
                     "odom")

def listener():
    rospy.init_node('listener', anonymous=True)
    rospy.Subscriber("/cmd_vel", Twist, callback)
    rospy.Subscriber('/gazebo/link_states', LinkStates, callback_transformation)
    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()


if __name__ == '__main__':
    listener()
