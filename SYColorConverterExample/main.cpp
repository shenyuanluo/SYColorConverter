//
//  main.cpp
//  SYColorConverterExample
//
//  Created by shenyuanluo on 2018/7/15.
//  Copyright © 2018年 http://blog.shenyuanluo.com/ All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include "YuvToRgb.h"
#include "RgbToYuv.h"


#define YUV_WIDTH  480              // 视频帧宽
#define YUV_HEIGHT 360              // 视频帧高
#define I420_BUFF_SIZE  259200      // I420 缓冲大小（YUV_WIDTH * YUV_HEIGHT * 1.5）
#define RGB_565_BUFF_SIZE 345600    // RGB565缓冲大小（YUV_WIDTH * YUV_HEIGHT * 2）
#define RGB_24_BUFF_SIZE  518400    // RGB24 缓冲大小（YUV_WIDTH * YUV_HEIGHT * 3）


#define LOOP_COUNT 1   // 循环次数


#pragma mark -- 获取当前时间
long long currTime()
{
    struct timeval tv;
    gettimeofday(&tv,NULL);
    
//    return tv.tv_sec;   // 秒
//    return tv.tv_sec * 1000 + tv.tv_usec/1000;    // 毫秒
    return tv.tv_sec * 1000000 + tv.tv_usec;    // 微妙
}


#pragma mark -- 测试 I420 转 RGB565
void textI420ToRgb565()
{
    YuvToRgb converter;
    converter.setMatrixType(matrix_normal);
    converter.setConvertType(convert_normal);
    
    unsigned char *yuv    = (unsigned char *)malloc(I420_BUFF_SIZE);
    unsigned char *rgb565 = (unsigned char *)malloc(RGB_565_BUFF_SIZE);
    if (NULL == yuv || NULL == rgb565)
    {
        printf("Malloc data buffer failure!\n");
        return;
    }
    FILE *fyuv    = fopen("XinWenLianBo_480x360_I420.yuv", "rb+");   // 打开 YUV 文件
    FILE *frgb565 = fopen("XinWenLianBo_480x360_RGB565.rgb", "wb+"); // 打开 RGB565 文件
    
    if (NULL == fyuv  || NULL == frgb565)
    {
        printf("Open file failure!\n");
        free(yuv);
        free(rgb565);
        yuv    = NULL;
        rgb565 = NULL;
        
        return;
    }
    long long sTime     = 0;
    long long eTime     = 0;
    long totalTime      = 0;
    long long startTime = 0;
    long long endTime   = 0;
    long interval       = 0;
    
    sTime = currTime();
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        startTime = currTime();
        while (!feof(fyuv))
        {
            // 清空内存
            memset(yuv, 0, I420_BUFF_SIZE);
            memset(rgb565, 0, RGB_565_BUFF_SIZE);
            
            fread(yuv, 1, I420_BUFF_SIZE, fyuv);  // 每次读取一帧 YUV 数据
            
            converter.I420ToRgb565(yuv, YUV_WIDTH, YUV_HEIGHT, rgb565);     // 转换成 RGB565
            
            fwrite(rgb565, RGB_565_BUFF_SIZE, 1, frgb565);
        }
        endTime = currTime();
        
        fseek(fyuv, 0, SEEK_SET);
        fseek(frgb565, 0, SEEK_SET);
        
        interval = endTime - startTime;
        fflush(stdout);
        printf("%ld\n", interval);
    }
    eTime = currTime();
    totalTime = eTime - sTime;
    printf("Total time : %ld\n", totalTime);
    
    fclose(fyuv);
    fclose(frgb565);
    free(yuv);
    free(rgb565);
    yuv    = NULL;
    rgb565 = NULL;
}

#pragma mark -- 测试 I420 转 RGB24
void textI420ToRgb24()
{
    YuvToRgb converter;
    converter.setMatrixType(matrix_normal);
    converter.setConvertType(convert_normal);
    
    unsigned char *yuv   = (unsigned char *)malloc(I420_BUFF_SIZE);
    unsigned char *rgb24 = (unsigned char *)malloc(RGB_24_BUFF_SIZE);
    if (NULL == yuv || NULL == rgb24)
    {
        printf("Malloc data buffer failure!\n");
        return;
    }
    FILE *fyuv   = fopen("XinWenLianBo_480x360_I420.yuv", "rb+");   // 打开 YUV 文件
    FILE *frgb24 = fopen("XinWenLianBo_480x360_RGB24.rgb", "wb+");  // 打开 RGB24 文件
    
    if (NULL == fyuv || NULL == frgb24)
    {
        printf("Open file failure!\n");
        free(yuv);
        free(rgb24);
        yuv   = NULL;
        rgb24 = NULL;
        
        return;
    }
    long long sTime     = 0;
    long long eTime     = 0;
    long totalTime      = 0;
    long long startTime = 0;
    long long endTime   = 0;
    long interval       = 0;
    
    sTime = currTime();
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        startTime = currTime();
        while (!feof(fyuv))
        {
            // 清空内存
            memset(yuv, 0, I420_BUFF_SIZE);
            memset(rgb24, 0, RGB_24_BUFF_SIZE);
            
            fread(yuv, 1, I420_BUFF_SIZE, fyuv);  // 每次读取一帧 YUV 数据
            
            converter.I420ToRgb24(yuv, YUV_WIDTH, YUV_HEIGHT, rgb24);    // 转换成 RGB24
            
            fwrite(rgb24, RGB_24_BUFF_SIZE, 1, frgb24);
        }
        endTime = currTime();
        
        fseek(fyuv, 0, SEEK_SET);
        fseek(frgb24, 0, SEEK_SET);
        
        interval = endTime - startTime;
        fflush(stdout);
        printf("%ld\n", interval);
    }
    eTime = currTime();
    totalTime = eTime - sTime;
    printf("Total time : %ld\n", totalTime);
    
    fclose(fyuv);
    fclose(frgb24);
    free(yuv);
    free(rgb24);
    yuv   = NULL;
    rgb24 = NULL;
}

