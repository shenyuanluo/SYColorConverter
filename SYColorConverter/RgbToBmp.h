//
//  RgbToBmp.h
//  SYColorConverterExample
//
//  Created by shenyuanluo on 2018/7/19.
//  Copyright © 2018年 http://blog.shenyuanluo.com/ All rights reserved.
//

#ifndef RgbToBmp_h
#define RgbToBmp_h

#include <stdio.h>
#include "Converter.h"


class RgbToBmp
{
private:
    unsigned int m_bmpHeaderLen;    // BMP 头长度
    unsigned int m_bmpFileInfoLen;  // BMP 文件信息长度
    
    EndianType   m_eType;           // 大小端类型
    
    /**
     检查大小端
     
     @return 大小端类型
     */
    EndianType CheckEndian();
    
public:
    RgbToBmp();
    
    ~RgbToBmp();
    
    
    /**
     RGB565 转 BMP
     
     @param inRgb RGB565 数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outBmpPath BMP 文件路径（输出）
     @return 转换是否成功
     */
    bool Rgb565ToBmp(unsigned char *inRgb, unsigned int width, unsigned int height, const char *outBmpPath) const;
    
    /**
     RGB24 转 BMP
     
     @param inRgb RGB24 数据（输入）
     @param width 帧-宽度
     @param height 帧-高度
     @param outBmpPath BMP 文件路径（输出）
     @return 转换是否成功
     */
    bool Rgb24ToBmp(unsigned char *inRgb, unsigned int width, unsigned int height, const char *outBmpPath) const;
};

#endif /* RgbToBmp_h */
