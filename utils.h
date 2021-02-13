//
// Created by Evgenii Zheltonozhskii on 13/02/2021.
//

#ifndef LBM_CPP_UTILS_H
#define LBM_CPP_UTILS_H

#include <iterator>

template <class T, std::size_t N>
std::ostream& operator<<(std::ostream& o, const std::array<T, N>& arr)
{
  std::copy(arr.cbegin(), arr.cend(), std::ostream_iterator<T>(o, " "));
  return o;
}

#endif // LBM_CPP_UTILS_H
