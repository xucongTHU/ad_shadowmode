@0x9f23f496404fa705;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::tap");



struct LgSfCibdebug{
  lgsfObjtoi @0: UInt8;
  lgsfObjinpathage @1: UInt8;
  lgsfObjinpathagethres @2: UInt8;
  lgSfDriveEngage @3: UInt8;
  lgsfObju16Resv1 @4: UInt16;
  lgsfObjtoiflag @5: UInt16;
  lgsfObjleadvel @6: Float32;
  lgsfObjttc @7: Float32;
  lgsfObjdesiredhitspd @8: Float32;
  lgsfBaserange @9: Float32;
  lgsfDistnear @10: Float32;
  lgsfObjXolc @11: Float32;
  lgsfHitdist @12: Float32;
  lgsfYawdist @13: Float32;
  lgsfHboffset @14: Float32;
  lgsfLboffset @15: Float32;
  lgsfPfoffset @16: Float32;
  lgsfIbaoffset @17: Float32;
  lgsfAbaoffset @18: Float32;
  lgsfOverallrdcfac @19: Float32;
  lgsfEbaoverallrdcfac @20: Float32;
  lgsfNeartgtrdcfac @21: Float32;
  lgsfVehbehvrdcfac @22: Float32;
  lgsfXolcrdcfac @23: Float32;
  lgsfHbrangethres @24: Float32;
  lgsfLbrangethres @25: Float32;
  lgsfPfrangethres @26: Float32;
  lgsfIbarangethres @27: Float32;
  lgsfAbarangethres @28: Float32;
  lgsfReqdecelcloseloop @29: Float32;
  lgsfReqdecel @30: Float32;
  lgsfDecelrelease @31: UInt8;
  lgsfAbareq @32: UInt8;
  lgsfHoldreq @33: UInt8;
  lgsfBrklevel @34: UInt8;
  lgsfEbareqflag @35: UInt16;
  lgsfHbreqflag @36: UInt16;
  lgsfLbreqflag @37: UInt16;
  lgsfPfreqflag @38: UInt16;
  lgsfAbareqflag @39: UInt16;
  lgsfU16Resv1 @40: UInt16;
}


struct LgSfFcwdebug{
  lgsfFcwTTCThres @0: Float32;
  lgsfObjTTC @1: Float32;
  lgsfChimesuppress @2: UInt8;
  lgsfLatwarningreq @3: UInt8;
  lgsfPrewarningreq @4: UInt8;
  lgsfAwbreq @5: UInt8;
  lgsfAwblevel @6: UInt8;
  lgsfU8Resv1 @7: UInt8;
  lgsfU8Resv2 @8: UInt8;
  lgsfU8Resv3 @9: UInt8;
  lgsfChimesuppressflag @10: UInt16;
  lgsfLatwarningflag @11: UInt16;
  lgsfPrewarningflag @12: UInt16;
  lgsfAwbflag @13: UInt16;
}

enum AebState {
  aebOff @0;
  aebFault @1;     
  aebPassive @2;   
  aebStandby @3;
  prefillActive @4;
  aebActive @5;
  ebaActive @6;
}

enum FcwState {
  fcwOff @0;     
  fcwFault @1;   
  fcwPassive @2;
  fcwStandby @3;
  fcwFirstLevel @4;
  fcwSecondLevel @5;
  fcwThirdLevel @6;
}

enum AwbState {
  awbOff @0;
  awbFault @1;
  awbPassive @2;
  awbStandby @3;
  awbActive @4;
}

