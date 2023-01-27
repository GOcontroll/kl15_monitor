#include "iio.h"
#include <unistd.h>
#include <stdint.h>

#include <stdio.h>

struct iio_context *iioContext;
struct iio_device *iioMCP;
const struct iio_channel *adcChannels[4];
uint16_t kl15 = 10000;
char buf[100];


int main(int argc, char **argv){
    
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
	
    while (kl15 > 5000) {
        iio_channel_attr_read(adcChannels[3], "raw", buf, sizeof(buf));
        kl15 = (uint16_t)((float)(strtof(buf, NULL) * 25.54));
        // printf("%d\n",kl15);
        usleep(500000);
    }

    system("shutdown now");
}