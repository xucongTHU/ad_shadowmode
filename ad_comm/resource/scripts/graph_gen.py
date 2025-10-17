import os
import sys
import re
import json
import pprint
from collections import defaultdict

import networkx as nx
import matplotlib.pyplot as plt
from netgraph import InteractiveGraph

def isIP(str):
    p = re.compile('^((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$')
    if p.match(str):
        return True
    else:
        return False

def plot_graph(platform: str,
               topic_graph: defaultdict(lambda: defaultdict(set)),
               output_dir = "/tmp/rscl_graph/"):
    if not os.path.exists(output_dir):
        os.makedirs(output_dir)
    DG = nx.DiGraph()
    edges_plt = []
    edges_plt_set = set()
    edges_plt_val_map = defaultdict(str)
    for edge_name, edges in topic_graph.items():
        if edge_name == "node_state":
            continue
        # print(edge_name, edges)
        for src_node in edges["src"]:
            for dst_node in edges["dst"]:
                edge_hash_str = src_node[0] + dst_node[0] + edge_name
                if edge_hash_str in edges_plt_set:
                    continue
                edges_plt_val_map[src_node[0] + "|" + dst_node[0]] += (
                    edge_name + " " + str(round(src_node[1], 1)) + "->" +
                    str(round(dst_node[1], 1)) + "\n")
                edges_plt_set.add(edge_hash_str)
        for src_node in edges["zmq_src"]:
            for dst_node in edges["zmq_dst"]:
                edge_hash_str = src_node[0] + dst_node[0] + edge_name
                if edge_hash_str in edges_plt_set:
                    continue
                edges_plt_val_map[src_node[0] + "|" +
                                  dst_node[0]] += ("zmq recv: " + edge_name +
                                                   " " +
                                                   str(round(dst_node[1], 1)) +
                                                   "\n")
                edges_plt_set.add(edge_hash_str)
    for k, v in edges_plt_val_map.items():
        src_node_item, dst_node_item = k.split("|")
        edges_plt.append((src_node_item, dst_node_item, {'label': v}))

    if len(edges_plt) == 0:
        print("skip empty " + platform)
        return
    DG.add_edges_from(edges_plt)

    with open(os.path.join(output_dir, "transmit_info.txt"), "a") as f:
        f.write("\n******************** Platform {} ********************".format(platform))
        f.write('\n')
        for items in edges_plt:
            if isIP(items[0]):
                f.write("Sender IP: \n\t" + items[0])
            else:
                f.write("pub node: \n\t" + items[0])
            f.write('\n')
            f.write("sub node: \n\t" + items[1])
            f.write('\n')
            f.write("topics: \n\t" + items[2]["label"].replace("\n", "\n\t"))
            f.write('\n')

    # # Show the graph using Matplotlib
    fig = plt.figure(1, figsize=(100, 80), dpi=60)

    edge_labels_nx = nx.get_edge_attributes(DG, 'label')
    edge_colors = dict()
    for k, _ in edge_labels_nx.items():
        if isIP(k[0]):
            edge_colors[k] = "blue"
        else:
            edge_colors[k] = "k"

    node_labels_nx = dict([(n, n) for n in DG.nodes()])
    node_colors = dict()
    for k, _ in node_labels_nx.items():
        if isIP(k):
            node_colors[k] = "lightgreen"
        else:
            node_colors[k] = "w"

    g = InteractiveGraph(DG,
                         arrows=True,
                         node_size=2,
                         node_layout="circular",
                         node_label_fontdict=dict(size=40),
                         node_edge_width=0.2,
                         node_color=node_colors,
                         edge_width=0.2,
                         edge_color = edge_colors,
                         edge_labels=edge_labels_nx,
                         edge_label_fontdict=dict(size=20),
                         edge_layout="curved",
                         edge_label_rotate=False,
                         node_labels=node_labels_nx)

    legend_text = "White circle: normal node\nGreen circle: zmq sender\n\nGray line: normal pub/sub\nBlue line: zmq tcp\n\ntopic_name 190.4->189.6: pub hz 190.4, sub hz 189.6\nzmq recv: topic_name 125.7: zmq recv topic hz 125.7"
    plt.text(0.9, 1, legend_text, fontsize=30, verticalalignment='center', horizontalalignment='left')
    fig.canvas.draw()
    fig.savefig(os.path.join(output_dir, platform + ".png"))
    plt.close(fig)
    DG.clear()


if __name__ == '__main__':
    if len(sys.argv) < 3:
        print("Usage: python3 graph_gen.py input_file output_path")
        exit(1)

    input_file = sys.argv[1]
    if not os.path.exists(input_file):
        print("{} not exist!".format(input_file))
        exit(2)

    platform_json_list = defaultdict(list)
    with open(input_file, 'r') as f:
        lines = f.readlines()
        for line in lines:
            try:
                json_obj = json.loads(line)
                platform_json_list[json_obj["hostinfo"].rsplit("_", 1)[0]].append(
                    json_obj["content"])
            except:
                print("Error parse json: " + line[0:100] + "...")

    for platform, content_list in platform_json_list.items():
        node_set = set()
        zmq_server_name = None
        topic_corr = defaultdict(lambda: defaultdict(set))
        for state_json in content_list:
            if "type" in state_json and state_json["type"] == "ZMQ":
                # This is zmq server state json
                if zmq_server_name is not None:
                    # print(state_json)
                    for zmq_recv in state_json["recvState"]["entries"]:
                        for sub in zmq_recv["topicState"]["sub"]:
                            topic_corr[sub["topicName"]]["zmq_src"].add(
                                (zmq_recv["sourceIp"], sub["hz"]))
                            topic_corr[sub["topicName"]]["zmq_dst"].add(
                                (zmq_server_name, sub["hz"]))
            else:
                # This is normal node state json
                if "daemon_server" in state_json[
                        "name"] or "recorder" in state_json["name"]:
                    continue
                node_set.add(state_json["name"])
                if "zmq_forward_bridge_server" in state_json["name"]:
                    zmq_server_name = state_json["name"]
                for pub in state_json["topicState"]["pub"]:
                    topic_corr[pub["topicName"]]["src"].add(
                        (state_json["name"], pub["hz"]))
                for sub in state_json["topicState"]["sub"]:
                    topic_corr[sub["topicName"]]["dst"].add(
                        (state_json["name"], sub["hz"]))

        plot_graph(platform, topic_corr, sys.argv[2])
