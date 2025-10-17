#!/usr/bin/env bash

source_ad() {
    if [ -z ${SENSEAUTO_INSTALLED_ROOT} ]; then
        export SENSEAUTO_INSTALLED_ROOT="/opt/senseauto_active"
    fi

    # Setup Python Path for rsclpy
    export PYTHONPATH=${SENSEAUTO_INSTALLED_ROOT}/senseauto-rscl/python/:$PYTHONPATH
    export PYTHONPATH=${SENSEAUTO_INSTALLED_ROOT}/senseauto-msgs/ad_msg_idl/:$PYTHONPATH
}

# Tab completion
_mainboard_d() {
    if [ -z "${RSCL_DAGS}" ]; then
        export RSCL_DAGS=$(find -L $SENSEAUTO_INSTALLED_ROOT -name "*.dag" -type f -exec basename {} \;)
    fi
    COMPREPLY=( $(compgen -W "${RSCL_DAGS[*]}" -- "${COMP_WORDS[COMP_CWORD]}") )
}

ad_activate() {
    if [ -z ${RSCL_SETUPED} ]; then
        export RSCL_SETUPED=1
        source_ad
        # complete -F _mainboard_d mainboard -d
    fi
}

export -f ad_activate
# ad_activate
