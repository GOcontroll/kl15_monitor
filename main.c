#include "iio.h"
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>

struct iio_context *iioContext;
struct iio_device *iioMCP;
const struct iio_channel *adcChannels[4];

uint16_t GocontrollProcessorboardSupply_ReadAdc(uint8_t supply) {
		char buf[100];
		size_t bufSize = sizeof buf;
		switch (supply) {
			//read raw (voltage) attributes
			case 0:
			iio_channel_attr_read(adcChannels[3], "raw", buf, bufSize);
			break;
			case 1:
			iio_channel_attr_read(adcChannels[0], "raw", buf, bufSize);
			break;
			case 2:
			iio_channel_attr_read(adcChannels[1], "raw", buf, bufSize);
			break;
		}
		//convert to milivolts and store it
		return (uint16_t)((float)(strtof(buf, NULL) * 25.54)); //25.54 = ((3.35/1023)/1.5)*11700	
}

int main(void){


    uint16_t kl15 = 10000;
    uint8_t count = 0;
    
    iioContext = iio_create_local_context();
    /* set up the mcp3004 iio channels*/
	
    uint8_t channel_count;
    iioMCP = iio_context_find_device(iioContext, "mcp3004");
    //loop through the channels available to this device
    for (uint8_t i = 0; i < iio_device_get_channels_count(iioMCP); ++i) {
        struct iio_channel *chn = iio_device_get_channel(iioMCP, i);
        //filter out the channels that have 2 attributes, these are the voltage channels
        if (iio_channel_get_attrs_count(chn) == 2) {
            adcChannels[channel_count] = chn;
            channel_count++;
        }
    }
	
    while (count <= 10) {
        if (system("systemctl is-active go-simulink")) {
            for(uint8_t i = 0; i <3; i++) {
                if (GocontrollProcessorboardSupply_ReadAdc(i) > 5000) {
                    count = 0;
                    break;
                } else if (i==2){
                    count ++;
                }
            }
        }
        usleep(500000);
    }
    system("shutdown now");
}