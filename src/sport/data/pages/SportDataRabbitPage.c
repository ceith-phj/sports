#include <sport/data/pages/SportDataRabbitPage.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include "stdlib.h"
#include "hlog.h"
#include "core/display_metrics.h"
#include <packages/apps/sports/include/presenter/SportPresenter.h>
#include <hm_toast.h>

#define TAG "SportRabbiteContainer"
#define RABBIR_TOP_GROUP "SportRabbitLayer.topGroup"
#define RABBIR_MIDDLE_GROUP "SportRabbitLayer.RabbitGroup"
#define RABBIR_BOTTOM_GROUP "SportRabbitLayer.bottomGroup"
#define RABBIR_ARROW "SportRabbitLayer.arrow"
#define CLONE_TOP_NAME "topGroup"
#define CLONE_MIDDEL_NAME "RabbitGroup"
#define CLONE_BOTTOM_NAME "bottomGroup"
#define CLONE_ARROW_NAME "arrow"
#define ICON_RT_HR_ANAMI_DEFAULT "images/sports/hr/icon_rt_hr_animation_0.png"
#define ICON_RT_HR_ANAMI_PATH "images/sports/hr"
#define ICON_RT_HR_ANAMI_PREFIX "icon_rt_hr_animation"

#define Icon_RABBIT_ANAMI_DEFAULT "images/sports/rabbit/rabbit.png"
#define ICON_RABBIT_ANAMI_PATH "images/sports/rabbit/rabbit"
#define ICON_RABBIT_ANAMI_PREFIX "rabbit"

#define Icon_VILLAIN_ANAMI_DEFAULT "images/sports/rabbit/villain.png"
#define ICON_VILLAIN_ANAMI_PATH "images/sports/rabbit/villain"
#define ICON_VILLAIN_ANAMI_PREFIX "villain"


#define MAX_DIFF_TIME 600
#ifdef HM_DISPLAY_IS_ROUND
#define RABBIT_WIDTH getDimension(380)
#define RABBIR_X getDimension(50)
#else

static H_Int32 getIntLayerX(const char * path)
{
    gr_application_t *app = getAppInstance();
    if(NULL == app)
    {
        return 0;
    }
    H_Int32 value = gre_get_layer_x(app, path);
    return value;
}
static H_Int32 getIntLayerW(const char * path)
{
    gr_application_t *app = getAppInstance();
    if(NULL == app)
    {
        return 0;
    }
    H_Int32 value = gre_get_layer_w(app, path);
    return value;
}
#define RABBIT_WIDTH getIntLayerW("SportAssistScreen.SportRabbitLayer")
#define RABBIR_X getIntLayerX("SportAssistScreen.SportRabbitLayer")

#endif


static bool time_status = false;

CONTAINER_FUNCTION_DEFINE();

static void _getRecoedPace(float data, const char *value, char *itemPath)
{
    char strff[256];
    char buffer[256] = {0};
    int min = (int)(1000.0f * data / 60.0f);
    int sec = (int)(1000.0f * data - 60.0f * min);
    memset(strff, 0, sizeof(strff));
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);

    if ((min > 0 && min < 30) && sec >= 0)
    {
        snprintf(strff, sizeof(strff), "%d\'%02d", min, sec);
    }
    else
    {
        snprintf(strff, sizeof(strff), "%s", "--");
    }

    setSBStringData(getAppInstance(), buffer, strff);
}

static void _getHr(uint32_t data, const char *value, char *itemPath)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);

    if (data > 0)
    {
        setSBIntData(getAppInstance(), buffer, (int)data);
    }
    else
    {
        setSBStringData(getAppInstance(), buffer, "--");
    }
}

static void _setDiffStatus(float data, const char *value, char *itemPath)
{
    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);

    if (data < 0.1f)
    {
        time_status = false;
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sport_rabbit_Lead_time"));
    }
    else
    {
        time_status = true;
        setSBStringData(getAppInstance(), buffer,
                        System_getLocaleText("id_sport_rabbit_Behind_time"));
    }
}

