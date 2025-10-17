@0xc0b3b0d8466c1960;

using RadarMsgDataType=import "/ad_msg_idl/ad_gac/radar_a_cm_data_type.capnp";

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl");

struct RadarTrack {
    id @0 :UInt8;
    idState @1 :UInt8;
    lifetime @2 :Float32;
    x @3 :Float32;
    y @4 :Float32;
    z @5 :Float32;
    vx @6 :Float32;
    vy @7 :Float32;
    ax @8 :Float32;
    ay @9 :Float32;
    rcs @10 :Float32;
    snr @11 :Float32;
    xRms @12 :Float32;
    yRms @13 :Float32;
    zRms @14 :Float32;
    vxRms @15 :Float32;
    vyRms @16 :Float32;
    axRms @17 :Float32;
    ayRms @18 :Float32;
    orientation @19 :Float32;
    orientRms @20 :Float32;
    yawRate @21 :Float32;
    heading @22 :Float32;
    length @23 :Float32;
    width @24 :Float32;
    height @25 :Float32;
    fusionSig @26 :UInt8;
    fusionCamID @27 :UInt8;
    yawRateRms @28 :Float32;
    lengthRms @29 :Float32;
    widthRms @30 :Float32;
    heightRms @31 :Float32;
    xQuality @32 :Float32;
    yQuality @33 :Float32;
    zQuality @34 :Float32;
    vxQuality @35 :Float32;
    vyQuality @36 :Float32;
    axQuality @37 :Float32;
    ayQuality @38 :Float32;
    orientationQuality @39 :Float32;
    yawRateQuality @40 :Float32;
    lengthQuality @41 :Float32;
    widthQuality @42 :Float32;
    heightQuality @43 :Float32;
    underpassProbability @44 :Float32;
    overpassProbability @45 :Float32;
    existProbability @46 :UInt8;
    movProperty @47 :UInt8;
    trackState @48 :UInt8;
    trackType @49 :UInt8;
    referencePoint @50 :UInt8;
    motionDirection @51 :UInt8;
}



struct RadarTrackArray {
    header @0 :RadarMsgDataType.Header;
    version @1 :RadarMsgDataType.Version;
    sensorId @2 :UInt8;
    radarState @3 :RadarMsgDataType.RadarState;
    trackList @4 :List(RadarTrack);
    guardnailInfo @5 :RadarMsgDataType.GuardnailInfo;
    timeStampInfo @6 :RadarMsgDataType.TimeStampInfo;
}



struct TrackList {
    data @0 :List(RadarTrack);
}
