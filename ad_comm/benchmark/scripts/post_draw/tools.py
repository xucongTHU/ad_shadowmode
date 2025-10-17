#!/usr/bin/env python3
# -*- coding: UTF-8 -*-
"""
Copyright (C) 2020 by SenseTime Group Limited. All rights reserved.
Guo Zhichong <guozhichong@sensetime.com>
Yue Dayu <yuedayu@sensetime.com>
"""

from typing import Optional, List, Tuple, Any, Union, Callable
from collections import OrderedDict
from datetime import datetime
from mpl_toolkits.axisartist.parasite_axes import HostAxes, ParasiteAxes
import matplotlib.dates as mdates
import matplotlib.pyplot as plt
import matplotlib.tri as mtri
import os
import json
import yaml
import re
import logging
import csv
import numpy as np

from mpl_toolkits.mplot3d import Axes3D


class InvalidConfiguration(Exception):

    def __init__(self, info="Please check file."):
        super().__init__(info)


class MultiFileMatch(Exception):

    def __init__(self, pattern, founds):
        super().__init__('Found more than one: {} - {}'.format(
            pattern, str(founds)))


logger = logging.getLogger(__name__)


class BaseFormatter(object):
    """ Base class for formatter
    """

    @staticmethod
    def encode(data: Any, **kwargs) -> str:
        pass

    @staticmethod
    def decode(data_str: str, **kwargs) -> Any:
        pass


class RawFormatter(BaseFormatter):
    """ Raw python formatter; only used for encode.
    """

    @staticmethod
    def encode(data: Any, **kwargs) -> str:
        return str(data)

    @staticmethod
    def decode(data_str: str, **kwargs) -> Any:
        pass


class JsonFormatter(BaseFormatter):
    """ Formatter for json
    """

    @staticmethod
    def encode(data: Any,
               sort_keys: bool = False,
               indent: int = 4,
               separators: Optional[Tuple[str, str]] = (',', ': '),
               **kwargs) -> str:
        return json.dumps(data,
                          sort_keys=sort_keys,
                          indent=indent,
                          separators=separators,
                          **kwargs)

    @staticmethod
    def decode(data_str: str, **kwargs) -> Any:

        def _mk_ordered_dict(input_data: List[Tuple[Any, Any]]) -> Any:
            return OrderedDict(input_data)

        return json.loads(data_str, object_pairs_hook=_mk_ordered_dict)


class YamlFormatter(BaseFormatter):
    """ Formatter for yaml
    """

    @staticmethod
    def encode(data: Any, **kwargs) -> str:

        class OrderedDumper(yaml.SafeDumper):

            def __new__(cls, *args, **kw):
                cls.add_representer(OrderedDict,
                                    OrderedDumper._dict_representer)
                return yaml.SafeDumper.__new__(cls)

            @staticmethod
            def _dict_representer(dumper, out_data):
                return dumper.represent_mapping(
                    yaml.resolver.BaseResolver.DEFAULT_MAPPING_TAG,
                    out_data.items())

        return yaml.dump(data,
                         None,
                         OrderedDumper,
                         default_flow_style=False,
                         allow_unicode=True)

    @staticmethod
    def decode(data_str: str, **kwargs) -> Any:

        class OrderedLoader(yaml.SafeLoader):

            def __new__(cls, *args, **kw):
                cls.add_constructor(
                    yaml.resolver.BaseResolver.DEFAULT_MAPPING_TAG,
                    OrderedLoader._ordered_construct_mapping)
                return yaml.SafeLoader.__new__(cls)

            @staticmethod
            def _ordered_construct_mapping(loader, node):
                loader.flatten_mapping(node)
                return OrderedDict(loader.construct_pairs(node))

        return yaml.load(data_str, OrderedLoader)


