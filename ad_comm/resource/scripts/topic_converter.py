import sys
import rsclpy
from optparse import OptionParser

kTopicNameMap = {
    "/_internal_channel/time/sim_time": "/internal_time/sim_time/11000",
    "node_state": "/internal_swc/node_state/12000",
    "component_state": "/internal_swc/component_state/12000",
    "/canbus/vehicle_report": "/canbus/vehicle_report/6100",
    "/canbus/chassis_acan": "/canbus/chassis_acan/6101",
    "/canbus/act_acan": "/canbus/act_acan/6102",
    "/sensor/lidar/fusion/point_cloud": "/sensor_lidar_fusion/point_cloud/6300",
    "/sensor/imu": "/sensor_ins/imu/6200",
    "/sensor/ins": "/sensor_ins/ins/6200",
    "/sensor/gnss": "/sensor_ins/gnss/6200",
    "/sensor/radar": "/sensor_radar/radar/6900",
    "/sensor/camera/center_camera_fov120/encode": "/camera_encode/center_camera_fov120_encode/4200",
    "/sensor/camera/center_camera_fov30/encode": "/camera_encode/center_camera_fov30_encode/4200",
    "/sensor/camera/left_front_camera/encode": "/camera_encode/left_front_camera_encode/4200",
    "/sensor/camera/left_rear_camera/encode": "/camera_encode/left_rear_camera_encode/4200",
    "/sensor/camera/rear_camera/encode": "/camera_encode/rear_camera_encode/4200",
    "/sensor/camera/right_front_camera/encode": "/camera_encode/right_front_camera_encode/4200",
    "/sensor/camera/right_rear_camera/encode": "/camera_encode/right_rear_camera_encode/4200",
    "/monitor/laelaps_info": "/monitor/laelaps_info/4191",
    "/monitor/monitor_info": "/monitor/monitor_info/4191",
    "/monitor/summary_info": "/monitor/summary_info/4191",
    "/canbus/vehicle_command": "/pilot_control/vehicle_command/4190",
    "/control/control_debug": "/pilot_control/control_debug/4190",
    "/control/dmc_debug": "/pilot_control/dmc_debug/4190",
    "/decision_planning/trajectory": "/pilot_planning/trajectory/4090",
    "/decision_planning/planning_debug": "/pilot_planning/planning_debug/4091",
    "/prediction/objects": "/pilot_planning/prediction_objects/4092",
    "/localization/navstate_info": "/localization/navstate_info/6010",
    "/localization/odomstate_info": "/localization/odomstate_info/6010",
    "/perception/fusion/object": "/perception_aggregator/fusion_object/4060 ",
    "/perception/fusion/raw_object": "/perception_aggregator/fusion_raw_object/4060",
    "/perception/front_radar/object": "/perception_aggregator/front_radar_object/4060",
    "/perception/front_left_radar/object": "/perception_aggregator/front_left_radar_object/4060",
    "/perception/front_right_radar/object": "/perception_aggregator/front_right_radar_object/4060",
    "/perception/rear_radar/object": "/perception_aggregator/rear_radar_object/4060",
    "/perception/rear_left_radar/object": "/perception_aggregator/rear_left_radar_object/4060",
    "/perception/rear_right_radar/object": "/perception_aggregator/rear_right_radar_object/4060",
    "/perception/center_camera_fov120/env_odd": "/perception_camera/env_odd/1014",
    "/perception/center_camera_fov120/road_geometry": "/perception_camera/road_geometry/1012",
    "/perception/center_camera_fov120/road_semantics": "/perception_camera/road_semantics/1016",
    "/perception/center_camera_fov120_gop/object": "/perception_camera/object_frame/1018",
    "/perception/center_camera_fov30/env_odd": "/perception_camera/env_odd/1015",
    "/perception/center_camera_fov30/object": "/perception_camera/object_frame/1021",
    "/perception/center_camera_fov30/road_geometry": "/perception_camera/road_geometry/1013",
    "/perception/center_camera_fov30/road_semantics": "/perception_camera/road_semantics/1017",
    "/perception/center_camera_fov30_gop/object": "/perception_camera/object_frame/1019",
    "/perception/nv_cameras/object": "/perception_camera/object_frame/1031",
    "/perception/nv_cameras/road_geometry": "/perception_camera/road_geometry/1020",
    "/ved/ved_info": "/ved/ved_info/4080",
    "/scene_mapping/local_map": "/scene_mapping/local_map/4070",
    "/scene_mapping/hd_map": "/scene_mapping/hd_map/4070",
    "/scene_mapping/fusion_lines": "/scene_mapping/fusion_lines/4070",
    "/scene_mapping/local_map_bin": "/scene_mapping/local_map_bin/4070",
    "/ehr/road_structure": "/routing_map/ehr_road_structure/5002",
    "/holo/road_structure": "/holo/road_structure_holo/5006",
    "/decision_planning/decision_debug": "/pilot_decision/decision_debug/5001",
    "/decision_planning/decision_output": "/pilot_decision/decision_output/5001",
    "/decision_planning/decision_target": "/pilot_decision/decision_target/5001",
    "/xpilot/xdebug": "/pilot_decision/xpilot_xdebug/5001",
    "/decision_planning/trajectory": "/pilot_planning/trajectory/4090",
    "/decision_planning/planning_debug": "/pilot_planning/planning_debug/4091",
    "/prediction/fusionobstacle": "/prediction/fusionobstacle/5007",
    "/prediction/objects": "/pilot_planning/prediction_objects/5004",
    "/hdmap/sub_map": "/hdmap/sub_map/5000",
    "/hdmap/route_info": "/hdmap/route_info/5000",
    "/hdmap/road_structure": "/hdmap/road_structure/5000",
    "/canbus/mrr": "/canbus/mrr/6100",
    "/decision_planning/drivercmdinfo": "/pilot_planning/driver_cmd_info/4090",
    "/perception/fusionlane": "/perception_camera/fusionlane/1012",
    "/decision_planning/planning_state": "/pilot_planning/planning_state/4090",
    "/nop/hmi/panel": "/pilot_decision/nop_hmi_panel/5001"
}

