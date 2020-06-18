/*
	This file contains some of the functions used get the device and
	subsystem capabilities.  To assist in displaying values from
	enumerations, this file contains a group of functions to convert
	the enumerated values to strings.
 */

#ifndef UTILITY_H_
#define UTILITY_H_

#include <stdio.h>
#include <unistd.h>
#include <memory.h>

#include "uldaq.h"

void ConvertInputModeToString(AiInputMode inputMode, char* modeStr);
void ConvertRangeToString(Range range, char* rangeStr);
void ConvertRangeToMinMax(Range range, double* min, double* max);
void ConvertTriggerTypeToString(TriggerType triggerType, char* triggerTypeStr);
void ConvertPortTypeToString(DigitalPortType portType, char* portTypeStr);
void ConvertPortIoTypeToString(DigitalPortIoType portIoType, char* portIoTypeStr);
void ConvertEventTypesToString(DaqEventType eventType, char* eventTypeStr);
void ConvertScanOptionsToString(ScanOption scanOptions, char* scanOptionsStr);
void ConvertDaqIChanTypeToString(DaqInChanType daqiChanType, char* daqiChanTypeStr);
void ConvertDaqOChanTypeToString(DaqOutChanType daqoChanType, char* daqoChanTypeStr);
void ConvertTCTypeToString(TcType type, char* typeStr);
void ConvertSensorConnectionTypeToString(SensorConnectionType type, char* typeStr);
void ConvertSensorConnectionTypeToString(SensorConnectionType type, char* typeStr);
UlError getDevInfoHasAi(DaqDeviceHandle daqDeviceHandle, int* hasAi);
UlError getDevInfoHasAo(DaqDeviceHandle daqDeviceHandle, int* hasAo);
UlError getDevInfoHasDio(DaqDeviceHandle daqDeviceHandle, int* hasDio);
UlError getDevInfoHasCtr(DaqDeviceHandle daqDeviceHandle, int* hasCtr);
UlError getDevInfoHasTmr(DaqDeviceHandle daqDeviceHandle, int* hasTmr);
UlError getDevInfoHasDaqi(DaqDeviceHandle daqDeviceHandle, int* hasDaqi);
UlError getDevInfoHasDaqo(DaqDeviceHandle daqDeviceHandle, int* hasDaqo);
UlError getAiInfoHasPacer(DaqDeviceHandle daqDeviceHandle, int* hasPacer);
UlError getAiInfoFirstTriggerType(DaqDeviceHandle daqDeviceHandle, TriggerType* triggerType, char* triggerTypeStr);
UlError getAiInfoRanges(DaqDeviceHandle daqDeviceHandle, AiInputMode inputMode, int *numberOfRanges, Range* ranges);
UlError getAiInfoFirstSupportedInputMode(DaqDeviceHandle daqDeviceHandle, int* numberOfChannels, AiInputMode *inputMode, char* inputModeStr);
UlError getAiInfoFirstSupportedRange(DaqDeviceHandle daqDeviceHandle, AiInputMode inputMode, Range* range, char* rangeStr);
UlError getAiInfoQueueTypes(DaqDeviceHandle daqDeviceHandle, int* queueTypes);
UlError getAiInfoHasTempChan(DaqDeviceHandle daqDeviceHandle, int* hasTempChan, int* numberOfChannels);
UlError getAiInfoIepeSupported(DaqDeviceHandle daqDeviceHandle, int* iepeSupported);
UlError getAiConfigTempChanConfig(DaqDeviceHandle daqDeviceHandle, int chan, char* chanTypeStr, char* sensorStr);
UlError getAoInfoHasPacer(DaqDeviceHandle daqDeviceHandle, int* hasPacer);
UlError getAoInfoFirstSupportedRange(DaqDeviceHandle daqDeviceHandle, Range* range, char* rangeStr);
UlError getDaqiChannelTypes(DaqDeviceHandle daqDeviceHandle, int* chanTypesMask);
UlError getDaqiInfoFirstTriggerType(DaqDeviceHandle daqDeviceHandle, TriggerType* triggerType, char* triggerTypeStr);
UlError getDaqoChannelTypes(DaqDeviceHandle daqDeviceHandle, int* chanTypesMask);
void resetCursor();
void clearEOL();
void cursorUp();
void flush_stdin();
int enter_press();

#endif /* UTILITY_H_ */
