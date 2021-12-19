#include <sport/data/pages/SportDataCompassPage.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include <packages/services/syssevice/manager/system_locale.h>
#include "vibrate_manager.h"
#include "core/display_metrics.h"
#if HMI_COMP_STATISTICS && !SIMULATOR
#include "hmstatistics.h"
#endif
#define CLONE_COMPASS_READY_PATH "CompassLayer.readcomponents"
#define CLONE_COMPASS_READY_NAME "readcomponents"
#define CLONE_COMPASS_POINT_PATH "CompassLayer.pointcomponents"
#define CLONE_COMPASS_POINT_NAME "pointcomponents"
#define TAG "SPORTDATACOMPASSPAGE"
#define PAGE_RESET 0
#define PAGE_READY 1
#define PAGE_POINT 2
#define ID_GROUP_POINT "CompassLayer.pointcomponents"
#define ID_GROUP_READY "CompassLayer.readcomponents"
#define ID_READY_CANVAS "compass_ready_canvas"
#define ID_POINT_BTN_ENABLE "CompassLayer.pointcomponents.BtnEnable"
#define ID_POINT_TOPLINE1 "CompassLayer.pointcomponents.TopLine1"
#define ID_POINT_BTN_DISABLE "CompassLayer.pointcomponents.BtnDisable"
#define ID_POINT_E_X "CompassLayer.pointcomponents.PointArrow.e_x"
#define ID_POINT_E_Y "CompassLayer.pointcomponents.PointArrow.e_y"
#define ID_POINT_S_X "CompassLayer.pointcomponents.PointArrow.s_x"
#define ID_POINT_S_Y "CompassLayer.pointcomponents.PointArrow.s_y"
#define ID_POINT_W_X "CompassLayer.pointcomponents.PointArrow.w_x"
#define ID_POINT_W_Y "CompassLayer.pointcomponents.PointArrow.w_y"
#define ID_POINT_N_X "CompassLayer.pointcomponents.PointArrow.n_x"
#define ID_POINT_N_Y "CompassLayer.pointcomponents.PointArrow.n_y"
#define ID_POINT_ARROW_DEGREE "CompassLayer.pointcomponents.PointArrow.arrowAngle"
#define ID_POINT_TEXT_DEGREE "CompassLayer.pointcomponents.PointArrow.text_degree"
#define ID_POINT_TEXT_DEGREE_ALPHA "CompassLayer.pointcomponents.PointArrow.text_degree_a"
#define ID_POINT_TEXT_ALT "CompassLayer.pointcomponents.PointArrow.text_alt"
#define ID_POINT_TEXT_BAR "CompassLayer.pointcomponents.PointArrow.text_bar"
#define ID_POINT_TEXT_DIR "CompassLayer.pointcomponents.PointArrow.text_dir"
#define ID_POINT_TEXT_DEGREE2_ALPHA "CompassLayer.pointcomponents.PointArrow.text_degree2_a"
#define ID_POINT_TEXT_MARK_ALPHA "CompassLayer.pointcomponents.PointArrow.text_mark_a"
#define ID_POINT_DIAL_BG_END_ANGLE "CompassLayer.pointcomponents.DialBg.endAngle"
#define ID_POINT_TOPLINE1_ANGLE "CompassLayer.pointcomponents.TopLine1.angle"
#define ID_POINT_TEXT_MARK "CompassLayer.pointcomponents.PointArrow.text_mark"
#define ID_POINT_TEXT_DEGREE2 "CompassLayer.pointcomponents.PointArrow.text_degree2"
#define ID_READY_TITLE_A "CompassLayer.readcomponents.Tips.title_a"
#define ID_READY_BALL_X "CompassLayer.readcomponents.Ball.ball_x"
#define ID_READY_BALL_Y "CompassLayer.readcomponents.Ball.ball_y"
#define ID_READY_TIP_A "CompassLayer.readcomponents.Tips.tip_a"
#if COMPASS_QUALITY_FULL
#define ID_POINT_DIAL_TEXT_DEGREE "CompassLayer.pointcomponents.DialText.dialangle"
#endif
#define _TO_STR(_x)  #_x
#define TO_STR(_x)  _TO_STR(_x)
#define COMPASS_FPS 60
#define READY_BALL_RESET_X 0
#define READY_BALL_RESET_Y 0
#define READY_BALL_TARGET_X 0
#define READY_BALL_TARGET_Y getDimension(166)
#define READY_BALL_TARGET_Y_STR TO_STR(READY_BALL_TARGET_Y)
#define READY_BALL_RADIUS getDimension(166)
#define COMPASS_MARK_INNER getDimension(184)
#define COMPASS_MARK_OUTER getDimension(200)
#define COMPASS_MARK_COLOR 0xffffffff

#define PI 3.141592653f
#define DEG_TO_RAD (PI / 180)
#define RAD_TO_DEG (180 / PI)

#define COMPASS_DIR_RADIUS getDimension(192)
#define COMPASS_POINT_ACC 0.35f

static H_Bool pointIsMark = H_False;
static H_Int32 pointMarkDegree = 0;
#define MAX_OFFSET_ANGLE 180

#define COMPASS_READY_TEXT_ALPHA_START 1

// StoryBoard Anim Define
#define GRE_ANIM_KEY "key"
#define GRE_ANIM_KEY_RATE "rate"
#define GRE_ANIM_KEY_DURATION "duration"
#define GRE_ANIM_KEY_OFFSET "offset"
#define GRE_ANIM_KEY_FROM "form"
#define GRE_ANIM_KEY_TO "to"
#define GRE_ANIM_KEY_DELTA "delta"

CONTAINER_FUNCTION_DEFINE();

static void onPageChanged(CompassContainer *compassContainer, H_Uint32 oldPage,
                          H_Uint32 newPage);

static void updateData(CompassContainer *compassContainer)
{
}

