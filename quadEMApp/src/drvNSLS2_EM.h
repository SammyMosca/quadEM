/*
 * drvNSLS2_EM.h
 * 
 * Asyn driver that inherits from the drvQuadEM class to control the NSLS2 electrometer / XBPM
 *
 * Author: Mark Rivers, Pete Siddons
 *
 * Created January 18th, 2016
 */

#include "drvQuadEM.h"

#define MAX_COMMAND_LEN 256
#define MAX_MODULES 1
#define MAX_PORTNAME_LEN 32
#define MAX_RANGES 5
#define DEVNAME "/dev/vipic"
#define P_DACString            "QE_DAC"                 /* asynInt32,    r/w */

/** Class to control the NSLS Precision Integrator */
class drvNSLS2_EM : public drvQuadEM {
public:
    drvNSLS2_EM(const char *portName, int moduleID, int ringBufferSize);
    ~drvNSLS2_EM();
    
    /* These are the methods we implement from asynPortDriver */
    void report(FILE *fp, int details);
                 
    /* These are the methods that are new to this class */
    virtual void exitHandler();
    /* This should be private but we call it from C so it needs to be public */
    void callbackFunc();

protected:
    /* These are the methods we implement from quadEM */
    virtual asynStatus writeInt32(asynUser *pasynUser, epicsInt32 value);
    virtual asynStatus setAcquire(epicsInt32 value);
    virtual asynStatus setRange(epicsInt32 value);
    virtual asynStatus setAveragingTime(epicsFloat64 value);  
    virtual asynStatus setBiasVoltage(epicsFloat64 value);
    virtual asynStatus readStatus();
    virtual asynStatus reset();
    
    int P_DAC;
    #define FIRST_NSLS2_COMMAND P_DAC
 
private:
    /* Our data */
    double ranges_[MAX_RANGES];
    epicsFloat64 rawData_[QE_MAX_INPUTS];
    int readingsAveraged_;
    int readingActive_;
    int firmwareVersion_;
    volatile unsigned int *fpgabase_;  //mmap'd fpga registers
    int memfd_;
    int intfd_;

    /* our functions */
    asynStatus getFirmwareVersion();
    asynStatus readMeter(int *adcbuf);
    asynStatus setDAC(int channel, int value);
    void mmap_fpga();
    asynStatus pl_open(int *fd);
};