struct LgSfLongScene{
  lgsfTarreqdecel @0: Float32;
  lgsfAebreq @1: UInt8;
  lgsfAebtardecvld @2: UInt8;
  lgsfAwbreqlvl @3: UInt8;
  lgsfAbareq @4: UInt8;
  lgsfAbalvl @5: UInt8;
  lgsfHwreq @6: UInt8;
  lgsfHalightreq @7: UInt8;
  lgsfBeltreq @8: UInt8;
  lgsfAebstate @9: AebState;
  lgsfFcwstate @10: FcwState;
  lgsfAwbstate @11: AwbState;
  lgsgShadowmode @12: UInt16;
  lgsfObjid @13: Int32;
  lgsfObjvisid @14: Int32;
  lgsfObjclass @15: Int32;
  lgsfObjposx @16: Float32;
  lgsfObjposy @17: Float32;
  lgsfObjvelx @18: Float32;
  lgsfObjvely @19: Float32;
  lgsfObjaccx @20: Float32;
  lgsfObjaccy @21: Float32;
  lgsfObjttc @22: Float32;
  cib @23: LgSfCibdebug;
  fcw @24: LgSfFcwdebug;
}
struct Machinests{
  aebstate @0: UInt8;
  fcwstate @1: UInt8;
  awbstate @2: UInt8;
  awbLevel @3: UInt8;
  hbastate @4: UInt8;
  hbaLevel @5: UInt8;
  prefillstate @6: UInt8;
  dwstate @7: UInt8;
}

struct Mtnrqrdintv{
  alatrqrd @0: Float32;
  alatrqrdqly @1: Int32;
  aposlgtrqrd @2: Float32;
  aneglgtrqrd @3: Float32;
  jerkneglgtrqrd @4: Float32;
  crvtraterqrdle @5: Float32;
  crvtraterqrdri @6: Float32;
  crvtrqrdle @7: Float32;
  crvtrqrdri @8: Float32;
}

struct Mtnrqrdwarn{
  alatrqrd @0: Float32;
  alatrqrdqly @1: Int32;
  aneglgtrqrdforwarn @2: Float32;
  aneglgtrqrdforintv @3: Float32;
}

struct MtnrqrdDiag{
  mtnrqrdintv @0: Mtnrqrdintv;
  mtnrqrdwarn @1: Mtnrqrdwarn;
  aneglgtrqrdforprimtarwarn @2: Float32;
}

struct DiagAccelrequest{
  prebrakeaccreqlimit @0: Float32;
  fullbrakeaccelerationrequest @1: Float32;
}

struct DiagDeactivate{
  deactivate @0: UInt8;
  vehiclestationarylongerthanxseconds @1: UInt8;
  collisionavoidedbybraking @2: UInt8;
  objconfnotok @3: UInt8;
  objidchanged @4: UInt8;
  delaydeactivation @5: UInt8;
  objstatedeactivation @6: UInt8;
  reserved @7: UInt8;
}

struct DiagOncominginhibit{
  targethighcurvature @0: UInt8;
  targetanglenotvalid @1: UInt8;
  reserved1 @2: UInt8;
  reserved2 @3: UInt8;
  hostcrvt @4: Float32;
  recentscurve @5: UInt8;
  notturningnow @6: UInt8;
  norecentanglespeed @7: UInt8;
  norecentstart @8: UInt8;
  moreheadon @9: UInt8;
  isvehicleofunknownclass @10: UInt8;
  reserved3 @11: UInt8;
  reserved4 @12: UInt8;
}

struct DiagCommoninhibit{
  spdinhibit @0: UInt8;
  reserved1 @1: UInt8;
  hostavoidinhibit @2: UInt8;
  hostdrivingstraightandtargetisoncoming @3: UInt8;
  oncominginhibit @4: DiagOncominginhibit;
}

struct DiagFullbraking{
  fullbrakingthreat @0: UInt8;
  steeringthreat @1: UInt8;
  brakingthreat @2: UInt8;
  accelerationthreat @3: UInt8;
  objectconfok @4: UInt8;
  reserved1 @5: UInt8;
  reserved2 @6: UInt8;
  reserved3 @7: UInt8;
}

struct DiagPrebraking{
  prebrakingthreat @0: UInt8;
  steeringthreat @1: UInt8;
  brakingthreat @2: UInt8;
  accelerationthreat @3: UInt8;
  objconfok @4: UInt8;
  reserved1 @5: UInt8;
  reserved2 @6: UInt8;
  reserved3 @7: UInt8;
}