static void cloneReadComponents(CompassContainer *compassContainer)
{
    char strff[256];
    gr_integer_kv_t data[2] =
    {
        {"x", 0},
        {"y", 0},
    };
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s", CLONE_COMPASS_READY_NAME);
    char *fqn = NULL;
    gre_clone_object(compassContainer->app, CLONE_COMPASS_READY_PATH, strff,
                     compassContainer->layerId, data, 2, &fqn);
    greal_free(fqn);
    fqn = NULL;
}
static void clonePointComponents(CompassContainer *compassContainer)
{
    char strff[256];
    gr_integer_kv_t data[2] =
    {
        {"x", 0},
        {"y", 0},
    };
    memset(strff, 0, sizeof(strff));
    snprintf(strff, sizeof(strff), "%s", CLONE_COMPASS_POINT_NAME);
    char *fqn = NULL;
    gre_clone_object(compassContainer->app, CLONE_COMPASS_POINT_PATH, strff,
                     compassContainer->layerId, data, 2, &fqn);
    greal_free(fqn);
    fqn = NULL;
}
static void destoryReadyBallAnim(CompassContainer *compassContainer)
{
    if (!compassContainer)
    {
        HLOG_E(TAG, " compassContainer is null [%s]\r\n", __func__);
        return;
    }
    if (compassContainer->screenContext->_grAppContext && compassContainer
            && compassContainer->g_ready_ball_anim)
    {
        gre_animation_destroy(compassContainer->screenContext->_grAppContext,
                              compassContainer->g_ready_ball_anim);
        compassContainer->g_ready_ball_anim = H_NULL;
    }
}
static const char *getCompassDirection(CompassContainer *compassContainer,
                                       H_Uint32 degree)
{
    if (degree <= 22)
    {
        return compassContainer->str_n;
    }
    else if (degree >= 22 && degree <= 67)
    {
        return compassContainer->str_ne;
    }
    else if (degree >= 67 && degree <= 112)
    {
        return compassContainer->str_e;
    }
    else if (degree >= 112 && degree <= 157)
    {
        return compassContainer->str_se;
    }
    else if (degree >= 157 && degree <= 202)
    {
        return compassContainer->str_s;
    }
    else if (degree >= 202 && degree <= 247)
    {
        return compassContainer->str_sw;
    }
    else if (degree >= 247 && degree <= 292)
    {
        return compassContainer->str_w;
    }
    else if (degree >= 292 && degree <= 337)
    {
        return compassContainer->str_nw;
    }
    else if (degree >= 337 && degree <= 360)
    {
        return compassContainer->str_n;
    }

    return compassContainer->str_n;
}
static int onCompassPointUpdate(struct gr_timer *timer, void *data, int dropped)
{
    CompassContainer *compassContainer = (CompassContainer *)data;

    if (!compassContainer)
    {
        HLOG_E(TAG, " compassContainer is null [%s]\r\n", __func__);
        return 0;
    }

    CALL_FINAL_FUNCTION(compassContainer->compassPresenter, CompassPresenter,
                        getCompassResult)
    (compassContainer->compassPresenter, &(compassContainer->currentValue));

    if (compassContainer->currentValue.accuracy != COMPASS_ACCURACY_SUCCESS)
    {
        HLOG_I(TAG, "onCompassPointUpdate: ACCURACY NOT SUCCESS:%d.\r\n",
               compassContainer->currentValue.accuracy);
        onPageChanged(compassContainer, compassContainer->currentPage,
                      PAGE_READY);
    }
    else
    {
        // HLOG_D(TAG,"onCompassPointUpdate: compassdegree:%d.\r\n", compassContainer->currentValue.compassdegree);
        // Degree from the sensor is assigned only to the pointTargetDegree each frame.
        compassContainer->pointTargetDegree =
            compassContainer->currentValue.compassdegree;
        H_Int32 delta = compassContainer->pointTargetDegree -
                        compassContainer->pointCurrentDegree;

        // Keep the delta value always in the range [-180, 180]
        if (delta > 180)
        {
            delta -= 360;
        }
        else if (delta < -180)
        {
            delta += 360;
        }

        // Low pass filter
        compassContainer->pointCurrentDegree = (compassContainer->pointCurrentDegree +
                                                (int)(
                                                        delta * COMPASS_POINT_ACC) + 360) % 360;
        // HLOG_D(TAG,"pointCurrentDegree:%d.\r\n", compassContainer->pointCurrentDegree);

        if (compassContainer->pointCurrentDegree != compassContainer->pointLastDegree)
        {
            // Dial FG
#if COMPASS_QUALITY_FULL
            // setSBFloatData(screenContext->_grAppContext, ID_POINT_DIAL_FG_DEGREE, (H_Float)(compassContainer->pointCurrentDegree));
            setSBFloatData(compassContainer->screenContext->_grAppContext,
                           ID_POINT_DIAL_TEXT_DEGREE,
                           (H_Float)(compassContainer->pointCurrentDegree));
#endif
            setSBFloatData(compassContainer->screenContext->_grAppContext,
                           ID_POINT_ARROW_DEGREE,
                           (H_Float)(compassContainer->pointCurrentDegree));
            // Dir text
            // Trig function eight times, look-up table method, better than eight rotations of the image.
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_N_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * (compassContainer->pointCurrentDegree -
                                                   90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_N_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * (compassContainer->pointCurrentDegree -
                                                   90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_E_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * compassContainer->pointCurrentDegree));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_E_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * compassContainer->pointCurrentDegree));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_S_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * (compassContainer->pointCurrentDegree +
                                                   90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_S_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * (compassContainer->pointCurrentDegree +
                                                   90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_W_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * (compassContainer->pointCurrentDegree +
                                                   180)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_W_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * (compassContainer->pointCurrentDegree +
                                                   180)));
            // Degree text
            H_Uint32 showDegree = 360 - compassContainer->pointCurrentDegree;

            if (Vibrate_getMotorType() ==
                    VIBRATE_MOTOR_TYPE_LINER) //dtermine whether it is a linear motor
            {
                H_Uint32 lastShowDegree = 360 - compassContainer->pointLastDegree;
                H_Uint32 offsetAngle = abs(showDegree -
                                           lastShowDegree); //Calculate offset angle
                bool flags =
                    false;                                      //judge special situations flags
                VibrateAction action = {0};
                action.type = VIBRATE_GENTLE_SHORT;

                if (offsetAngle > MAX_OFFSET_ANGLE)
                {
                    offsetAngle = 360 - offsetAngle;
                    flags = true;
                }

                for (int i = 0; i <= offsetAngle; i++)
                {
                    if ((showDegree > lastShowDegree && !flags) || (flags
                            && showDegree < lastShowDegree))
                    {
                        if (0 == (showDegree - i) % 90)
                        {
                            Vibrate_play(&action, 1, 0);
                            break;
                        }
                    }
                    else if ((showDegree < lastShowDegree && !flags) || (flags
                             && showDegree > lastShowDegree))
                    {
                        if (0 == (showDegree + i) % 90)
                        {
                            Vibrate_play(&action, 1, 0);
                            break;
                        }
                    }
                }
            }

            char buf[10];
            snprintf(buf, sizeof(buf), "%d%s", showDegree, "\xC2\xB0");
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_DEGREE, buf);
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_DEGREE2, buf);
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_DIR,
                            getCompassDirection(compassContainer, showDegree));

            // When the user clicks the mark button.
            if (pointIsMark)
            {
                H_Int32 deltaangle = pointMarkDegree - showDegree;
                // Set the TopLine position.
                setSBFloatData(compassContainer->screenContext->_grAppContext,
                               ID_POINT_TOPLINE1_ANGLE,
                               deltaangle);

                if (deltaangle < 0)
                {
                    deltaangle += 360;
                }

                // HLOG_D(TAG,"onCompassPointUpdate: mark:%d, show:%d, delta:%d.\r\n", pointMarkDegree, showDegree, deltaangle);
                // Set the angle of the red arc background.
                if (deltaangle < 180)
                {
                    setSBFloatData(compassContainer->screenContext->_grAppContext,
                                   ID_POINT_DIAL_BG_END_ANGLE,
                                   (H_Float)(-90 + deltaangle));
                    snprintf(buf, sizeof(buf), "%d%s", deltaangle, "\xC2\xB0");
                    setSBStringData(compassContainer->screenContext->_grAppContext,
                                    ID_POINT_TEXT_MARK, buf);
                }
                else
                {
                    setSBFloatData(compassContainer->screenContext->_grAppContext,
                                   ID_POINT_DIAL_BG_END_ANGLE,
                                   (H_Float)(deltaangle - 450));
                    snprintf(buf, sizeof(buf), "%d%s", (360 - deltaangle), "\xC2\xB0");
                    setSBStringData(compassContainer->screenContext->_grAppContext,
                                    ID_POINT_TEXT_MARK, buf);
                }
            }

            compassContainer->pointLastDegree = compassContainer->pointCurrentDegree;
        }

        // Update altitude and pressure
        if (compassContainer->currentValue.has_pressure)
        {
            HLOG_D(TAG, "pressure:%.2f altitude:%.2f\n\r",
                   compassContainer->currentValue.pressure,
                   compassContainer->currentValue.altitude);
            char tmpbuf[32];
            snprintf(tmpbuf, sizeof(tmpbuf), compassContainer->str_pa,
                     (int)(compassContainer->currentValue.pressure));
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_BAR, tmpbuf);
            snprintf(tmpbuf, sizeof(tmpbuf), compassContainer->str_m,
                     (int)(compassContainer->currentValue.altitude));
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_ALT, tmpbuf);
        }

