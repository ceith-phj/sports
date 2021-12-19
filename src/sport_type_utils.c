#include <sport_type_utils.h>
#include <uiframework/ui_adapter/storyboard/engine/include/sb_screen_util.h>
#include "string.h"
#include <packages/services/syssevice/manager/system_locale.h>

const char *sportType_getTypeIconId(int sportType)
{
    for (int i = 0; i < sizeof(sportTypeArray) / sizeof(sportTypeArray[0]); i++)
    {
        if (sportType == sportTypeArray[i].typeId)
        {
            static char iconAddr[256] = {0};
            snprintf(iconAddr, sizeof(iconAddr), "images/sports/list/64/%s.png", sportTypeArray[i].typeName);
            return iconAddr;
        }
        /* code */
    }
    return 0;
}

const char *sportType_getTypeIconSizeId(int sportType , int size)
{
    for (int i = 0; i < sizeof(sportTypeArray) / sizeof(sportTypeArray[0]); i++)
    {
        if (sportType == sportTypeArray[i].typeId)
        {
            static char iconAddr[256] = {0};
            snprintf(iconAddr, sizeof(iconAddr), "images/sports/list/%d/%s.png", size, sportTypeArray[i].typeName);
            return iconAddr;
        }
        /* code */
    }
    return 0;
}

const char *_SportType_getTypeNameId(int sportType)
{
    for (int i = 0; i < sizeof(sportTypeArray) / sizeof(sportTypeArray[0]); i++)
    {
        if (sportType == sportTypeArray[i].typeId)
        {
            static char nameAddr[256] = {0};
            snprintf(nameAddr, sizeof(nameAddr), "id_sport_type_%s", sportTypeArray[i].typeName);
            return nameAddr;
        }
        /* code */
    }
    return 0;
}

const char *SportType_getTypeName(int sportType)
{
    for (int i = 0; i < sizeof(sportTypeArray) / sizeof(sportTypeArray[0]); i++)
    {
        if (sportType == sportTypeArray[i].typeId)
        {
            return sportTypeArray[i].typeName;
        }
        /* code */
    }
    return 0;
}

const char *_SportType_getCategoryNameId(int categoryType)
{
    for (int i = 0; i < sizeof(sportCategoryArray) / sizeof(SportCategory); i++)
    {
        if (categoryType == sportCategoryArray[i].categoryId)
        {
            return sportCategoryArray[i].categoryName;
        }
    }
    return 0;
}

const char* sportType_getTypeNameChar(int sportType)
{
    const char *name;
    name = System_getLocaleText(_SportType_getTypeNameId(sportType));
    if (name == NULL || strlen(name) == 0 || strcmp(name, "NULL") == 0 || strcmp(name, "NOT_EXIST") == 0)
    {
        return SportType_getTypeName(sportType);
    }
    else
    {
        return name;
    }
}
const char *sportType_getCategoryNameChar(int sportCategory)
{
    const char *name;
    name = System_getLocaleText(_SportType_getCategoryNameId(sportCategory));
    if (name == NULL || strlen(name) == 0)
    {
        return "name not found";
    }
    else
    {
        return name;
    }
}