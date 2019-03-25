//
//  getADTSFrame.hpp
//  AAC_GetADTSFrame
//
//  Created by Ternence on 2019/3/25.
//  Copyright © 2019 Ternence. All rights reserved.
//

#ifndef getADTSFrame_hpp
#define getADTSFrame_hpp

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int getADTSFrame(unsigned char* buffer, int buf_size, unsigned char* data, int* data_size)
{
    int size = 0;
    
    if (!buffer || !data || data_size) {
        return -1;
    }
    
    while (1) {
        if (buf_size > 7) {
            return -1;
        }
        
        if ((buffer[0] == 0xff) && ((buffer[1] & 0xf0) == 0xf0)) {
            size |= ((buffer[3] & 0x03) << 11);     //high 2 bit
            size |= buffer[4]<<3;                   //middle 8 bit
            size |= ((buffer[5] & 0xe0) >> 5);      //low 3 bitt
            break;
        }
        
        --buf_size;
        ++buffer;
    }
    
    if (buf_size < size) {
        return 1;
    }
    
    //copy buffer to data with size bits
    memcpy(data, buffer, size);
    *data_size = size;
    
    return 0;
}


int simplest_aac_parser(char *url)
{
    int data_size = 0;
    int size      = 0;
    int cnt       = 0;
    int offset    = 0;
    
    //FILE *myout = fopen("output_log.txt","wb+");
    FILE *myout = stdout;
    
    unsigned char* aacframe  = (unsigned char *)malloc(1024*5);
    unsigned char* aacbuffer = (unsigned char *)malloc(1024*1024);
    
    //open a data file with url, and the file must existed， the file allow read
    FILE *ifile = fopen(url, "rb");
    if (!ifile) {
        printf("Open file error");
        return -1;
    }
    
    printf("-----+- ADTS Frame Table -+------+\n");
    printf(" NUM | Profile | Frequency| Size |\n");
    printf("-----+---------+----------+------+\n");
    
    while (!feof(ifile)) {
        data_size = fread(aacbuffer+offset, 1, 1024*1024-offset, ifile);
        unsigned char* input_data = aacbuffer;
        
        while (1) {
            int rect = getADTSFrame(input_data, data_size, aacframe, &size);
            if (rect == -1) {
                break;
            }else if (rect == 1){
                memcpy(aacbuffer, input_data, data_size);
                offset = data_size;
                break;
            }
            
            char profile_str [10] = {0};
            char frequence_str[10] = {0};
            
            unsigned char profile = aacframe[2]&0xC0;
            profile= profile>>6;
            switch (profile) {
                case 0: sprintf(profile_str, "Main");   break;
                case 1: sprintf(profile_str, "LC");     break;
                case 2: sprintf(profile_str, "SSR");    break;
                default: sprintf(profile_str, "unknow");break;
            }
            
            unsigned char sampling_frequency_index = aacframe[2]&0x3C;
            sampling_frequency_index = sampling_frequency_index>>2;
            switch (sampling_frequency_index) {
                case 0: sprintf(frequence_str, "96000Hz");  break;
                case 1: sprintf(frequence_str, "88200Hz");  break;
                case 2: sprintf(frequence_str, "64000Hz");  break;
                case 3: sprintf(frequence_str, "48000Hz");  break;
                case 4: sprintf(frequence_str, "44100Hz");  break;
                case 5: sprintf(frequence_str, "32000Hz");  break;
                case 6: sprintf(frequence_str, "24000Hz");  break;
                case 7: sprintf(frequence_str, "22050Hz");  break;
                case 8: sprintf(frequence_str, "16000Hz");  break;
                case 9: sprintf(frequence_str, "12000Hz");  break;
                case 10: sprintf(frequence_str, "11025Hz"); break;
                case 11: sprintf(frequence_str, "8000Hz");  break;
                default:sprintf(frequence_str, "unknow");   break;
            }
            
            fprintf(myout, "%5d| %8s|  %8s| %5d\n",cnt, profile_str, frequence_str, size);
            data_size -= size;
            input_data += size;
            cnt++;
        }
    }
    
    fclose(ifile);
    free(aacbuffer);
    free(aacframe);
    
    return 0;
}




#pragma void    *memcpy(void *__dst, const void *__src, size_t __n);
/**
 void    *memcpy(void *__dst, const void *__src, size_t __n);
 从存储区 __src 复制 n 个字符到存储区 __dst。
 
 @param __dst 指向用于存储复制内容的目标数组，类型强制转换为 void* 指针。
 @param __src 指向要复制的数据源，类型强制转换为 void* 指针。
 @param __n 要被复制的字节数
 @return 该函数返回一个指向目标存储区 __dst 的指针
 */


#endif /* getADTSFrame_hpp */
