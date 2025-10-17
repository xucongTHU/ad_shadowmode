@0xe3f710e851b6dc0c;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("gac::rscl");

struct DataService2EWritedataCalib {
    dIDIndex @0 :UInt16;
    writeData @1 :UInt8;
}




struct DataService2EWritedataCalibResp {
    dIDIndex @0 :UInt16;
    errcode @1 :UInt8;
}



struct DataService22ReaddataCalib {
    dIDIndex @0 :UInt16;
}



struct DataService22ReaddataCalibResp {
    dIDIndex @0 :UInt16;
    errcode @1 :UInt8;
    dataLength @2 :UInt8;
    readData @3 :List(UInt8);
}



struct RoutineService31Calib {
    dIDIndex @0 :UInt16;
    routineControlType @1 :UInt8;
}



struct RoutineService31CalibResp {
    dIDIndex @0 :UInt16;
    routineControlType @1 :UInt8;
    errcode @2 :UInt8;
    routineStatus @3 :UInt8;
    dataLength @4 :UInt8;
    requestData @5 :List(UInt8);
}
