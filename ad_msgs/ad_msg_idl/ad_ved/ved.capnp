@0xfa051b4b49ffe783;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::ved");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct VEDInfo {
    # Header include seq, timestamp(last node pub time), and frame_id(sensor
    # model)
    header @0 :StdHeader.Header;

    # The time of pose measurement, in nano second.
    measurementTimeNs @1 :UInt64;

    # Report ved status
    # when INVALID or CONVERGING, ved result is not reliable and should not be
    # used.
    statusInfo @2 :Text;

    # Linear velocity of the Vehicle Reference Point(VRP) in the Vehicle
    # reference frame
    # x for Forward, y for Left, z for Up, in meters per second
    linearVelocity @3 :StdGeometry.Vector3;

    # Linear acceleration of the Vehicle Reference Point(VRP) in the Vehicle
    # reference frame
    # x for Forward, y for Left, z for Up, in meters per second^2
    linearAcceleration @4 :StdGeometry.Vector3;

    # Angular velocity of the VRP in the Vehicle reference frame
    # x for Forward, y for Left, z for Up, in rad per second
    angularVelocity @5 :StdGeometry.Vector3;

    # Quaternion of the Vehicle Frame(FLU) relative to the Road Frame(FLU)
    q @6 :StdGeometry.Quaternion;

    # Translation of the Vehicle Frame(FLU) relative to the Road Frame(FLU)
    t @7 :StdGeometry.Vector3;
}
