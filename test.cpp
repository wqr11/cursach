#include <iostream>
#include <math.h>

double F(double x, double y)
{
    return std::sqrt(x) + std::cbrt(y) - 1.0 / std::sqrt(x * x + y * y * y);
}

int main()
{
    // int n;
    double x, y;

    // std::cout << "n = ";
    // std::cin >> n;
    std::cout << "x y = ";
    std::cin >> x >> y;

    std::cout << "F = " << F(x, y) << std::endl;
}