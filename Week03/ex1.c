#include <stdio.h>
#include <stdlib.h>
int const_tri(int *p, int n)
{
    if (n == 0)
        return 0;
    if (n == 1 || n == 2)
        return 1;

    int temp = 0;
    p[0] = 0;
    p[1] = 1;
    p[2] = 1;
    for (int i = 3; i <= n; i++)
    {
        p[i] = p[i - 3] + p[i - 2] + p[i - 1];
    }

    return p[n];
}
int main()
{
    const int x = 1;
    const int *q = &x;
    int memsel[3];
    int *p = memsel;
    *p = *q;
    *(p + 1) = *q;
    *(p + 2) = 2 * (*q);

    printf("%p\n%p\n%p\n", p, p + 1, p + 2);

    if (p == memsel && p + 1 == memsel + 1 && p + 2 == memsel + 2)
    {
        printf("contiguos\n");
    }

    int n;
    printf("Enter value of n:");
    scanf("%d", &n);
    int *const pp = (int *)malloc((n + 1) * sizeof(int));
    int check = const_tri(pp, n);
    printf("%d", check);
}