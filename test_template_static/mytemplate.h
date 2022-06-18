#ifndef _TEMPLETAE_H_
#define _TEMPLETAE_H_

template <typename T>
T my_max(T t1, T t2)
// const T& my_max(const T& t1, const T& t2)
{
    if (t1 > t2)
        return t1;
    return t2;
}

#endif