class FileIOHelper(object):
    """ Static class of file helper
    """

    @staticmethod
    def read(filepath: str,
             default_formatter: Optional[BaseFormatter] = None) \
            -> Optional[OrderedDict]:
        """ Read json or yaml file

        Args:
            filepath: File path to read.
            default_formatter: Optional, default formatter when cannot distinguish file type.

        Returns:
            None or OrderedDict contained file content.
        """
        with open(filepath, 'r') as f_in:
            data_str = f_in.read()
        if os.path.splitext(filepath)[-1] == '.json':
            return JsonFormatter.decode(data_str)
        if os.path.splitext(filepath)[-1] == '.yaml':
            return YamlFormatter.decode(data_str)
        if default_formatter is None:
            default_formatter = YamlFormatter
        return default_formatter.decode(data_str)

    @staticmethod
    def write(data: OrderedDict, filepath: str,
              default_formatter: Optional[BaseFormatter] = None) \
            -> None:
        """ Write data to file.

        Args:
            data: Data to write
            filepath: File path to write
            default_formatter: Optional, default formatter when cannot distinguish file type

        Returns:
            None
        """
        if os.path.splitext(filepath)[-1] == '.json':
            data_str = JsonFormatter.encode(data)
        elif os.path.splitext(filepath)[-1] == '.yaml':
            data_str = YamlFormatter.encode(data)
        else:
            if default_formatter is None:
                default_formatter = YamlFormatter
            data_str = default_formatter.encode(data)
        with open(filepath, 'w') as f_out:
            f_out.write(data_str)


class DictHelper(object):
    """ Helper functions of dict
    """

    @staticmethod
    def walk(cur_path: List[str], cur_args: Any, internal_handler: Callable,
             leave_handler: Callable):
        if isinstance(cur_args, dict):
            result = OrderedDict()
            for i in cur_args:
                result[i] = DictHelper.walk(cur_path + [i], cur_args[i],
                                            internal_handler, leave_handler)
            return internal_handler(cur_path, result)
        if os.environ.get('nvidia_type') != 'xavier':
            if isinstance(cur_args, List[dict]):
                result = OrderedDict()
                for dict_elem in cur_args:
                    for i in dict_elem:
                        result[i] = DictHelper.walk(cur_path + [i],
                                                    dict_elem[i],
                                                    internal_handler,
                                                    leave_handler)
                return internal_handler(cur_path, result)
        return leave_handler(cur_path, cur_args)

    @staticmethod
    def get_key(data: dict,
                specified_keys: Optional[List[str]] = None,
                raise_error: bool = True,
                default_return: Any = None) \
            -> Any:
        """ Walk in dict, and get child data

        Args:
            data: dict to walk
            specified_keys: walk path; if None, return data.
            raise_error: raise error if True; otherwise will return default_return.
            default_return: default return value when raise_error if False and error occurred.

        Returns:
            child node data or default_return

        """
        if specified_keys is None:
            specified_keys = list()

        def _raise(cur_data):
            raise InvalidConfiguration('KEY: {} not found in json: {}'.format(
                key, JsonFormatter.encode(cur_data)))

        def _return(_):
            return default_return

        error_check = _raise if raise_error is True else _return
        if specified_keys is None:
            return data
        for key in specified_keys:
            if isinstance(data, dict):
                if key not in data.keys():
                    return error_check(data)
                data = data[key]
            else:
                return error_check(data)
        return data

    @staticmethod
    def has_keys(data: dict, keys: List[Any]) -> bool:
        """ Whether data has all keys
        """
        for key in keys:
            if key not in data.keys():
                return False
        return True

    @staticmethod
    def has_path(data: dict,
                 path_keys: List[Any],
                 leave_node: bool = False) -> bool:
        """ Whether data has path
        """
        if len(path_keys) == 0:
            if leave_node and isinstance(data, dict):
                return False
            return True
        key = path_keys[0]
        if key not in data.keys():
            return False
        return DictHelper.has_path(data[key], path_keys[1:], leave_node)

    @staticmethod
    def set_value(data: dict, path_keys: List[Any], value: Any) -> bool:
        """ set value for specific node
        """
        try:
            if DictHelper.has_path(data, path_keys, True):
                cursor = data
                for key in path_keys[:-1]:
                    cursor = cursor[key]
                cursor[path_keys[-1]] = value
                return True
        except Exception as e:
            print('Failed to set dict value for keys: {} to {}'.format(
                path_keys, value))
            print('Due to: {}'.format(e))
        finally:
            return False

    @staticmethod
    def create_nested_dict(keys: List[Any],
                           last_default_value: Any = None) -> dict:
        """ create nested dict
        """
        if len(keys) == 0:
            if last_default_value is None:
                return dict()
            return last_default_value
        return {
            keys[0]: DictHelper.create_nested_dict(keys[1:],
                                                   last_default_value)
        }


