#include <sport/data/pages/SportGoalPage.h>
#include <packages/apps/sports/include/presenter/SportPresenter.h>
#define ID_SPORT_GOAL_CONTAINER "SPORT_GOAL_CONTAINER"

CONTAINER_FUNCTION_DEFINE();

static void initSportGoalContainer(SportGoalPage *sportGoalPage)
{
    SportGoalContainer *sportGoalContainer = createSportGoalContainer(
                sportGoalPage->app);

    if (sportGoalContainer)
    {
        sportGoalContainer->create(sportGoalPage->app, sportGoalContainer,
                                   sportGoalPage->layerId);
        sportGoalContainer->initSportGoalContainer(sportGoalContainer);
        sportGoalPage->sportGoalContainer = sportGoalContainer;
        addContainer(sportGoalPage->screenContext, ID_SPORT_GOAL_CONTAINER,
                     sportGoalPage->sportGoalContainer);
    }
}

static void updateData(SportGoalPage *sportGoalPage)
{
    sport_item_data _sport_item_data = {0};
    goal_data _goal_data = getSportsScreenPresenter()->getGoal();
    getSportsDataPresenter()->getItemData(ITEM_PATTERN_GOAL,
                                          _goal_data.sport_goal_type, &_sport_item_data);

    switch (_goal_data.sport_goal_type)
    {
        case 2:
            sportGoalPage->sportGoalContainer->setLayerData(
                sportGoalPage->sportGoalContainer, getSportsScreenPresenter()->getSportType(),
                _goal_data.sport_goal_type, _goal_data.value * 60, _sport_item_data.goal.time,
                H_NULL);
            break;

        case 3:
        {
            sport_type_t sportTypet = (sport_type_t)
                                      getSportsScreenPresenter()->getSportType();
            sport_group_t group = sportGroup_get(sportTypet);

            if (SPORT_GROUP_POOL_SWIMMING == group)
            {
                sportGoalPage->sportGoalContainer->setLayerData(
                    sportGoalPage->sportGoalContainer, getSportsScreenPresenter()->getSportType(),
                    _goal_data.sport_goal_type, _goal_data.value,
                    _sport_item_data.goal.distance / 100.0f, H_NULL);
            }
            else
            {
                sportGoalPage->sportGoalContainer->setLayerData(
                    sportGoalPage->sportGoalContainer, getSportsScreenPresenter()->getSportType(),
                    _goal_data.sport_goal_type, _goal_data.value,
                    _sport_item_data.goal.distance / 1000.0f, H_NULL);
            }

            break;
        }

        case 4:
            sportGoalPage->sportGoalContainer->setLayerData(
                sportGoalPage->sportGoalContainer, getSportsScreenPresenter()->getSportType(),
                _goal_data.sport_goal_type, _goal_data.value, _sport_item_data.goal.calories,
                H_NULL);
            break;

        case 5:
            sportGoalPage->sportGoalContainer->setLayerData(
                sportGoalPage->sportGoalContainer, getSportsScreenPresenter()->getSportType(),
                _goal_data.sport_goal_type, _goal_data.value, _sport_item_data.goal.trips,
                H_NULL);
            break;

        case 6:
            sportGoalPage->sportGoalContainer->setLayerData(
                sportGoalPage->sportGoalContainer, getSportsScreenPresenter()->getSportType(),
                _goal_data.sport_goal_type, _goal_data.value, _sport_item_data.goal.count,
                H_NULL);
            break;

        case 7:
            sportGoalPage->sportGoalContainer->setLayerData(
                sportGoalPage->sportGoalContainer, getSportsScreenPresenter()->getSportType(),
                _goal_data.sport_goal_type, _goal_data.value, _sport_item_data.goal.aerobic,
                H_NULL);
            break;

        case 8:
            sportGoalPage->sportGoalContainer->setLayerData(
                sportGoalPage->sportGoalContainer, getSportsScreenPresenter()->getSportType(),
                _goal_data.sport_goal_type, _goal_data.value, _sport_item_data.goal.anaerobic,
                H_NULL);
            break;

        default:
            break;
    }
}

SportGoalPage *newSportGoalView(ScreenContext *screenContext, char *layerId)
{
    SportGoalPage *sportGoalPage = H_NULL;
    createContainer(sportGoalPage, SportGoalPage, sportGoalPage->container);
    H_ASSERT(sportGoalPage);
    sportGoalPage->screenContext = screenContext;
    sportGoalPage->app = screenContext->_grAppContext;
    sportGoalPage->layerId = layerId;
    initSportGoalContainer(sportGoalPage);
    sportGoalPage->update = updateData;
    return sportGoalPage;
}

static bool handleTouchEvent(ScreenContext *screenContext,
                             ScreenContainer *container, const char *id, const SBTouchEvent eventId,
                             const SBEventInfo *info)
{
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
    SportGoalPage *sportGoalPage = (SportGoalPage *)container;

    if (sportGoalPage->sportGoalContainer)
    {
        ((ScreenContainer *)sportGoalPage->sportGoalContainer)->destroy(screenContext,
                (ScreenContainer *)sportGoalPage->sportGoalContainer);
        removeContainer(screenContext, ID_SPORT_GOAL_CONTAINER);
        sportGoalPage->sportGoalContainer = NULL;
    }
}