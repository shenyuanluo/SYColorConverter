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
#include "Converter.h"

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


class YuvToRgb
{
private:
    MatrixType  m_mType;    // 转换矩阵类型
    ConvertType m_cType;    // 转换方法类型
    EndianType  m_eType;    // 大小端类型
    
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
    void yuv2rgb(int y, int u, int v, int* r, int* g, int* b) const;
    
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
    
    
    /* RGB565 内存布局（注意大小端）
                   R                     G                     B
         +---------------------------------------------------------------+
   高字节 | R | R | R | R | R | G | G | G | G | G | G | B | B | B | B | B |  低字节
         +---------------------------------------------------------------+
           0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
     */
    
    /**
     I420 转 RGB565

     @param inYuv I420数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outRgb RGB565数据（输出）
     @return 转换是否成功
     */
    bool I420ToRgb565(unsigned char* inYuv, unsigned int width, unsigned int height, unsigned char* outRgb) const;
    
    /**
     NV12 转 RGB565
     
     @param inYuv NV12数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outRgb RGB565数据（输出）
     @return 转换是否成功
     */
    bool Nv12ToRgb565(unsigned char* inYuv, unsigned int width, unsigned int height, unsigned char* outRgb) const;
    
    /**
     NV21 转 RGB565
     
     @param inYuv NV21数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outRgb RGB565数据（输出）
     @return 转换是否成功
     */
    bool Nv21ToRgb565(unsigned char* inYuv, unsigned int width, unsigned int height, unsigned char* outRgb) const;
    
    
    /* RGB24 内存布局（注意大小端）
                      B                               G                               R
          +-----------------------------------------------------------------------------------------------+
    高字节 | B | B | B | B | B | B | B | B | G | G | G | G | G | G | G | G | R | R | R | R | R | R | R | R |   低字节
          +-----------------------------------------------------------------------------------------------+
            0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
     */
    
    /**
     I420 转 RGB24

     @param inYuv I420数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outRgb RGB24数据（输出）
     @return 转换是否成功
     */
    bool I420ToRgb24(unsigned char* inYuv, unsigned int width, unsigned int height, unsigned char* outRgb) const;
    
    /**
     NV12 转 RGB24
     
     @param inYuv NV12数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outRgb RGB24数据（输出）
     @return 转换是否成功
     */
    bool Nv12ToRgb24(unsigned char* inYuv, unsigned int width, unsigned int height, unsigned char* outRgb) const;
    
    /**
     NV21 转 RGB24
     
     @param inYuv NV21数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outRgb RGB24数据（输出）
     @return 转换是否成功
     */
    bool Nv21ToRgb24(unsigned char* inYuv, unsigned int width, unsigned int height, unsigned char* outRgb) const;
    
};

#endif /* YuvToRgb_hpp */
