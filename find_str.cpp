/*
找到字符串的最长无重复字符子串
给定一个数组arr，返回arr的最长无的重复子串的长度(无重复指的是所有字母都不相同)。
*/
#include <iostream>
#include <map>
using namespace std;

// int findCharPosition(const std::string& s, int l, int r) {
//     for (int i = l; i < r && i < s.length(); i++) {
//         if (s[i] == s[r]) {
//             return i;
//         }
//     }
//     return -1;
// }

// int main() {
//     std::cout << "请输入一个字符串，按回车键结束" << std::endl;
//     std::string s;
//     std::cin >> s;
    
//     int maxNoDupCount = 0; //当前最长不重复子串长度
//     int l = 0, r = 0;
//     while (l < s.length() && r < s.length()) {
//         int pos = findCharPosition(s, l, r);
//         if (pos < 0) { //不重复
//             if (maxNoDupCount < r - l) {
//                 maxNoDupCount = r - l;
//                 std::cout << "当前最长不重复子串：" << s.substr(l, r - l) << std::endl;
//             }
//             r++;
//         } else {
//             l = pos + 1;
//         }
//     }

//     return 0;
// }

int findPosition(int* a, int n, int l, int r) {
    for (int i = l; i < r && i < n; i++) {
        if (a[i] == a[r]) {
            return i;
        }
    }
    return -1;
}

int main(){
    // int n;
    // std::cin >> n;
    // if (n <= 0) {
    //     return -1;
    // }

    // int *a = new int[n];
    // for(int i = 0; i < n; i++)
    //     std::cin >> a[i];

    // int maxNoDupCount = 0; //当前最长不重复子串长度
    // int l = 0, r = 0;
    // while (l < n && r < n) {
    //     int pos = findPosition(a, n, l, r);
    //     if (pos < 0) {
    //         r++;
    //         if (maxNoDupCount < r - l) {
    //             maxNoDupCount = r - l;
    //         }
    //     } else {
    //         l = pos + 1;
    //         r++;
    //     }
    // }

    // std::cout << maxNoDupCount << std::endl;
    // delete[] a;
    // return 0;

    int n;
    std::cin >> n;
    if (n <= 0) {
        return -1;
    }

    int *a = new int[n];
    for(int i = 0; i < n; i++)
        std::cin >> a[i];

    int maxNoDupCount = 0; //当前最长不重复子串长度
    std::map<int, int> mp; //值-index
    int l = 0, r = 0;
    while (l < n && r < n) {
        auto iter = mp.find(a[r]);
        if (iter == mp.end()) {
            mp[a[r]] = r;
            r++;
            if (maxNoDupCount < r - l) {
                maxNoDupCount = r - l;
            }
        } else {
            l = iter->second + 1;
            r++;
            mp.clear();
            for (int i = l; i < r; i++) {
                mp[a[i]] = i;
            }
        }
    }

    std::cout << maxNoDupCount << std::endl;
    delete[] a;
    return 0;
}