#if HMI_COMP_STATISTICS && !SIMULATOR

        if (!compassContainer->pointUsed2Sec)
        {
            if (compassContainer->pointUsedTick > 50)
            {
                compassContainer->pointUsed2Sec = H_True;
                compassContainer->pointUsedTick = 0;
                hm_statis_data_add_no_kv(COMPASSAPP_HOME_PAGE_2S_EID, false);
            }
            else
            {
                compassContainer->pointUsedTick = compassContainer->pointUsedTick + 1;
            }
        }

#endif
    }

    return 0;
}
static int onCompassReadyUpdate(struct gr_timer *timer, void *data, int dropped)
{
    CompassContainer *compassContainer = (CompassContainer *)data;

    if (!compassContainer)
    {
        HLOG_E(TAG, " compassContainer is null [%s]\r\n", __func__);
        return 0;
    }

    CALL_FINAL_FUNCTION(compassContainer->compassPresenter, CompassPresenter,
                        getCompassResult)
    (compassContainer->compassPresenter, &(compassContainer->currentValue));

    if (compassContainer->currentValue.accuracy == COMPASS_ACCURACY_SUCCESS)
    {
        HLOG_I(TAG, "onCompassReadyUpdate: COMPASS_ACCURACY_SUCCESS.\r\n");

        if (Vibrate_getMotorType() ==
                VIBRATE_MOTOR_TYPE_LINER) //dtermine whether it is a linear motor
        {
            VibrateAction action = {0};
            action.type = VIBRATE_STRONG_SHORT;
            Vibrate_play(&action, 1, 0);
        }

        onPageChanged(compassContainer, compassContainer->currentPage, PAGE_POINT);
    }
    else
    {
        // Degree from the sensor is assigned only to the readyTargetDegree each frame.
        compassContainer->readyTargetDegree =
            compassContainer->currentValue.readydegree;
        H_Int32 delta = compassContainer->readyTargetDegree -
                        compassContainer->readyCurrentDegree;

        // HLOG_D(TAG, "onCompassReadyUpdate:target_degree:%d, curr_degree:%d, delta:%d\n\r", appData->readyTargetDegree, appData->readyCurrentDegree, delta);
        // Keep the delta value always in the range [-180, 180]
        if (delta > 180)
        {
            delta -= 360;
        }
        else if (delta < -180)
        {
            delta += 360;
        }

        // Low pass filter
        compassContainer->readyCurrentDegree = (compassContainer->readyCurrentDegree +
                                                (int)(
                                                        delta * COMPASS_POINT_ACC) + 360) % 360;
        // Calculate a new position instead of rotating to a new position.
        H_Int32 point_x = READY_BALL_RADIUS * cosf(DEG_TO_RAD *
                          (compassContainer->readyCurrentDegree - 90));
        H_Int32 point_y = READY_BALL_RADIUS * sinf(DEG_TO_RAD *
                          (compassContainer->readyCurrentDegree - 90));
        // Set the new position of the ball.
        setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_BALL_X,
                     point_x);
        setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_BALL_Y,
                     point_y);

        if (compassContainer->g_ready_canvas != H_NULL)
        {
            // Draw the position where the ball has passed.
            int mark_degree = compassContainer->readyCurrentDegree / 2 * 2;
            float cos = cosf(DEG_TO_RAD * (mark_degree - 90));
            float sin = sinf(DEG_TO_RAD * (mark_degree - 90));
            gre_canvas_stroke_line(compassContainer->g_ready_canvas,
                                   compassContainer->readyCanvasCenterX + COMPASS_MARK_INNER * cos,
                                   compassContainer->readyCanvasCenterY + COMPASS_MARK_INNER * sin,
                                   compassContainer->readyCanvasCenterX + COMPASS_MARK_OUTER * cos,
                                   compassContainer->readyCanvasCenterY + COMPASS_MARK_OUTER * sin,
                                   COMPASS_MARK_COLOR);
        }
    }

    return 0;
}
static void onEnterAnimComplete(gr_application_t *app, void *handle,
                                void *param)
{
    HLOG_I(TAG, "CompassReady entrance animation completed.\r\n");
    CompassContainer *compassContainer = (CompassContainer *)param;

    if (!compassContainer)
    {
        HLOG_E(TAG, " compassContainer is null [%s]\r\n", __func__);
        return;
    }

    compassContainer->g_timerHander = gre_timer_create_timeout(app, 40, 40,
                                      compassContainer,
                                      onCompassReadyUpdate);
}
static void initBallAnim(CompassContainer
                         *compassContainer)
{
    if (!compassContainer)
    {
        HLOG_E(TAG, " compassContainer is null [%s]\r\n", __func__);
        return;
    }

    HLOG_I(TAG, "initBallAnim. %p\r\n", compassContainer->screenContext);

    if (compassContainer->screenContext->_grAppContext && compassContainer)
    {
        // Init ReadyAnimation
        if (compassContainer->g_ready_ball_anim == H_NULL)
        {
            HLOG_I(TAG, "ball_anim init.\r\n");
            compassContainer->g_ready_ball_anim = gre_animation_create(
                    compassContainer->screenContext->_grAppContext,
                    COMPASS_FPS, 0, onEnterAnimComplete, compassContainer);

            if (compassContainer->g_ready_ball_anim != H_NULL)
            {
                setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_BALL_X,
                             READY_BALL_RESET_X);
                setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_BALL_Y,
                             READY_BALL_RESET_Y);
                setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_TITLE_A,
                             COMPASS_READY_TEXT_ALPHA_START);
                setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_TIP_A,
                             COMPASS_READY_TEXT_ALPHA_START);
                // Animation of the ball falling.
                gr_animation_data_t balldata[5] = {0};
                balldata[0].key = GRE_ANIM_KEY_DURATION;
                balldata[0].val = "1000";
                balldata[1].key = GRE_ANIM_KEY_RATE;
                balldata[1].val = "bounce";
                balldata[2].key = GRE_ANIM_KEY_TO;
                balldata[2].val = READY_BALL_TARGET_Y_STR;
                balldata[3].key = GRE_ANIM_KEY;
                balldata[3].val = ID_READY_BALL_Y;
                balldata[4].key = GRE_ANIM_KEY_OFFSET;
                balldata[4].val = "500";
                gre_animation_add_step(compassContainer->screenContext->_grAppContext,
                                       compassContainer->g_ready_ball_anim,
                                       balldata, sizeof(balldata) / sizeof(balldata[0]));
                // Animation of the texts fade-in.
                gr_animation_data_t tipsdata[5] = {0};
                tipsdata[0].key = GRE_ANIM_KEY_DURATION;
                tipsdata[0].val = "350";
                tipsdata[1].key = GRE_ANIM_KEY_RATE;
                tipsdata[1].val = "easein";
                tipsdata[2].key = GRE_ANIM_KEY_TO;
                tipsdata[2].val = "255";
                tipsdata[3].key = GRE_ANIM_KEY_OFFSET;
                tipsdata[3].val = "500";
                tipsdata[4].key = GRE_ANIM_KEY;
                tipsdata[4].val = ID_READY_TITLE_A;
                gre_animation_add_step(compassContainer->screenContext->_grAppContext,
                                       compassContainer->g_ready_ball_anim,
                                       tipsdata, sizeof(tipsdata) / sizeof(tipsdata[0]));
                tipsdata[3].key = GRE_ANIM_KEY_OFFSET;
                tipsdata[3].val = "580";
                tipsdata[4].key = GRE_ANIM_KEY;
                tipsdata[4].val = ID_READY_TIP_A;
                gre_animation_add_step(compassContainer->screenContext->_grAppContext,
                                       compassContainer->g_ready_ball_anim,
                                       tipsdata, sizeof(tipsdata) / sizeof(tipsdata[0]));
            }
        }
    }
}