class DirHelper(object):
    """ Dir helper

    Attributes:
        _dir_path: path of opened dir.
        _file_tree: dir structure of opened dir_path.
        _is_open: whether opened a dir.
    """

    def __init__(self, dir_path: str = ''):
        self._dir_path = dir_path
        self._file_tree = dict()
        self._is_open = False
        if len(dir_path) > 0:
            self.open(dir_path)

    def open(self, dir_path: str) -> dict:
        dir_path = os.path.realpath(dir_path)
        self._file_tree[dir_path] = DirHelper._create_tree(dir_path)
        self._dir_path = dir_path
        self._is_open = (self._file_tree[dir_path] is not None)
        return self._file_tree

    @property
    def is_open(self):
        return self._is_open

    @property
    def tree(self) -> dict:
        return self._file_tree

    @property
    def path(self) -> str:
        return self._dir_path

    def get_files(self,
                  search_path: List[str] = None,
                  is_recursive: bool = True,
                  suffix: str = None,
                  patterns: Any = None,
                  single_match: bool = False) -> Any:
        """ Get all file matches requirements
        """
        if not self._is_open:
            return list()
        search_node = self._file_tree[self._dir_path]
        if search_path is not None:
            for path in search_path:
                if isinstance(search_node, dict):
                    if path not in search_node.keys():
                        return list()
                    search_node = search_node[path]
                else:
                    return list()
        if is_recursive:
            all_files = DirHelper._get_all_file_with_suffix(
                search_node, suffix)
        else:
            all_files = list()
            for c in search_node:
                if isinstance(search_node[c], str):
                    if suffix is None or (suffix is not None
                                          and search_node[c].endswith(suffix)):
                        all_files.append(search_node[c])
        if patterns is None:
            return all_files
        return DirHelper._walk_to_match(all_files, patterns, single_match)

    @staticmethod
    def _walk_to_match(files: List[str],
                       patterns: Any,
                       single_match: bool = True) -> Any:
        if isinstance(patterns, str):
            logger.debug("Try to match pattern: {}".format(patterns))
            return DirHelper._get_file_in_pattern(files, patterns,
                                                  single_match)
        ret = list()
        for sub_pattern in patterns:
            ret.append(
                DirHelper._walk_to_match(files, sub_pattern, single_match))
        return ret

    @staticmethod
    def _get_file_in_pattern(files, pattern: str, single_match: bool = False):
        founds = list()
        for file in files:
            found_string = re.findall(pattern, file)
            if len(found_string) > 0:
                founds.append(found_string[0])
        if single_match and len(founds) > 1:
            logger.error('Found more than one: {} - {}'.format(
                pattern, JsonFormatter.encode(founds)))
            raise MultiFileMatch(pattern, founds)
        return founds

    @staticmethod
    def _get_all_file_with_suffix(root: Union[dict, str],
                                  suffix: str = None) -> List[str]:
        if isinstance(root, str):
            if suffix is not None:
                if root.endswith(suffix):
                    return [root]
                return list()
            else:
                return [root]
        else:
            cur_all_files = []
            for c in root:
                cur_all_files += DirHelper._get_all_file_with_suffix(
                    root[c], suffix)
            return cur_all_files

    @staticmethod
    def _create_tree(path: str) -> Union[str, dict, None]:
        if os.path.isfile(path):
            return path
        elif os.path.isdir(path):
            cur_tree = dict()
            for file_obj in os.listdir(path):
                ret = DirHelper._create_tree(os.path.join(path, file_obj))
                if ret is not None:
                    cur_tree[file_obj] = ret
            return cur_tree
        else:
            return None


def is_float(value):
    pattern = re.compile(r'^[-+]?[0-9]*\.?[0-9]+([eE][-+]?[0-9]+)?$')
    return bool(pattern.match(value))


