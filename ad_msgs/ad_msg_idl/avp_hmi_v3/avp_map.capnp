@0xbe595454702ff2b7;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_hmi_v3");

using StdMsgsHeader = import "/ad_msg_idl/std_msgs/header.capnp";

using import "/ad_msg_idl/avp_smart_slam/avp_smart_slam.capnp".MapInfo;

# 按钮状态
struct MapBtnState {
    # false视为禁用
    visible @0: Bool;
    # false视为禁用
    enabled @1: Bool;
}

# 地图类型
enum AvpMapType {
    # 普通
    normal @0;
    # 常用
    favorite @1;
}

# 车位
struct AvpMapSlot {
    # 车位ID
    slotId @0: Int64 = -1;
    # 车位备注
    slotName @1: Text;
}

# 具体地图属性信息
struct AvpMapItem {
    # 地图ID
    mapId @0: Int64 = -1;
    # 地图名字
    mapName @1: Text;
    # 地图名字是否可编辑
    mapNameCanEdit @2: Bool;
    # 地图类型
    mapType @3: AvpMapType;
    # 地图类型是否可编辑
    mapTypeCanEdit @4: Bool;
    # 地图类型不可编辑时的提示
    mapTypeCannotEditHints @5: Text;
    # 地图创建时间戳，单位ns
    mapCreatedTime @6: UInt64;
    # 地图创建时间戳是否可编辑
    mapCreatedTimeCanEdit @7: Bool;
    # 地图标签
    mapLabels @8: List(Text);
    # 地图标签是否可编辑
    mapLabelCanEdit @9: Bool;
    # 收藏车位
    mapFavoriteSlots @10: List(AvpMapSlot);
    # 收藏车位是否可编辑
    mapFavoriteSlotsCanEdit @11: Bool;
    # 循迹终点车位
    mapTargetSlotId @12: UInt64;
    # 循迹终点车位是否可编辑
    mapTargetSlotIdCanEdit @13: Bool;
    # 地图已使用次数
    mapUsedCount @14: UInt64;
    # 地图已使用次数是否可编辑
    mapUsedCountCanEdit @15: Bool;
    # 是否当前停车场
    mapIsCurrent @16: Bool;
}

# 地图管理信息
struct AvpMapList {
    # 地图管理按钮是否可用
    btnState @0: MapBtnState;
    # 当前地图列表（各个地图的属性）
    maps @1: List(AvpMapItem);
    # 地图名字显示字符个数上限
    mapNameLenLimit @2: UInt64;
    # 地图标签显示字符个数上限
    mapLabelLenLimit @3: UInt64;
    # 地图标签个数上限
    mapLabelsCntLimit @4: UInt64;
    # 收藏车位个数上限
    mapFavoriteSlotsCntLimit @5: UInt64;
    # 普通地图个数上限
    normalMapCntLimit @6: UInt64;
    # 常用地图个数上限
    favoriteMapCntLimit @7: UInt64;
}

# 用户操作类型
enum AvpMapCommand {
    # 更新地图属性
    update @0;
    # 删除地图
    delete @1;
    # 预览地图
    preview @2;
}

# 错误码
enum AvpMapErrorCode {
    ok @0;
    invalidOperation @1;
    opsFailed @2;
    unknown @3;
}

# 用户操作请求信息
struct AvpMapReq {
    header @0: StdMsgsHeader.Header;
    # 用户操作类型
    command @1: AvpMapCommand;
    # 用户操作后的地图信息（如果是删除或者预览，只需填充mapId字段即可）
    mapInfo @2: AvpMapItem;
}

# 用户操作请求的应答
struct AvpMapResp {
    header @0: StdMsgsHeader.Header;
    # 操作结果
    errorCode @1: AvpMapErrorCode;
    # 地图数据
    visualizationMapInfo @2: MapInfo;
}