#pragma mark -- 测试 RGB24 转 I420
void textRgb24ToI420()
{
    RgbToYuv converter;
    converter.setMatrixType(matrix_normal);
    converter.setConvertType(convert_normal);
    
    unsigned char *rgb24 = (unsigned char *)malloc(RGB_24_BUFF_SIZE);
    unsigned char *yuv   = (unsigned char *)malloc(I420_BUFF_SIZE);
    if (NULL == rgb24 || NULL == yuv)
    {
        printf("Malloc data buffer failure!\n");
        return;
    }
    FILE *frgb24 = fopen("XinWenLianBo_480x360_RGB24.rgb", "rb+");  // 打开 RGB24 文件
    FILE *fyuv   = fopen("XinWenLianBo_480x360_I420.yuv", "wb+");   // 打开 YUV 文件
    
    if (NULL == frgb24 || NULL == fyuv)
    {
        printf("Open file failure!\n");
        free(yuv);
        free(rgb24);
        yuv   = NULL;
        rgb24 = NULL;
        
        return;
    }
    long long sTime     = 0;
    long long eTime     = 0;
    long totalTime      = 0;
    long long startTime = 0;
    long long endTime   = 0;
    long interval       = 0;
    
    sTime = currTime();
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        startTime = currTime();
        while (!feof(frgb24))
        {
            // 清空内存
            memset(rgb24, 0, RGB_24_BUFF_SIZE);
            memset(yuv, 0, I420_BUFF_SIZE);
            
            fread(rgb24, RGB_24_BUFF_SIZE, 1, frgb24);  // 每次读取一帧 RGB24 数据
            
            converter.Rgb24ToI420(rgb24, YUV_WIDTH, YUV_HEIGHT, yuv);   // 转换成 I420
            
            fwrite(yuv, I420_BUFF_SIZE, 1, fyuv);
        }
        endTime = currTime();
        
        fseek(fyuv, 0, SEEK_SET);
        fseek(frgb24, 0, SEEK_SET);
        
        interval = endTime - startTime;
        fflush(stdout);
        printf("%ld\n", interval);
    }
    eTime = currTime();
    totalTime = eTime - sTime;
    printf("Total time : %ld\n", totalTime);
    
    fclose(fyuv);
    fclose(frgb24);
    free(yuv);
    free(rgb24);
    yuv   = NULL;
    rgb24 = NULL;
}

#pragma mark -- 测试 RGB565 转 I420
void textRgb565ToI420()
{
    RgbToYuv converter;
    converter.setMatrixType(matrix_normal);
    converter.setConvertType(convert_table);
    
    unsigned char *rgb565 = (unsigned char *)malloc(RGB_565_BUFF_SIZE);
    unsigned char *yuv    = (unsigned char *)malloc(I420_BUFF_SIZE);
    if (NULL == rgb565 || NULL == yuv)
    {
        printf("Malloc data buffer failure!\n");
        return;
    }
    FILE *frgb565 = fopen("XinWenLianBo_480x360_RGB565.rgb", "rb+");  // 打开 RGB565 文件
    FILE *fyuv    = fopen("XinWenLianBo_480x360_I420.yuv", "wb+");    // 打开 YUV 文件
    
    if (NULL == frgb565 || NULL == fyuv)
    {
        printf("Open file failure!\n");
        free(yuv);
        free(rgb565);
        yuv    = NULL;
        rgb565 = NULL;
        
        return;
    }
    long long sTime     = 0;
    long long eTime     = 0;
    long totalTime      = 0;
    long long startTime = 0;
    long long endTime   = 0;
    long interval       = 0;
    
    sTime = currTime();
    for (int i = 0; i < LOOP_COUNT; i++)
    {
        startTime = currTime();
        while (!feof(frgb565))
        {
            // 清空内存
            memset(rgb565, 0, RGB_565_BUFF_SIZE);
            memset(yuv, 0, I420_BUFF_SIZE);
            
            fread(rgb565, RGB_565_BUFF_SIZE, 1, frgb565);  // 每次读取一帧 RGB565 数据
            
            converter.Rgb565ToI420(rgb565, YUV_WIDTH, YUV_HEIGHT, yuv);   // 转换成 I420
            
            fwrite(yuv, I420_BUFF_SIZE, 1, fyuv);
        }
        endTime = currTime();
        
        fseek(fyuv, 0, SEEK_SET);
        fseek(frgb565, 0, SEEK_SET);
        
        interval = endTime - startTime;
        fflush(stdout);
        printf("%ld\n", interval);
    }
    eTime = currTime();
    totalTime = eTime - sTime;
    printf("Total time : %ld\n", totalTime);
    
    fclose(fyuv);
    fclose(frgb565);
    free(yuv);
    free(rgb565);
    yuv    = NULL;
    rgb565 = NULL;
}


int main(int argc, const char * argv[])
{
//    textI420ToRgb565();
    
//    textI420ToRgb24();
    
//    textRgb24ToI420();
    
    textRgb565ToI420();
    
    return 0;
}
