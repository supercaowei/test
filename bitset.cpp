#include <stdio.h>
#include <bitset>
#include <unistd.h>

class Bitset {
public:
    Bitset(size_t cnt) : bitCnt(cnt), uintCnt(bitCnt >> 5), lastUintBitCnt(bitCnt % 32) {
        bits = (uint32_t*)malloc((uintCnt + 1) * sizeof(uint32_t));
        memset(bits, 0, (uintCnt + 1) * sizeof(uint32_t));
    }

    ~Bitset() {
        free(bits);
    }

    //某一位置1或0，默认置1
    Bitset& set(size_t pos, bool val = true) {
        if (pos >= bitCnt) {
            return *this;
        }
        if (val) {
            bits[pos >> 5] |= (1 << (pos % 32));
        } else {
            bits[pos >> 5] &= ~(1 << (pos % 32));
        }
        return *this;
    }

    //所有位置1
    Bitset& set() {
        memset(bits, 0xff, (uintCnt + 1) * 4);
        return *this;
    }

    //某一位置0
    Bitset& reset(size_t pos) {
        return set(pos, false);
    }

    //所有位置0
    Bitset& reset() {
        memset(bits, 0, (uintCnt + 1) * 4);
        return *this;
    }

    //所有位取反
    void flip() {
        for (int i = 0; i < uintCnt + 1; i++) {
            bits[i] = ~bits[i];
        }
    }

    bool test(size_t pos) const {
        assert(pos < bitCnt);
        if (pos >= bitCnt) {
            return false;
        }
        return bits[pos >> 5] & (1 << (pos % 32));
    }

    size_t findFirst() {
        return findNext(0);
    }

    size_t findNext(size_t startPos) {
        if (startPos >= bitCnt) {
            return bitCnt;
        }

        //如果startPos不是uint32边界，就先找startPos到下一个uint32边界这几个比特位。
        //比如startPos为70，希望从第70位开始找下一个为1的比特位。那就取下标为2（70/32）的uint32，
        //通过和0xFFFFFFC0（~((1<<6)-1)）按位与来计算，检查其高26（32-(70%32)）位是否有1。
        size_t startIndex = (startPos >> 5); //除以32等于startPos所在的uint32下标
        size_t nonalignBits = (startPos % 32); //该uint32低位没对齐uint32的位数
        if (nonalignBits && (bits[startIndex] & (~((1 << nonalignBits) - 1)))) {
            for (size_t pos = nonalignBits; pos < 32; pos++) {
                if (bits[startIndex] & (1 << pos)) {
                    return (startIndex << 5 ) + pos;
                }
            }
            startIndex++;
        }
        //找中间的完整uint32
        for (size_t i = startIndex; i < uintCnt; i++) {
            if (bits[i]) { //第i个uint32不是0，就一位位的找
                for (size_t pos = 0; pos < 32; pos++) {
                    if (bits[i] & (1 << pos)) {
                        return (i << 5) + pos;
                    }
                }
            }
        }
        //还是没找到，就找最后lastUintBitCnt这几位
        if (lastUintBitCnt && (bits[uintCnt] & ((1 << lastUintBitCnt) - 1))) {
            for (size_t pos = 0; pos < lastUintBitCnt; pos++) {
                if (bits[uintCnt] & (1 << pos)) {
                    return (uintCnt << 5) + pos;
                }
            }
        }
        return bitCnt; //没找到，返回bitCnt表示所有bit位都是0。
    }

    //有多少个bit位是1
    size_t count() const {
        size_t cnt = 0;
        for (size_t i = 0; i < uintCnt; i++) {
            for (size_t j = 0; j < 32; j++) {
                if ((bits[i] >> j) & 1) {
                    cnt++;
                }
            }
        }
        for (size_t j = 0; j < lastUintBitCnt; j++) {
            if ((bits[uintCnt] >> j) & 1) {
                cnt++;
            }
        }
        return cnt;
    }

    //返回比特位数（包含为0和为1的位）
    size_t size() const {
        return bitCnt;
    }

    //是否存在值为1的比特位
    bool any() const {
        for (size_t i = 0; i < uintCnt; i++) {
            if (bits[i]) {
                return true;
            }
        }
        if (lastUintBitCnt && (bits[uintCnt] & ((1 << lastUintBitCnt) - 1))) {
            return true;
        }
        return false;
    }

    //是否所有位都是0
    bool none() const {
        return !any();
    }

    //是否所有位都是1
    bool all() const {
        for (size_t i = 0; i < uintCnt; i++) {
            if (~bits[i]) { //取反不等于0，则原值肯定有值为0的比特位
                return false;
            }
        }
        if (lastUintBitCnt && ((~bits[uintCnt]) & ((1 << lastUintBitCnt) - 1))) {
            return false;
        }
        return true;
    }

private:
    uint32_t* bits; //比特数据内容
    const size_t bitCnt; //比特位总位数
    const size_t uintCnt; //bitCnt除32得到的uint个数。bitCnt == uintCnt * 32 + lastUintBitCnt。
    const size_t lastUintBitCnt; //bitCnt没对齐32的情况下取余剩下的位数
};

