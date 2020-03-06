#pragma once

#ifndef MEMORY_MAPPING_VAJA_6
#define MEMORY_MAPPING_VAJA_6

void Timer_Init();		

// COMMS INIT
#define HOST_TIME_HOLD_0_MIN			800
#define HOST_TIME_HOLD_0_MAX			2000
#define HOST_TIME_RELEASE_MIN			20
#define HOST_TIME_RELEASE_MAX			200

// hold time while waiting high flag from sensor
#define RESPONSE_TO_LOW_MIN				75
#define RESPONSE_TO_LOW_MIN				85

// holding high time, after which the main data transfer sequence starts
#define CLIENT_RESPONSE_TIME_MIN		75
#define CLIENT_RESPONSE_TIME_MIN		85

//DATA
#define TIME_SIGNAL_LOW_MIN				48
#define	TIME_SIGNAL_LOW_MAX				55
#define TIME_SIGNAL_HIGH_0_MIN			22
#define TIME_SIGNAL_HIGH_0_MAX			30
#define TIME_SIGNAL_HIGH_1_MIN			68
#define TIME_SIGNAL_HIGH_1_MAX			75

// TERMINATE DATA SIGNAL
#define	SENSOR_RELEASE_BUS_TIME_MIN		45
#define	SENSOR_RELEASE_BUS_TIME_MAX		45

#endif
