@0xd751255db996b917;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::tap");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using TapCommon = import "/ad_msg_idl/ad_tap/as_common.capnp";

enum TargetObjColor {
    none @0;
    none2 @1;
    none3 @2;
    yellow @3;
    red @4;
}

enum FcwStatus {
    off @0;
    on @1;
    failure @2;
}

enum AebStatus {
    off @0;
    on @1;
    failure @2;
    inhibited @3;
}

enum FcwWarnDist {
    normal @0;
    near @1;
    far @2;
}

enum FcwActiveStatus {
    noRequest @0;
    level1 @1;
    level2 @2;
    level3 @3;
}

enum AebActiveStatus {
    noActive @0;
    preFill @1;
    active @2;
}

enum FcwAebTextInfo {
    defaultText @0;
    fcwIsActive @1;
    fcwIsSwitchedOn @2;
    unableToSwitchOnFcw @3;
    fcwIsSwitchedOff @4;
    fcwIsInErrorMode @5;
    aebIsActive @6;
    aebIsSwitchedOn @7;
    unableToSwitchOnAeb @8;
    aebIsSwitchedOff @9;
    aebIsInErrorMode @10;
}

enum DrivingSoundWarn {
    defaultWarning @0;
    tipWarning @1;
    activeWarning @2;
    failedWarning @3;
    safetyWarning @4;
    warningSound @5;
}

enum CtaWorkingStatus {
    default @0;
    off @1;
    onlyWaring @2;
    waringBrake @3;
}

struct AsDiaplayLgSafe {
    targetLgsf @0: TapCommon.AsTarget;
    targetColor @1: TargetObjColor;
    fcwSts @2: FcwStatus;
    aebSts @3: AebStatus;
    fcwWarnDist @4: FcwWarnDist;
    fcwActiveSts @5: FcwActiveStatus;
    aebActiveSts @6: AebActiveStatus;
    fcwAebTextInfo @7: FcwAebTextInfo;
    drivingSoundWarn @8: DrivingSoundWarn;
}

struct AsDiaplayCta{
    targetRcta @0: TapCommon.AsTarget;
    targetFcta @1: TapCommon.AsTarget;
    rctaSound @2: TapCommon.OnOffStatus;
    rctaLeftWarn @3: TapCommon.OnOffStatus;
    rctaRightWarn @4: TapCommon.OnOffStatus;
    fctaSound @5: TapCommon.OnOffStatus;
    fctaLeftWarn @6: TapCommon.OnOffStatus;
    fctaRightWarn @7: TapCommon.OnOffStatus;
    rctaWorkingStatus @8: CtaWorkingStatus;
    fctaWorkingStatus @9: CtaWorkingStatus;
}

struct AsDiaplayBsd{
    targetBsd @0: TapCommon.AsTarget;
    targetLca @1: TapCommon.AsTarget;
    # 0:Default 1:灯光+声音 2: 关闭 3: 灯光
    switchReq @2: UInt32;
    # 0：Inactive,1：Warning_level_1,2：Warning_Level_2,3：Invalid
    bsdWarnLeft @3: UInt32;
    # 0：Inactive,1：Warning_level_1,2：Warning_Level_2,3：Invalid
    bsdWarnRight @4: UInt32;
    bscSound @5: TapCommon.OnOffStatus;
    # 0：Inactive,1：Warning_level_1,2：Warning_Level_2,3：Invalid
    lcaWarnLeft @6: UInt32;
    # 0：Inactive,1：Warning_level_1,2：Warning_Level_2,3：Invalid
    lcaWarnRight @7: UInt32;
}

struct AsDiaplayRcw{
    targetRcw @0: TapCommon.AsTarget;
    # 0:reserve,1:On,2:Off,3:reserve
    rcwSwitchReq @1: UInt32;
    # 0:default,1:Off,2:On
    rcwWorkingStatus @2: UInt32;
    rcwCollisionWarn @3: TapCommon.OnOffStatus;
}

struct AsDiaplayDow{
    targetDow @0: TapCommon.AsTarget;
    # 0:default,1:On,2:Off,3:reserve
    dowSwitchReq @1: UInt32;
    # 0：Inactive,1：Warning_level_1,2：Warning_Level_2,3：Invalid
    dowLeftWarn @2: UInt32;
    # 0：Inactive,1：Warning_level_1,2：Warning_Level_2,3：Invalid
    dowRightWarn @3: UInt32;
}

struct AsDisplay{
  header @0 : StdHeader.Header;
  # AEB/FCW
  lgsf @1:AsDiaplayLgSafe;
  # FCTA/RCTA
  cta @2:AsDiaplayCta;
  # BSD/LCA
  bsd @3:AsDiaplayBsd;
  # RCW
  rcw @4:AsDiaplayRcw;
  # DOW
  dow @5:AsDiaplayDow;
  resv1 @6:UInt32;
  resv2 @7:UInt32;
  resv3 @8:UInt32;
  resv4 @9:UInt32;
  resv5 @10:UInt32;
  resv6 @11:UInt32;
  resv7 @12:Float32;
  resv8 @13:Float32;
  resv9 @14:Float32;
  resv10 @15:Float32;
  resv11 @16:Float32;
  resv12 @17:Float32;
  resv13 @18:Float32;
  resv14 @19:Float32;
  resv15 @20:Float32;
}
