@0xd37926e490ea0d0d;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl");

struct FloatVector {
    data @0 :List(Float32);
}



struct GuardnailInfo {
    confidence @0 :Float32;
    dxStart @1 :Float32;
    dxEnd @2 :Float32;
    guardnailC0 @3 :Float32;
    guardnailC1 @4 :Float32;
    guardnailC2 @5 :Float32;
    guardnailC3 @6 :Float32;
    reserved1 @7 :Float32;
    reserved2 @8 :Float32;
}



struct Header {
    seq @0 :UInt32;
    stamp @1 :Time;
    frameId @2 :Text;
}



struct RadarState {
    sensorId @0 :UInt8;
    nvmReadStatus @1 :UInt8;
    nvmWriteStatus @2 :UInt8;
    persistentError @3 :UInt8;
    temporaryError @4 :UInt8;
    interferenceError @5 :UInt8;
    temperatureError @6 :UInt8;
    voltageError @7 :UInt8;
    maxDistance @8 :UInt16;
    sortIndex @9 :UInt8;
    radarPower @10 :UInt8;
    ctrlRelay @11 :UInt8;
    outputType @12 :UInt8;
    sendQuality @13 :UInt8;
    sendExtinfo @14 :UInt8;
    motionRxState @15 :UInt8;
    rcsThreshold @16 :UInt8;
    blockError @17 :UInt8;
    broadcastError @18 :UInt8;
    electricAxisError @19 :UInt8;
    configError @20 :UInt8;
    calibrationError @21 :UInt8;
    connectorDirection @22 :UInt8;
    can0WorkMode @23 :UInt8;
    can1WorkMode @24 :UInt8;
    dualCanMode @25 :UInt8;
    timmingMode @26 :UInt8;
    powerMode @27 :UInt8;
    performanceMode @28 :UInt8;
    radarPosition @29 :UInt8;
    hwError @30 :UInt8;
    modulationStatus @31 :UInt8;
    failureFlag @32 :UInt8;
    sleepSig @33 :UInt8;
    objAziAngleCalib @34 :Float32;
}



struct Time {
    sec @0 :UInt32;
    nsec @1 :UInt32;
}



struct TimeStampInfo {
    timeStampStatus @0 :UInt16;
    timeStamp @1 :UInt64;
    autoSarTimeStamp @2 :UInt64;
    reserved1 @3 :UInt64;
    reserved2 @4 :UInt64;
}



struct Version {
    major @0 :UInt8;
    minor @1 :UInt8;
    patch @2 :UInt8;
}
