/*
 * flowmeter.h
 *
 *  Created on: Jan 14, 2021
 *      Author: danie
 */

#ifndef APP_FLOWMETER_H_
#define APP_FLOWMETER_H_

#define FLOW_TIMEOUT_FLOWMETER 300 //ms
#define FLOW_TIMEOUT_WTF 3000 //ms


typedef struct
{
    uint64_t RevsOdo;
    uint32_t RevsSession;
    double TimeWtf;
    double cal_xn[12];
}
TD_FLOWMETER;

extern TD_FLOWMETER flowhot, flowcold;

#endif /* APP_FLOWMETER_H_ */
