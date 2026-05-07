#include <stc15w408as.H>
#include <INTRINS.H> //标准变量库

//标准的循环延迟
void delay(unsigned int xms) {
    unsigned char i, j;
    while (xms--) {
        // 以下代码段可实现约 1ms 的延时 (@12.000MHz)
        i = 12;          // 对应 12MHz 的参数 1
        j = 169;         // 对应 12MHz 的参数 2
        do {
            while (--j);
        } while (--i);
    }
}