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

    int index = 0;

    for (int k = 0; x[k+1] > p && k+1 < N; k++) {}

    if (x[k+1] > p)
    {
        return (p - x[k]) / (x[k+1] - x[k]) * (y[k+1] - y[k]) + y[k];
    }
    else
    {
        return static_cast<T>(-1);
    }

 }

} // namespace tools

#endif
