#ifndef TOOLS_H
#define TOOLS_H

namespace tools
{

 /*
 ** Function NAME: linear_interpolate
 **
 ** Restriction:
 **    x must be monotonically increasing
 */
 template <class T>
 T linear_interpolate (T  *x,
                       T  *y,
                       T   p,
                       int N)
 {

    int index;

    for (index = 0; x[index+1] < p && index+1 < N; index++) {}

    if (x[index+1] >= p)
    {
        return (p - x[index]) / (x[index+1] - x[index]) * (y[index+1] - y[index]) + y[index];
    }
    else
    {
        return static_cast<T>(-1);
    }

 }

} // namespace tools

#endif
