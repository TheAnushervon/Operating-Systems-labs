#include <stdio.h>
#include <math.h>
struct Point
{
    double x, y;
};
double distance(struct Point first, struct Point second)
{
    return sqrt(pow((first.x - second.x), 2) + pow((first.y - second.y), 2));
}
double area(struct Point a, struct Point b, struct Point c)
{
    return 0.5 * (a.x * b.y - b.x * a.y + b.x * c.y - c.x * b.y + c.x * a.y - a.x * c.y);
}
int main()
{
    struct Point A, B, C;
    A.x = 2.5;
    A.y = 6;
    B.x = 1;
    B.y = 2.2;
    C.x = 10;
    C.y = 6;
    double dis = distance(A, B);
    double ar = area(A, B, C);
    printf("distance a-b: %f\n", dis);
    printf("area: %f", ar);
}