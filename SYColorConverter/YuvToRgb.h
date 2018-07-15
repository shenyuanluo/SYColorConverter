//
//  YuvToRgb.h
//  SYColorConverterExample
//
//  Created by shenyuanluo on 2018/7/15.
//  Copyright © 2018年 http://blog.shenyuanluo.com/ All rights reserved.
//

#ifndef YuvToRgb_h
#define YuvToRgb_h

#include <stdio.h>

/* ========== YUV --> RGB 的变换矩阵 ==========
 1、常规转换标准
 __ __     __                         __ __ __
 | R |     | 1.0,  0.0,       1.370705 | | Y'|
 | G |  =  | 1.0, -0.337633, -0.698001 | | U |
 | B |     | 1.0,  1.732446,  0.0      | | V |
 -- --     --                         -- -- --
 
 2、标清电视标准：BT.601
 __ __     __                       __ __ __
 | R |     | 1.0,  0.0,      1.13983 | | Y'|
 | G |  =  | 1.0, -0.39465, -0.5806  | | U |
 | B |     | 1.0,  2.03211,  0.0     | | V |
 -- --     --                       -- -- --
 
 3、高清电视标准：BT.709
 __ __     __                       __ __ __
 | R |     | 1.0,  0.0,      1.28033 | | Y'|
 | G |  =  | 1.0, -0.21482, -0.38059 | | U |
 | B |     | 1.0,  2.12798,  0.0     | | V |
 -- --     --                       -- -- --
 
 */

/* 转换矩阵类型 */
typedef enum __matrixType {
    matrix_normal           = 0,    // 常规转换标准
    matrix_bt_601           = 1,    // 标清电视标准：BT.601
    matrix_bt_709           = 2,    // 高清电视标准：BT.709
}MatrixType;

/* 转换方法类型 */
typedef enum __convertType {
    convert_normal          = 0,    // 常规方法：浮点运算，精度高
    convert_bitMult         = 1,    // 通过位移来避免浮点运算，精度低
    convert_bitAdd          = 2,    // 通过位移来避免乘法运算，精度低
    convert_table           = 3,    // 查表法（也是位移计算），精度低
}ConvertType;

/* 大小端类型 */
typedef enum __endianType {
    endian_little           = 0,    // 小端
    endian_big              = 1,    // 大端
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


class YuvToRgb
{
private:
    MatrixType  m_mType;    // 转换矩阵类型
    ConvertType m_cType;    // 转换方法类型
    
    int m_rv[256];  // 计算 R 值使用的 V 表
    int m_gu[256];  // 计算 G 值使用的 U 表
    int m_gv[256];  // 计算 G 值使用的 V 表
    int m_bu[256];  // 计算 B 值使用的 U 表
    
    /**
     检查大小端
     
     @return 大小端类型
     */
    EndianType CheckEndian();
    
    /**
     初始化 YUV 颜色表
     */
    void initYuvTable();
    
    /**
     将单个像素 YUV 转成 RGB

     @param y 像素点的 Y 值（取值范围：0~255）
     @param u 像素点的 U(Cb) 值（取值范围：-128~127）
     @param v 像素点的 V(Cr) 值（取值范围：-128~127）
     @param r 像素点的 R 值（取值范围：0~255）
     @param g 像素点的 G 值（取值范围：0~255）
     @param b 像素点的 B 值（取值范围：0~255）
     */
    void yuv2rgb(int y, int u, int v, int *r, int *g, int *b) const;
    
public:
    YuvToRgb();
    
    ~YuvToRgb();
    
    /**
     设置转换方法类型

     @param cType 转换方法类型
     */
    void setConvertType(ConvertType cType);
    
    /**
     设置转换矩阵类型

     @param mType 转换矩阵类型
     */
    void setMatrixType(MatrixType mType);
    
    /**
     I420 转 RGB565

     @param inYuv yuv-I420数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outRgb RGB565数据（输出）
     @return 转换是否成功
     */
    bool I420ToRgb565(unsigned char *inYuv, unsigned int width, unsigned int height, unsigned char *outRgb);
    
    /**
     yuv-I420 转 RGB24

     @param inYuv yuv-I420数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outRgb RGB24数据（输出）
     @return 转换是否成功
     */
    bool I420ToRgb24(unsigned char *inYuv, unsigned int width, unsigned int height, unsigned char *outRgb);
};

#endif /* YuvToRgb_hpp */