invalid_topic = set()


def read_bag(target_bag_path):
    print("*" * 26 + "py bag reader" + "*" * 26)
    bag_reader = rsclpy.BagReader(target_bag_path)
    if not bag_reader.is_valid():
        print("bag is invalid!Exit")
        return
    bag_header = bag_reader.get_bag_header()
    print("bag file: " + target_bag_path)
    print("begin_time: " + str(bag_header.begin_time))
    print("end_time: " + str(bag_header.end_time))

    while True:
        tmp = bag_reader.read_next_message()
        if tmp is None:
            break
        yield tmp


def write_bag(target_bag_path):
    attr = rsclpy.BagWriterAttribute()
    attr.flags.compress_mode = rsclpy.CompressionMode.Lz4


if __name__ == '__main__':
    parser = OptionParser(
        usage="usage: python3 topic_converter.py dmppcl.00000.rsclbag -k")

    parser.add_option("-b", "--bag", help="Please input bag",
                      type="str", dest="bag")

    parser.add_option("-k",
                      "--keep",
                      dest="keep_old",
                      help="keep old msgs if not in autosar topic set",
                      default=False,
                      action='store_true')
    parser.add_option("-r",
                      "--reverse",
                      dest="reverse_to_old",
                      help="reverse bag to the version before autosar",
                      default=False,
                      action='store_true')
    (options, args) = parser.parse_args(sys.argv)

    if not options.bag:
        print("Please input -b bagname to specify a rsclbag file!")
        exit(1)
    bag_name = "dmppcl_autosar"
    if options.reverse_to_old:
        bag_name = "dmppcl_origin"
        kTopicNameMapReverse = dict()
        for key in kTopicNameMap:
            value = kTopicNameMap[key]
            kTopicNameMapReverse[value] = key
        kTopicNameMap = kTopicNameMapReverse
    attr = rsclpy.BagWriterAttribute()
    attr.flags.compress_mode = rsclpy.CompressionMode.Lz4
    bag_writer = rsclpy.BagWriter(bag_name, attr)

    for msg in read_bag(options.bag):
        info = rsclpy.ChannelInfo()
        if msg.channel_name not in kTopicNameMap:
            if options.keep_old:
                info.channel_name = msg.channel_name
            else:
                invalid_topic.add(msg.channel_name)
                continue
        else:
            info.channel_name = kTopicNameMap[msg.channel_name]
        info.type = msg.message_type
        info.descriptor = msg.descriptor
        bag_writer.add_serialized_message(
            msg.timestamp, msg.message_buffer, info)
    if not options.keep_old:
        for channel_name in invalid_topic:
            print("Topic name {} invalid!".format(channel_name))
