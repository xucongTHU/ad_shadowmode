@0xcb60464ee0c83210;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_smart_slam");
using Header = import "/ad_msg_idl/std_msgs/header.capnp";
using Geo = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct OdomStateInfo {
    header @0 :Header.Header;
    # Header include seq, timestamp(last node pub time), and frame_id(sensor model)
    measurementTimeNs @1 :UInt64;
    # The time of pose measurement, in nano second.
    statusInfo @2 :Text;
    # Report localization status
    # when INVALID or CONVERGING, localization result is not reliable and should not be used.
    debugInfo @3 :Text;
    # For debug
    positionFlu @4 :Geo.Point;
    # Position of the Vehicle Reference Point(VRP) in the Odometry reference frame(Forward/Left/Up).
    # The VRP is the car center(Forward/Left/Up).
    # Odometry reference frame's origin is the VRP of start point.
    # x for Forward, y for Left, z for Up Height, in meters.
    positionFluStdDev @5 :Geo.Point;
    # Position uncertainty(standard deviation in meters)
    roll @6 :Float64;
    # Attitude in euler angle form to describe the orientation of a VRP frame with respect to a Odometry reference frame(Forward/Left/Up).
    # The roll, in (-pi/2, pi/2), corresponds to a rotation around the Vehicle Forward-axis.
    # The pitch, in [-pi, pi), corresponds to a rotation around the Vehicle Left-axis.
    # The yaw, in [-pi, pi), corresponds to a rotation around the Vehicle Up-axis.
    # The direction of rotation follows the right-hand rule.
    pitch @7 :Float64;

    yaw @8 :Float64;

    attitudeStdDev @9 :Geo.Point;
    # Attitude uncertainty(standard deviation in radians)
    linearVelocity @10 :Geo.Vector3;
    # Linear velocity of the VRP in the Vehicle reference frame
    # x for Forward, y for Left, z for Up, in meters per second
    linearAcceleration @11 :Geo.Vector3;
    # Linear acceleration of the VRP in the Vehicle reference frame
    # x for Forward, y for Left, z for Up, in meters per power second
    angularVelocity @12 :Geo.Point;
    # Angular velocity of the VRP in the Vehicle reference frame
    # x across Forward axes, y across Left axes,
    # z across Up axes, in radians per second

    originId @13 :UInt64;
    # The origin id is to represent the number of trajectories
    # start from 0, plus by 1 if dr restart occurs
}
struct NavStateInfo {
    header @0 :Header.Header;
    # Header include seq, timestamp(last node pub time), and frame_id(sensor model)
    measurementTimeNs @1 :UInt64;
    # The time of pose measurement, in nano second.
    statusInfo @2 :Text;
    # Report localization status
    # when INVALID or CONVERGING, localization result is not reliable and should not be used.
    debugInfo @3 :Text;
    # For debug
    positionEnu @4 :Geo.Point;
    # Position of the Vehicle Reference Point(VRP) in the Map reference frame(East/North/Up).
    # The VRP is the car center(Forward/Left/Up).
    # x for East, y for North, z for Up Height, in meters.
    positionEnuStdDev @5 :Geo.Point;
    # Position uncertainty(standard deviation in meters)
    longitude @6 :Float64;
    # Position of the Vehicle Reference Point(VRP) in the WGS84 reference ellipsoid coordinate system.
    # longitude in degrees, ranging from -180 to 180.
    # latitude in degrees, ranging from -90 to 90.
    # ellipsoid height in meters.
    latitude @7 :Float64;

    altitude @8 :Float64;

    roll @9 :Float64;
    # Attitude in euler angle form to describe the orientation of a VRP frame with respect to a Map reference frame(ENU).
    # The roll, in (-pi/2, pi/2), corresponds to a rotation around the Vehicle Forward-axis.
    # The pitch, in [-pi, pi), corresponds to a rotation around the Vehicle Left-axis.
    # The yaw, in [-pi, pi), corresponds to a rotation around the Vehicle Up-axis.
    # The yaw is the angle of the Vehicle head w.r.t the ENU reference frame, East = 0, North = pi/2, anti-clockwise in radius.
    # The direction of rotation follows the right-hand rule.
    pitch @10 :Float64;

    yaw @11 :Float64;

    attitudeStdDev @12 :Geo.Point;
    # Attitude uncertainty(standard deviation in radians)
    linearVelocity @13 :Geo.Vector3;
    # Linear velocity of the VRP in the Vehicle reference frame
    # x for Forward, y for Left, z for Up, in meters per second
    linearVelocityGlobal @14 :Geo.Vector3;
    # Linear velocity of the VRP in the ENU reference frame
    # x for East, y for North, z for Up, in meters per second
    linearAcceleration @15 :Geo.Vector3;
    # Linear acceleration of the VRP in the Vehicle reference frame
    # x for Forward, y for Left, z for Up, in meters per power second
    linearAccelerationGlobal @16 :Geo.Vector3;
    # Linear acceleration of the VRP in the ENU reference frame
    # x for East, y for North, z for Up, in meters per power second
    angularVelocity @17 :Geo.Point;
    # Angular velocity of the VRP in the Vehicle reference frame
    # x across Forward axes, y across Left axes,
    # z across Up axes, in radians per second

    originLongitude @18 :Float64;
    originLatitude @19 :Float64;
    originAltitude @20 :Float64;
    # Position of the origin in the WGS84 reference ellipsoid coordinate system.
    # longitude in degrees, ranging from -180 to 180.
    # latitude in degrees, ranging from -90 to 90.
    # ellipsoid height in meters.

    regionID @21 :Int16;
}