const void _getExerciseTime(float exerciseTime, const char *value,
                            char *itemPath)
{
    char strff[256];
    memset(strff, 0, sizeof(strff));
    int hour = exerciseTime / 3600;
    int minute = (exerciseTime - hour * 3600) / 60;
    int sec = (exerciseTime - hour * 3600 - minute * 60);

    if (hour == 0 && minute == 0)
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]), "0:%02d", sec);
    }
    else if (hour == 0 && minute > 0)
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]), "%d:%02d", minute, sec);
    }
    else if (hour > 0)
    {
        snprintf(strff, sizeof(strff) / sizeof(strff[0]), "%d:%02d:%02d", hour, minute,
                 sec);
    }

    char buffer[256] = {0};
    memset(buffer, 0, sizeof(buffer));
    snprintf(buffer, sizeof(buffer), value, itemPath);
    setSBStringData(getAppInstance(), buffer, strff);
}

static void _cloneTopGroup(SportRabbiteContainer *sportRabbiteContainer)
{
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.topLeftHite.topLeftHite", RABBIR_TOP_GROUP);
    setSBStringData(getAppInstance(), strff, System_getLocaleText("id_rt_pace"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.topRightHite.topRightHite",
             RABBIR_TOP_GROUP);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_rt_pace_avg"));
    gr_integer_kv_t data[2] =
    {
        {"x", RABBIR_X},
        {"y", 0},
    };
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s", CLONE_TOP_NAME);
    char *fqn = NULL;
    gre_clone_object(sportRabbiteContainer->app, RABBIR_TOP_GROUP, strff,
                     sportRabbiteContainer->layerId, data, 2, &fqn);
    greal_free(fqn);
    fqn = NULL;
}

static void _cloneRabbitGroup(SportRabbiteContainer *sportRabbiteContainer)
{
    char strff[256];
    gr_integer_kv_t data[2] =
    {
        {"x", RABBIR_X},
        {"y", 0},
    };
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s", CLONE_MIDDEL_NAME);
    char *fqn = NULL;
    gre_clone_object(sportRabbiteContainer->app, RABBIR_MIDDLE_GROUP, strff,
                     sportRabbiteContainer->layerId, data, 2, &fqn);
    greal_free(fqn);
    fqn = NULL;
    sportRabbiteContainer->rabbitContainer = createFrameImgAnimContainer(
                sportRabbiteContainer->app);

    if (sportRabbiteContainer->rabbitContainer)
    {
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.RabbitGroup.rabbit.imageRabbit",
                 sportRabbiteContainer->layerId);
        addContainer(sportRabbiteContainer->screenContext, strff,
                     sportRabbiteContainer->rabbitContainer);
        sportRabbiteContainer->rabbitContainer->create(sportRabbiteContainer->app,
                sportRabbiteContainer->rabbitContainer, strff, ICON_RABBIT_ANAMI_PATH,
                ICON_RABBIT_ANAMI_PREFIX, "png", 3);
        sportRabbiteContainer->rabbitContainer->setup(
            sportRabbiteContainer->rabbitContainer, 0, 5, 1, H_NULL, H_NULL);
        // sportRabbiteContainer->rabbitContainer->start(
        //     sportRabbiteContainer->rabbitContainer);
    }

    sportRabbiteContainer->villainContainer = createFrameImgAnimContainer(
                sportRabbiteContainer->app);

    if (sportRabbiteContainer->villainContainer)
    {
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.RabbitGroup.villain.imageVillain",
                 sportRabbiteContainer->layerId);
        addContainer(sportRabbiteContainer->screenContext, strff,
                     sportRabbiteContainer->villainContainer);
        sportRabbiteContainer->villainContainer->create(sportRabbiteContainer->app,
                sportRabbiteContainer->villainContainer, strff, ICON_VILLAIN_ANAMI_PATH,
                ICON_VILLAIN_ANAMI_PREFIX, "png", 3);
        sportRabbiteContainer->villainContainer->setup(
            sportRabbiteContainer->villainContainer, 0, 5, 1, H_NULL, H_NULL);
        // sportRabbiteContainer->villainContainer->start(
        //     sportRabbiteContainer->villainContainer);
    }
}

