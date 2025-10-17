import argparse
import rsclpy
import logging
import plotly
from plotly.subplots import make_subplots
import plotly.graph_objs as go
import numpy as np
import traceback
import os

from typing import Sequence


logger = logging.getLogger("RSCLBAG_CHECK")

# if true, bag time as x axis. otherwise, seq a x axis
TIME_AS_X_AXIS = False
X_AXIS_TITLE = "Message Index in bag"
TIME_BASE = 0


def ns_to_ms(ns):
    return ns / 1000000.0


def ns_to_sec(ns):
    return ns / 1000000000.0


class MessageInfo:
    def __init__(self, bag_msg):
        self.writer = bag_msg.message_header.unique_node_id

        self.pub_time = bag_msg.message_header.stamp
        self.frame_seq = bag_msg.message_header.seq

        self.user_header_time = None
        msg_json = bag_msg.message_json
        if 'header' in msg_json:
            try:
                self.user_header_time = msg_json["header"]["time"]["nanoSec"]
            except:
                del msg_json
                self.user_header_time = None

        self.received_time = bag_msg.timestamp
        self.msg_size = len(bag_msg.message_buffer)


class OverallReport:
    def __init__(self, base_sec, end_sec):
        self.base_sec = base_sec
        self.end_sec = end_sec
        # len = to_sec(end_time - base_sec)
        self.x_axis = list(range(int(end_sec - base_sec) + 1))
        self.total_frame = [0] * len(self.x_axis)
        self.total_frame_drop = [0] * len(self.x_axis)
        self.total_size = [0] * len(self.x_axis)


class FrameDropResult:
    def __init__(self):
        self.x_axis = []
        self.disorder_vals = []
        self.duplicate_vals = []
        self.dropped_vals = []

        self.disorder_seq = []
        self.duplicate_seq = []
        self.dropped_seq = []


class IntervalDiffResult:
    def __init__(self):
        self.x_axis = []

        # different between time for consequence frame
        self.user_header_time_diff = []
        self.rscl_pub_time_diff = []
        self.bag_time_diff = []

        # different between header for same frame
        self.user_header_rscl_header_time_diff_ = []


class LatencyResult:
    def __init__(self):
        self.x_axis = []

        self.comm_latency = []


class MsgSizeTrendResult:
    def __init__(self):
        self.x_axis = []

        self.msg_size_trend = []


