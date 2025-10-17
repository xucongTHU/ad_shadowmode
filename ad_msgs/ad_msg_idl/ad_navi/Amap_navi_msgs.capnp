@0xb1a6c775b03a2bb2;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::amap_navigation");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using Geo = import "/ad_msg_idl/std_msgs/geometry.capnp";

struct GlobalRouteList {
  header @0 :StdHeader.Header;
  steps @1 :List(NavStep);                  # 导航段(Segment)的集合
  routeLength @2 :UInt32;                   # 导航路线的距离
  id @3 : UInt64;                      # 导航段id，这里定义有点奇怪，后续根据数据确认是否为路线id
}

# 这里需要注意，导航侧定义的坐标点是int32，startPoint/endPoint收到的坐标小数点需要左移5位
struct NavStep {
  stepId @0 :UInt64;                            # 导航段ID
  length @1 :UInt64;                        # 导航段长度[m]
  startPoint @2 :Geo.Point;                 # 导航段起点坐标
  endPoint @3 :Geo.Point;                   # 导航段终点坐标
  mainAction @4 :UInt8;                # 基本导航动作，如左转、右转、直行等 enum MainAction
  assistantAction @5 :UInt8;      # 辅助导航动作，进入辅路，进入隧道等 enum AssistantAction
  links @6 :List(Link);                     # 导航段的link串
}

# 这里需要注意，导航侧定义的坐标点是int32，geometry收到的坐标小数点需要左移5位
struct Link {
  linkId @0 :UInt64;                        # 道路Id
  length @1 :UInt32;                        # 道路长度
  roadClass @2 :UInt8;                  # 道路等级 enum RoadClass
  formway @3 :UInt8;                      # 道路类型 enum Formway
  linkType @4 :UInt8;                 # Link类型 enum LinkType
  linkName @5 :Text;                        # link所在道路名称
  speedLimit @6 :UInt8;                     # 道路限速
  laneNum @7 :UInt8;                        # 车道数量,行驶方向的车道数量
  avgSpeed @8 :UInt8;                       # 平均车速
  isInnerRd @9 :Bool;                       # 是否为内部道路
  isRestricted @10 :Bool;                   # 是否为限行道路
  isParkingRd @11 :Bool;                    # 是否为泊车道路
  isToll @12 :Bool;                         # 是否包含收费站
  geometry @13 :List(Geo.Point);            # link行点信息
  hasMultiOut @14 :Bool;                    # 是否具有岔路/多路口
  hasTrafficLight @15 :Bool;                #是否有红绿灯
  #### 以下信息Amap SDK应当可以透出，但是目前Lite项目中未定义，建议之后增加
  ## drivingDirection @14 :UInt8;              # 0 - 单向通行，1 - 双向通行
  ## mainAction @15 :MainAction;               # 基本导航动作
  ## assistantAction @16 :AssistantAction;     # 辅助导航动作
  ## hasMultiOut @17 :Bool;                    # 是否有岔路，0否，1是
  ## hasMixFork @18 :Bool;                     # 是否有混淆路口, 0否，1是
  ## hasTrafficLight @19 :Bool;                # 是否有红绿灯，0否，1是
  ## hasParallel @20 :Bool;                    # 是否存在平行路
  ####
}

# 导航指令 - 即实时导航信息字段
struct AmapNaviInfo {
  header @0 :StdHeader.Header;
  isOutOfRoute @1 :Bool;                                        # 是否偏航
  naviType @2 :UInt8;                                        # 在线导航模式 enum NaviType
  iconType @3 :UInt8;                                        # 导航推荐动作 enum IconType
  curRetainDistance @4 :Int32;                                  # 导航推荐动作距离[m]
  allLength @5 :Int32;                                          # 导航全程长度[m]
  pathRetainDistance @6 :Int32;                                 # 路线剩余距离[m]
  laneInfo @7 :LaneInfo;                                        # 车道前背景信息，巡航需要全部背景车道
  curSpeedLimit @8 :Int32;                                      # 当前限速[km/h]，巡航状态也需要
  curLink @9 :Int32;                                            # 当前小路段Link id，巡航状态也需要
  curStep @10 :Int32;                                            # 当前导航段Step id，巡航状态也需要
  distanceToNextLink @11 :Int32;                                # 离下个link的距离[m]，巡航状态也需要
  curRoadNameNavi @12 :Text;                                    # 当前所在道路名称
  curRoadLevelForNavi @13 :UInt8;                           # 当前所在道路等级 enum RoadClass
  cameraInfo @14 :List(CameraInfo);                             # 摄像头限速信息，巡航状态也需要
  trafficFacilityInfo @15 :List(TrafficFacilityInfo);           # 道路设施信息，巡航状态也需要
  trafficLightInfo @16 :List(TrafficLightInfo);                 # 前方红绿灯信息，巡航状态也需要
  entranceExit @17 :UInt8;                               # 距离前方/入口匝道的距离 enum EntranceExit
  naviText @18 :Text;                                           # 导航TTS语音播报
  trafficInfo @19 :TrafficStatus;                             # 前方交通拥堵状态
  elevatedRoadStatus @20 :UInt8;                    # 在主路上（高架上）态，高架上下标识 enum ElevatedRoadType
  parallelRoadStatus @21 :UInt8;                    # 在主辅路（无高架）态，主辅路标识 enum ParallelRoadType
  tollGateInfo @22 :List(TollGateInfo);                         # 前方收费站信息
  electronicSpeedLimitInfo @23 :List(ElectronicSpeedLimitInfo); # 电子限速
  weatherInfo @24 :List(WeatherInfo);                           # 实时天气信息
  trafficEventInfo @25 :List(TrafficEventInfo);                 # 交通事件信息
}

