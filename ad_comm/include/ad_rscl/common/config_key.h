/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * Zhang Yi <zhangyi15@senseauto.com>
 */

#pragma once

namespace senseAD {
namespace rscl {
namespace common {

// for parameter reuse, Node of key/value pairs of scalar
constexpr const char* kArgKey = "arg";

// module config, can be squence or map
constexpr const char* kModuleConfigKey = "module_config";

// loading module's dynamic library, must be scalar
constexpr const char* kModuleLibraryKey = "module_library";

// synced components, can be sequence or map
constexpr const char* kComponentsKey = "components";

// timer components, can be seqence or map
constexpr const char* kTimerComponentsKey = "timer_components";

// class name of a component, msut be scalar
constexpr const char* kClassNameKey = "class_name";

// the config of a component, must be map
constexpr const char* kConfigKey = "config";

// the custom parameters of the config of a component,
// be ware it would be overriden by parameters provided in
// command arguments, must be map
constexpr const char* kCustomParamKey = "custom_param";

// the name of the node, must be scalar
constexpr const char* kNameKey = "name";

// timer component OnProc() function interval rate, must be scalar(int)
constexpr const char* kIntervalMsKey = "interval_ms";

// timer component interval rate using sim_time, must be scalar(int)
constexpr const char* kSimTimeIntervalKey = "sim_time_interval";

// config paramters specific for individual modules, must be map
constexpr const char* kConfigParamKey = "config_param";

// node param of config parameters, must be map
constexpr const char* kNodeParamKey = "node_param";

// external file of config parameters, must be map
constexpr const char* kExtParamFileKey = "ext_param_file";

// extends the external file to the node_param without extra key
constexpr const char* kExtendsKey = "extends";

// sensor scheme modules of external file, must be map
constexpr const char* kSensorSchemeKey = "sensor_scheme";

// sensor scheme control modules, must be map
constexpr const char* kControlKey = "control";

// sensor scheme localization modules, must be map
constexpr const char* kLocalizationKey = "localization";

// sensor scheme sensor modules, must be map
constexpr const char* kSensorKey = "sensor";

// the primary file path for sensor scheme modules, must be scalar
constexpr const char* kPrimaryKey = "primary";

// the special file path for sensor scheme modules,
// it will override the primary file for corresponding module,
// must be scalar
constexpr const char* kOverrideKey = "override";

// vehicle node config module, must be map
constexpr const char* kVehicleNodeConfigKey = "vehicle_node_config";

constexpr const char* kVersionKey = "version";
// file path of vehicle node config, must be scalar
constexpr const char* kConfigPathKey = "config_path";

// bash script config, can be squence or map
constexpr const char* kBashScriptConfigKey = "bash_script_config";

// the command for bash script config, must be scalar
constexpr const char* kCommandKey = "command";

// config for ad log 
constexpr const char* kCustomLogCfgKey              = "adlog_config";
constexpr const char* kLogCfgLevelKey               = "level";
constexpr const char* kLogCfgPatternKey             = "pattern";
constexpr const char* kLogCfgApendModeKey           = "append_mode";
constexpr const char* kLogCfgTostderrKey            = "to_console";
constexpr const char* kLogCfgFileRootPathKey        = "file_root_path";
constexpr const char* kLogCfgCreateDirKey           = "create_dir";
constexpr const char* kLogCfgSimpleFileKey          = "simple_file";
constexpr const char* kLogCfgRotateFileKey          = "rotating_file";
constexpr const char* kLogCfgFileMaxSizeKey         = "file_max_size_mb";
constexpr const char* kLogCfgFileMaxCntKey          = "file_max_cnt";
constexpr const char* kLogCfgAsyncKey               = "async";
constexpr const char* kLogCfgAsyncThreadNumsKey     = "async_thread_nums";
constexpr const char* kLogCfgAsyncQueueLenKey       = "async_queue_length";
constexpr const char* kLogCfgAsyncOverflowPolicyKey = "async_overflow_policy";

}  // namespace common
}  // namespace rscl
}  // namespace senseAD
