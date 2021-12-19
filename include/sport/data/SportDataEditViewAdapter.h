/*
* =====================================================================================
*
*  Copyright (C) 2021. Huami Ltd, unpublished work. This computer program includes
*  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
*  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
*  All Rights Reserved.
*
*  Author: liubingyan@zepp.com
*
* =====================================================================================
*/

#include <widget/pageScrollerInLayer/ViewPager.h>
#include <sb_screen_context.h>
#include <gre/sdk/grelib.h>
#include "packages/apps/sports/include/presenter/sport_data_presenter.h"
#include "packages/services/sport/misc/sport_misc.h"
#include "ui/Sports/include/sport/data/pages/SportDataPageEmpty.h"
#include "packages/apps/sports/include/manager/sports_item_type.h"

typedef struct _adapter_data_t
{
    uint16_t page;
    uint16_t pos;
    uint16_t page_cnt;
    sport_page_handle *handle;
    sport_item_edit_t sportItemEdit;
    void *data;
}adapter_data_t;

ViewPagerAdapter *newViewEditPagerAdapter(ScreenContext *context);