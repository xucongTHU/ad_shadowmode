@0xe6af7ba7cdd9da68;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_fusion_map");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

struct Point {
    x @0 :Int16;  #unit:mm
    y @1 :Int16;  #unit:mm
}

struct Line {
    points @0 :List(Point);    # 线段有两个端点
    heightLevel @1 :UInt8;     # 线段的高低属性，跟所属Contour的type的通过性相关
    # 0: Low 低
    # 1: High 高
    # 2: Unknown 未知
}

struct Contour {
    timestampNs @0 :UInt64;

    # 0: UNKNOWN 未知
    # 1: FREE 自由
    # 2: PEDESTRIAN 行人
    # 3: VEHICLE 车辆
    # 4: BIKE 自行车
    # 5: TRAFFICBARREL 交通锥桶 
    # 6: NOPARKINGSIGN 禁停标志
    # 7: CART 推车
    # 8: COLUMN 柱子
    # 9: WALL 墙
    # 10: CURB 路沿
    # 11: WHEELSTOP 车轮定位器
    # 12: CONE 锥桶
    # 13: OTHER 其他
    type @1 :UInt8;           # 障碍物类型，用关联目标的类型

    lifeTime @2 :UInt64;
    lines @3 :List(Line);
}

struct ContourFrame {
    header @0 :StdHeader.Header;
    contours @1 :List(Contour);  #轮廓列表
}