static void startBallAnim(CompassContainer
                          *compassContainer)
{
    HLOG_I(TAG, "startBallAnim.\r\n");

    if (!compassContainer)
    {
        HLOG_E(TAG, " compassContainer is null [%s]\r\n", __func__);
        return;
    }

    if (compassContainer->screenContext->_grAppContext && compassContainer)
    {
        // Init ReadyAnimation
        initBallAnim(compassContainer);

        // Fire ReadyAnimation or skip
        if (compassContainer->g_ready_ball_anim != H_NULL)
        {
            HLOG_I(TAG, "ball_anim init success.\r\n");
            gre_animation_trigger(compassContainer->screenContext->_grAppContext,
                                  compassContainer->g_ready_ball_anim);
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_TITLE_A,
                         COMPASS_READY_TEXT_ALPHA_START);
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_TIP_A,
                         COMPASS_READY_TEXT_ALPHA_START);
        }
        else
        {
            HLOG_I(TAG, "ball_anim init fail, skip.\r\n");
            // Animation create fail, skip. Run the callback directly.
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_BALL_X,
                         READY_BALL_TARGET_X);
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_BALL_Y,
                         READY_BALL_TARGET_Y);
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_TITLE_A,
                         255);
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_READY_TIP_A,
                         255);
            onEnterAnimComplete(compassContainer->screenContext->_grAppContext, H_NULL,
                                compassContainer->screenContext);
        }
    }

    HLOG_I(TAG, "startBallAnim done.\r\n");
}
static void onPageChanged(CompassContainer
                          *compassContainer, H_Uint32 oldPage,
                          H_Uint32 newPage)
{
    switch (newPage)
    {
        case PAGE_READY:
            HLOG_I(TAG, "Switch to CompassReady.\r\n");

            // Clear Point State
            if (compassContainer->g_timerHander != H_NULL)
            {
                gre_timer_clear_timeout(compassContainer->screenContext->_grAppContext,
                                        &(compassContainer->g_timerHander));
                compassContainer->g_timerHander = H_NULL;
            }

            gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                    ID_GROUP_POINT, 0);
            gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                    ID_GROUP_READY, 1);
            compassContainer->currentPage = PAGE_READY;

            if (compassContainer->g_ready_canvas == H_NULL)
            {
                compassContainer->g_ready_canvas = gre_get_canvas(
                                                       compassContainer->screenContext->_grAppContext,
                                                       ID_READY_CANVAS);
            }

            if (compassContainer->g_ready_canvas != H_NULL)
            {
                HLOG_D(TAG, "Got ready_canvas.\r\n");
                gre_canvas_clear_rect(compassContainer->g_ready_canvas, 0, 0, getDimension(480),
                                      getDimension(480));
                gre_canvas_set_line_width(compassContainer->g_ready_canvas, 2);
                gre_canvas_get_dimensions(compassContainer->g_ready_canvas,
                                          &(compassContainer->readyCanvasCenterX),
                                          &(compassContainer->readyCanvasCenterY));
                compassContainer->readyCanvasCenterX = compassContainer->readyCanvasCenterX / 2;
                compassContainer->readyCanvasCenterY = compassContainer->readyCanvasCenterY / 2;
            }

            startBallAnim(compassContainer);
            break;

        case PAGE_POINT:
            HLOG_I(TAG, "Switch to CompassPoint.\r\n");

            // Clear Ready State
            if (compassContainer->g_timerHander != H_NULL)
            {
                gre_timer_clear_timeout(compassContainer->screenContext->_grAppContext,
                                        &(compassContainer->g_timerHander));
                compassContainer->g_timerHander = H_NULL;
            }

            destoryReadyBallAnim(compassContainer);
            gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                    ID_GROUP_POINT, 1);
            gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                    ID_GROUP_READY, 0);

            if (pointIsMark)
            {
                gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                        ID_POINT_BTN_ENABLE, 1);
                gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                        ID_POINT_TOPLINE1, 1);
                gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                        ID_POINT_BTN_DISABLE, 0);
                setSBIntData(compassContainer->screenContext->_grAppContext,
                             ID_POINT_TEXT_DEGREE_ALPHA, 0);
                setSBIntData(compassContainer->screenContext->_grAppContext,
                             ID_POINT_TEXT_DEGREE2_ALPHA, 255);
                setSBIntData(compassContainer->screenContext->_grAppContext,
                             ID_POINT_TEXT_MARK_ALPHA, 255);
                setSBFloatData(compassContainer->screenContext->_grAppContext,
                               ID_POINT_DIAL_BG_END_ANGLE,
                               (H_Float)(-90));
                setSBFloatData(compassContainer->screenContext->_grAppContext,
                               ID_POINT_TOPLINE1_ANGLE,
                               (H_Float)(0));
                pointMarkDegree = 360 - compassContainer->pointCurrentDegree;
            }
            else
            {
                gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                        ID_POINT_BTN_DISABLE, 1);
                gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                        ID_POINT_BTN_ENABLE, 0);
                gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                        ID_POINT_TOPLINE1, 0);
                setSBIntData(compassContainer->screenContext->_grAppContext,
                             ID_POINT_TEXT_DEGREE_ALPHA, 255);
                setSBIntData(compassContainer->screenContext->_grAppContext,
                             ID_POINT_TEXT_DEGREE2_ALPHA, 0);
                setSBIntData(compassContainer->screenContext->_grAppContext,
                             ID_POINT_TEXT_MARK_ALPHA, 0);
                setSBFloatData(compassContainer->screenContext->_grAppContext,
                               ID_POINT_DIAL_BG_END_ANGLE,
                               (H_Float)(-90));
                setSBFloatData(compassContainer->screenContext->_grAppContext,
                               ID_POINT_TOPLINE1_ANGLE,
                               (H_Float)(0));
                char buf[10];
                snprintf(buf, sizeof(buf), "%d%s", 0, "\xC2\xB0");
                setSBStringData(compassContainer->screenContext->_grAppContext,
                                ID_POINT_TEXT_MARK, buf);
                pointMarkDegree = 0;
            }

            compassContainer->currentPage = PAGE_POINT;
