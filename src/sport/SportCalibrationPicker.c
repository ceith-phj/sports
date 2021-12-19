#include <sport/SportCalibrationPicker.h>
#include <hm_toast.h>
#include <system_locale.h>
#include "sports_manager.h"
#include <presenter/SportPresenter.h>
#include "sport_engine_utils.h"

#include <hlog.h>
#define TAG "SportPicker"
#define GET_NUMBER_INFO                                                                                                  \
    float currentDistance = *(float *)sportManager()->dataManager()->getRtData(SPORT_RT_ATTR_TYPE_DISTANCE_TOTAL) / 1000; \
    system_unit distanceUnit = sportManager()->utilsManager()->common()->getSystemUnit();                              \
    currentDistance = distanceUnit == METRIC_SYSTEM ? currentDistance : sportEngine_utils()->km2mile(currentDistance);   \
    uint16_t item1Max = currentDistance * 1.5;                                                                           \
    uint16_t item1Min = currentDistance * 0.5;
float sportCalibrationPickerGetData(PickerWidget *picker)
{
    GET_NUMBER_INFO
    float choiceDistance = picker->tool.getNumberByIndex(picker->index[0],
                              item1Min, item1Max, 1) * 1000 + picker->index[1] * 10;
    choiceDistance = ((distanceUnit == METRIC_SYSTEM) ? choiceDistance : sportEngine_utils()->mile2km(choiceDistance));
    return choiceDistance;
}

static void _PickerCallback(PickerWidget *picker, pickerEventTypeE Event,
                            pickerUpdateInfo *updateInfo)
{
    if (PICKER_COMPLETE == Event)
    {
        /* 点击完成，用来保存数据与转跳页面 */
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_SAVE_CALIBRATION);
        return;
    }
    else if (PICKER_CANCEL == Event)
    {
        return;
    }
    else if (PICKER_UPDATE == Event)
    {
        getSportsScreenPresenter()->sendUIEvent(SPORTUI_EVENT_UPDATE_CALIBATION);
        return;
    }
}

void sportCalibrationPickerRestore(PickerWidget *picker, uint32_t distance)
{
    GET_NUMBER_INFO
    picker->index[0] = picker->tool.getNumberIndex(distance / 1000, item1Min,
                       item1Max, 1);
    picker->index[1] = (distance % 1000) / 10;
    picker->setValue(picker, true);
}

void sportCalibrationPickerOpen(ScreenContext *context,
                                PickerWidget **PickerHandle, char *pickerName)
{
    /* sportManager()->utilsManager()->resUtils()->getSystemUnit(); */
    GET_NUMBER_INFO
    /* 创建picker */
    PickerWidget *picker = creatPickerWidget(context);
    *PickerHandle = picker;
    addContainer(context, pickerName, picker);
    /* 初始化数据 */
    /* 设置picker初始化数据 subTitle 不需的话要填 NULL */
    pickerInitData data =
    {
        .type = PICKER_DOUBLE,
        .subTitle = NULL,
        .title = (char *)System_getLocaleText("id_sport_calibration_title"),
    };
    /* 初始化数据 */
    uint16_t item1IndexNum = picker->tool.getIndexMax(item1Min, item1Max,
                             1); //使用工具获取第一列项目数，输入参数必须“合法合理”
    uint16_t item2IndexNum = 100;
    /* 填充列表信息， 支持参数 连接符，单位，循环，是否为字符串 */
    pickerItemInfo item1 = {.length = item1IndexNum, .connector = ".", .unit = NULL, .supportLoop = true, .isString = false};
    pickerItemInfo item2 = {.length = item2IndexNum, .connector = NULL, .unit = "km", .supportLoop = true, .isString = false};

    if (distanceUnit == METRIC_SYSTEM)
    {
        item2.unit = (char *)System_getLocaleText("id_spr_km_unit");
    }
    else
    {
        item2.unit = (char *)System_getLocaleText("id_spr_mile_unit");
    }

    if (item1IndexNum < 3)
    {
        item1.supportLoop = false;
    }

    /* 填充列表值 ，直接填充，或使用工具填充 */
    char *itemStrs1 = H_MALLOC(SELECTOR_TEXT_MAX * item1IndexNum);
    char *itemStrs2 = H_MALLOC(SELECTOR_TEXT_MAX * item2IndexNum);

    if (picker->tool.initNumberData(itemStrs1, item1Min, item1Max,
                                    1) != item1IndexNum)
    {
        HLOG_W(TAG, "initNumberData Not meeting expectations");
    }

    if (picker->tool.initNumberDataCustom(itemStrs2, 0, 99, 1,
                                          "%02d") != item2IndexNum)
    {
        HLOG_W(TAG, "initNumberData Not meeting expectations");
    }

    item1.itemStrs = (char *)itemStrs1;
    item2.itemStrs = (char *)itemStrs2;
    data.itemInfo[0] = item1;
    data.itemInfo[1] = item2;
    /* 必须先填充数据，再初始化 */
    picker->init(context, &data, picker);
    picker->index[0] = picker->tool.getNumberIndex((uint16_t)currentDistance,
                       item1Min, item1Max, 1);
    picker->index[1] = picker->tool.getNumberIndex((currentDistance -
                       (uint16_t)currentDistance) * 100, 0, 99, 1);
    picker->setValue(picker, false);
    picker->setCallback(picker, _PickerCallback);
    picker->show(picker);
    /* 数据初始化之后，填充的数据可以释放 */
    H_FREE(itemStrs1);
    H_FREE(itemStrs2);
}