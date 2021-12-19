#include <sport/data/pages/SportDataCadenceAssistantPage.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include <packages/apps/sports/include/manager/sports_setting_manager.h>
#include <widget/frameImgAnimation/frameImgAnimation.h>
#include <hm_toast.h>

#define CLONE_PATH "CadenceAssistantLayer.components"
#define CLONE_NAME "components"

CONTAINER_FUNCTION_DEFINE();

static void _init()
{
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.hite", CLONE_PATH);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_spset_cadence_assistant"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.unit", CLONE_PATH);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_rt_stride_freq"));
}

static void initCadenceAssistantContainer(CadenceAssistantContainer
        *cadenceAssistantContainer)
{
    _init();
    char strff[256];
    gr_integer_kv_t data[2] =
    {
        {"x", 0},
        {"y", 0},
    };
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s", CLONE_NAME);
    char *fqn = NULL;
    gre_clone_object(cadenceAssistantContainer->app, CLONE_PATH, strff,
                     cadenceAssistantContainer->layerId, data, 2, &fqn);
    greal_free(fqn);
    fqn = NULL;
}

static void _setValue(float data, const char *value, char *itemPath)
{
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), value, itemPath);
    setSBIntData(getAppInstance(), strff, (int)data);
}

static void updateData(CadenceAssistantContainer *cadenceAssistantContainer)
{
    //更新数据
    // sport_item_data itemInfo = {0};
    // getSportsDataPresenter()->getItemData(ITEM_PATTERN_CADENCE_ASSISTANT, 0, &itemInfo);
    sport_step_assist_data_t _sport_step_assist_data_t = {0};
    sportSetting_getSettingManager()->stepAssist->get(
        SportContrlService_getOps()->getSportType(), &_sport_step_assist_data_t);
    _setValue(_sport_step_assist_data_t.steps_per_min, "%s.components.value",
              cadenceAssistantContainer->layerId);
}

CadenceAssistantContainer *newCadenceAssistantContainer(
    ScreenContext *screenContext, char *layerId)
{
    CadenceAssistantContainer *cadenceAssistantContainer = H_NULL;
    createContainer(cadenceAssistantContainer, CadenceAssistantContainer,
                    cadenceAssistantContainer->container);
    H_ASSERT(cadenceAssistantContainer);
    cadenceAssistantContainer->screenContext = screenContext;
    cadenceAssistantContainer->app = screenContext->_grAppContext;
    cadenceAssistantContainer->layerId = layerId;
    initCadenceAssistantContainer(cadenceAssistantContainer);
    cadenceAssistantContainer->update = updateData;
    return cadenceAssistantContainer;
}

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    CadenceAssistantContainer *cadenceAssistantContainer =
        (CadenceAssistantContainer *)container;
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.%s", cadenceAssistantContainer->layerId,
             CLONE_NAME);

    if (eventId == TOUCH_SELECT)
    {
        Toast_show(System_getLocaleText("id_spset_cadence_assistant"));
    }

    return false;
}

static bool handleKeyEvent(ScreenContext *screenContext,
                           ScreenContainer *container, const HmKeyEvent *info)
{
    return false;
}

static bool handleGestureEvent(ScreenContext *screenContext,
                               ScreenContainer *container, const char *id, const SBGestureEvent eventId)
{
    return false;
}

static bool handleEvent(ScreenContext *screenContext,
                        ScreenContainer *container, const char *id, const char *event)
{
    return false;
}

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    CadenceAssistantContainer *cadenceAssistantContainer =
        (CadenceAssistantContainer *)container;

    if (cadenceAssistantContainer)
    {
        char strff[256];
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s", cadenceAssistantContainer->layerId,
                 CLONE_NAME);
        gre_delete_object(cadenceAssistantContainer->app, strff);
    }
}