#include "graph.h"
#include <nds.h>

int factorial(int n)
{
    if (n == 0) return 1;
    return n*factorial(n-1);
}

int choose(int n, int k)
{
    return (factorial(n)/(factorial(k)*factorial(n-k)));
}

double evaluateFunction(int x, std::vector<double> coefficients, PointVector points)
{
    double s = coefficients[0];
    for (size_t i = 1; i < coefficients.size(); ++i)
    {
        double xi = (*(points.end()-i-1))->X;
        //printf("%f %f\n",(*(points.end()-i))->X,(*(points.end()-i))->Y);
        s = s * (x - xi) + coefficients[i];
        if (s < -2*SCREEN_WIDTH || s > 2*SCREEN_WIDTH) return s;
    }

    return s;
}

double Node::difference()
{
    if (left.get() == NULL || right.get() == NULL) return point->Y;
    if (calculated) return value;

    calculated = true;
    value = (left->difference() - right->difference())/(leftX() - rightX());
    return value;
}

double Node::leftX()
{
    if (left.get() == NULL) return point->X;
    return left->leftX();
}

double Node::rightX()
{
    if (right.get() == NULL) return point->X;
    return right->rightX();
}
