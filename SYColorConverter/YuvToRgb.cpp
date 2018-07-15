//
//  YuvToRgb.cpp
//  SYColorConverterExample
//
//  Created by shenyuanluo on 2018/7/15.
//  Copyright © 2018年 http://blog.shenyuanluo.com/ All rights reserved.
//

#include "YuvToRgb.h"

// 夹紧函数定义：限制在两者范围之间
#define CLAMP(x, low, high)  (((x) > (high)) ? (high) : (((x) < (low)) ? (low) : (x)))


#pragma mark - Public
YuvToRgb::YuvToRgb()
{
    m_mType = matrix_normal;
    m_cType = convert_normal;
    
    initYuvTable();
}

YuvToRgb::~YuvToRgb()
{
    
}

#pragma mark -- 设置转方法阵类型
void YuvToRgb::setConvertType(ConvertType cType)
{
    m_cType = cType;
}

#pragma mark -- 设置转换矩阵类型
void YuvToRgb::setMatrixType(MatrixType mType)
{
    m_mType = mType;
}

#pragma mark -- I420 转 RGB565
bool YuvToRgb::I420ToRgb565(unsigned char *inYuv, unsigned int width, unsigned int height, unsigned char *outRgb)
{
    if (NULL == inYuv || NULL == outRgb || 0 == width || 0 == height)
    {
        return false;
    }
    const long len = width * height;
    unsigned char *yData, *uData, *vData;   // 分量 Y、U、V 数据
    int y, u, v;      // 当前像素点 Y、U、V 值
    int r, g, b;      // 当前像素点 R、G、B 值
    unsigned char colorLow;     // 转换后 rgb565 颜色值-低字节
    unsigned char colorHigh;    // 转换后 rgb565 颜色值-高字节
    yData = inYuv;              // 获取 Y 数据地址
    uData = yData + len;        // 获取 U 数据地址
    vData = uData + (len>>2);   // 获取 V 数据地址
    
    int idx = 0;    // 转换后 rgb565 内存地址下标
    // 遍历所有像素点
    for (int row = 0; row < height; row++)  // 遍历所有行
    {
        for (int col = 0; col < width; col++)   // 遍历所有列
        {
            /*
             提取每个像素点 Y'CbCr 值
             由于 U(Cb)、V(Cr) 取值范围是 [﹣128, 127]；
             而在存储时，为了方便存储，跟 Y 数据一样，统一用一个(无符号)字节表示，即取值范围是 [0, 255];
             所以在读取时，需要 将 U(Cb)、V(Cr)的值减去 128，使其变为 ：[﹣128, 127]。
             */
            
            // 由于查表法在表格创建时，已经减去了 128，所以在此处不用再减
            if (convert_table == m_cType)
            {
                y = (int)yData[row * width + col];
                u = (int)uData[(row/2) * (width/2) + (col/2)];
                v = (int)vData[(row/2) * (width/2) + (col/2)];
            }
            else
            {
                y = (int)yData[row * width + col];
                u = (int)(uData[(row/2) * (width/2) + (col/2)] - 128);
                v = (int)(vData[(row/2) * (width/2) + (col/2)] - 128);
            }
            
            yuv2rgb(y, u, v, &r, &g, &b);   // 计算对应 R、G、B 值
            
            /* RGB565 内存布局（注意大小端）
                           R                     G                     B
                 +---------------------------------------------------------------+
           高字节 | R | R | R | R | R | G | G | G | G | G | G | B | B | B | B | B |  低字节
                 +---------------------------------------------------------------+
                   0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
             */
            
            // 重置
            colorLow  = 0;
            colorHigh = 0;
            
            // 填充高字节：R(5bit) + G(3bit)
            colorHigh = r & 0xF8;
            colorHigh |= (g & 0xE0)>>5;
            
            // 填充低字节：G(3bit) + B(5bit)
            colorLow = (g & 0x1C)<<3;
            colorLow |= (b & 0xF8)>>3;
            
            switch (CheckEndian())
            {
                case endian_little: // 小端
                {
                    outRgb[idx++] = colorLow;
                    outRgb[idx++] = colorHigh;

                }
                    break;
                    
                case endian_big:    // 大端
                {
                    outRgb[idx++] = colorHigh;
                    outRgb[idx++] = colorLow;
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
    return true;
}

#pragma mark -- I420 转 RGB24
bool YuvToRgb::I420ToRgb24(unsigned char *inYuv, unsigned int width, unsigned int height, unsigned char *outRgb)
{
    if (inYuv == NULL || outRgb == NULL || 0 == width || 0 == height)
    {
        return false;
    }
    const long len = width * height;
    unsigned char *yData, *uData, *vData;
    int y, u, v;      // 当前像素点 Y、U、V 值
    int r, g, b;      // 当前像素点 R、G、B 值
    yData = inYuv;              // 获取 Y 数据地址
    uData = yData + len;        // 获取 U 数据地址
    vData = uData + (len>>2);   // 获取 V 数据地址
    
    int idx = 0;
    for (int row = 0; row < height; row++)  // 遍历所有行
    {
        for (int col = 0; col < width; col++)   // 遍历所有列
        {
            /*
             提取每个像素点 Y'CbCr 值
             由于 U(Cb)、V(Cr) 取值范围是 [﹣128, 127]；
             而在存储时，为了方便存储，跟 Y 数据一样，统一用一个(无符号)字节表示，即取值范围是 [0, 255];
             所以在读取时，需要 将 U(Cb)、V(Cr)的值减去 128，使其变为 ：[﹣128, 127]。
             */
            
            // 由于查表法在表格创建时，已经减去了 128，所以在此处不用再减
            if (convert_table == m_cType)
            {
                y = (int)yData[row * width + col];
                u = (int)uData[(row/2) * (width/2) + (col/2)];
                v = (int)vData[(row/2) * (width/2) + (col/2)];
            }
            else
            {
                y = (int)yData[row * width + col];
                u = (int)(uData[(row/2) * (width/2) + (col/2)] - 128);
                v = (int)(vData[(row/2) * (width/2) + (col/2)] - 128);
            }
            
            yuv2rgb(y, u, v, &r, &g, &b);
            
            /* RGB24 内存布局（注意大小端）
                                   B                               G                               R
                   +-----------------------------------------------------------------------------------------------+
             高字节 | B | B | B | B | B | B | B | B | G | G | G | G | G | G | G | G | R | R | R | R | R | R | R | R |   低字节
                   +-----------------------------------------------------------------------------------------------+
                     0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15  16  17  18  19  20  21  22  23
             */
            
            switch (CheckEndian())
            {
                case endian_little: // 小端
                {
                    outRgb[idx++] = (char)r;
                    outRgb[idx++] = (char)g;
                    outRgb[idx++] = (char)b;
                    
                }
                    break;
                    
                case endian_big:    // 大端
                {
                    outRgb[idx++] = (char)b;
                    outRgb[idx++] = (char)g;
                    outRgb[idx++] = (char)r;
                }
                    break;
                    
                default:
                    break;
            }
        }
    }
    return true;
}


#pragma mark - Private
#pragma mark -- 检查大小端
EndianType YuvToRgb::CheckEndian()
{
    union {
        char c[4];
        unsigned int num;
    } endianUnion = {'l', '?', '?', 'b'};
    
    if ('l' == (char)endianUnion.num)   // 取首字节判断
    {
        return endian_little;
    }
    else // 'b' == (char)endianUnion.num
    {
        return endian_big;
    }
}

#pragma mark -- 初始化 YUV 颜色表
void YuvToRgb::initYuvTable()
{
    int rv = 0;     // 计算 R 值 V 系数
    int gu = 0;     // 计算 G 值 U 系数
    int gv = 0;     // 计算 G 值 V 系数
    int bu = 0;     // 计算 B 值 U 系数
    switch (m_mType)
    {
        case matrix_normal:  // 常规标准
        {
            rv = 351;
            gu = 86;
            gv = 179;
            bu = 444;
        }
            break;
            
        case matrix_bt_601:  // BT.601 标准
        {
            rv = 292;
            gu = 101;
            gv = 149;
            bu = 520;
        }
            break;
            
        case matrix_bt_709:  // BT.709 标准
        {
            rv = 328;
            gu = 55;
            gv = 97;
            bu = 545;
        }
            break;
            
        default:
            break;
    }
    
    for (int i = 0; i < 256; i++)
    {
        m_rv[i] = ((i - 128) * rv)>>8;
        m_gu[i] = ((i - 128) * gu)>>8;
        m_gv[i] = ((i - 128) * gv)>>8;
        m_bu[i] = ((i - 128) * bu)>>8;
    }
}

#pragma mark -- 将单个像素 YUV 转成 RGB
void YuvToRgb::yuv2rgb(int y, int u, int v, int *r, int *g, int *b) const
{
    switch (m_mType)
    {
        case matrix_normal:
        {
            switch (m_cType)
            {
                case convert_normal: // 常规方法：浮点运算，精度高
                {
                    *r = y                  + (1.370705 * v);
                    *g = y - (0.337633 * u) - (0.698001 * v);
                    *b = y + (1.732446 * u);
                }
                    break;
                    
                case convert_bitMult:  // 通过位移来避免浮点运算，精度低
                {
                    *r = ((256 * y             + (351 * v))>>8);
                    *g = ((256 * y - (86  * u) - (179 * v))>>8);
                    *b = ((256 * y + (444 * u))            >>8);
                }
                    break;
                    
                case convert_bitAdd:   // 通过位移来避免乘法运算，精度低
                {
                    *r = (((y<<8) + (v<<8) + (v<<6) + (v<<4) + (v<<3) + (v<<2) + (v<<1) + v)                   >> 8);
                    *g = (((y<<8) - (u<<6) - (u<<4) - (u<<2) - (u<<1) - (v<<7) - (v<<5) - (v<<4) - (v<<1) - v) >> 8);
                    *b = (((y<<8) + (u<<8) + (u<<7) + (u<<5) + (u<<4) + (u<<3) + (u<<2))                       >> 8);
                }
                    break;
                    
                case convert_table: // 查表法（也是位移计算），精度低
                {
                    *r = y + m_rv[v];
                    *g = y - m_gu[u] - m_gv[v];
                    *b = y + m_bu[u];
                }
                    break;
                    
                default:
                    break;
            }
        }
            break;
            
        case matrix_bt_601:
        {
            switch (m_cType)
            {
                case convert_normal: // 常规方法：浮点运算，精度高
                {
                    *r = (y               + 1.13983 * v);
                    *g = (y - 0.39465 * u - 0.5806  * v);
                    *b = (y + 2.03211 * u);
                }
                    break;
                    
                case convert_bitMult:  // 通过位移来避免浮点运算，精度低
                {
                    *r = ((256 * y           + 292 * v)>>8);
                    *g = ((256 * y - 101 * u - 149 * v)>>8);
                    *b = ((256 * y + 520 * u)          >>8);
                }
                    break;
                    
                case convert_bitAdd:   // 通过位移来避免乘法运算，精度低
                {
                    *r = (((y<<8) + (v<<8) + (v<<5) + (v<<2))                                   >>8);
                    *g = (((y<<8) + (u<<6) + (u<<5) + (u<<2) + u + (v<<7) + (v<<4) + (v<<2) + v)>>8);
                    *b = (((y<<8) + (u<<9) + (u<<3))                                            >>8);
                }
                    break;
                    
                case convert_table: // 查表法（也是位移计算），精度低
                {
                    *r = y + m_rv[v];
                    *g = y - m_gu[u] - m_gv[v];
                    *b = y + m_bu[u];
                }
                    break;
                    
                default:
                    break;
            }
        }
            break;
            
        case matrix_bt_709:
        {
            switch (m_cType)
            {
                case convert_normal: // 常规方法：浮点运算，精度高
                {
                    *r = (y               + 1.28033 * v);
                    *g = (y - 0.21482 * u - 0.38059 * v);
                    *b = (y + 2.12798 * u);
                }
                    break;
                    
                case convert_bitMult:  // 通过位移来避免浮点运算，精度低
                {
                    *r = ((256 * y           + 328 * v)>>8);
                    *g = ((256 * y - 55  * u - 97  * v)>>8);
                    *b = ((256 * y + 545 * u)          >>8);
                }
                    break;
                    
                case convert_bitAdd:   // 通过位移来避免乘法运算，精度低
                {
                    *r = (((y<<8) + (v<<8) + (v<<6) + (v<<3))                                   >>8);
                    *g = (((y<<8) + (u<<5) + (u<<4) + (u<<2) + (u<<1) + u + (v<<6) + (v<<5) + v)>>8);
                    *b = (((y<<8) + (u<<9) + (u<<5) +u)                                         >>8);
                }
                    break;
                    
                case convert_table: // 查表法（也是位移计算），精度低
                {
                    *r = y + m_rv[v];
                    *g = y - m_gu[u] - m_gv[v];
                    *b = y + m_bu[u];
                }
                    break;
                    
                default:
                    break;
            }
        }
            break;
            
        default:
            break;
    }
    *r = CLAMP(*r, 0, 255);
    *g = CLAMP(*g, 0, 255);
    *b = CLAMP(*b, 0, 255);
}