def plot_ts_csv(file_path):
    # Initialize a dictionary to store data
    data = {}
    data_mean = {}

    # Read the CSV file
    with open(file_path, 'r') as file:
        try:
            reader = csv.DictReader((row.replace('\0', '') for row in file),
                                    delimiter=',')
            headers = reader.fieldnames
            for header in headers:
                data[header] = []

            for row in reader:
                for header in headers:
                    if row[header] is None or not is_float(row[header]):
                        continue
                    data[header].append(float(row[header]))
        except:
            print("Failed to plot_ts_csv {}".format(file_path))
            return data_mean, ""

    # Plot the data
    plot_font = {
        'weight': 'normal',
        'size': 23,
    }
    plt.figure(dpi=150, figsize=(24, 16))

    timestamps = [datetime.fromtimestamp(ts / 1e9) for ts in data["timestamp"]]
    for header, values in data.items():
        if header == "timestamp" or header == "idle":
            continue
        plt.plot(timestamps, values, label=header)

    # Add labels and legend
    plt.xlabel('Timestamp', plot_font)
    plt.ylabel('Values', plot_font)
    plt.tick_params(labelsize=23)
    plt.legend(prop=plot_font)
    plt.grid(linestyle="--", linewidth=0.3)
    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))

    # Show the plot
    plot_file = file_path.rsplit(".", maxsplit=1)[0] + ".png"
    plt.savefig(plot_file)
    plt.close('all')
    for k, v in data.items():
        if k == "timestamp":
            continue
        data_mean[k] = sum(v) / len(v)
    return data_mean, plot_file


# specializaion
def plot_proc_res_csv(file_path: str):
    # Initialize a dictionary to store data
    data = {}
    data_mean = {}
    handle = {}

    # Read the CSV file
    with open(file_path, 'r') as file:
        try:
            reader = csv.DictReader((row.replace('\0', '') for row in file),
                                    delimiter=',')
            headers = reader.fieldnames
            for header in headers:
                data[header] = []

            for row in reader:
                for header in headers:
                    if row[header] is None or not is_float(row[header]):
                        print(row[header])
                        continue
                    data[header].append(float(row[header]))
        except:
            print("Failed to plot_proc_res_csv {}".format(file_path))
            return data_mean, ""

    # Plot the data
    plot_font = {
        'weight': 'normal',
        'size': 23,
    }
    fig = plt.figure(dpi=150, figsize=(24, 16))

    host = HostAxes(fig, [0.1, 0.1, 0.75, 0.8])
    par1 = ParasiteAxes(host, sharex=host)
    par2 = ParasiteAxes(host, sharex=host)
    host.parasites.append(par1)
    host.parasites.append(par2)

    host.set_ylabel('cpu', plot_font)
    host.set_xlabel('Timestamp', plot_font)

    host.axis['right'].set_visible(False)
    host.axis['left'].major_ticklabels.set_fontsize(23)
    host.axis['left'].label.set_fontsize(23)
    host.axis['bottom'].label.set_fontsize(23)
    host.axis['bottom'].major_ticklabels.set_fontsize(23)
    par1.axis['right'].set_visible(True)
    par1.set_ylabel('thread number')

    par1.axis['right'].major_ticklabels.set_visible(True)
    par1.axis['right'].major_ticklabels.set_fontsize(23)
    par1.axis['right'].label.set_visible(True)
    par1.axis['right'].label.set_fontsize(23)

    par2.set_ylabel('rss(KB)', plot_font)
    offset = (100, 0)
    new_axisline = par2.get_grid_helper(
    ).new_fixed_axis  # "_grid_helper"与"get_grid_helper()"等价，可以代替

    par2.axis['right2'] = new_axisline(loc='right', axes=par2, offset=offset)
    par2.axis['right2'].major_ticklabels.set_fontsize(23)
    par2.axis['right2'].label.set_fontsize(23)

    fig.add_axes(host)

    timestamps = [datetime.fromtimestamp(ts / 1e9) for ts in data["timestamp"]]
    plt.gca().xaxis.set_major_formatter(mdates.DateFormatter('%H:%M:%S'))
    for header, values in data.items():
        if header == "timestamp":
            continue
        if header == "cpu_used" or header == "cpu_user" or header == "cpu_system":
            p, = host.plot(timestamps, values, label=header)
            handle[header] = p
        elif header == "thread_number":
            p, = par1.plot(timestamps, values, label=header)
            handle[header] = p
        elif header == "rss_kb":
            p, = par2.plot(timestamps, values, label="rss")
            handle[header] = p

    host.legend(prop=plot_font)
    #轴名称，刻度值的颜色
    host.axis['left'].label.set_color(handle["cpu_used"].get_color())
    par1.axis['right'].label.set_color(handle["thread_number"].get_color())
    par2.axis['right2'].label.set_color(handle["rss_kb"].get_color())

    # Show the plot
    plot_file = file_path.rsplit(".", maxsplit=1)[0] + ".png"
    plt.savefig(plot_file)
    plt.close('all')

    for k, v in data.items():
        if k == "timestamp":
            continue
        data_mean[k] = sum(v) / len(v)
    return data_mean, plot_file