class SingleTopicChecker:
    def __init__(self, topic_name: str):
        # 0 unsure, 1 yes, 2 no
        self.have_received_time = 0
        self.topic_name = topic_name
        self.last_status = None
        self.infos = {}

    def _check_frame_drop(self, writer: str, infos: Sequence[MessageInfo]):
        # verification
        none_count = 0
        for info in infos:
            if info.frame_seq is None:
                none_count += 1
        if none_count > 0:
            if none_count != len(infos):
                logger.critical(
                    "Unexpected case. seq = None && not all none for %s::%s::_check_frame_drop", self.topic_name, writer)
            else:
                logger.info(
                    "Skip for %s::%s::_check_frame_drop", self.topic_name, writer)
            return None, None

        # check logic
        out = FrameDropResult()

        out.x_axis = list(range(len(infos) - 1))

        for idx, val in enumerate(infos[:-1]):
            nxt_val = infos[idx + 1]
            seq_interval = (nxt_val.frame_seq, val.frame_seq)
            diff = seq_interval[0] - seq_interval[1]

            disorder_val = 0
            duplicate_val = 0
            dropped_val = 0
            if diff < 0:
                logger.critical(
                    "Unexpected case. seq diff(%d) < 0 for %s::%s::_check_frame_drop", diff, self.topic_name, writer)
                out.disorder_seq.append((nxt_val.received_time, 1))
                disorder_val = 1

            elif diff == 0:
                logger.critical(
                    "Unexpected case. seq diff == 0 for %s::%s::_check_frame_drop", self.topic_name, writer)
                out.duplicate_seq.append((nxt_val.received_time, 1))
                duplicate_val = 1

            elif diff != 1:
                out.dropped_seq.append((nxt_val.received_time, diff))
                dropped_val = diff

            out.disorder_vals.append(disorder_val)
            out.duplicate_vals.append(duplicate_val)
            out.dropped_vals.append(dropped_val)

        return out, [
            go.Scatter(x=out.x_axis, y=out.disorder_vals,
                       line=dict(width=2, color='rgba(255, 0, 0, 0.8)'),
                       name='亂序發生 來自{}'.format(writer)),
            go.Scatter(x=out.x_axis, y=out.duplicate_vals,
                       line=dict(width=2, color='rgba(128, 255, 0, 0.8)'),
                       name='重復幀發生 來自{}'.format(writer)),
            go.Scatter(x=out.x_axis, y=out.dropped_vals,
                       line=dict(width=2, color='rgba(0, 0, 255, 0.8)'),
                       name='掉幀發生 來自{}'.format(writer)),
        ]

    def _check_publish_interval(self, writer: str, infos: Sequence[MessageInfo]):
        out = IntervalDiffResult()
        have_user_header_time = infos[0].user_header_time is not None
        have_pub_time = infos[0].pub_time is not None

        for idx, info in enumerate(infos[:-1]):
            nxt_info = infos[idx + 1]
            # check 0. user filled header time interval
            if have_user_header_time:
                out.user_header_time_diff.append(
                    ns_to_ms(nxt_info.user_header_time - info.user_header_time))

            # check 1. Publish Interval
            if have_pub_time:
                out.rscl_pub_time_diff.append(
                    ns_to_ms(nxt_info.pub_time - info.pub_time))

            # check 2. Received Interval
            out.bag_time_diff.append(
                ns_to_ms(nxt_info.received_time - info.received_time))

        # check 3. The different between the interval
        if have_user_header_time and have_pub_time:
            out.user_header_rscl_header_time_diff_ = [
                e[1] - e[0] for e in zip(out.user_header_time_diff, out.rscl_pub_time_diff)]

        if TIME_AS_X_AXIS:
            out.x_axis = [ns_to_sec(e.received_time - TIME_BASE)
                          for e in infos[:-1]]
        else:
            out.x_axis = list(range(len(infos) - 1))

        graph_objects = [
            go.Scatter(x=out.x_axis, y=out.bag_time_diff,
                       line=dict(width=2, color='rgba(255, 0, 0, 0.8)'),
                       name='幀間間隔 [最終rsclbag接收時間] 來自{}'.format(writer)),
        ]
        if have_user_header_time:
            graph_objects.append(
                go.Scatter(x=out.x_axis, y=out.user_header_time_diff,
                           line=dict(width=2, color='rgba(0, 255, 0, 0.8)'),
                           name='幀間間隔 [消息中用戶填的header時間] 來自{}'.format(writer))
            )
        if have_pub_time:
            graph_objects.append(
                go.Scatter(x=out.x_axis, y=out.rscl_pub_time_diff,
                           line=dict(width=2, color='rgba(0, 0, 255, 0.8)'),
                           name='幀間間隔 [消息中RSCL自動填的header時間] 來自{}'.format(writer))
            )
        if have_user_header_time and have_pub_time:
            graph_objects.append(
                go.Scatter(x=out.x_axis, y=out.user_header_rscl_header_time_diff_,
                           line=dict(width=2, color='yellow'),
                           name='幀間間隔差 [RSCL填的間隔-用戶填的間隔] 來自{}'.format(writer))
            )

        return out, graph_objects

    def _check_latency(self, writer: str, infos: Sequence[MessageInfo]):
        # Received Time - Publish Time
        out = LatencyResult()
        all_same = True
        for info in infos:
            if info.pub_time is None:
                logger.info(
                    "Skip for %s::%s::_check_latency. No pub time", self.topic_name, writer)
                return None, None
            if info.received_time != info.pub_time:
                all_same = False
            out.comm_latency.append(
                ns_to_ms(info.received_time - info.pub_time))

        if all_same:
            logger.info(
                "Skip for %s::%s::_check_latency. No real receive time", self.topic_name, writer)
            return None, None

        if TIME_AS_X_AXIS:
            out.x_axis = [ns_to_sec(e.received_time - TIME_BASE)
                          for e in infos]
        else:
            out.x_axis = list(range(len(infos)))

        return out,  go.Scatter(x=out.x_axis, y=out.comm_latency,
                                line=dict(width=2, color='red'),
                                name='Latency for {}'.format(writer))

    def _check_msg_size_trend(self, writer: str, infos: Sequence[MessageInfo]):
        out = MsgSizeTrendResult()
        out.msg_size_trend = [(e.received_time, e.msg_size) for e in infos]
        yaxis = [e.msg_size for e in infos]
        if TIME_AS_X_AXIS:
            out.x_axis = [ns_to_sec(e.received_time - TIME_BASE)
                          for e in infos]
        else:
            out.x_axis = list(range(len(infos)))
        return out, go.Scatter(x=out.x_axis, y=yaxis,
                               line=dict(width=2, color='blue'),
                               name='Message Size From {}'.format(writer))

    def load_data(self, bag_msg):
        info = MessageInfo(bag_msg)

        if info.writer not in self.infos:
            self.infos[info.writer] = []

        self.infos[info.writer].append(info)

    def finish(self, report_dump_path: str, overall_report: OverallReport):
        fig = make_subplots(
            rows=4, cols=1, subplot_titles=(
                "掉幀相關指標, 目標topic [{}]".format(self.topic_name),
                "消息間隔相關指標, 目標topic [{}]".format(self.topic_name),
                "收發延時, 目標topic [{}] 發送方到rsclbag".format(self.topic_name),
                "消息大小, 目標topic [{}]".format(self.topic_name),
            )
        )
        x_title = X_AXIS_TITLE
        if TIME_AS_X_AXIS:
            x_title += ", 時間基數: {}".format(ns_to_sec(TIME_BASE))
        fig.update_xaxes(title_text=x_title, row=1, col=1)
        fig.update_xaxes(title_text=x_title, row=2, col=1)
        fig.update_xaxes(title_text=x_title, row=3, col=1)
        fig.update_xaxes(title_text=x_title, row=4, col=1)

        fig.update_yaxes(title_text="count", row=1, col=1)
        fig.update_yaxes(title_text="ms", row=2, col=1)
        fig.update_yaxes(title_text="ms", row=3, col=1)
        fig.update_yaxes(title_text="byte", row=4, col=1)

        for writer, infos in self.infos.items():
            if len(infos) == 0:
                continue
            for info in infos:
                overall_report.total_frame[int(
                    ns_to_sec(info.received_time) - overall_report.base_sec)] += 1

            drop_info, drop_objs = self._check_frame_drop(writer, infos)
            if drop_objs is not None:
                for obj in drop_objs:
                    fig.add_trace(obj, row=1, col=1)
                for ts, cnt in drop_info.dropped_seq:
                    overall_report.total_frame_drop[int(
                        ns_to_sec(ts) - overall_report.base_sec)] += cnt

            _, interval_objs = self._check_publish_interval(writer, infos)
            for obj in interval_objs:
                fig.add_trace(obj, row=2, col=1)

            _, latency_obj = self._check_latency(writer, infos)
            if latency_obj is not None:
                fig.add_trace(latency_obj, row=3, col=1)

            msg_size_info, msg_size_obj = self._check_msg_size_trend(
                writer, infos)
            for ts, size in msg_size_info.msg_size_trend:
                overall_report.total_size[int(
                    ns_to_sec(ts) - overall_report.base_sec)] += size
            fig.add_trace(msg_size_obj, row=4, col=1)

        fig.write_html(os.path.join(report_dump_path,
                                    "chart_for_{}.html".format(self.topic_name.replace("/", "-"))))


