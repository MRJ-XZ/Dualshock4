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

char buffer[256] = {0};
char command[29] = "sudo chmod a+rw /dev/hidraw0";
int main(int argc, char** argv){
    struct hidraw_report_descriptor rpt_desc;
    struct hidraw_devinfo info;
    struct stat status;
    int fd = 0 , desc_size;
    
    for(int i=1;;i++){
        if(stat(&command[16] , &status) == 0){
            system(command);
            fd = open(&command[16] ,  O_RDWR | O_NONBLOCK);
            ioctl(fd , HIDIOCGRAWNAME(256) , buffer);
            std::string name = "Sony Interactive Entertainment Wireless Controller";
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
    while(true){
        usleep(4000);
        read(fd , buffer , 64);
        for(int i=0 ; i < 10;i++){
            printf("%hhx\t" , buffer[i]);
            if((i) % 10 == 9)
                printf("\n");
        }
    }
}