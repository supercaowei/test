#include <stdio.h>
#include <bitset>

int main() {
    std::bitset<32> bs("1100");
    bs = (bs << 2);
    printf("bits count: %llu\n", bs.to_ullong());
    return 0;
}

