//
//  Converter.h
//  SYColorConverterExample
//
//  Created by shenyuanluo on 2018/7/16.
//  Copyright © 2018年 http://blog.shenyuanluo.com/ All rights reserved.
//

#ifndef Converter_h
#define Converter_h

// 夹紧函数定义：限制在两者范围之间
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))

/* 转换矩阵类型 */
typedef enum __matrixType {
    Matrix_normal           = 0,    // 常规转换标准
    Matrix_bt_601           = 1,    // 标清电视标准：BT.601
    Matrix_bt_709           = 2,    // 高清电视标准：BT.709
}MatrixType;

/* 转换方法类型 */
typedef enum __convertType {
    Convert_normal          = 0,    // 常规方法：浮点运算，精度高
    Convert_bitMult         = 1,    // 通过位移来避免浮点运算，精度低
    Convert_bitAdd          = 2,    // 通过位移来避免乘法运算，精度低
    Convert_table           = 3,    // 查表法（也是位移计算），精度低
}ConvertType;

/* 大小端类型 */
typedef enum __endianType {
    Endian_little           = 0,    // 小端
    Endian_big              = 1,    // 大端
}EndianType;

/* RGB 颜色结构体 */
typedef struct __rgb {
    unsigned char R;    // R 分量
    unsigned char G;    // G 分量
    unsigned char B;    // B 分量
}RGB;

/* YUV 颜色结构体 */
typedef struct __yuv {
    unsigned char Y;    // Y 分量
    unsigned char U;    // U 分量
    unsigned char V;    // V 分量
}YUV;

#endif /* Converter_h */
