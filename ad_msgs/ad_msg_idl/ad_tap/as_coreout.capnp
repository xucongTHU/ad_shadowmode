@0xa440f867f96c09ee;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::tap");

using StdHeader = import "/ad_msg_idl/std_msgs/header.capnp";

# topic is: "/tap/as_core_out"

struct AsLongSafeObject{
  index @0: UInt8;
  status @1: UInt8;
  age @2: UInt16;
  visTrkid @3: Int32;
  fusTrkid @4: Int32;
  objectClass @5: Int32;
  detectionSensor @6: Int32;
  longPosn @7: Float32;
  latPosn @8: Float32;
  heading @9: Float32;
  speed @10: Float32;
  latVel @11: Float32;
  longVel @12: Float32;
  latAccel @13: Float32;
  longAccel @14: Float32;
  ttc @15: Float32;
  xolc @16: Float32;
  latEst @17: Float32;
  accelLatReq @18: Float32;
  accelLgtReq @19: Float32;
  length @20: Float32;
  width @21: Float32;
  predLongAccel @22: Float32;
  predLatAccel @23: Float32;
  predAccel @24: Float32;
  predCvt @25: Float32;
  offsLgtPred @26: Float32;
  offsLatPred @27: Float32;
  offsLatManoeuvre @28: Float32;
  offsLatInPathPrimaryTarget @29: Float32;
  offsLatInPathCloseEdge @30: Float32;
  offsLatInPathFarEdge @31: Float32;
  offsLatIntersection @32: Float32;
  offsLatMultiTarget @33: Float32;
  absHeading @34: Float32;
  lengthSideLgt @35: Float32;
  lengthSideLat @36: Float32;
  sinRotation @37: Float32;
  cosRotation @38: Float32;
  inpathCurrent @39: UInt8;
  inpathPredict @40: UInt8;
  matchConf @41: UInt8;
  motionType @42: UInt8;
  stationary @43: UInt8;
  movingOncoming @44: UInt8;
  paraVehPred @45: UInt8;
  obsIsVehicle @46: UInt8;
  obsIsMotorVeh @47: UInt8;
  curvedMotion @48: UInt8;
  sideClosest @49: UInt8;
}
struct AsScenarioInfo {
    objEvaluator @0 :UInt32;
    resv1 @1 :UInt32;
    resv2 @2 :UInt32;
    resv3 @3 :UInt32;
}

struct AsTimeInfo {
    curTime @0 :UInt64;
    fusTime @1 :UInt64;
    roadgeometryTime @2 :UInt64;
    vehicleTime @3 :UInt64;
}

struct AsLongSafeOut {
    longsafeAeb @0 :AsLongSafeObject;
    longsafeFcw @1 :AsLongSafeObject;
    longsafeScenario @2 : AsScenarioInfo;
    longsafeRollCnt @3 :UInt8;
    longsafeMajorVersion @4 :UInt8;
    longsafeMinorVersion @5 :UInt8;
}

struct AsOnComingObject{
  vcsLongPosn @0: Float32;
  vcsLongVel @1: Float32;
  vcsLongAccel @2: Float32;
  vcsLatPosn @3: Float32;
  vcsLatVel @4: Float32;
  vcsLatAccel @5: Float32;
  width @6: Float32;
  length @7: Float32;
  vcsRmDis @8: Float32;
  curvHeading @9: Float32;
  objectClass @10: UInt8;
  fusionSource @11: UInt8;
  trackStatus @12: UInt8;
  movement @13: UInt8;
  cmbbPrimaryConfidence @14: UInt8;
  fcwConfidence @15: UInt8;
  maturity @16: UInt8;
  visTrkID @17: UInt8;
  fOncoming @18: UInt8;
  trackId @19: UInt8;
  oncomConfidence @20: UInt8;
  collisionProb @21: UInt8;
}

struct AsOnComingOut {
  oncomTarget1 @0: AsOnComingObject;
  oncomTarget2 @1: AsOnComingObject;
  oncomTarget3 @2: AsOnComingObject;
  asOncomingRollcnt @3: UInt8;
  oncomingMajorVersion @4: UInt8;
  oncomingMinorVersion @5: UInt8;
}

