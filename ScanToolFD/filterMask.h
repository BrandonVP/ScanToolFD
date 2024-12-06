/*
 ===========================================================================
 Name        : filterMask.h
 Author      : Brandon Van Pelt
 Created	 : 12/6/2024
 Description : Set filter and mask
 ===========================================================================
 */

#ifndef _FILTERMASK_H
#define _FILTERMASK_H

#include "common.h"

typedef enum capture_filterMask_btn {
	BTN_filterMask_CANLabel,    // Turned OFF
	BTN_filterMask_CAN1Label,   // Turned OFF
	BTN_filterMask_CAN2Label,   // Turned OFF
	BTN_filterMask_CAN3Label,   // Turned OFF
	BTN_filterMask_WiFiLabel,   // Turned OFF

	BTN_filterMask_filterLabel, // Turned OFF
	BTN_filterMask_filter1,
	BTN_filterMask_filter2,
	BTN_filterMask_filter3,
	BTN_filterMask_filterWiFi,

	BTN_filterMask_maskLabel,   // Turned OFF
	BTN_filterMask_mask1,
	BTN_filterMask_mask2,
	BTN_filterMask_mask3,
	BTN_filterMask_maskWiFi,

	BTN_filterMask_openLabel,   // Turned OFF
	BTN_filterMask_open1,
	BTN_filterMask_open2,
	BTN_filterMask_open3,
	BTN_filterMask_openWifi,    // Turned OFF

	BTN_filterMask_button_count // Turned OFF
}Capture_FilterMask_BTN;

#ifdef _FILTERMASK_C

void FILTERMASK_clearLocalVar();
uint8_t FILTERMASK_createFilterMaskBtns();
void FILTERMASK_filterMask(int userInput);

#else

uint8_t FILTERMASK_createFilterMaskBtns();
void FILTERMASK_filterMask(int userInput);

#endif // _FILTERMASK_C
#endif // _FILTERMASK_H