int64_t getCurrentTimeNs() {
    int64_t cur_time = 0;
	struct timespec spec;
	clock_gettime(CLOCK_MONOTONIC, &spec);
	cur_time = (int64_t)spec.tv_sec * 1000000 + spec.tv_nsec / 1000;
    return cur_time;
}

template <size_t N>
inline size_t findNext(const std::bitset<N>& bitmask, size_t startPos = 0) {
    size_t i = startPos;
    while (i < N && !bitmask.test(i)) {
        i++;
    }
    return i;
}

int main() {
    auto t1 = getCurrentTimeNs();

    Bitset bs1(267);
    printf("sizeof my bitset %zu\n", sizeof(bs1));
    printf("my bitset: any %d, none %d, all %d\n", bs1.any(), bs1.none(), bs1.all());

    bs1.flip();
    printf("my bitset after flip: any %d, none %d, all %d\n", bs1.any(), bs1.none(), bs1.all());

    bs1.reset();
    printf("my bitset after reset: any %d, none %d, all %d\n", bs1.any(), bs1.none(), bs1.all());

    bs1.set();
    printf("my bitset after set: any %d, none %d, all %d\n", bs1.any(), bs1.none(), bs1.all());

    bs1.reset();
    printf("my bitset after reset: any %d, none %d, all %d\n", bs1.any(), bs1.none(), bs1.all());

    bs1.set(0);
    bs1.set(1);
    bs1.set(2);
    bs1.set(6);
    bs1.set(11);
    bs1.set(13);
    bs1.set(20);
    bs1.set(23);
    bs1.set(24);
    bs1.set(31);
    bs1.set(32);
    bs1.set(42);
    bs1.set(55);
    bs1.set(63);
    bs1.set(74);
    bs1.set(87);
    bs1.set(99);
    bs1.set(102);
    bs1.set(118);
    bs1.set(121);
    bs1.set(133);
    bs1.set(145);
    bs1.set(158);
    bs1.set(162);
    bs1.set(177);
    bs1.set(183);
    bs1.set(194);
    bs1.set(206);
    bs1.set(212);
    bs1.set(223);
    bs1.set(224);
    bs1.set(238);
    bs1.set(249);
    bs1.set(255);
    bs1.set(263);
    bs1.set(266);
    bs1.set(267);
    size_t _1cnt = bs1.count();
    bs1.flip();
    printf("set bit count before flip: %zu, after flip %zu.\n", _1cnt, bs1.count());

    size_t i = 0;
    while (i < 267) {
        i = bs1.findNext(0);
        if (i >= 267) {
            break;
        }
        printf("value 1 pos: %lu\n", i);
        bs1.set(i, false);
        i++;
    }

    auto t2 = getCurrentTimeNs();
    printf("My bitset cost %lldns.\n", (t2 - t1));


    std::bitset<256> bs2;
    printf("sizeof std::bitset %zu\n", sizeof(bs2));
    bs2.set(0);
    bs2.set(1);
    bs2.set(2);
    bs2.set(6);
    bs2.set(11);
    bs2.set(13);
    bs2.set(20);
    bs2.set(23);
    bs2.set(24);
    bs2.set(31);
    bs2.set(32);
    bs2.set(42);
    bs2.set(55);
    bs2.set(63);
    bs2.set(74);
    bs2.set(87);
    bs2.set(99);
    bs2.set(102);
    bs2.set(118);
    bs2.set(121);
    bs2.set(133);
    bs2.set(145);
    bs2.set(158);
    bs2.set(162);
    bs2.set(177);
    bs2.set(183);
    bs2.set(194);
    bs2.set(206);
    bs2.set(212);
    bs2.set(223);
    bs2.set(224);
    bs2.set(238);
    bs2.set(249);
    bs2.set(255);
    i = 0;
    while (i < 256) {
        i = findNext(bs2, 0);
        printf("value 1 pos: %lu\n", i);
        bs2.set(i, false);
        i++;
    }

    auto t3 = getCurrentTimeNs();
    printf("std::bitset cost %lldns.\n", (t3 - t2));


    for (i = 0; i < 256; i++) {
        bs1.set(i, (i % 2));
        bs1.test(i);
    }

    auto t4 = getCurrentTimeNs();
    printf("My bitset set and test cost %lldns.\n", (t4 - t3));

    for (i = 0; i < 256; i++) {
        bs2.set(i, (i % 2));
        bs2.test(i);
    }

    auto t5 = getCurrentTimeNs();
    printf("std::bitset set and test cost %lldns.\n", (t5 - t4));

    return 0;
}
