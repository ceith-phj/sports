#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_context.h>
#include "sport_service.h"
importScreen(SportList);
importScreen(Sport);

/* course */
importScreen(SportCourseList);
importScreen(SportCourseDetail);

/* setting */
importScreen(SportPicker);
importScreen(SportSetting);
importScreen(SportSettingDialog);
importScreen(SportSettingAssistant);
importScreen(SportSettingGoal);
importScreen(SportSettingUseTrainingEffect);
importScreen(SportSettingUseWorkoutStatistic);
importScreen(SportSettingReminder);
importScreen(SportSettingMaxHrZone);
importScreen(SportSettingReserveHrZone);
importScreen(SportSettingAutoLap);
importScreen(SportSettingAutoPause);
importScreen(SportSettingVirtualPacer);
importScreen(SportSettingCadenceAssistant);
importScreen(SportSettingReminderMode);
importScreen(SportSettingDrink);
importScreen(SportSettingEnergy);
importScreen(SportSettingBack);

// importScreen(SportSettingTrain);
importScreen(SportSettingMore);
importScreen(SportSettingSwimPoolLength);
importScreen(SportSettingWearHand);

// importScreen(SportSettingPaceStrategy);
importScreen(SportCategoryList);
importScreen(SportDetailList);
importScreen(SportTypeInit);
importScreen(SportItemList);
importScreen(SportSubItemlist);
importScreen(SportChartList);
importScreen(SportItemEdit);
importScreen(SportItemSetting);
importScreen(SportItemSorting);
importScreen(SportItemPageList);
importScreen(SportDataList);

/* Auto Recog */
importScreen(SportAutoRecog);

// importScreen(SportList_Base);
importScreen(SportEdit);
importScreen(SportEditSort);
/* --import end-- */

REGISTER_SCREEN(SportApp){
    registerScreen(SportList),
    registerScreen(Sport),
    registerScreen(SportTypeInit),

    /* course */
    registerScreen(SportCourseList),
    registerScreen(SportCourseDetail),

    /* setting */
    registerScreen(SportPicker),
    registerScreen(SportSetting),
    registerScreen(SportSettingDialog),
    registerScreen(SportSettingAssistant),
    registerScreen(SportSettingGoal),
    registerScreen(SportSettingUseTrainingEffect),
    registerScreen(SportSettingUseWorkoutStatistic),
    registerScreen(SportSettingReminder),
    registerScreen(SportSettingMaxHrZone),
    registerScreen(SportSettingReserveHrZone),
    registerScreen(SportSettingAutoLap),
    registerScreen(SportSettingAutoPause),
    registerScreen(SportSettingVirtualPacer),
    registerScreen(SportSettingCadenceAssistant),
    registerScreen(SportSettingReminderMode),
    registerScreen(SportSettingDrink),
    registerScreen(SportSettingEnergy),
    registerScreen(SportSettingBack),

//    registerScreen(SportSettingTrain),
    registerScreen(SportSettingMore),
    registerScreen(SportSettingSwimPoolLength),
    registerScreen(SportSettingWearHand),

    registerScreen(SportCategoryList),
    registerScreen(SportDetailList),
    registerScreen(SportItemList),
    registerScreen(SportSubItemlist),
    registerScreen(SportChartList),

    // registerScreen(SportList_Base),
    registerScreen(SportEdit),
    registerScreen(SportEditSort),
    registerScreen(SportItemEdit),
    registerScreen(SportItemSetting),
    registerScreen(SportItemSorting),
    registerScreen(SportAutoRecog),

    /* --regist end-- */
    registerScreen(SportItemPageList),
    registerScreen(SportDataList),
};

APPLICATION_DECLARE(SportApp){
    SERVCIE_DECLARE(onSportServiceInit, H_NULL),
    ADD_SCREEN(SportApp),
    ADD_NO_WIDGET(SportApp),
};
