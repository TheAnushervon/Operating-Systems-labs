#include <stdio.h>
#include <string.h>

typedef long long ll;

void convertBase(ll num, ll fromBase, ll toBase) {
    if (fromBase < 2 || toBase > 10 || fromBase > 10 || toBase < 2) {
        printf("Conversion is not possible!");
        return;
    }

    ll decimal = 0;
    ll power = 1;
    ll originalNum = num;
    
    while (originalNum > 0) {
        if (originalNum % 10 >= fromBase) {
            printf("Conversion is not possible!");
            return;
        }
        decimal = decimal + (originalNum % 10) * power;
        power = power * fromBase;
        originalNum /= 10;
    }

    ll result = 0;
    power = 1;
    while (decimal > 0) {
        result += (decimal % toBase) * power;
        power = power * 10;
        decimal /= toBase;
    }

    printf("%lld\n", result);
}

int main() {
    ll num, fromBase, toBase;

    scanf("%lld %lld %lld", &num, &fromBase, &toBase);
    convertBase(num, fromBase, toBase);

    return 0;
}