struct AsOverTakingObject{
  vcsLongPosn @0: Float32;
  vcsLongVel @1: Float32;
  vcsLongAccel @2: Float32;
  vcsLatPosn @3: Float32;
  vcsLatVel @4: Float32;
  vcsLatAccel @5: Float32;
  width @6: Float32;
  length @7: Float32;
  vcsHeading @8: Float32;
  confidence @9: Float32;
  objectClass @10: UInt8;
  fusionSource @11: UInt8;
  trackStatus @12: UInt8;
  visTrkID @13: UInt8;
  trackId @14: UInt8;
}

struct AsOverTakingOut {
  overtakeTargetRearLeft @0: AsOverTakingObject;
  overtakeTargetRearRight @1: AsOverTakingObject;
  overtakeTargetFrontLeft @2: AsOverTakingObject;
  overtakeTargetFrontRight @3: AsOverTakingObject;
  asOvertakingRollcnt @4: UInt8;
  overtakingMajorVersion @5: UInt8;
  overtakingMinorVersion @6: UInt8;
}

struct AsSfObjInfo {
  crashPointXl @0: Float32;
  crashProb @1: Float32;
  objTTC @2: Float32;
  objID @3: Int32;
  objHeading @4: Float32;
  objLongPos @5: Float32;
  objLatPos @6: Float32;
  objLongVel @7: Float32;
  objLatVel @8: Float32;
  objLongAccel @9: Float32;
  objLatAceel @10: Float32;
  objAlertCnt @11: Float32;
  objAlertCntThresh @12: Float32;
  objInZone @13: Float32;
  objAlertLevel @14: Float32;
}

struct AsDowOut{
  leftAlert @0 :UInt32;
  rightAlert @1 :UInt32;
  leftObj @2 :AsSfObjInfo;
  rightObj @3 :AsSfObjInfo;
}

struct AsLcdaOut{
  bsdLeftAlert @0 :UInt32;
  bsdRightAlert @1 :UInt32;
  cvwLeftAlert @2 :UInt32;
  cvwRightAlert @3 :UInt32;
  bsdLeftObj @4 :AsSfObjInfo;
  bsdRightObj @5 :AsSfObjInfo;
  cvwLeftObj @6 :AsSfObjInfo;
  cvwRightObj @7 :AsSfObjInfo;
}

struct AsCtsOut{
  fctaLeftAlert @0 :UInt32;
  fctaRightAlert @1 :UInt32;
  rctaLeftAlert @2 :UInt32;
  rctaRightAlert @3 :UInt32;
  fctaLeftObj @4 :AsSfObjInfo;
  fctaRightObj @5 :AsSfObjInfo;
  rctaLeftObj @6 :AsSfObjInfo;
  rctaRightObj @7 :AsSfObjInfo;
}

struct AsRcwOut{
  rcwAlert @0 :UInt32;
  obj @1 :AsSfObjInfo;
}

struct AsSideFeatureOut {
  sfRollingCnt @0: UInt8;
  dow @1: AsDowOut;
  lcda @2: AsLcdaOut;
  cts @3: AsCtsOut;
  rcw @4: AsRcwOut;
}

struct AsCoreOut {
    longsafe    @0 :AsLongSafeOut;
    sideFeature @1: AsSideFeatureOut;
    onComing @2: AsOnComingOut;
    overTaking @3: AsOverTakingOut;
    time @4: AsTimeInfo;
    activesafetyRollingcnt  @5 :UInt8;
    majorVersion  @6 :UInt8;
    middleVersion  @7 :UInt8;
    minorVersion  @8 :UInt8;
}

struct  AsCoreOutMsg {
    header @0 : StdHeader.Header;
    longsafeAeb @1 :AsLongSafeObject;
    longsafeFcw @2 :AsLongSafeObject;
    onComing @3: AsOnComingOut;
    overTaking @4: AsOverTakingOut;
    activesafetyRollingcnt  @5 :UInt8;
    majorVersion  @6 :UInt8;
    middleVersion  @7 :UInt8;
    minorVersion  @8 :UInt8;
}