struct ElectronicSpeedLimitInfo {
  electronicSpeedLimitType @0 :UInt8;        # 电子限速类型 enum ElectronicSpeedLimitType
  speedLimitValue @1 :Int32;                                    # 电子限速值
  electronicSpeedLimitDist @2 :Int32;                           # 自车到电子限速距离[m]
}

struct WeatherInfo {
  weatherName @0 :Text;                                         # 天气名称                                 
  alertLevelName @1 :Text;                                      # 预警名称
  weatherId @2 :Int32;                                          # 天气ID
  alertLevelId @3 :Int32;                                       # 警告等级
  distanceToCar @4 :Int32;                                      # 自车到天气路段距离
}

struct TrafficEventInfo {
  trafficEventType @0 :UInt8;                        # 交通事件类型 enum TrafficEventType
  occupiedLaneType @1 :UInt8;                        # 占用车道类型，占哪边车道 enum OccupiedLaneType
  trafficEventDistance @2 :Int32;                               # 到达事件的距离[m]，处于事件范围中时持续填0
}

# 张仲凯20250410：这里需要确认，laneTypes应该是个list，包含收费站多条车道信息，目前看博世定义只有一个值
struct TollGateInfo {
  laneTypes @0 : List(UInt8);                            # 收费站车道类型 enum TollLaneType
  distance @1 :Int32;                                           # 到收费站的距离
}

struct CameraInfo {
  cameraDistance @0 :UInt32;                                    # 当前位置到摄像头的距离[m]
  cameraSpeed @1 :UInt32;                                       # 摄像头的限速值[km/h]
  cameraType @2 :UInt8;                                    # 摄像头的类型 enum CameraType
}

struct TrafficLightInfo {
  trafficLightInfo @0 :List(TrafficLight);
  length @1 :Int32;                                             # 红绿灯位置，从link起点偏移量
  linkIndex @2 :Int32;                                          # 导航端index
  linkId @3 :Int32;                                             # link id
}

struct TrafficLight{
  startTime @0 :Int64;                                          # 状态开始时间
  endTime @1 :Int64;                                            # 状态结束时间
  trafficLightState @2 :UInt8;                      # 红绿灯灯态 enum TrafficLightState
}

struct LaneInfo {
  foregroundLane @0 :List(UInt8);                            # 前景车道 enum LaneType
  backgroundLane @1 :List(UInt8);                            # 背景车道 enum LaneType
}

struct TrafficFacilityInfo {
  trafficFacilityDistance @0 :UInt32;                           # 自车到道路设施的距离[m]
  trafficFacilitySpeed @1 :UInt32;                              # 限速值 [km/h]
  trafficFacilityType @2 :UInt8;                  # 道路设施类型 enum TrafficFacilityType
}

struct TrafficStatus {
  length @0 :Float64;                                           # 交通状态路段的拥堵状态长度
  startLinkIdx @1 :Int32;                                       # 路况起始link id
  endLinkIndex @2 :Int32;                                       # 路况结束link id
  startSegmentIdx @3 :Int32;                                    # 同路况状态的起始Seg索引
  endSegmentIdx @4 :Int32;                                      # 同路况状态的结束Seg索引
  trafficState @5 :UInt8;                               # 交通拥堵状态 enum TrafficState
}
