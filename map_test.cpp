#include <stdio.h>
#include <map>

int main() {
    int count = 100;
    srand((unsigned int)time(NULL));
    for (int j = 0; j < count; j++) {
        std::map<int, int> m;
        for (int i = 0; m.size() < 10000; i++) {
            m[rand() % 1000000] = (rand() % 1000000);
        }
        int key = rand() % 1000000;
        auto iter = m.begin();
        while (iter != m.end()) {
            //iOS解码帧的时间戳与解码前时间戳会有微秒级误差，因此比较毫秒
            if (iter->first < key) {
                iter = m.erase(iter);
                assert(iter == m.begin());
            } else {
                m.erase(iter);
                break;
            }
        }
        int mapFirstKey = m.empty() ? -1 : m.begin()->first;
        printf("key %d, map first key %d, map size %lu.\n", key, mapFirstKey, m.size());
    }
    return 0;
}
