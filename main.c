#include "iio.h"
#include <unistd.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#define MODULINE4 0
#define MODULINEMINI 1
#define MODULINEDISP 2

struct iio_context *iioContext;
struct iio_device *iioMCP;
const struct iio_channel *adcChannels[4];

uint16_t GocontrollProcessorboardSupply_ReadAdc(uint8_t supply) {
    char buf[6];
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

    return (uint16_t)((float)(strtof(buf, NULL) * 25.54)); //25.54 = ((3.35/1023)/1.5)*11700
}

int main(int argc, char** argv){
    printf("V1.0.0\n");
    uint8_t count = 0;
    int type;
    //get the controller type from the argument or default to moduline 4
    if (argc == 2) {
        type = atoi(argv[1]);
    } else {
        type = 0;
    }
    iioContext = iio_create_local_context();

    // get the mcp3004 from the context
    iioMCP = iio_context_find_device(iioContext, "mcp3004");
    if (!iioMCP) {
        fprintf(stderr,"Unable to get IIO device.");
        exit(EXIT_FAILURE);
    }
    uint8_t channel_count = 0;

    //loop through the channels available to this device
    for (uint8_t i = 0; i < iio_device_get_channels_count(iioMCP); ++i) {
        struct iio_channel *chn = iio_device_get_channel(iioMCP, i);
        //filter out the channels that have 2 attributes, these are the voltage channels
        if (iio_channel_get_attrs_count(chn) == 2) {
            adcChannels[channel_count] = chn;
            channel_count++;
        }
    }

    if (channel_count < 4) {
        fprintf(stderr,"Invalid ADC channel count.");
        exit(EXIT_FAILURE);}

    //set the number of kl15s present on this controller
    uint8_t kl15s;
    switch (type) {
        case MODULINE4 ... MODULINEMINI:
            kl15s=3;
            break;
        case MODULINEDISP:
            kl15s=1;
            break;
        default:
            kl15s=3;
    }

    //endless loop till the controller shuts down or the program gets terminated from the outside
    while (count <= 20) {
        if (system("systemctl is-active go-simulink")) {
            for(uint8_t i = 0; i <kl15s; i++) {
                if (GocontrollProcessorboardSupply_ReadAdc(i) > 5000) {
                    count = 0;
                    break;
                } else if (i==kl15s-1){
                    count ++;
                }
            }
        } else {sleep(5);} //if simulink is active lower the checking rate as it is assumed simulink will shut down the controller when necessary
        usleep(500000);
    }
    system("shutdown now");
}