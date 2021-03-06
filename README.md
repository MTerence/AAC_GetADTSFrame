# AAC_GetADTSFrame
AAC码流解析,从AAC码流中分析得到它的基本单元ADTS frame

# 关于AAC
AAC，全称Advanced Audio Coding，是一种专为声音数据设计的文件压缩格式。与MP3不同，它采用了全新的算法进行编码，更加高效，具有更高的“性价比”。利用AAC格式，可使人感觉声音质量没有明显降低的前提下，更加小巧。苹果ipod、诺基亚手机支持AAC格式的音频文件。
优点：相对于mp3，AAC格式的音质更佳，文件更小。
不足：AAC属于有损压缩的格式，与时下流行的APE、FLAC等无损格式相比音质存在“本质上”的差距。加之，传输速度更快的USB3.0和16G以上大容量MP3正在加速普及，也使得AAC头上“小巧”的光环不复存在。

# 特点
①提升的压缩率：可以以更小的文件大小获得更高的音质；
②支持多声道：可提供最多48个全音域声道；
③更高的解析度：最高支持96KHz的采样频率；
④提升的解码效率：解码播放所占的资源更少；
杜比实验室的结论
①128Kbps的AAC立体声音乐被专家认为不易察觉到与原来未压缩音源的区别；
②AAC格式在96Kbps码率的表现超过了128Kbps的MP3格式；
③同样是128Kbps，AAC格式的音质明显好于MP3；
④AAC是唯一一个，能够在所有的EBU试听测试项目的获得“优秀”的网络广播格式。
总的来讲，AAC可以说是极为全面的编码方式，一方面，多声道和高采样率的特点使得它非常适合未来的DVD－Audio；另一方面，低码率下的高音质则使它也适合移动通讯、网络电话、在线广播等领域，真是全能的编码方式。


# 音频码流在视频播放器中的位置如下所示

![image text](https://github.com/MTerence/AAC_GetADTSFrame/blob/master/source/20160118101456758.png)

这个小Demo是一个AAC码流解析程序，该程序可以从AAC码流中分析得到它的基本单元ADTS frame，并且可以简单解析ADTS frame 首部的字段，通过该程序可以实现不同的AAC码流处理功能

# 原理

AAC原始码流（又称为裸流）是由一个一个的ADTS Frame 组成的，他们的结构如下：

![image text](https://github.com/MTerence/AAC_GetADTSFrame/blob/master/source/20160118101611729.jpeg)


其中 每个ADTSframe之间y通过syncword（同步字）进行分割。同步字为0xFFF（二进制“111111111111”）。AAC码流解析的步骤就是首先从码流中搜索0x0FFF，分离出ADTS frame，然后再分析ADTS的首部各个字段，这个小Demo即实现了上述的两个步骤


# 代码

int simplest_aac_parser(char *url){
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



