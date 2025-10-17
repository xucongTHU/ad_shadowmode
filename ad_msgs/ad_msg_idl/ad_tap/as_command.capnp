@0xd0c144a7696152f7;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::tap");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";
using TapCommon = import "/ad_msg_idl/ad_tap/as_common.capnp";

enum AebStatusCmd {
    off @0;
    on @1;
    failure @2;
    notActive @3;
    active @4;
    preFill @5;
}
enum FcwStatusCmd {
    noRequest @0;
    level1 @1;
    level2 @2;
    level3 @3;
}
enum FcwMode {
    off @0;
    on @1;
    active @2;
    failure @3;
}

struct AsCmdLgSafe {
  lgsfTarget @0: TapCommon.AsTarget;
  aebTarDec @1: Float32;
  holdType @2: UInt32;
  aebReq @3: TapCommon.OnOffStatus;
  awbReq @4: TapCommon.OnOffStatus;
  preFillReq @5: TapCommon.OnOffStatus;
  abaReq @6: TapCommon.OnOffStatus;
  beltReq @7: TapCommon.OnOffStatus;
  aebStatus @8: AebStatusCmd;
  fcwStatus @9: FcwStatusCmd;
  fcwMode @10: FcwMode;
  abaLevel @11: UInt8;
  awbLevel @12: UInt8;
  aebReqValid @13: UInt8;
  # Bit0: 是否为影子模式
  # Bit1: AEB
  # Bit2: EBA
  # Bit3: FCW
  # Bit4: DW
  # Bit5: AWB
  shadowMode @14: UInt8;
  resv1 @15: UInt32;
  resv2 @16: Float32;
  resv3 @17: Float32;

}
enum CtbCollisionDir {
    noCollsion @0;
    left @1;
    right @2;
    leftRight @3;
}

struct AsCmdCtb {
  rctbReq @0: TapCommon.OnOffStatus;
  rctbValid @1: TapCommon.OnOffStatus;
  fctbReq @2: TapCommon.OnOffStatus;
  fctbValid @3: TapCommon.OnOffStatus;
  rctbDecReq @4: Float32;
  fctbDecReq @5: Float32;
  rctbCollisionDir @6: CtbCollisionDir;
  fctbCollisionDir @7: CtbCollisionDir;
  shadowMode @8: UInt32;
  resv1 @9: UInt32;
  resv2 @10: Float32;
  resv3 @11: Float32;
}

struct AsCommand {
  header @0 : StdHeader.Header;
  lgsf @1 : AsCmdLgSafe;
  ctb @2 : AsCmdCtb;
  shadowMode @3: UInt32;
  resv1 @4: UInt32;
  resv2 @5: UInt32;
  resv3 @6: UInt32;
  resv4 @7: UInt32;
  resv5 @8: UInt32;
  resv6 @9: UInt32;
  resv7 @10: Float32;
  resv8 @11: Float32;
  resv9 @12: Float32;
  resv10 @13: Float32;
  resv11 @14: Float32;
  resv12 @15: Float32;
  resv13 @16: Float32;
  resv14 @17: Float32;
  resv15 @18: Float32;
}
