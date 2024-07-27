# perf script event handlers, generated by perf script -g python
# Licensed under the terms of the GNU GPL License version 2

# The common_* event handler fields are the most useful fields common to
# all events.  They don't necessarily correspond to the 'common_*' fields
# in the format files.  Those fields not available as handler params can
# be retrieved using Python functions of the form common_*(context).
# See the perf-script-python Documentation for the list of available functions.

from __future__ import print_function
import os
import sys
from pprint import pprint
from typing import NamedTuple

###
sys.path.append(os.environ['PERF_EXEC_PATH'] +
                '/scripts/python/Perf-Trace-Util/lib/Perf/Trace')
from Core import *
from perf_trace_context import *
###


class Node(NamedTuple):
    name: str
    duration: float

    @staticmethod
    def from_dict(d: dict) -> 'Node':
        pass


def trace_end():
    print("in trace_end")


def process_event(event: dict):
    if "callchain" in event:
        stacktrace = event["callchain"]
        for entry in reversed(stacktrace):
            name: str = entry.get("sym", {}).get("name", "[unknown]")
            if name.find("Matrix") != -1:
                pprint(list(reversed(stacktrace)))
                break
	# else:
	# 	name = event.get("symbol", "[unknown]")
	# 	libtype = self.get_libtype_from_dso(event.get("dso"))
	# 	node = self.find_or_create_node(node, name, libtype)

def trace_unhandled(context, event_fields_dict, perf_sample_dict):
    print(get_dict_as_string(event_fields_dict))
    print('Sample: {'+get_dict_as_string(perf_sample_dict['sample'], ', ')+'}')


def print_header(event_name, cpu, secs, nsecs, pid, comm):
    print("%-20s %5u %05u.%09u %8u %-20s " %
          (event_name, cpu, secs, nsecs, pid, comm), end="")


def get_dict_as_string(a_dict, delimiter=' '):
    return delimiter.join(['%s=%s' % (k, str(v))for k, v in sorted(a_dict.items())])