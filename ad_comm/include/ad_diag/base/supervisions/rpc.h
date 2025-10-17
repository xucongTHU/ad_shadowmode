/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * KitFung <fengzhongjie@senseauto.com>
 */

#pragma once

#include <deque>
#include <functional>
#include <memory>
#include <vector>
#include <list>
#include <thread>
#include <chrono>
#include <string>
#include <unordered_map>

#include "ad_base/common/socket.h"
#include "ad_diag/base/supervisions/common.h"
#include "ad_diag/base/supervisions/rpc_message.h"

namespace senseAD {
namespace diag {

typedef uint32_t Token;

enum class SupervisionChangeStatus : uint8_t {
    ONLINE = 0,
    DISABLED = 1,
    ENABLED = 2,
    OFFLINE = 3
};

// Class used for monitor and supervision to exchange data
// Expected Flow:
// - For report upload:             (TCP)
//   1. client    ====(register)====>   server
//   2. client   <====(reg ack)====     server
//   3. client    ====(report)====>     server
//   4. client    ====(report)====>     server
//   5. client    ====(report)====>     server
//   n. client   <====(control)====     server
//
// - For alive check:               (TCP)
//   1. client    ====(ping)=====>      server
//   2. client   <====(pong)=====       server
//
// [Static Mode]
// - For static discovery    (No discovery needed)
//   1. client(preset info) ====(start connect)===>   server
//
// [Discovery Mode]
// - For server online discovery    (BOARDCAST)
//   1. server   ====(boardcast)===>    client
class SupervisionRPC {
 public:
    class Client {
     public:
        using SupervisionControlCommandHandler =
            std::function<void(const ControlMessage&)>;
        using TcpClientPtrs =
            std::list<std::shared_ptr<senseAD::base::common::TcpFrameClient>>;

        static Client* NewStaticModeClient(
            const std::vector<std::string>& server_addrs);
        static Client* NewDiscoveryModeClient(
            const std::string& discovery_addr);

        ~Client();

        void Connect();
        void Disconnect();

        Token RegisterSupervision(const SupervisionEntity& entity,
                                  const SupervisionStatus* status);
        void UnRegisterSupervision(Token supervision_token);
        void SendReport(Token token,
                        const SupervisionReport& base_report,
                        const char* serialized_full_report_ptr = nullptr,
                        const size_t full_report_size = 0);

        // For Enable/Disable control
        void RegisterSupervisionControlCommandHandler(
            Token token, SupervisionControlCommandHandler handler);
        void UnRegisterSupervisionControlCommandHandler(Token token);

     private:
        Client();
        void InitStaticMode(const std::vector<std::string>& server_addrs);
        void InitDiscoveryMode(const std::string& discovery_addr);

        //   void ServerDiscoveryThread();
        void ConnectedCliThread();
        void DisconnectedCliThread();

        void HandleMessagesFromServer(
            senseAD::base::common::TcpFrameClient* ser);

        //! ------ Running State -------
        bool inited_ = false;
        bool running_ = false;
        bool static_mode_ = true;

        // For static mode
        std::vector<std::string> server_addrs_;
        // For discovery mode
        std::string discovery_addr_;

        //! ------ Supervision Handle ------
        std::mutex sve_mtx_;
        Token next_sve_token_ = 0;
        std::unordered_map<Token, SupervisionEntity> supervision_entities_;
        std::unordered_map<Token, const SupervisionStatus*> supervision_status_;

        std::mutex ch_mtx_;
        std::unordered_map<Token, SupervisionControlCommandHandler>
            control_command_handlers_;

        //! ------ report store --------
        std::mutex report_mtx_;
        std::unordered_map<Token, std::deque<ReportMessage>> reports_;
        std::unordered_map<Token, ReportMessage> last_report_;

        //! ------ tcp clients --------
        std::mutex cli_mtx_;
        TcpClientPtrs connected_clis_;
        TcpClientPtrs disconnected_clis_;
        std::atomic<bool> sers_need_reg_{false};

        std::thread connected_cli_thread_;
        std::thread disconnected_cli_thread_;

        std::shared_ptr<void> log_mgr_ref_;
    };

    class Server {
     public:
        using ReportHandleFn = std::function<void(const SupervisionEntity&,
                                                  const SupervisionReport&,
                                                  const char*,
                                                  const size_t)>;

        using SupervisionChangeStatusHandleFn = std::function<void(
            const SupervisionChangeStatus, const SupervisionEntity)>;

        Server();
        ~Server();

        void Bind(int port = 0);
        void Unbind();

        void SetReportHandler(const SupervisionCppType type,
                              const ReportHandleFn& handle_fn);
        void UnsetReportHandler(const SupervisionCppType type);

        void SetSupervisionChangeStatusHandler(
            const SupervisionChangeStatusHandleFn& fn);
        void UnsetSupervisionChangeStatusHandler();

        void BoardcastControlMessage(const SupervisionGroup& enabled_group,
                                     const ImportantLevel& enabled_level);

     private:
        void RecvThread();
        void TrackMaintainThread();
        SupervisionChangeStatusHandleFn CurrentSupervisionChangeStatusHandler();

        //! -------- For transmit ----------
        std::unique_ptr<senseAD::base::common::TcpFrameServer> ser_;

        std::mutex handler_mtx_;
        std::unordered_map<int, ReportHandleFn> report_handlers_;

        bool running_ = false;
        std::thread recv_thread_;
        std::thread track_thread_;

        //! -------- For supervision aliveness ---------
        struct SupervisionTrackRecord {
            SupervisionEntity entity;
            bool enabled;
            std::chrono::time_point<std::chrono::steady_clock> last_interact_tp;
        };
        std::mutex track_mtx_;
        std::unordered_map<SupervisionUniqueId, SupervisionTrackRecord>
            track_record_;

        std::mutex change_handler_mtx_;
        SupervisionChangeStatusHandleFn change_handler_;

        std::shared_ptr<void> log_mgr_ref_;
    };

    static std::shared_ptr<Client> GlobalClient();
    static std::shared_ptr<Server> GlobalServer();
};

}  // namespace diag
}  // namespace senseAD