def main(bag_path: str, output_report: str, start_time: int, end_time: int, topics):
    global TIME_BASE

    attr = rsclpy.BagReaderAttribute()
    if start_time > 0:
        attr.begin_time = start_time
    if end_time > 0:
        attr.end_time = end_time
    if topics is not None and len(topics) > 0:
        attr.included_channels = set(topics)
    attr.excluded_channels = set([
        "node_state",
        "component_state",
        "/internal_swc/node_state/12000",
        "/internal_swc/component_state/12000"
    ])
    bag_reader = rsclpy.BagReader(bag_path, attr)
    bag_header = bag_reader.get_bag_header()
    TIME_BASE = bag_header.begin_time

    checkers = {}

    logger.info("*** Start Read Data ***")
    i = 0
    while True:
        msg = bag_reader.read_next_message()
        if msg is None:
            break
        channel_name = str(msg.channel_name)
        if not channel_name in checkers:
            checkers[channel_name] = SingleTopicChecker(channel_name)
        checkers[channel_name].load_data(msg)
        i += 1
        if i % 5000 == 0:
            logger.info("Processed {} message".format(i))

    logger.info("*** Start Processing Data ***, Total Message: {}".format(i))
    overall_report = OverallReport(
        base_sec=int(ns_to_sec(bag_header.begin_time)),
        end_sec=int(ns_to_sec(bag_header.end_time))
    )

    for k, checker in checkers.items():
        logger.info("** Processing for {} **".format(k))
        checker.finish(output_report, overall_report)

    fig = make_subplots(
        rows=3, cols=1, subplot_titles=(
            "總消息數",
            "總掉幀",
            "總Size",
        )
    )
    fig.add_trace(go.Scatter(x=overall_report.x_axis, y=overall_report.total_frame,
                             line=dict(width=2, color='blue'),
                             name='總幀數'), row=1, col=1)
    fig.add_trace(go.Scatter(x=overall_report.x_axis, y=overall_report.total_frame_drop,
                             line=dict(width=2, color='blue'),
                             name='掉幀'), row=2, col=1)
    fig.add_trace(go.Scatter(x=overall_report.x_axis, y=overall_report.total_size,
                             line=dict(width=3, color='blue'),
                             name='Message Size'), row=3, col=1)
    x_title = "Bag Time, 時間基數(sec): {}".format(
        ns_to_sec(overall_report.base_sec))
    fig.update_xaxes(title_text=x_title, row=1, col=1)
    fig.update_xaxes(title_text=x_title, row=1, col=1)
    fig.update_xaxes(title_text=x_title, row=2, col=1)
    fig.update_yaxes(title_text="count", row=1, col=1)
    fig.update_yaxes(title_text="count", row=2, col=1)
    fig.update_yaxes(title_text="byte", row=3, col=1)
    fig.write_html(os.path.join(output_report,
                                "chart_for_all.html"))


if __name__ == '__main__':
    logging.basicConfig(level=logging.INFO)

    parser = argparse.ArgumentParser(
        description='rsclbag topic to csv converter tool',
        formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('-i', '--bag',
                        help='rsclbag path', required=True)
    parser.add_argument('-o', '--output_report',
                        help='report output path', required=True)
    parser.add_argument('--start_time',
                        type=int,
                        help='Start Time (if only want to analysis a sub section of the bag)', default=-1)
    parser.add_argument('--end_time',
                        type=int,
                        help='End Time (if only want to analysis a sub section of the bag)', default=-1)
    parser.add_argument('-t', '--topics', action='append',
                        help='Only Analysis the given topics')
    parser.add_argument('--xaxis', type=str,
                        choices=["time", "index"], default="time")

    args = parser.parse_args()
    if args.xaxis == "time":
        TIME_AS_X_AXIS = True
        X_AXIS_TITLE = "Bag Time (sec)"

    main(args.bag, args.output_report,
         args.start_time, args.end_time, args.topics)