#if HMI_COMP_STATISTICS && !SIMULATOR
            compassContainer->pointUsedTick = 0;
            compassContainer->pointUsed2Sec = H_False;
#endif
            compassContainer->g_timerHander = gre_timer_create_timeout(
                                                  compassContainer->screenContext->_grAppContext,
                                                  40, 40, compassContainer, onCompassPointUpdate);
            break;

        case PAGE_RESET:
            HLOG_I(TAG, "Reset UI.\r\n");

            // Clear Timer
            if (compassContainer->g_timerHander != H_NULL)
            {
                gre_timer_clear_timeout(compassContainer->screenContext->_grAppContext,
                                        &(compassContainer->g_timerHander));
                compassContainer->g_timerHander = H_NULL;
            }

            // Clear ball anim
            destoryReadyBallAnim(compassContainer);

            // Clear canvas
            if (compassContainer->g_ready_canvas != H_NULL)
            {
                HLOG_D(TAG, "Got ready_canvas.\r\n");
                gre_canvas_clear_rect(compassContainer->g_ready_canvas, 0, 0, getDimension(480),
                                      getDimension(480));
                gre_canvas_set_line_width(compassContainer->g_ready_canvas, 2);
                gre_canvas_get_dimensions(compassContainer->g_ready_canvas,
                                          &(compassContainer->readyCanvasCenterX),
                                          &(compassContainer->readyCanvasCenterY));
                compassContainer->readyCanvasCenterX = compassContainer->readyCanvasCenterX / 2;
                compassContainer->readyCanvasCenterY = compassContainer->readyCanvasCenterY / 2;
                gre_destroy_canvas(compassContainer->screenContext->_grAppContext,
                                   ID_READY_CANVAS);
            }

            // Clear point mark
            gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                    ID_POINT_BTN_DISABLE, 1);
            gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                    ID_POINT_BTN_ENABLE, 0);
            gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                    ID_POINT_TOPLINE1, 0);
            setSBFloatData(compassContainer->screenContext->_grAppContext,
                           ID_POINT_DIAL_BG_END_ANGLE,
                           (H_Float)(-90));
            setSBFloatData(compassContainer->screenContext->_grAppContext,
                           ID_POINT_TOPLINE1_ANGLE,
                           (H_Float)(0));
            pointMarkDegree = 0;
            // Dial FG
            // setSBFloatData(screenContext->_grAppContext, ID_POINT_DIAL_FG_DEGREE, 0);
