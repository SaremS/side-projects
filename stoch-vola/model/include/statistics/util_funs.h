#ifndef UTIL_FUNS_H
#define UTIL_FUNS_H

#include <vector>

namespace utilfuns {
    double mean(const std::vector<double>& data);
    double standardDeviation(const std::vector<double>& data, const int& ddof = 0);
    double quantile(const std::vector<double>& data, double q);
}

#endif
