#!/bin/bash

set -e

rm -rf /tmp/unittest_ad_log.*
LOG_level=info ./unittest_ad_log --gtest_filter="ADLOG.LogFlush"

rm -rf /tmp/unittest_ad_log.*
LOG_level=info ./unittest_ad_log --gtest_filter="ADLOG.LogLevel"

rm -rf /tmp/unittest_ad_log.*
LOG_level=invalid ./unittest_ad_log --gtest_filter="ADLOG.LogLevel"

rm -rf /tmp/unittest_ad_log.*
LOG_level=critical ./unittest_ad_log --gtest_filter="ADLOG.LogLevel"

rm -rf /tmp/unittest_ad_log.*
LOG_level=error ./unittest_ad_log --gtest_filter="ADLOG.LogLevel"
