@0xcfc935b1ea297849;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::mem_driving");

using StdGeometry = import "/ad_msg_idl/std_msgs/geometry.capnp";
using LocalMap = import "/ad_msg_idl/L2Plus_localmap_msgs/local_msgs.capnp";

struct Route {
    # 唯一id, 实际是path
    id @0 :Text;
    name @1 :Text;
    alias @2 :Text;   #input on publish
    path @3 :Text;
    description @4 :Text;
    # trajectory, 文件名, 独立请求
    trajectory @5 :Text;
    trajectoryLen @6 :Float32;
    # 路口id列表
    intersections @7 :List(Text);
    # 建图中不关注
    isPublished @8 :Bool;

    # 时间戳, 单秒s
    createdAt @9 :Text;
    updatedAt @10 :Text;
    isDeleted @11 :Bool;
    mileage @12 :Float32;
    duration @13 :UInt32;
}

struct ResponseStatus {
#    enum Code {
#        ok @0;
#        failed @1;
#    }

    code @0 :UInt8;
    # if failed, show error
    error @1 :UInt8;
}

# learning control
struct LearningRequest {
#    enum Command {
#        start @0;
#        stop @1;
#        getState @2;
#    }

    command @0 :UInt8;
    routeId @1 :Text;
}

struct LearningResponse {
#    enum State {
#       idle @0;
#       busy @1;
#    }

    status @0 :ResponseStatus;
    # 学习中不返回route.traj/route.intersections;
    # 结束学习需要返回完整route
    route @1 :Route;
    state @2 :UInt8;
    # 学习中需要返回持续时间和里程
    # duration unit: second
    duration @3 :UInt32;
    # mileage unit: km
    mileage @4 :Float32;
}

struct RouteManageRequest {
#    enum Command {
#        getRoute @0;
#        getTrajectory @1;
#        getIntersection @2;
#        publishRoute @3;
#        deleteRoute @4;
#        getSnapshot @5;
#   }

    command @0 :UInt8;
    routeId @1 :Text;
    trajectoryId @2 :Text;
    intersectionId @3 :Text;

    # routeName & routeDesc: publish route only
    routeName @4 :Text;
    routeDescription @5 :Text;
}

struct RouteManageResponse {
    status @0 :ResponseStatus;
    routes @1 :List(Route);
    # traj: json form
    trajectory @2 :Text;
    # intersection: json form
    intersection @3 :Text;
    intersectionSnapshot @4 :Text;
}

# mem driving control
struct DrivingControlRequest {
#    enum Command {
#        start @0;
#        stop @1;
#        getState @2;
#    }

    command @0 :UInt8;
    routeId @1 :Text;
}

struct DrivingControlResponse {
#    enum DrivingState {
#        idle @0;
#        running @1;
#        standBy @2;
#        stopped @3;
#    }

    status @0 :ResponseStatus;
    drivingState @1 :UInt8;
    routeId @2 :Text;
    # routeLen unit: km
    routeLen @3 :Float32;
    restRouteLen @4 :Float32;
    # duration unit: second
    duration @5 :UInt32;
    takeoverHint @6 :Bool;
    #mnop memmap match state
    mapMatch @7 :Int32;
    #mnop memmap nodeinfo
    nodeId @8 :UInt32;
}

struct DrivingModeRequest {
    # hnopEnable与mnopEnable互斥，不能同时为True
    # 可支持状态为False/False, False/True, True/False
    hnopEnable @0 :Bool;
    mnopEnable @1 :Bool;
}

struct DrivingModeResponse {
    hnopStatus @0 :ResponseStatus;
    mnopStatus @1 :ResponseStatus;
}