#include <stdio.h>
#include <float.h>
#include <stdlib.h>
#include <limits.h>
void *add_d(const void *a, const void *b)
{
    double result = *(double *)a + *(double *)b;
    double *res_ptr = malloc(sizeof(double));
    *res_ptr = result;
    return res_ptr;
}
void *add_i(const void *a, const void *b)
{
    int result = *(int *)a + *(int *)b;
    int *res_ptr = malloc(sizeof(int));
    *res_ptr = result;
    return res_ptr;
}
void *multiply_d(const void *a, const void *b)
{
    int result = *(double *)a * *(double *)b;
    double *res_ptr = malloc(sizeof(double));
    *res_ptr = result;
    return res_ptr;
}

void *multiply_i(const void *a, const void *b)
{
    int result = *(int *)a * *(int *)b;
    int *res_ptr = malloc(sizeof(int));
    *res_ptr = result;
    return res_ptr;
}

void *max_d(const void *a, const void *b)
{
    double max_a = *(double *)a;
    double max_b = *(double *)b;
    double result = (max_b > max_a) ? max_b : max_a;
    double *res_ptr = malloc(sizeof(double));
    *res_ptr = result;
    return res_ptr;
}

void *max_i(const void *a, const void *b)
{
    int max_a = *(int *)a;
    int max_b = *(int *)b;
    int result = (max_b > max_a) ? max_b : max_a;
    int *res_ptr = malloc(sizeof(int));
    *res_ptr = result;
    return res_ptr;
}
void *aggregate(void *base, size_t size, int n,
                void *initial_value, void *(*opr)(const void *, const void *))
{
    char *current = (char *)base;
    void *result = initial_value;

    for (int i = 0; i < n; i++)
    {
        result = opr(result, current);
        current += size;
    }
    return result;
}

int main()
{
    double values_d[5] = {1.0, 2.0, 3.0, 4.0, 5.0};
    double initial_value_a = 0;
    double *sum_result_d = (double *)aggregate(values_d, sizeof(double), 5, &initial_value_a, add_d);
    printf("Sum of double:%lf\n", *sum_result_d);
    double initial_value_m = 1;
    double *mult_result_d = (double *)aggregate(values_d, sizeof(double), 5, &initial_value_m, multiply_d);
    printf("Multiplication of double:%lf\n", *mult_result_d);
    double initial_value_gr = DBL_MIN;
    double *greater_result_d = (double *)aggregate(values_d, sizeof(double), 5, &initial_value_gr, max_d);
    printf("Max element of double type:%lf\n\n", *greater_result_d);

    int values_i[5] = {1, 2, 3, 4, 10};
    int initial_value_gri = INT_MIN;
    int initial_value_ai = 0;
    int *sum_result_i = (int *)aggregate(values_i, sizeof(int), 5, &initial_value_ai, add_i);
    printf("Sum of int:%d\n", *sum_result_i);
    int initial_value_mi = 1;
    int *mult_result_i = (int *)aggregate(values_i, sizeof(int), 5, &initial_value_mi, multiply_i);
    printf("Multiplication of int:%d\n", *mult_result_i);

    int *greater_result_i = (int *)aggregate(values_i, sizeof(int), 5, &initial_value_gri, max_i);
    printf("Max element of int type:%d\n", *greater_result_i);

    return 0;
}