#if COMPASS_QUALITY_FULL
            setSBFloatData(compassContainer->screenContext->_grAppContext,
                           ID_POINT_DIAL_TEXT_DEGREE, 0);
#endif
            setSBFloatData(compassContainer->screenContext->_grAppContext,
                           ID_POINT_ARROW_DEGREE, 0);
            // Dir text
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_N_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * (-90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_N_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * (-90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_E_X,
                         COMPASS_DIR_RADIUS * cosf(0));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_E_Y,
                         COMPASS_DIR_RADIUS * sinf(0));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_S_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * (90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_S_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * (90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_W_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * (180)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_W_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * (180)));
            // Degree text
            char buf[10];
            snprintf(buf, sizeof(buf), "%d%s", 0, "\xC2\xB0");
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_DEGREE, buf);
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_DEGREE2, buf);
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_MARK, buf);
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_DIR,
                            getCompassDirection(compassContainer, 0));
            gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                    ID_GROUP_POINT, 0);
            gre_set_control_visible(compassContainer->screenContext->_grAppContext,
                                    ID_GROUP_READY, 0);
#if HMI_COMP_STATISTICS && !SIMULATOR
            compassContainer->pointUsedTick = 0;
            compassContainer->pointUsed2Sec = H_False;
#endif
            compassContainer->currentPage = PAGE_RESET;
            break;

        default:
            break;
    }
}
static void initText(CompassContainer
                     *compassContainer)
{
    compassContainer->str_n = System_getLocaleText("id_cps_0");
    compassContainer->str_w = System_getLocaleText("id_cps_1");
    compassContainer->str_s = System_getLocaleText("id_cps_2");
    compassContainer->str_e = System_getLocaleText("id_cps_3");
    compassContainer->str_ne = System_getLocaleText("_cps_4");
    compassContainer->str_nw = System_getLocaleText("_cps_5");
    compassContainer->str_se = System_getLocaleText("_cps_6");
    compassContainer->str_sw = System_getLocaleText("_cps_7");
    compassContainer->str_pa = System_getLocaleText("_cps_10");
    compassContainer->str_m = System_getLocaleText("_cps_11");
}
static void initCompassContainer(CompassContainer
                                 *compassContainer)
{
    cloneReadComponents(compassContainer);
    clonePointComponents(compassContainer);
    CALL_FUNCTION(SUPER_PTR(compassContainer->compassPresenter, Presenter),
                  Presenter, init)
    (SUPER_PTR(compassContainer->compassPresenter, Presenter));
    setScreenPresenter(compassContainer->screenContext,
                       compassContainer->compassPresenter);
    CALL_FINAL_FUNCTION(compassContainer->compassPresenter, CompassPresenter,
                        getCompassResult)
    (compassContainer->compassPresenter, &(compassContainer->currentValue));

    if (compassContainer->currentValue.accuracy != COMPASS_ACCURACY_SUCCESS)
    {
        HLOG_I(TAG, "onCompassPointUpdate: ACCURACY NOT SUCCESS:%d.\r\n",
               compassContainer->currentValue.accuracy);
        onPageChanged(compassContainer, compassContainer->currentPage,
                      PAGE_READY);
    }
    else
    {
        // HLOG_D(TAG,"onCompassPointUpdate: compassdegree:%d.\r\n", compassContainer->currentValue.compassdegree);
        // Degree from the sensor is assigned only to the pointTargetDegree each frame.
        compassContainer->pointTargetDegree =
            compassContainer->currentValue.compassdegree;
        H_Int32 delta = compassContainer->pointTargetDegree -
                        compassContainer->pointCurrentDegree;

        // Keep the delta value always in the range [-180, 180]
        if (delta > 180)
        {
            delta -= 360;
        }
        else if (delta < -180)
        {
            delta += 360;
        }

        // Low pass filter
        compassContainer->pointCurrentDegree = (compassContainer->pointCurrentDegree +
                                                (int)(
                                                        delta * COMPASS_POINT_ACC) + 360) % 360;
        // HLOG_D(TAG,"pointCurrentDegree:%d.\r\n", compassContainer->pointCurrentDegree);

        if (compassContainer->pointCurrentDegree != compassContainer->pointLastDegree)
        {
            // Dial FG
#if COMPASS_QUALITY_FULL
            // setSBFloatData(screenContext->_grAppContext, ID_POINT_DIAL_FG_DEGREE, (H_Float)(compassContainer->pointCurrentDegree));
            setSBFloatData(compassContainer->screenContext->_grAppContext,
                           ID_POINT_DIAL_TEXT_DEGREE,
                           (H_Float)(compassContainer->pointCurrentDegree));
#endif
            setSBFloatData(compassContainer->screenContext->_grAppContext,
                           ID_POINT_ARROW_DEGREE,
                           (H_Float)(compassContainer->pointCurrentDegree));
            // Dir text
            // Trig function eight times, look-up table method, better than eight rotations of the image.
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_N_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * (compassContainer->pointCurrentDegree -
                                                   90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_N_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * (compassContainer->pointCurrentDegree -
                                                   90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_E_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * compassContainer->pointCurrentDegree));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_E_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * compassContainer->pointCurrentDegree));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_S_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * (compassContainer->pointCurrentDegree +
                                                   90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_S_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * (compassContainer->pointCurrentDegree +
                                                   90)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_W_X,
                         COMPASS_DIR_RADIUS * cosf(DEG_TO_RAD * (compassContainer->pointCurrentDegree +
                                                   180)));
            setSBIntData(compassContainer->screenContext->_grAppContext, ID_POINT_W_Y,
                         COMPASS_DIR_RADIUS * sinf(DEG_TO_RAD * (compassContainer->pointCurrentDegree +
                                                   180)));
            // Degree text
            H_Uint32 showDegree = 360 - compassContainer->pointCurrentDegree;

            if (Vibrate_getMotorType() ==
                    VIBRATE_MOTOR_TYPE_LINER) //dtermine whether it is a linear motor
            {
                H_Uint32 lastShowDegree = 360 - compassContainer->pointLastDegree;
                H_Uint32 offsetAngle = abs(showDegree -
                                           lastShowDegree); //Calculate offset angle
                bool flags =
                    false;                                      //judge special situations flags
                VibrateAction action = {0};
                action.type = VIBRATE_GENTLE_SHORT;

                if (offsetAngle > MAX_OFFSET_ANGLE)
                {
                    offsetAngle = 360 - offsetAngle;
                    flags = true;
                }

                for (int i = 0; i <= offsetAngle; i++)
                {
                    if ((showDegree > lastShowDegree && !flags) || (flags
                            && showDegree < lastShowDegree))
                    {
                        if (0 == (showDegree - i) % 90)
                        {
                            Vibrate_play(&action, 1, 0);
                            break;
                        }
                    }
                    else if ((showDegree < lastShowDegree && !flags) || (flags
                             && showDegree > lastShowDegree))
                    {
                        if (0 == (showDegree + i) % 90)
                        {
                            Vibrate_play(&action, 1, 0);
                            break;
                        }
                    }
                }
            }

            char buf[10];
            snprintf(buf, sizeof(buf), "%d%s", showDegree, "\xC2\xB0");
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_DEGREE, buf);
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_DEGREE2, buf);
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_DIR,
                            getCompassDirection(compassContainer, showDegree));

            // When the user clicks the mark button.
            if (pointIsMark)
            {
                H_Int32 deltaangle = pointMarkDegree - showDegree;
                // Set the TopLine position.
                setSBFloatData(compassContainer->screenContext->_grAppContext,
                               ID_POINT_TOPLINE1_ANGLE,
                               deltaangle);

                if (deltaangle < 0)
                {
                    deltaangle += 360;
                }

                // HLOG_D(TAG,"onCompassPointUpdate: mark:%d, show:%d, delta:%d.\r\n", pointMarkDegree, showDegree, deltaangle);
                // Set the angle of the red arc background.
                if (deltaangle < 180)
                {
                    setSBFloatData(compassContainer->screenContext->_grAppContext,
                                   ID_POINT_DIAL_BG_END_ANGLE,
                                   (H_Float)(-90 + deltaangle));
                    snprintf(buf, sizeof(buf), "%d%s", deltaangle, "\xC2\xB0");
                    setSBStringData(compassContainer->screenContext->_grAppContext,
                                    ID_POINT_TEXT_MARK, buf);
                }
                else
                {
                    setSBFloatData(compassContainer->screenContext->_grAppContext,
                                   ID_POINT_DIAL_BG_END_ANGLE,
                                   (H_Float)(deltaangle - 450));
                    snprintf(buf, sizeof(buf), "%d%s", (360 - deltaangle), "\xC2\xB0");
                    setSBStringData(compassContainer->screenContext->_grAppContext,
                                    ID_POINT_TEXT_MARK, buf);
                }
            }

            compassContainer->pointLastDegree = compassContainer->pointCurrentDegree;
        }

        // Update altitude and pressure
        if (compassContainer->currentValue.has_pressure)
        {
            HLOG_D(TAG, "pressure:%.2f altitude:%.2f\n\r",
                   compassContainer->currentValue.pressure,
                   compassContainer->currentValue.altitude);
            char tmpbuf[32];
            snprintf(tmpbuf, sizeof(tmpbuf), compassContainer->str_pa,
                     (int)(compassContainer->currentValue.pressure));
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_BAR, tmpbuf);
            snprintf(tmpbuf, sizeof(tmpbuf), compassContainer->str_m,
                     (int)(compassContainer->currentValue.altitude));
            setSBStringData(compassContainer->screenContext->_grAppContext,
                            ID_POINT_TEXT_ALT, tmpbuf);
        }