static void _cloneBottomGroup(SportRabbiteContainer *sportRabbiteContainer)
{
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.bottomLeftHite.bottomLeftHite",
             RABBIR_BOTTOM_GROUP);
    setSBStringData(getAppInstance(), strff,
                    System_getLocaleText("id_sport_rabbit_Lead_time"));
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.bottomRightHite.bottomRightHite",
             RABBIR_BOTTOM_GROUP);
    setSBStringData(getAppInstance(), strff, System_getLocaleText("id_rt_hr"));
    gr_integer_kv_t data[2] =
    {
        {"x", RABBIR_X},
        {"y", 0},
    };
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s", CLONE_BOTTOM_NAME);
    char *fqn = NULL;
    gre_clone_object(sportRabbiteContainer->app, RABBIR_BOTTOM_GROUP, strff,
                     sportRabbiteContainer->layerId, data, 2, &fqn);
    greal_free(fqn);
    fqn = NULL;
    frameImgAnimation *container = createFrameImgAnimContainer(
                                       sportRabbiteContainer->app);

    if (container)
    {
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.bottomGroup.bottomHeart.bottomHeart",
                 sportRabbiteContainer->layerId);
        addContainer(sportRabbiteContainer->screenContext, strff, container);
        container->create(sportRabbiteContainer->app, container, strff,
                          ICON_RT_HR_ANAMI_PATH, ICON_RT_HR_ANAMI_PREFIX, "png", 8);
        container->setup(container, 0, 8, 1, H_NULL, H_NULL);
        // container->start(container);
    }
}

static void _cloneArrow(SportRabbiteContainer *sportRabbiteContainer)
{
    int arrowWidth = 0;
    int arrowHeight = 0;
    gre_load_image(sportRabbiteContainer->app,
                   "images/sports/rabbit/rabbit_arrow.png");
    gre_get_image_size(sportRabbiteContainer->app,
                       "images/sports/rabbit/rabbit_arrow.png", &arrowWidth, &arrowHeight);
    sportRabbiteContainer->arrowWidth = arrowWidth;
    sportRabbiteContainer->arrowHeight = arrowHeight;
    int rabbitWidth = 0;
    int rabbitHeight = 0;
    gre_load_image(sportRabbiteContainer->app, "images/sports/rabbit/rabbit.png");
    gre_get_image_size(sportRabbiteContainer->app,
                       "images/sports/rabbit/rabbit.png", &rabbitWidth, &rabbitHeight);
    sportRabbiteContainer->rabbitWidth = rabbitWidth;
    sportRabbiteContainer->rabbitHeight = rabbitHeight;
    int villainWidth = 0;
    int villainHeight = 0;
    gre_load_image(sportRabbiteContainer->app, "images/sports/rabbit/villain.png");
    gre_get_image_size(sportRabbiteContainer->app,
                       "images/sports/rabbit/villain.png", &villainWidth, &villainHeight);
    sportRabbiteContainer->villainWidth = villainWidth;
    sportRabbiteContainer->villainHeight = villainHeight;
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.%s.%s.%s", sportRabbiteContainer->layerId,
             CLONE_MIDDEL_NAME, "rabbit", "rabbitx");
    setSBIntData(sportRabbiteContainer->app, strff,
                 getDimension(-((sportRabbiteContainer->displayWidth / 2) -
                                (sportRabbiteContainer->rabbitWidth / 2) - RABBIR_X)));
    int rabbitX = getSBIntData(sportRabbiteContainer->app, strff);
    sportRabbiteContainer->rabbitX = rabbitX;
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.%s.%s.%s", sportRabbiteContainer->layerId,
             CLONE_MIDDEL_NAME, "rabbit", "rabbity");
    int rabbitY = getSBIntData(sportRabbiteContainer->app, strff);
    sportRabbiteContainer->rabbitY = rabbitY;
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.%s.%s.%s", sportRabbiteContainer->layerId,
             CLONE_MIDDEL_NAME, "villain", "villainx");
    setSBIntData(sportRabbiteContainer->app, strff,
                 getDimension(((sportRabbiteContainer->displayWidth / 2) -
                               (sportRabbiteContainer->rabbitWidth / 2) - RABBIR_X)));
    int villainX = getSBIntData(sportRabbiteContainer->app, strff);
    sportRabbiteContainer->villainX = villainX;
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.%s.%s.%s", sportRabbiteContainer->layerId,
             CLONE_MIDDEL_NAME, "villain", "villainy");
    int villainY = getSBIntData(sportRabbiteContainer->app, strff);
    sportRabbiteContainer->villainY = villainY;
}

