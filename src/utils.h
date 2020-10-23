#ifndef DEMO_UTILS_H
#define DEMO_UTILS_H

#include <iostream>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;


void strip_args(char *s);

// 实现argsort功能
template<typename T>
vector<int> argsort(const vector<T> &array)
{
    const int array_len(array.size());
    vector<int> array_index(array_len, 0);
    for (int i = 0; i < array_len; ++i)
        array_index[i] = i;

    sort(array_index.begin(), array_index.end(),
         [&array](int pos1, int pos2)
         { return (array[pos1] > array[pos2]); });

    return array_index;
}


template<typename _Tp>
int softmax(const _Tp *src, _Tp *dst, int length)
{
    const _Tp alpha = *std::max_element(src, src + length);
    _Tp denominator{0};

    for (int i = 0; i < length; ++i)
    {
        dst[i] = std::exp(src[i] - alpha);
        denominator += dst[i];
    }

    for (int i = 0; i < length; ++i)
    {
        dst[i] /= denominator;
    }

    return 0;
}


#endif //DEMO_UTILS_H