#if HMI_COMP_STATISTICS && !SIMULATOR

        if (!compassContainer->pointUsed2Sec)
        {
            if (compassContainer->pointUsedTick > 50)
            {
                compassContainer->pointUsed2Sec = H_True;
                compassContainer->pointUsedTick = 0;
                hm_statis_data_add_no_kv(COMPASSAPP_HOME_PAGE_2S_EID, false);
            }
            else
            {
                compassContainer->pointUsedTick = compassContainer->pointUsedTick + 1;
            }
        }

#endif
    }
}

CompassContainer *newCompassContainer(
    ScreenContext *screenContext, char *layerId)
{
    CompassContainer *compassContainer = H_NULL;
    createContainer(compassContainer, CompassContainer,
                    compassContainer->container);
    H_ASSERT(compassContainer);
    compassContainer->screenContext = screenContext;
    compassContainer->app = screenContext->_grAppContext;
    compassContainer->layerId = layerId;
    compassContainer->compassPresenter = CompassPresenter_new();
    System_setKeyFilter(HM_KEY_CODE_SHORTCUT, HM_KEY_ACT_FILTER_CLICK);
    initText(compassContainer);
    initCompassContainer(compassContainer);
    compassContainer->update = updateData;
    return compassContainer;
}

static void destroy(ScreenContext *screenContext, ScreenContainer *container)
{
    CompassContainer *compassContainer =
        (CompassContainer *)container;

    if (!compassContainer)
    {
        HLOG_E(TAG, " compassContainer is null [%s]\r\n", __func__);
        return;
    }

    if (screenContext->_grAppContext && compassContainer)
    {
        destoryReadyBallAnim(compassContainer);
        onPageChanged(compassContainer, compassContainer->currentPage, PAGE_RESET);
    }

    System_resetKeyFilter(HM_KEY_CODE_SHORTCUT);

    if (compassContainer->compassPresenter != H_NULL)
    {
        Presenter *basePresenter = SUPER_PTR(compassContainer->compassPresenter,
                                             Presenter);
        CALL_VOID(destory, basePresenter);
        freeScreenPresenter(screenContext, CompassPresenter);
    }

    if (compassContainer)
    {
        char strff[256];
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s", compassContainer->layerId,
                 CLONE_COMPASS_POINT_NAME);
        gre_delete_object(compassContainer->app, strff);
        memset(strff, 0, sizeof(strff));
        snprintf(strff, sizeof(strff), "%s.%s", compassContainer->layerId,
                 CLONE_COMPASS_READY_NAME);
        gre_delete_object(compassContainer->app, strff);
    }
}

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
    HLOG_D(TAG, "%s   id:%s  eventId:%d   info:%d \r\n", __func__, id, eventId,
           info->x);
    CompassContainer *compassContainer =
        (CompassContainer *)container;

    if (!compassContainer)
    {
        HLOG_E(TAG, " compassContainer is null [%s]\r\n", __func__);
        return false;
    }

    if (compassContainer->compassPresenter != H_NULL)
    {
        CALL_FINAL_FUNCTION(compassContainer->compassPresenter, CompassPresenter,
                            userActivite)
        (compassContainer->compassPresenter);
    }

    if (screenContext->_grAppContext && compassContainer)
    {
        if (pointIsMark)
        {
            gre_set_control_visible(screenContext->_grAppContext, ID_POINT_BTN_DISABLE, 1);
            gre_set_control_visible(screenContext->_grAppContext, ID_POINT_BTN_ENABLE, 0);
            gre_set_control_visible(screenContext->_grAppContext, ID_POINT_TOPLINE1, 0);
            setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_DEGREE_ALPHA, 255);
            setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_DEGREE2_ALPHA, 0);
            setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_MARK_ALPHA, 0);
            setSBFloatData(screenContext->_grAppContext, ID_POINT_DIAL_BG_END_ANGLE,
                           (H_Float)(-90));
            setSBFloatData(screenContext->_grAppContext, ID_POINT_TOPLINE1_ANGLE,
                           (H_Float)(0));
            char buf[10];
            snprintf(buf, sizeof(buf), "%d%s", 0, "\xC2\xB0");
            setSBStringData(screenContext->_grAppContext, ID_POINT_TEXT_MARK, buf);
            pointIsMark = H_False;
            pointMarkDegree = 0;
        }
        else
        {
            gre_set_control_visible(screenContext->_grAppContext, ID_POINT_BTN_ENABLE, 1);
            gre_set_control_visible(screenContext->_grAppContext, ID_POINT_TOPLINE1, 1);
            gre_set_control_visible(screenContext->_grAppContext, ID_POINT_BTN_DISABLE, 0);
            setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_DEGREE_ALPHA, 0);
            setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_DEGREE2_ALPHA, 255);
            setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_MARK_ALPHA, 255);
            setSBFloatData(screenContext->_grAppContext, ID_POINT_DIAL_BG_END_ANGLE,
                           (H_Float)(-90));
            setSBFloatData(screenContext->_grAppContext, ID_POINT_TOPLINE1_ANGLE,
                           (H_Float)(0));
            pointMarkDegree = 360 - compassContainer->pointCurrentDegree;
            pointIsMark = H_True;
