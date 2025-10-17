/*
 * Copyright (C) 2023 by SenseTime Group Limited. All rights reserved.
 * zhoupenghui <zhoupenghui@senseauto.com>
 */

#pragma once

#include <stddef.h>
#include <math.h>

namespace senseAD {
namespace base {
namespace common {

struct CommonStatisticMeassure {
    size_t count{0UL};

    double std_dev{0.0};

    double sum_val{0.0};

    double mean_val{0.0};
    double min_val{0.0};
    double max_val{0.0};

    // Valid Delta is absolute value
    double mean_abs_delta{-1.0};
    double min_abs_delta{-1.0};
    double max_abs_delta{-1.0};
};

template <typename Iterable>
CommonStatisticMeassure CalculateCommonStatisticMeassure(
    const Iterable &arr, const size_t arr_size) {
    CommonStatisticMeassure meassure;
    meassure.count = arr_size;

    if (meassure.count > 0) {
        meassure.min_val = meassure.max_val = arr[0];

        for (const auto &v : arr) {
            meassure.sum_val += v;
            if (meassure.min_val > v) {
                meassure.min_val = v;
            }
            if (meassure.max_val < v) {
                meassure.max_val = v;
            }
        }
        meassure.mean_val = meassure.sum_val / arr_size;
    }

    if (meassure.count > 1) {
        double abs_delta_sum = static_cast<double>(arr[1] - arr[0]);
        abs_delta_sum = abs_delta_sum > 0.0 ? abs_delta_sum : -abs_delta_sum;
        double delta_sqr_sum = abs_delta_sum * abs_delta_sum;

        meassure.min_abs_delta = meassure.max_abs_delta = abs_delta_sum;

        for (size_t i = 2; i < arr_size; ++i) {
            double abs_delta = static_cast<double>(arr[i] - arr[i - 1]);
            abs_delta = abs_delta > 0.0 ? abs_delta : -abs_delta;

            abs_delta_sum += abs_delta;
            delta_sqr_sum += abs_delta * abs_delta;
            if (meassure.min_abs_delta > abs_delta) {
                meassure.min_abs_delta = abs_delta;
            }
            if (meassure.max_abs_delta < abs_delta) {
                meassure.max_abs_delta = abs_delta;
            }
        }

        meassure.mean_abs_delta = abs_delta_sum / (arr_size - 1);

        for (const auto &v : arr) {
            auto mean_diff = v - meassure.mean_val;
            meassure.std_dev += mean_diff * mean_diff;
        }
        meassure.std_dev /= meassure.count - 1;
    }

    return meassure;
}

}  // namespace common
}  // namespace base
}  // namespace senseAD
