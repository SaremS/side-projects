#include <cmath>
#include <algorithm>
#include <stdexcept>

#include "statistics/util_funs.h"

namespace utilfuns {

  double mean(const std::vector<double>& data) {
      if (data.empty()) {
          throw std::invalid_argument("Data vector is empty.");
      }
      
      double sum = 0.0;
      for (double value : data) {
          sum += value;
      }
      return sum / data.size();
  }

  double standardDeviation(const std::vector<double>& data, const int& ddof) {
      if (ddof < 0) {
        throw std::invalid_argument("Degrees of freedom must be greater than or equal to zero.");
      }
      
      if (data.size() < 2) {
        return 0.0;
      }

      double meanValue = mean(data);
      double sumSquares = 0.0;
      
      for (double value : data) {
          sumSquares += (value - meanValue) * (value - meanValue);
      }
      
      return std::sqrt(sumSquares / (data.size() - ddof)); 
  }

  double quantile(const std::vector<double>& data, double q) {
      if (data.empty()) {
          throw std::invalid_argument("Data vector is empty.");
      }
      if (q < 0.0 || q > 1.0) {
          throw std::invalid_argument("Quantile must be between 0 and 1.");
      }
      
      std::vector<double> sortedData = data;
      std::sort(sortedData.begin(), sortedData.end());
      
      double pos = q * (sortedData.size() - 1);
      size_t lowerIndex = static_cast<size_t>(pos);
      size_t upperIndex = lowerIndex + 1;
      double weight = pos - lowerIndex;
      
      if (upperIndex >= sortedData.size()) {
          return sortedData[lowerIndex];
      }
      
      return sortedData[lowerIndex] * (1.0 - weight) + sortedData[upperIndex] * weight;
  }
} 
