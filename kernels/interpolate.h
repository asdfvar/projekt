#ifndef TOOLS_H
#define TOOLS_H

namespace tools
{

 /*
 ** Function NAME: linear_interpolate
 */
 template <class T>
 T linear_interpolate (T x1,
                       T x2,
                       T y1,
                       T y2,
                       T x)
 {

    return (x - x1) / (x2 - x1) * (y2 - y1) + y1;

 }

} // namespace tools

#endif
