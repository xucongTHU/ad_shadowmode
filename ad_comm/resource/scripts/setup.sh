#!/bin/sh

PYTHONPATH=/opt/senseauto_active/senseauto-rscl/python/:${PYTHONPATH}
PYTHONPATH=/opt/senseauto_active/senseauto-msgs/ad_msg_idl/:${PYTHONPATH}
export PYTHONPATH

LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:/opt/senseauto/senseauto-rscl/active/lib/:/opt/senseauto/senseauto-3rdparty/active/3rdparty/lib/
export LD_LIBRARY_PATH
