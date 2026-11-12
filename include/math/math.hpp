#pragma once

inline int floor_div(int a, int b)
{
    int d = a / b;
    int r = a % b;
    return (r != 0 && (r < 0) != (b < 0)) ? d - 1 : d;
}
