#ifndef SPORT_CALIBRATION_PICKER_H
#define SPORT_CALIBRATION_PICKER_H

#include <widget/picker/pickerWidget.h>
void sportCalibrationPickerOpen(ScreenContext *context, PickerWidget **PickerHandle, char *pickerName);
float sportCalibrationPickerGetData(PickerWidget *picker);
void sportCalibrationPickerRestore(PickerWidget *picker, uint32_t distance);

#endif