static void setImage(SportRabbiteContainer *sportRabbiteContainer,
                     float diffTime, float diffDistance)
{
    int spacing = RABBIT_WIDTH - sportRabbiteContainer->rabbitWidth -
                  sportRabbiteContainer->villainWidth - abs(sportRabbiteContainer->rabbitX) - abs(
                      sportRabbiteContainer->villainX) + sportRabbiteContainer->moveWidth;
    int forIndex = spacing / sportRabbiteContainer->arrowWidth;
    char strff[256] = {0};

    for (int k = 0; k < forIndex; k++)
    {
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s%d", sportRabbiteContainer->layerId,
                 CLONE_ARROW_NAME, k);
        gre_delete_object(sportRabbiteContainer->app, strff);
    }

    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.arrow.imageArrow", "SportRabbitLayer");
    sportRabbiteContainer->moveWidth = (diffTime / (float)MAX_DIFF_TIME *
                                        (RABBIT_WIDTH - sportRabbiteContainer->rabbitWidth));
    int MovingDistance = (diffTime / (float)MAX_DIFF_TIME *
                          (RABBIT_WIDTH - sportRabbiteContainer->rabbitWidth)) / 2;

    // HLOG_I(TAG, "%s MovingDistance:%d diffTime:%f\r\n", __func__, MovingDistance, diffTime);
    if (diffDistance < 0.1f)
    {
        setSBStringData(getAppInstance(), strff,
                        "images/sports/rabbit/rabbit_arrow.png");
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s.%s.%s", sportRabbiteContainer->layerId,
                 CLONE_MIDDEL_NAME, "villain", "villainx");
        setSBIntData(sportRabbiteContainer->app, strff,
                     getDimension(sportRabbiteContainer->villainX + MovingDistance));
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s.%s.%s", sportRabbiteContainer->layerId,
                 CLONE_MIDDEL_NAME, "rabbit", "rabbitx");
        setSBIntData(sportRabbiteContainer->app, strff,
                     getDimension(sportRabbiteContainer->rabbitX - MovingDistance));
        spacing = RABBIT_WIDTH - sportRabbiteContainer->rabbitWidth -
                  sportRabbiteContainer->villainWidth - abs(sportRabbiteContainer->rabbitX) - abs(
                      sportRabbiteContainer->villainX) + sportRabbiteContainer->moveWidth;
        forIndex = spacing / sportRabbiteContainer->arrowWidth;

        // HLOG_I(TAG, "%s spacing:%d forIndex:%d\r\n", __func__, spacing, forIndex);
        for (int i = 0; i < forIndex; i++)
        {
            gr_integer_kv_t data[2] =
            {
                {"x", getDimension((RABBIR_X + sportRabbiteContainer->rabbitWidth + abs(sportRabbiteContainer->rabbitX + MovingDistance) + i * sportRabbiteContainer->arrowWidth))},
                {"y", getDimension((sportRabbiteContainer->villainY + (sportRabbiteContainer->villainHeight - sportRabbiteContainer->arrowHeight)))},
            };
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%s%d", CLONE_ARROW_NAME, i);
            char *fqn = NULL;
            gre_clone_object(sportRabbiteContainer->app, RABBIR_ARROW, strff,
                             sportRabbiteContainer->layerId, data, 2, &fqn);
            greal_free(fqn);
            fqn = NULL;
        }
    }
    else
    {
        setSBStringData(getAppInstance(), strff,
                        "images/sports/rabbit/villain_arrow.png");
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s.%s.%s", sportRabbiteContainer->layerId,
                 CLONE_MIDDEL_NAME, "villain", "villainx");
        setSBIntData(sportRabbiteContainer->app, strff,
                     getDimension(sportRabbiteContainer->villainX - MovingDistance));
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s.%s.%s", sportRabbiteContainer->layerId,
                 CLONE_MIDDEL_NAME, "rabbit", "rabbitx");
        setSBIntData(sportRabbiteContainer->app, strff,
                     getDimension(sportRabbiteContainer->rabbitX + MovingDistance));
        spacing = RABBIT_WIDTH - sportRabbiteContainer->rabbitWidth -
                  sportRabbiteContainer->villainWidth - abs(sportRabbiteContainer->rabbitX) - abs(
                      sportRabbiteContainer->villainX) + sportRabbiteContainer->moveWidth;
        forIndex = spacing / sportRabbiteContainer->arrowWidth;

        // HLOG_I(TAG, "%s spacing:%d forIndex:%d\r\n", __func__, spacing, forIndex);
        for (int j = 0; j < forIndex; j++)
        {
            gr_integer_kv_t data1[2] =
            {
                {"x", getDimension((RABBIR_X + sportRabbiteContainer->villainWidth + abs(sportRabbiteContainer->villainX - MovingDistance) + j * sportRabbiteContainer->arrowWidth))},
                {"y", getDimension((sportRabbiteContainer->villainY + (sportRabbiteContainer->villainHeight - sportRabbiteContainer->arrowHeight)))},
            };
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%s%d", CLONE_ARROW_NAME, j);
            char *fqn1 = NULL;
            gre_clone_object(sportRabbiteContainer->app, RABBIR_ARROW, strff,
                             sportRabbiteContainer->layerId, data1, 2, &fqn1);
            greal_free(fqn1);
            fqn1 = NULL;
        }
    }
}

