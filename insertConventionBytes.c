#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

/**
请将data中所有等于0x000000、0x000001、0x000002、0x000003连续字节中的第二个0x00后面插入一个0x03。如：
0x 00 00 00 ------> 0x 00 00 (03) 00
0x 00 00 01 ------> 0x 00 00 (03) 01
0x 00 00 02 ------> 0x 00 00 (03) 02
0x 00 00 03 ------> 0x 00 00 (03) 03
0x 00 00 04 ------> 0x 00 00 04
0x 00 00 00 01 ------> 0x 00 00 (03) 00 01
0x 00 00 00 00 04 ------> 0x 00 00 (03) 00 00 04
0x 00 00 00 00 01 ------> 0x 00 00 (03) 00 00 (03) 01

要求：
(1) 不使用其他内存，就地处理。处理成功就返回0，处理失败（如空间不足）就返回非0值。
(2) 考虑内存安全（无内存越界，无内存泄漏）。
(3) 有时间的话，考虑最优时间复杂度和空间复杂度。
*/

int insertConventionBytes(uint8_t* data, uint32_t size, uint32_t capacity) {
    //先计算要插入多少字节
    int cnt = 0;
    for (int i = 0; i < size - 2; ++i) {
        if (data[i] == 0 && data[i + 1] == 0 && (data[i + 2] == 0 || data[i + 2] == 1 || data[i + 2] == 2 || data[i + 2] == 3)) {
            ++cnt;
            i += 1;
        }
    }
    //空间不够，返回失败
    if (size + cnt > capacity) {
        return -1;
    }
    //从后往前挪动和填充字节
    int idx = size - 1; //读index
    int i = size + cnt - 1; //写index
    int insertCnt = 0;
    while (i >= 0 && idx >= 0) {
        data[i] = data[idx];
        if (data[idx] == 0 || data[idx] == 1 || data[idx] == 2 || data[idx] == 3) {
            int zeroCnt = 0; //当前idx前面有多少个连续的0x00
            for (int j = idx - 1; j >= 0 && data[j] == 0; j--) {
                zeroCnt++;
            }
            //0x00 00 data[idx] 插入1个0x03，变成 0x00 00 03 data[idx]
            //0x00 00 00 data[idx] 插入1个0x03，变成 0x00 00 03 00 data[idx]
            //0x00 00 00 00 data[idx] 插入2个0x03，变成 0x00 00 03 00 00 03 data[idx]
            //0x00 00 00 00 00 data[idx] 插入2个0x03，变成 0x00 00 03 00 00 03 00 data[idx]
            //规律：data[idx]前有zeroCnt个0，就插入zeroCnt / 2个0x03
            while (zeroCnt > 0) {
                if (((zeroCnt--) % 2) == 0) {
                    data[--i] = 3;
                    insertCnt++;
                }
                data[--i] = data[--idx];
            }
        }
        --idx;
        --i;
    }
    assert(insertCnt == cnt);
    return size + cnt;
}

int main() {
    //每行16字节
    uint8_t data[100] = {0xB0, 0x00, 0x00, 0x00, 0x00, 0x01, 0x42, 0x01, 0x01, 0x00, 0x5D, 0xA0, 0x00, 0x01, 0x44, 0x14, 
                         0x00, 0x00, 0x01, 0x0C, 0x01, 0xFF, 0xFF, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0x10, 
                         0x5D, 0x15, 0x00, 0x00, 0xC0, 0x90, 0x01, 0x60, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
                         0x05, 0xA2, 0x00, 0x50, 0x16, 0x54, 0x10, 0x00, 0x00, 0x02, 0xC0, 0x2C, 0x00, 0x00, 0x00, 0x00};
    int len = insertConventionBytes(data, 64, sizeof(data));
    assert(len > 0);
    if (len < 0) {
        printf("capacity of data is not enough!\n");
        return -1;
    }
    for (int i = 0; i < len; ++i) {
       printf("0x%02X%s", data[i], (i == len - 1 ? "\n" : ", "));
    }
    uint8_t correctData[] = {0xB0, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x01, 0x42, 0x01, 0x01, 0x00, 0x5D, 0xA0, 0x00, 0x01, 0x44, 0x14,
                             0x00, 0x00, 0x03, 0x01, 0x0C, 0x01, 0xFF, 0xFF, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x03, 0x00, 0x10, 
                             0x5D, 0x15, 0x00, 0x00, 0xC0, 0x90, 0x01, 0x60, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 0x03, 0x00, 0x00, 
                             0x05, 0xA2, 0x00, 0x50, 0x16, 0x54, 0x10, 0x00, 0x00, 0x03, 0x02, 0xC0, 0x2C, 0x00, 0x00, 0x03, 0x00, 0x00};
    assert(memcmp(data, correctData, len) == 0);
    return 0;
}