struct DiagCritlgteval{
  deactivate @0: DiagDeactivate;
  commoninhibit @1: DiagCommoninhibit;
  fullbraking @2: DiagFullbraking;
  prebraking @3: DiagPrebraking;
}

struct CmbbDiag{
  accelrequest @0: DiagAccelrequest;
  critlgteval @1: DiagCritlgteval;
  cmbbstatus @2: UInt8;
  reserved1 @3: UInt8;
  reserved2 @4: UInt8;
  reserved3 @5: UInt8;
}
struct DiagStartConditions{
  btnAboveLimit @0:UInt8;
  brakeThreatNumber @1:Float32;
  objectIsValid @2:UInt8;
  driverCanNotEasilySteerAway @3:UInt8;
  btnTrigger @4:UInt8;
  cmbbTrigger @5:UInt8;
  visionOnlyTrigger @6:UInt8;
  startConditionsMet @7:UInt8;
  warningType @8:UInt8;
  brakeGainConditionsMet @9:UInt8;
  comfortDistanceTrigger @10:UInt8;
  brakeGainConditionsBit @11:UInt8;
  warningState @12:UInt8;
  resv1 @13:UInt8;
}

struct DiagInhibitCdnSts{
  inhibitFcw @0:UInt8;
  belowLowSpeedLimit @1:UInt8;
  vehicleNotRollingForward @2:UInt8;
  fcwDisabledByFaultManager @3:UInt8;
  sceniaroCheckNotOk @4:UInt8;
  resv1 @5:UInt8;
  resv2 @6:UInt8;
  resv3 @7:UInt8;
}

struct CllsnFwdWarnCtrlDebug{
  decelReq  @0: Float32;
  decelEna  @1: UInt8;
  brkGainReq  @2: Float32;
  brkGainMaxDecel  @3: Float32;
  brkGainEna  @4:UInt8;
  cllsnThreat  @5:UInt8;
  bltLvl  @6:UInt8;
  dwActive  @7:UInt8;
  warnReq  @8:UInt8;
  warnObjType  @9:UInt8;
}

struct FcwDiag{
  startConditions @0: DiagStartConditions;
  inhibitCdnSts @1: DiagInhibitCdnSts;
  acttnDataFromCllsnFwdWarnCtrl @2: CllsnFwdWarnCtrlDebug;
  resv1 @3: UInt8;
  resv2 @4: UInt8;
}

struct BrkarbDiag{
  cllsnthreat @0: UInt8;
  brkarbresult @1: UInt8;
  asysftydecelreqdendbyspdredn @2: UInt8;
  reserved @3: UInt8;
  decelrequestbrkarb @4: Float32;
}

struct SceniaroDiag{
  egoMotionType @0: UInt8;
  sceniaroClass @1: UInt8;
  aebSceniaroResult @2: UInt8;
  fcwSceniaroResult @3: UInt8;
  confDelay @4: UInt8;
  sceniaroFailReason @5: UInt8;
  reserved1 @6: UInt8;
  reserved2 @7: UInt8;
}

struct DiagAebfunc{
  mtnrqrd @0: MtnrqrdDiag;
  cmbb @1: CmbbDiag;
  fcw @2: FcwDiag;
  brkarb @3: BrkarbDiag;
  sceniaro @4: SceniaroDiag;
}

struct LgSfLtap{
  statemachine @0: Machinests;
  aebfunc @1: DiagAebfunc;
}

struct LgSfDiagInfo{
  lgsfInhibitmask @0: UInt32;
  lgsfDiagflag @1: UInt32;
}

struct LgSafeOut{
  lgsfLongscene @0: LgSfLongScene;
  lgsfLtap @1: LgSfLtap;
  lgsfDiag @2: LgSfDiagInfo;
  lgsfUseLtap @3: UInt8;
  lgsfMajorversion @4: UInt8;
  lgsfMinorversion @5: UInt8;
  lgsfRlc @6: UInt8;
}

struct AsControl{
    lgsf @0 :LgSafeOut;
}