static void calculatingTime(SportRabbiteContainer *sportRabbiteContainer,
                            sport_item_data itemInfo)
{
    float rabbitPace = getSportsScreenPresenter()->getRabbitPace() / 1000.0f;
    // HLOG_I(TAG, "%s rabbitPace：%f\r\n", __func__, rabbitPace);
    float rabbitDis = itemInfo.vrd.exTime / 1000.0f / rabbitPace;
    float diffDistance = rabbitDis - itemInfo.vrd.dis;
    _setDiffStatus(diffDistance, "%s.bottomGroup.bottomLeftHite.bottomLeftHite",
                   sportRabbiteContainer->layerId);
    float diffTime = abs(diffDistance) * rabbitPace;
    _getExerciseTime(diffTime, "%s.bottomGroup.bottomLeftValue.bottomLeftValue",
                     sportRabbiteContainer->layerId);

    // HLOG_I(TAG, "%s rabbitDis:%f diffDistance:%f diffTime:%f\r\n", __func__, rabbitDis, diffDistance, diffTime);
    if (diffTime > MAX_DIFF_TIME)
    {
        diffTime = MAX_DIFF_TIME;
    }

    setImage(sportRabbiteContainer, diffTime, diffDistance);
}

static void updateData(SportRabbiteContainer *sportRabbiteContainer)
{
    sport_item_data itemInfo = {0};
    getSportsDataPresenter()->getItemData(ITEM_PATTERN_RABBITE, 0, &itemInfo);
    int min = (int)(1000.0f * itemInfo.vrd.pace / 60.0f);
    int sec = (int)(1000.0f * itemInfo.vrd.pace - 60.0f * min);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.RabbitGroup.villain.imageVillain",
             sportRabbiteContainer->layerId);

    if ((min > 0 && min < 30) && sec >= 0)
    {
        // sportRabbiteContainer->startVillain(sportRabbiteContainer);
        setSBStringData(getAppInstance(), strff,
                        "images/sports/rabbit/villain.png");
    }
    else
    {
        // sportRabbiteContainer->stopVillain(sportRabbiteContainer);
        setSBStringData(getAppInstance(), strff,
                        "images/sports/rabbit/villain_stop.png");
    }

    calculatingTime(sportRabbiteContainer, itemInfo);
    _getRecoedPace(itemInfo.vrd.pace, "%s.topGroup.topLeftVlaue.topLeftValue",
                   sportRabbiteContainer->layerId);
    _getRecoedPace(itemInfo.vrd.avgPace, "%s.topGroup.topRightValue.topRightValue",
                   sportRabbiteContainer->layerId);
    _getHr(itemInfo.vrd.hr, "%s.bottomGroup.bottomRightValue.bottomRightValue",
           sportRabbiteContainer->layerId);
}

