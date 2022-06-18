#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

float mairuzijin = 100000; //买入资金，单位人民币元
float huilv = 6.5203; //1美元等于多少人民币
float mairujiage = 126; //买入时每股价格，单位美元
float xingquanjia = 0.02; //行权价，单位美元
float xingquanshichangjiage = 180; //行权时每股市场价，单位美元
float maichushuilv = 0.2; //股票转让税率
float yishourue = 0; //同一纳税年度内股票已收入额
float yinashuie = 0; //同一纳税年度内股票收入已纳税额

//输入应纳税所得额（单位元）
//输出应纳税额（单位元）
static float table[7][4] = {
    {0, 36000, 0.03, 0},
    {36000, 144000, 0.1, 2520},
    {144000, 300000, 0.2, 16920},
    {300000, 420000, 0.25, 31920},
    {420000, 660000, 0.30, 52920},
    {660000, 960000, 0.35, 85920},
    {960000, __INT_MAX__, 0.45, 181920}
};

void printTable() {
    for (int i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
        printf("应纳税所得额在%d到%d之间的部分：税率为%f，速算扣除数为%f\n", 
            (int)table[i][0], (int)table[i][1], table[i][2], table[i][3]);
    }
    printf("应纳税额 = (应纳税所得额 + 已行权所得额) * 对应区间税率 - 对应区间速算扣除数 - 已行权纳税额\n");
}

float nashuie(float yingnashuisuodee, char* jisuangongshi, size_t size) 
{
    memset(jisuangongshi, 0, size);

    if (yingnashuisuodee <= 0) {
        return 0;
    }
    
    for (int i = 0; i < sizeof(table) / sizeof(table[0]); i++) {
        if (yingnashuisuodee > table[i][0] && yingnashuisuodee <= table[i][1]) {
            float ret = (yingnashuisuodee + yishourue) * table[i][2] - table[i][3] - yinashuie;
            snprintf(jisuangongshi, size, "(%f + %f) * %f - %f - %f = %f", 
                yingnashuisuodee, yishourue, table[i][2], table[i][3], yinashuie, ret);
            return ret;
        }
    }

    return 0;
}

float readFloat(float fallback) {
    int i = 0;
    char temp[100] = {0};

    char c = getchar();
    if (c == '\n') {
        return fallback;
    }
    
    while (c != '\n')
    {
        temp[i++] = c;
        c = getchar();
    }

    return (float)atof(temp);
}

int main() 
{
    printf("请输入当前美元与人民币兑换汇率（默认值为%f）：", huilv);
    huilv = readFloat(huilv);
    printf("请输入股票买入价格（可能低于市场价，单位美元，默认值%f）：", mairujiage);
    mairujiage = readFloat(mairujiage);
    printf("请输入买入资金（单位人民币元，默认值%f）：", mairuzijin);
    mairuzijin = readFloat(mairuzijin);
    float mairugushu = floor(mairuzijin / mairujiage / huilv);
    mairuzijin = mairujiage * mairugushu * huilv;
    printf("您一共花费%f元买入了%d股\n\n", mairuzijin, (int)mairugushu);

    printf("请输入本次行权时市场价，单位美元（默认值为%f）：", xingquanshichangjiage);
    xingquanshichangjiage = readFloat(xingquanshichangjiage);
    float xingquangushu = mairugushu;
    printf("请输入本次行权股数（必须为整数，默认值为%d）：", (int)xingquangushu);
    xingquangushu = floor(readFloat(xingquangushu));
    printf("请输入本次行权之前，今年已行权应纳税所得额（单位人民币元，默认值为%f）：", yishourue);
    yishourue = readFloat(yishourue);
    printf("请输入本次行权之前，今年已行权已纳税额度（单位人民币元，默认值为%f）：", yinashuie);
    yinashuie = readFloat(yinashuie);
    float yingnashuisuodee = (xingquanshichangjiage - xingquanjia) * xingquangushu * huilv;
    printf("行权应纳税所得额 = (行权时市场价格 - 基本行权价) * 行权股数 * 汇率\n");
    printf("本次行权应纳税所得额（单位人民币元）：(%f - %f) * %d * %f = %f\n", 
        xingquanshichangjiage, xingquanjia, (int)xingquangushu, huilv, yingnashuisuodee);
    printTable();
    char jisuangongshi[200] = {0};
    float yingnashuie = nashuie(yingnashuisuodee, jisuangongshi, 200);
    printf("本次行权应纳税额（单位人民币元）：%s\n\n", jisuangongshi);

    float maichujiage = xingquanshichangjiage;
    printf("请输入本次卖出股票价格（可能高于或低于市场价），单位美元（默认值为%f）：", maichujiage);
    maichujiage = readFloat(maichujiage);
    float maichugushu = xingquangushu;
    printf("请输入本次卖出股数（必须为整数，默认值为%d）：", (int)maichugushu);
    maichugushu = floor(readFloat(maichugushu));
    float maichusuodee = 0;
    float maichunashuie = 0;
    if (maichujiage <= xingquanshichangjiage) {
        printf("卖出价格低于行权价格，股票转让所得额为0，转让应纳税额为0\n");
    } else {
        printf("股票转让应纳税所得额 = (转让价格 - 行权时市场价) * 转让股数 * 汇率\n");
        maichusuodee = (maichujiage - xingquanshichangjiage) * maichugushu * huilv;
        printf("本次股票转让应纳税所得额：(%f - %f) * %f * %f = %f\n", maichujiage, xingquanshichangjiage,
            maichugushu, huilv, maichusuodee);
        printf("股票转让应纳税额 = 股票转让应纳税所得额 * 股票转让税率（固定为20%%）\n");
        maichunashuie = maichusuodee * maichushuilv;
        printf("本次股票转让应纳税额：%f * %f = %f\n", maichusuodee, maichushuilv, maichunashuie);
    }
    printf("\n");

    printf("收益分析：收益 = 卖出收入 - 买入成本 - 行权纳税额 - 转让纳税额\n");
    float maichushouru = maichujiage * maichugushu * huilv;
    printf("卖出收入（卖出价格 * 卖出股数 * 汇率，单位人民币元）：%f * %f * %f = %f\n", maichujiage, maichugushu, huilv, maichushouru);
    float mairuchengben = mairuzijin / mairugushu * maichugushu;
    printf("买入成本（买入资金 / 买入股数 * 卖出股数，单位人民币元）：%f / %f * %f = %f\n", mairuzijin, mairugushu, maichugushu, mairuchengben);
    float shouyi = maichushouru - mairuchengben - yingnashuie - maichunashuie;
    printf("本次股票转让净收益：%f - %f - %f - %f = %f\n", maichushouru, mairuchengben, yingnashuie, maichunashuie, shouyi);

    return 0;
}
