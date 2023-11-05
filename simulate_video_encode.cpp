#include <iostream>
#include <list>
#include <map>
#include <random>
#include <algorithm>
#include <assert.h>

enum FrameType {
    U = 0,
    I,
    P,
    B
};

const char* frameTypes[] = {"U", "I", "P", "B"};

struct Frame {
    int64_t pts = 0;
    int64_t dts = 0;
    FrameType type;
    int bNum = 0;

    void dump() {
        std::cout << frameTypes[type];
        if (type == B) {
            std::cout << bNum;
        }
        std::cout << ", pts " << pts << ", dts " << dts 
            << ", cts " << (pts - dts) << std::endl;
    }
};

int main()
{
    const int gopFrameCount = 25, fps = gopFrameCount, bFrameCount = 3;
    float frameIntervalMs = 1000.0f / fps;
    int frameIndex = -1, bFrameIndex = 0;
    int64_t lastPts = 1000, lastDts = 0;
    FrameType lastType;
    std::list<Frame> inputFrameBuffer; //pts有序

    std::default_random_engine e;
    std::uniform_int_distribution<int> u(2,200); // 左闭右闭区间
    e.seed(time(0));
 
    for (int i = 0; i < 100; i++) {
        //送入编码器
        Frame frame {
            // .pts = lastPts + u(e)
            .pts = lastPts + (int)frameIntervalMs
        };
        lastPts = frame.pts;

        frameIndex = (++frameIndex % gopFrameCount); //[0, gopFrameCount)
        if (frameIndex == 0) {
            frame.type = I;
            lastType = I;
            bFrameIndex = 0;
        } else if (frameIndex == gopFrameCount - 1) {
            frame.type = P;
            lastType = P;
            bFrameIndex = 0;
        } else if (lastType == I || lastType == P || (lastType == B && bFrameIndex < bFrameCount)) {
            frame.type = B;
            lastType = B;
            bFrameIndex++;
            frame.bNum = bFrameIndex;
        } else {
            frame.type = P;
            lastType = P;
            bFrameIndex = 0;
        }
        inputFrameBuffer.push_back(frame);

        auto rectifyDts = [&]() -> std::list<Frame> {
            std::list<Frame> outputFrameBuffer; //dts有序
            if (lastType == I) { //如果是I帧，只会有I帧1个
                assert(inputFrameBuffer.size() == 1);
                auto& frame = *inputFrameBuffer.rbegin();
                frame.dts = std::max(frame.pts - (int64_t)(frameIntervalMs * 2), lastDts + 1);
                lastDts = frame.dts;
                outputFrameBuffer.push_back(std::move(frame));
                inputFrameBuffer.clear();
            } else {
                assert(lastType == P);
                std::vector<int> dtsIndexes;
                if (inputFrameBuffer.size() == 4) {
                    //输入 b1 b2 b3 P
                    //输出 P  b2 b1 b3
                    dtsIndexes = {3, 1, 0, 2};
                } else if (inputFrameBuffer.size() == 3) {
                    //输入 b1 b2 P
                    //输出 P  b1 b2
                    dtsIndexes = {2, 0, 1};
                } else if (inputFrameBuffer.size() == 2) {
                    //输入 b1 P
                    //输出 P  b1
                    dtsIndexes = {1, 0};
                } else if (inputFrameBuffer.size() == 1) {
                    //输入 P
                    //输出 P
                    dtsIndexes = {0};
                } else {
                    assert(0);
                }
                int64_t minCts = INT_MAX;
                auto ptsIter = inputFrameBuffer.begin();
                for (int j = 0; ptsIter != inputFrameBuffer.end(); j++, ptsIter++) {
                    //dtsIter移到dtsIndex位置
                    auto dtsIter = inputFrameBuffer.begin();
                    std::advance(dtsIter, dtsIndexes[j]);
                    dtsIter->dts = ptsIter->pts;
                    minCts = std::min(minCts, dtsIter->pts - dtsIter->dts);
                    outputFrameBuffer.push_back(*dtsIter);
                }
                if (minCts < 0) {
                    for (auto iter = outputFrameBuffer.begin(); iter != outputFrameBuffer.end(); iter++) {
                        iter->dts += minCts; //保证每个cts都 >= 0
                        if (iter->dts <= lastDts) {
                            iter->dts = lastDts + 1;
                        }
                        lastDts = iter->dts;
                    }
                }
                inputFrameBuffer.clear();
            }
            return outputFrameBuffer;
        };

        //编码输出
        if (lastType == I || lastType == P) {
            auto outputFrameBuffer = rectifyDts();
            for (auto iter = outputFrameBuffer.begin(); iter != outputFrameBuffer.end(); iter++) {
                iter->dump();
            }
        }
    }
    return 0;
}