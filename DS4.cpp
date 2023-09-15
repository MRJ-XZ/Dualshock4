#include <linux/types.h>
#include <linux/input.h>
#include <linux/hidraw.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string>

char buffer[256] = {0}, command[29] = "sudo chmod a+rw /dev/hidraw0";
unsigned char output[9] = {0};
std::string name = "Sony Interactive Entertainment Wireless Controller",
    data = "report",
    light = "light",
    shock = "shock";
int fd = 0 , desc_size;

void get_data(){
    read(fd , buffer , 64);
    for(int i=0 ; i < 10;i++){
        printf("%hhx\t" , buffer[i]);
        if((i) % 10 == 9)
            printf("\n");
    }
}
void lightbar_demo(){
    for (int i = 0; i < 256; i++)
    {
        output[7] = (char)(255 - i);
        output[8] = (char)i;
        write(fd , output , 9);
        usleep(10000);
    }
    for (int i = 0; i < 256; i++)
    {
        output[7] = (char)i;
        output[8] = (char)(255 - i);
        write(fd , output , 9);
        usleep(10000);
    }
}
void shock_demo(){
    for(int i=0;i < 200;i++){
        output[5] = (char)i;
        write(fd , output , 9);
        usleep(20000);
    }
    for(int i=200;i >= 0; i--){
        output[5] = (char)i;
        write(fd , output , 9);
        usleep(20000);        
    }
    sleep(2);
}
int main(int argc, char** argv){
    struct hidraw_report_descriptor rpt_desc;
    struct hidraw_devinfo info;
    struct stat status;
    
    for(int i=1;;i++){
        if(stat(&command[16] , &status) == 0){
            system(command);
            fd = open(&command[16] ,  O_RDWR | O_NONBLOCK);
            ioctl(fd , HIDIOCGRAWNAME(256) , buffer);
            if(name.compare(buffer) == 0){
                // printf("%d\n" , desc_size);
                // rpt_desc.size = desc_size;
                // ioctl(fd , HIDIOCGRDESC , &rpt_desc);
                // for(int i=0 ; i < rpt_desc.size ;i++){
                //     printf("%hhx\t" , rpt_desc.value[i]);
                //     if(i % 8 == 1)
                //         printf("\n");
                // }    
                for (int j = 0; j < 256; j++)
                    buffer[j] = 0;                
                break;
            }
            else{
                close(fd);
                command[12] = '-';
                system(command);
                snprintf(&command[27] , 2 , "%d" , i);
                command[12] = '+';
            }
        }
        else if(i < 6)
            command[27] = i;
        else{
            printf("DS4 device not found.");
            return -1;            
        }
    }
    output[0] = 0x5;
    output[6] = 0x0;
    output[7] = 0x0;
    output[8] = 0x0;
    if(data.compare(argv[1]) == 0){
        while(true){
            get_data();
            usleep(4000);
        }
    }
    else if(light.compare(argv[1]) == 0){
        output[1] = 0x2;
        while(true)
            lightbar_demo();
    }
    else if(shock.compare(argv[1]) == 0){
        output[1] = 0x1;
        output[4] = 0xC0;   
        while(true)
            shock_demo();
    }
}