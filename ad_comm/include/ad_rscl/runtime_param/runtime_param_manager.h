/*
 * Copyright (C) 2022 by SenseTime Group Limited. All rights reserved.
 */
#pragma once

#include <map>
#include <mutex>
#include <utility>
#include <string>
#include <memory>

#include "ad_base/common/shutdownable.h"
#include "ad_base/common/macros.h"
#include "ad_base/common/multicast_rpc.h"
#include "ad_log/log_manager.hpp"
#include "ad_rscl/idl/runtime_param.pb.h"
#include "ad_log/ad_log.hpp"

namespace senseAD {
namespace rscl {
namespace param {
constexpr const char *kMultiCastIp = "239.255.0.1";
constexpr uint16_t kMultiCastPort = 12348;
using RuntimeParamSetCallBack = std::function<bool(
    const std::string &, const std::string &, const std::string &)>;
using RuntimeParamGetCallBack =
    std::function<std::string(const std::string &, const std::string &)>;
using senseAD::base::common::MultiCastRpcClient;
using senseAD::base::common::MultiCastRpcServer;
class RuntimeParamManagerClient {
 public:
    ~RuntimeParamManagerClient() {}
    static std::shared_ptr<RuntimeParamManagerClient> GetInstancePtr(
        const std::string &node_name);
    bool SetParamValue(const std::string &dest_node,
                       const std::string &param_name,
                       const std::string &param_value);
    std::string GetParamValue(const std::string &dest_node,
                              const std::string &param_name);
    void SetParamValue(const std::string &dest_node,
                       const std::map<std::string, std::string> &params);
    void NodeDiscovery();
    void GetNodeAll(const std::string &dest_node);
    void GetAll();
    void PrintCurrentParams();
    inline std::map<std::string, std::map<std::string, std::string>> GetConfig()
        const {
        return configs_;
    }

 private:
    std::string node_name_;
    std::map<std::string, std::map<std::string, std::string>> configs_;
    DECLARE_AD_SINGLETON(RuntimeParamManagerClient)
};

struct Param {
    std::string value;
    RuntimeParamSetCallBack set_f{nullptr};
    RuntimeParamGetCallBack get_f{nullptr};
};

class RuntimeParamManager : public ::Shutdownable {
 public:
    virtual ~RuntimeParamManager();
    void Init();

    /**
     * @brief Add a param
     *
     * @param node_name Name of component
     * @param param_name Name of added param
     * @param param_value value of added param
     * @param set_cb set param callback function, called if param value changed
     * @param get_cb get param callback function, called if recv get param cmd
     * @param replace if the param has been added, replace it or not, default is
     * not
     * @return True if success to add param, or false
     */
    bool AddParam(const std::string &node_name,
                  const std::string &param_name,
                  const std::string &param_value,
                  const RuntimeParamSetCallBack &set_cb,
                  const RuntimeParamGetCallBack &get_cb,
                  bool replace = false);
    /**
     * @brief Add params
     *
     * @param node_name Name of component
     * @param param_cbs map struct is: map<param_name, Param>
     * @param replace if the param has been added, replace it or not, default is
     * not
     * @return True if success to add all params, or false
     */
    bool AddParam(const std::string &node_name,
                  const std::map<std::string, Param> &param_cbs,
                  bool replace = false);
    /**
     * @brief replace callback function of a param
     *
     * @param node_name Name of component
     * @param param_name name of param
     * @param set_cb set param callback function, called if param value changed
     * @param get_cb get param callback function, called if recv get param cmd
     * @param insert if this param is not found in this node, replace it or not,
     * default is not
     * @return True if success change callback function, or false
     */
    bool SetParamCallBack(const std::string &node_name,
                          const std::string &param_name,
                          const RuntimeParamSetCallBack &set_cb,
                          const RuntimeParamGetCallBack &get_cb,
                          bool insert = false);
    /**
     * @brief replace callback function of params
     *
     * @param node_name Name of component
     * @param cbs map of param name and param's new callback functions
     * @param insert if param is not found in this node, create new param or
     * not, default is not
     * @return True if success change all params' callback function, or false
     */
    bool SetParamCallBack(
        const std::string &node_name,
        const std::map<
            std::string,
            std::pair<RuntimeParamSetCallBack, RuntimeParamGetCallBack>> &cbs,
        bool insert = false);
    /**
     * @brief replace value of a param
     *
     * @param node_name Name of component
     * @param param_name name of param
     * @param param_value new value of param
     * @param insert if param is not found in this node, create new param or
     * not, default is not
     * @return True if success change this param's value, or false
     */
    bool SetParamValue(const std::string &node_name,
                       const std::string &param_name,
                       const std::string &param_value,
                       bool insert = false);
    /**
     * @brief replace value of params
     *
     * @param node_name Name of component
     * @param params map of params' name and params' new value
     * @param insert if param is not found in this node, create new param or
     * not, default is not
     * @return True if success change all params' value, or false
     */
    bool SetParamValue(const std::string &node_name,
                       const std::map<std::string, std::string> &params,
                       bool insert = false);
    /**
     * @brief get value of a param
     *
     * @param node_name Name of component
     * @param param_name name of param
     * @return param's value or
     * "PARAM_NOT_FOUND"/"NODE_NAME_IS_EMPTY"/"PARAM_NAME_IS_EMPTY"
     */
    std::string GetParamValue(const std::string &node_name,
                              const std::string &param_name);
    /**
     * @brief show all params' name and value of this node
     */
    void PrintCurrentParams();

    std::map<std::string, std::map<std::string, Param>> GetConfig() {
        return configs_;
    }

 private:
    void ShutdownImpl() override;
    std::string MsgProcess(const std::string &msg);
    bool HasParam(const std::string &node_name, const std::string &param_name);
    bool HasParam(const std::map<std::string, Param> &param_map,
                  const std::string &param_name);

    bool inited_{false};
    std::mutex cfg_mutex_;
    // {node name, {param name, Param}}
    std::map<std::string, std::map<std::string, Param>> configs_{};
    std::unique_ptr<MultiCastRpcServer> server_;

    DECLARE_AD_SINGLETON(RuntimeParamManager)
};
}  // namespace param
}  // namespace rscl
}  // namespace senseAD
