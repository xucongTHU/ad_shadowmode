@0xe6b4e529449f48ed;

using Cxx = import "/capnp/c++.capnp";
$Cxx.namespace("senseAD::msg::avp_perception");
using CxxStruct = import "/capnp/c++struct.capnp";
$CxxStruct.structNamespace("senseAD::msgstruct::avp_perception");

enum ObjectLabel {
    unknown @0;
    pedestrian @1;
    vehicle @2;
    vehicleCar @3;
    vehicleSuv @4;
    vehicleVan @5;
    vehicleTruck @6;
    vehiclePickupTruck @7;
    vehicleBus @8;
    vehicleTaxi @9;
    vehicleEmergency @10;
    vehicleSchoolBus @11;
    vehicleOthers @12;
    vehicleEnd @13;
    bike @14;
    noPersonVehicle @15;
    bikeBicycle @16;
    bikeBikebig @17;
    bikeBikesmall @18;
    bikeEnd @19;
    trafficlightAll @20;
    trafficlightEnd @21;
    trafficsignAll @22;
    trafficsignEnd @23;
    obstacleAll @24;
    obstacleEnd @25;
    roadmarkerAll @26;
    roadmarkerEnd @27;
}
