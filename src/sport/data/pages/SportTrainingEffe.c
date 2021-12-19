#include <sport/data/pages/SportTrainingEffe.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include <packages/apps/sports/include/manager/sports_setting_manager.h>
#include <hlog.h>
#include "record/record_text_aglin_utils.h"
#define TAG "SportTrainingEffe"
#include "core/display_metrics.h"
#include "sports/include/manager/sports_manager.h"

#define CLONE_LAYER "SportAssistScreen.TrainingEffeLayer"
#define CLONE_PATH "TrainingEffeLayer.components"
#define CLONE_NAME "components"
#define rtData_f(type)                    (*(float *)sportManager()->dataManager()->getRtData(type))
#define rtData_u(type)                  (*(uint32_t *)sportManager()->dataManager()->getRtData(type))

CONTAINER_FUNCTION_DEFINE();

static void _setLeftArrow(char *itemPath, float value)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teleftarrow.left_angle", itemPath);

    if (value > 5.0f)
    {
        value = 5.0f;
    }

    H_Float left_arrow = 0.0f;

    if ((value < 0.000001f) && (value > -0.000001f))
    {
        left_arrow = -141;
    }
    else
    {
        left_arrow = (0.0f - (value / 5.0f) * 104.0f);
        left_arrow = -141 - left_arrow;
    }

    HLOG_I(TAG, "%s left_arrow:%f\r\n", __func__, left_arrow);
    setSBFloatData(getAppInstance(), buffer, left_arrow);
}

static void _setLeftValue(char *itemPath, float value)
{
    char strff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%.1f", value);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teAerobic.aeronic", itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}

static void _setRightArrow(char *itemPath, float value)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.terightarrow.right_angle", itemPath);

    if (value > 5.0f)
    {
        value = 5.0f;
    }

    H_Float rihjt_arrow = 0.0f;

    if ((value < 0.000001f) && (value > -0.000001f))
    {
        rihjt_arrow = 141;
    }
    else
    {
        rihjt_arrow = (0.0f + (value / 5.0f) * 104.0f);
        rihjt_arrow = 141 - rihjt_arrow;
    }

    HLOG_I(TAG, "%s rihjt_arrow:%f\r\n", __func__, rihjt_arrow);
    setSBFloatData(getAppInstance(), buffer, rihjt_arrow);
}

static void _setRightValue(char *itemPath, float value)
{
    char strff[256] = {0};
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%.1f", value);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teAnaerobic.anaerobic", itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}

static void _setMaxArrowAttr(TrainingEffeContainer *trainingEffeContainer,
                             gr_application_t *app, char *itemPath)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_posx", itemPath);
    setSBIntData(getAppInstance(), buffer,
                 ((trainingEffeContainer->layerW / 2) - (42 / 2)));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_posy", itemPath);
    setSBIntData(getAppInstance(), buffer, 22);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_centerX", itemPath);
    setSBFloatData(getAppInstance(), buffer, trainingEffeContainer->layerW / 2.0f);
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.arrow.arrow_centerY", itemPath);
    setSBFloatData(getAppInstance(), buffer, trainingEffeContainer->layerH / 2.0f);
}
static void _setLeftImage(char *itemPath, float aerobicTe)
{
    HLOG_I(TAG, "%s aerobicTe:%f itemPath:%s\r\n", __func__, aerobicTe, itemPath);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teleft.te_left_image", itemPath);

    if ((aerobicTe < 0.000001f) && (aerobicTe > -0.000001f))
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_bg.png");
    }

    if (aerobicTe > 0.0f && aerobicTe <= 0.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_one.png");
    }

    if (aerobicTe >= 1.0f && aerobicTe <= 1.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_two.png");
    }

    if (aerobicTe >= 2.0f && aerobicTe <= 2.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_three.png");
    }

    if (aerobicTe >= 3.0f && aerobicTe <= 3.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_four.png");
    }

    if (aerobicTe >= 4.0f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/left_five.png");
    }

    _setLeftValue(itemPath, aerobicTe);
    _setLeftArrow(itemPath, aerobicTe);
}

static void _setRightImage(char *itemPath, float anAerobicTe)
{
    HLOG_I(TAG, "%s anAerobicTe:%f itemPath:%s\r\n", __func__, anAerobicTe,
           itemPath);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teright.te_right_image", itemPath);

    if ((anAerobicTe < 0.000001f) && (anAerobicTe > -0.000001f))
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_bg.png");
    }

    if (anAerobicTe > 0.0f && anAerobicTe <= 0.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_one.png");
    }

    if (anAerobicTe >= 1.0f && anAerobicTe <= 1.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_two.png");
    }

    if (anAerobicTe >= 2.0f && anAerobicTe <= 2.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_three.png");
    }

    if (anAerobicTe >= 3.0f && anAerobicTe <= 3.9f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_four.png");
    }

    if (anAerobicTe >= 4.0f)
    {
        setSBStringData(getAppInstance(), buffer, "images/record/right_five.png");
    }

    _setRightValue(itemPath, anAerobicTe);
    _setRightArrow(itemPath, anAerobicTe);
}