static void startSport(SportRabbiteContainer *sportRabbiteContainer)
{
    // sportRabbiteContainer->rabbitContainer->start(sportRabbiteContainer->rabbitContainer);
    // sportRabbiteContainer->villainContainer->start(sportRabbiteContainer->villainContainer);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.RabbitGroup.rabbit.imageRabbit",
             sportRabbiteContainer->layerId);
    setSBStringData(getAppInstance(), strff,
                    "images/sports/rabbit/rabbit.png");
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.RabbitGroup.villain.imageVillain",
             sportRabbiteContainer->layerId);
    setSBStringData(getAppInstance(), strff,
                    "images/sports/rabbit/villain.png");
}

static void stopSport(SportRabbiteContainer *sportRabbiteContainer)
{
    // sportRabbiteContainer->rabbitContainer->stop(sportRabbiteContainer->rabbitContainer);
    // sportRabbiteContainer->villainContainer->stop(sportRabbiteContainer->villainContainer);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.RabbitGroup.rabbit.imageRabbit",
             sportRabbiteContainer->layerId);
    setSBStringData(getAppInstance(), strff,
                    "images/sports/rabbit/rabbit_stop.png");
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.RabbitGroup.villain.imageVillain",
             sportRabbiteContainer->layerId);
    setSBStringData(getAppInstance(), strff,
                    "images/sports/rabbit/villain_stop.png");
}

static void startVillain(SportRabbiteContainer *sportRabbiteContainer)
{
    sportRabbiteContainer->villainContainer->start(
        sportRabbiteContainer->villainContainer);
}

static void stopVillain(SportRabbiteContainer *sportRabbiteContainer)
{
    sportRabbiteContainer->villainContainer->stop(
        sportRabbiteContainer->villainContainer);
    char strff[256];
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s.RabbitGroup.villain.imageVillain",
             sportRabbiteContainer->layerId);
    setSBStringData(getAppInstance(), strff,
                    "images/sports/rabbit/villain_stop.png");
}

static void initContainer(SportRabbiteContainer *sportRabbiteContainer)
{
    _cloneTopGroup(sportRabbiteContainer);
    _cloneRabbitGroup(sportRabbiteContainer);
    _cloneBottomGroup(sportRabbiteContainer);
    _cloneArrow(sportRabbiteContainer);
    updateData(sportRabbiteContainer);
}

