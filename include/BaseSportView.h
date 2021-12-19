#ifndef BASE_SPORT_VIEW_H
#define BASE_SPORT_VIEW_H

#include <gre/gre.h>
#include <hlog.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define SCREEN_SPORT_NAME Sport

#define STR_MATCH(x,y)   (!strcmp(x,y))
#define SCROLL_DRAG_THREHOLD 20

typedef struct
{
    int x;
    int y;
} SPoint;

enum EDragType
{
    DRAG_NONE = 0,
    DRAG_HOR,
    DRAG_VER,
};

#ifdef __cplusplus
}
#endif

#endif