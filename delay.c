#include <REGX52.H> //c52库
#include <INTRINS.H> //标准变量库

//标准的循环延迟
void delay(unsigned int xms) {
    unsigned char i, j;
    while (xms--) {
        i = 1;          // 中层循环次数（精确计算）
        j = 248;        // 内层循环次数
        do {
            while (--j); // 内层：248次循环，约996μs
        } while (--i);   // 中层+内层总约1000μs（1ms）
    }
}