SportRabbiteContainer *newSportRabbiteContainer(ScreenContext *screenContext,
        char *layerId)
{
    SportRabbiteContainer *sportRabbiteContainer = H_NULL;
    createContainer(sportRabbiteContainer, SportRabbiteContainer,
                    sportRabbiteContainer->container);
    H_ASSERT(sportRabbiteContainer);
    sportRabbiteContainer->screenContext = screenContext;
    sportRabbiteContainer->app = screenContext->_grAppContext;
    sportRabbiteContainer->layerId = layerId;
    sportRabbiteContainer->update = updateData;
    sportRabbiteContainer->startSport = startSport;
    sportRabbiteContainer->stopSport = stopSport;
    sportRabbiteContainer->startVillain = startVillain;
    sportRabbiteContainer->stopVillain = stopVillain;
    sportRabbiteContainer->displayWidth = getDisplayMetrics()->width;
    initContainer(sportRabbiteContainer);
    return sportRabbiteContainer;
}

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    SportRabbiteContainer *sportRabbiteContainer = (SportRabbiteContainer *)
            container;
    char topGroupRight[256];
    memset(topGroupRight, 0, sizeof(topGroupRight));
    snprintf(topGroupRight, sizeof(topGroupRight), "%s.%s.%s",
             sportRabbiteContainer->layerId,
             CLONE_TOP_NAME, "topRightValue");
    char topGroupLeft[256];
    memset(topGroupLeft, 0, sizeof(topGroupLeft));
    snprintf(topGroupLeft, sizeof(topGroupLeft), "%s.%s.%s",
             sportRabbiteContainer->layerId,
             CLONE_TOP_NAME, "topLeftVlaue");
    char bottomGroupLeft[256];
    memset(bottomGroupLeft, 0, sizeof(bottomGroupLeft));
    snprintf(bottomGroupLeft, sizeof(bottomGroupLeft), "%s.%s.%s",
             sportRabbiteContainer->layerId,
             CLONE_BOTTOM_NAME, "bottomLeftValue");
    char bottomGroupRight[256];
    memset(bottomGroupRight, 0, sizeof(bottomGroupRight));
    snprintf(bottomGroupRight, sizeof(bottomGroupRight), "%s.%s.%s",
             sportRabbiteContainer->layerId,
             CLONE_BOTTOM_NAME, "bottomRightValue");

    if (eventId == TOUCH_SELECT)
    {
        if (!strcmp(id, topGroupLeft))
        {
            Toast_show(System_getLocaleText("id_rt_pace"));
        }

        if (!strcmp(id, topGroupRight))
        {
            Toast_show(System_getLocaleText("id_rt_pace_avg"));
        }

        if (!strcmp(id, bottomGroupLeft))
        {
            if (time_status)
            {
                Toast_show(System_getLocaleText("id_sport_rabbit_Behind_time"));
            }
            else
            {
                Toast_show(System_getLocaleText("id_sport_rabbit_Lead_time"));
            }
        }

        if (!strcmp(id, bottomGroupRight))
        {
            Toast_show(System_getLocaleText("id_rt_hr"));
        }
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
    SportRabbiteContainer *sportRabbiteContainer = (SportRabbiteContainer *)
            container;

    if (sportRabbiteContainer)
    {
        time_status = false;
        char strff[256];
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s", sportRabbiteContainer->layerId,
                 CLONE_TOP_NAME);
        gre_delete_object(sportRabbiteContainer->app, strff);
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s", sportRabbiteContainer->layerId,
                 CLONE_MIDDEL_NAME);
        gre_delete_object(sportRabbiteContainer->app, strff);
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s", sportRabbiteContainer->layerId,
                 CLONE_BOTTOM_NAME);
        gre_delete_object(sportRabbiteContainer->app, strff);
        int spacing = RABBIT_WIDTH - sportRabbiteContainer->rabbitWidth -
                      sportRabbiteContainer->villainWidth - abs(sportRabbiteContainer->rabbitX) - abs(
                          sportRabbiteContainer->villainX) + sportRabbiteContainer->moveWidth;
        int forIndex = spacing / sportRabbiteContainer->arrowWidth;

        for (int i = 0; i < forIndex; i++)
        {
            memset(strff, 0, sizeof(strff));
            snprintf(strff, sizeof(strff), "%s.%s%d", sportRabbiteContainer->layerId,
                     CLONE_ARROW_NAME, i);
            gre_delete_object(sportRabbiteContainer->app, strff);
        }

        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.bottomGroup.bottomHeart.bottomHeart",
                 sportRabbiteContainer->layerId);
        removeContainer(screenContext, strff);
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.RabbitGroup.rabbit.imageRabbit",
                 sportRabbiteContainer->layerId);
        removeContainer(screenContext, strff);
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.RabbitGroup.villain.imageVillain",
                 sportRabbiteContainer->layerId);
        removeContainer(screenContext, strff);
    }
}