#if HMI_COMP_STATISTICS && !SIMULATOR
            hm_statis_data_add_no_kv(COMPASS_LOCKPOSITION_EID, false);
#endif
        }
    }

    return true;
}

static bool handleKeyEvent(ScreenContext *screenContext,
                           ScreenContainer *container,  const HmKeyEvent *info)
{
    HLOG_D(TAG, "%s\r\n", __func__);
    CompassContainer *compassContainer =
        (CompassContainer *)container;

    if (!compassContainer)
    {
        HLOG_E(TAG, " compassContainer is null [%s]\r\n", __func__);
        return false;
    }

    if (compassContainer->compassPresenter != H_NULL)
    {
        CALL_FINAL_FUNCTION(compassContainer->compassPresenter, CompassPresenter,
                            userActivite)
        (compassContainer->compassPresenter);
    }

    if (info->code == HM_KEY_CODE_SHORTCUT)
    {
        if (info->action == HM_KEY_ACTION_CLICK)
        {
            if (screenContext->_grAppContext && compassContainer
                    && compassContainer->currentPage == PAGE_POINT)
            {
                if (pointIsMark)
                {
                    gre_set_control_visible(screenContext->_grAppContext, ID_POINT_BTN_DISABLE, 1);
                    gre_set_control_visible(screenContext->_grAppContext, ID_POINT_BTN_ENABLE, 0);
                    gre_set_control_visible(screenContext->_grAppContext, ID_POINT_TOPLINE1, 0);
                    setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_DEGREE_ALPHA, 255);
                    setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_DEGREE2_ALPHA, 0);
                    setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_MARK_ALPHA, 0);
                    setSBFloatData(screenContext->_grAppContext, ID_POINT_DIAL_BG_END_ANGLE,
                                   (H_Float)(-90));
                    setSBFloatData(screenContext->_grAppContext, ID_POINT_TOPLINE1_ANGLE,
                                   (H_Float)(0));
                    char buf[10];
                    snprintf(buf, sizeof(buf), "%d%s", 0, "\xC2\xB0");
                    setSBStringData(screenContext->_grAppContext, ID_POINT_TEXT_MARK, buf);
                    pointIsMark = H_False;
                    pointMarkDegree = 0;
                }
                else
                {
                    gre_set_control_visible(screenContext->_grAppContext, ID_POINT_BTN_ENABLE, 1);
                    gre_set_control_visible(screenContext->_grAppContext, ID_POINT_TOPLINE1, 1);
                    gre_set_control_visible(screenContext->_grAppContext, ID_POINT_BTN_DISABLE, 0);
                    setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_DEGREE_ALPHA, 0);
                    setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_DEGREE2_ALPHA, 255);
                    setSBIntData(screenContext->_grAppContext, ID_POINT_TEXT_MARK_ALPHA, 255);
                    setSBFloatData(screenContext->_grAppContext, ID_POINT_DIAL_BG_END_ANGLE,
                                   (H_Float)(-90));
                    setSBFloatData(screenContext->_grAppContext, ID_POINT_TOPLINE1_ANGLE,
                                   (H_Float)(0));
                    pointMarkDegree = 360 - compassContainer->pointCurrentDegree;
                    pointIsMark = H_True;
#if HMI_COMP_STATISTICS && !SIMULATOR
                    hm_statis_data_add_no_kv(COMPASS_LOCKPOSITION_EID, false);
#endif
                }
            }

            return true;
        }
    }

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