# specializaion
def plot_case_avg(data: dict, dump_path: str):
    handle = {}

    # Plot the data
    plot_font = {
        'weight': 'normal',
        'size': 23,
    }
    fig = plt.figure(dpi=150, figsize=(24, 16))

    host = HostAxes(fig, [0.1, 0.2, 0.75, 0.8])
    par1 = ParasiteAxes(host, sharex=host)
    par2 = ParasiteAxes(host, sharex=host)
    host.parasites.append(par1)
    host.parasites.append(par2)

    host.set_ylabel('time(ms)', plot_font)
    # host.set_xlabel('case name', plot_font)

    host.axis['right'].set_visible(False)
    host.axis['left'].major_ticklabels.set_fontsize(23)
    host.axis['left'].label.set_fontsize(23)
    host.axis['bottom'].label.set_fontsize(23)
    host.axis['bottom'].major_ticklabels.set_fontsize(15)
    host.axis['bottom'].major_ticklabels.set_rotation(45)
    host.axis['bottom'].major_ticklabels.set_ha('right')
    # print(dir(host.axis['bottom'].major_ticklabels))
    par1.axis['right'].set_visible(True)
    par1.set_ylabel('cpu(% total)')

    par1.axis['right'].major_ticklabels.set_visible(True)
    par1.axis['right'].major_ticklabels.set_fontsize(23)
    par1.axis['right'].label.set_visible(True)
    par1.axis['right'].label.set_fontsize(23)

    par2.set_ylabel('rss(KB)', plot_font)
    offset = (100, 0)
    new_axisline = par2.get_grid_helper(
    ).new_fixed_axis  # "_grid_helper"与"get_grid_helper()"等价，可以代替

    par2.axis['right2'] = new_axisline(loc='right', axes=par2, offset=offset)
    par2.axis['right2'].major_ticklabels.set_fontsize(23)
    par2.axis['right2'].label.set_fontsize(23)

    fig.add_axes(host)

    case_name_list = data["case_name"]
    case_name_idx = range(len(case_name_list))
    plt.xticks([idx for idx in case_name_idx], case_name_list)
    for header, values in data.items():
        if header == "case_name":
            continue
        if header in [
                "latency_max_ms", "latency_avg_ms", "publish_max_ms",
                "publish_avg_ms", "jitter_avg_ms"
        ]:
            p, = host.plot(case_name_idx, values, label=header)
            handle[header] = p
        elif header == "cpu_used":
            p, = par1.plot(case_name_idx, values, label=header)
            handle[header] = p
        elif header == "rss_kb":
            p, = par2.plot(case_name_idx, values, label=header)
            handle[header] = p

    host.legend(prop=plot_font)
    #轴名称，刻度值的颜色
    # host.axis['left'].label.set_color(handle["cpu_used"].get_color())
    # par1.axis['right'].label.set_color(handle["thread_number"].get_color())
    # par2.axis['right2'].label.set_color(handle["rss_kb"].get_color())

    # Show the plot
    plt.savefig(dump_path)
    plt.close('all')

# specializaion
def plot_tuner_profile_csv(file_path: str):
    data = np.genfromtxt(file_path, dtype=None, delimiter=',', names=True)
    save_path = os.path.dirname(file_path)

    draw_label = ["publish_avg_ms", "latency_avg_ms", "pub_cpu_used", "sub_cpu_used"]
    draw_file = []

    for label in draw_label:
        fig = plt.figure(dpi=150, figsize=(24, 16))
        fig.suptitle(label + " ranges for pub hz and msg size", fontsize = 12)
        ax = plt.axes(projection='3d')

        ax.plot_trisurf(data["hz"],data["msg_bytes"], data[label], linewidth=1, antialiased=True, edgecolor='Gray', cmap='viridis')
        ax.set_xlabel('hz', fontsize=18)
        ax.set_ylabel('msg_bytes', fontsize=18)
        ax.set_zlabel(label, fontsize=18)
        ax.view_init(25, -15)
        file1 = os.path.join(save_path, "tuner_" + label + "_v1.png")
        plt.savefig(file1, pad_inches=0)
        ax.view_init(25, -80)
        file2 = os.path.join(save_path, "tuner_" + label + "_v2.png")
        plt.savefig(file2, pad_inches=0)
        draw_file.append(file1)
        draw_file.append(file2)
    return draw_file
    # plt.show()