static void _setHr(char *itemPath, uint32_t value)
{
    HLOG_I(TAG, "%s currentEx:%d itemPath:%s\r\n", __func__, value, itemPath);
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), "%s.teCurrentEx.currentEx", itemPath);
    setSBIntData(getAppInstance(), buffer, value);
}

static void FB_refTe(TrainingEffeContainer *trainingEffeContainer,
                     gr_application_t *app, char *itemPath, float aerobic,
                     float anAerobic, float hr)
{
    char hite[256] = {0};
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teTitle.title", itemPath);
    setSBStringData(getAppInstance(), hite,
                    System_getLocaleText("id_sportrecord_tr_effe_title"));
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teAerobicHite", itemPath);
    set_text_aglin(hite, System_getLocaleText("id_sportrecord_tr_eff_aer"),
                   "fonts/allfont-Regular.ttf", 28);
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teAerobicHite.aerHite", itemPath);
    setSBStringData(getAppInstance(), hite,
                    System_getLocaleText("id_sportrecord_tr_eff_aer"));
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teAnaerobicHite", itemPath);
    set_text_aglin(hite, System_getLocaleText("id_sportrecord_tr_eff_anaer"),
                   "fonts/allfont-Regular.ttf", 28);
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teAnaerobicHite.anaerHite", itemPath);
    setSBStringData(getAppInstance(), hite,
                    System_getLocaleText("id_sportrecord_tr_eff_anaer"));
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teCurrentExHite", itemPath);
    set_text_aglin(hite, System_getLocaleText("id_rt_hr"),
                   "fonts/allfont-Regular.ttf", 28);
    memset(hite, 0, sizeof(hite));
    snprintf(hite, sizeof(hite), "%s.teCurrentExHite.curexhite", itemPath);
    setSBStringData(getAppInstance(), hite,
                    System_getLocaleText("id_rt_hr"));
    _setMaxArrowAttr(trainingEffeContainer, app, itemPath);
    _setLeftImage(itemPath, aerobic);
    _setRightImage(itemPath, anAerobic);
    _setHr(itemPath, hr);
}


static void updateData(TrainingEffeContainer *trainingEffeContainer)
{
    float aerobic = rtData_f(SPORT_RT_ATTR_TYPE_OTHER_AEROBIC_TE);
    float anAerobic = rtData_f(SPORT_RT_ATTR_TYPE_OTHER_ANAEROBIC_TE);
    uint32_t hr = rtData_u(SPORT_RT_ATTR_TYPE_HR);
    char itemPath[256] = {0};
    memset(itemPath, 0, sizeof(itemPath));
    snprintf(itemPath, sizeof(itemPath), "%s.%s", trainingEffeContainer->layerId,
             CLONE_NAME);
    FB_refTe(trainingEffeContainer,
             trainingEffeContainer->app, itemPath, aerobic, anAerobic, hr);
}
static void initTrainingEffContainer(TrainingEffeContainer
                                     *trainingEffeContainer)
{
    trainingEffeContainer->layerX = gre_get_layer_x(trainingEffeContainer->app,
                                    CLONE_LAYER);
    trainingEffeContainer->layerY = gre_get_layer_y(trainingEffeContainer->app,
                                    CLONE_LAYER);
    trainingEffeContainer->layerW = gre_get_layer_w(trainingEffeContainer->app,
                                    CLONE_LAYER);
    trainingEffeContainer->layerH = gre_get_layer_h(trainingEffeContainer->app,
                                    CLONE_LAYER);
    char strff[256];
    gr_integer_kv_t data[4] =
    {
        {"x", trainingEffeContainer->layerX},
        {"y", trainingEffeContainer->layerY},
        {"width", trainingEffeContainer->layerW},
        {"height", trainingEffeContainer->layerH},
    };
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s", CLONE_NAME);
    char *fqn = NULL;
    gre_clone_object(trainingEffeContainer->app, CLONE_PATH, strff,
                     trainingEffeContainer->layerId, data, 4, &fqn);
    greal_free(fqn);
    fqn = NULL;
}

TrainingEffeContainer *newTrainingEffeContainer(
    ScreenContext *screenContext, char *layerId)
{
    TrainingEffeContainer *trainingEffeContainer = H_NULL;
    createContainer(trainingEffeContainer, TrainingEffeContainer,
                    trainingEffeContainer->container);
    H_ASSERT(trainingEffeContainer);
    trainingEffeContainer->screenContext = screenContext;
    trainingEffeContainer->app = screenContext->_grAppContext;
    trainingEffeContainer->layerId = layerId;
    initTrainingEffContainer(trainingEffeContainer);
    updateData(trainingEffeContainer);
    trainingEffeContainer->update = updateData;
    return trainingEffeContainer;
}

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    TrainingEffeContainer *trainingEffeContainer =
        (TrainingEffeContainer *)container;

    if (trainingEffeContainer)
    {
        char strff[256];
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s", trainingEffeContainer->layerId,
                 CLONE_NAME);
        gre_delete_object(trainingEffeContainer->app, strff);
    }
}

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    return false;
}

static bool handleKeyEvent(ScreenContext *screenContext,
                           ScreenContainer *container,  const HmKeyEvent *info)
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
