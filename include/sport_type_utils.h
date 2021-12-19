/*
 * ======================================================
 *
 *  Copyright (C) 2020. Huami Ltd, unpublished work. This computer program includes
 *  Confidential, Proprietary Information and is a Trade Secret of Huami Ltd.
 *  All use, disclosure, and/or reproduction is prohibited unless authorized in writing.
 *  All Rights Reserved.
 *
 *  Author :  wangyiju@huami.com
 *  Time : 2020-09-15
 *
 *  Describe : sports presenter for sports app
 *
 * ======================================================
 */

#ifndef SPORTS_TYPE_UTIL_H
#define SPORTS_TYPE_UTIL_H

#include<hformat.h>
#include "sport_type_auto.h"
#include "packages/apps/sports/include/manager/sports_list_manager.h"

#ifdef __cplusplus
extern "C"
{
#endif

#define SPORT_TYPE_SIZE SPORT_TYPE_MAX

    typedef struct SportType
    {
        H_Int16 typeId;
        char *typeName;
        H_Uint8 typeGroup;
    } SportType;

    typedef struct
    {
        sport_category categoryId;
        char *categoryName;
    } SportCategory;

    static const SportCategory sportCategoryArray[SPORT_CATEGORY_MAX] = {
        {SPORT_CATEGORY_RUNAWAY, "id_sport_category_runaway"},     /**< 00' 跑走类. */
        {SPORT_CATEGORY_RIDING, "id_sport_category_riding"},       /**< 01' 骑行类. */
        {SPORT_CATEGORY_SWIM, "id_sport_category_swim"},           /**< 02' 游泳类. */
        {SPORT_CATEGORY_OUTDOOR, "id_sport_category_outdoor"},     /**< 03' 户外类. */
        {SPORT_CATEGORY_INDOOR, "id_sport_category_indoor"},       /**< 04' 室内类. */
        {SPORT_CATEGORY_DANCE, "id_sport_category_dance"},         /**< 05' 舞蹈类. */
        {SPORT_CATEGORY_FIGHTWITH, "id_sport_category_fightwith"}, /**< 06' 搏击类. */
        {SPORT_CATEGORY_BALL, "id_sport_category_ball"},           /**< 07' 球类类. */
        {SPORT_CATEGORY_WATER, "id_sport_category_water"},         /**< 08' 水上类. */
        {SPORT_CATEGORY_ICESNOW, "id_sport_category_icesnow"},     /**< 09' 冰雪类. */
        {SPORT_CATEGORY_EXTREME, "id_sport_category_extreme"},     /**< 10' 极限类. */
        {SPORT_CATEGORY_LEISURE, "id_sport_category_leisure"},     /**< 11' 休闲类. */
        {SPORT_CATEGORY_CHESS, "id_sport_category_chess"},         /**< 12' 棋牌类. */
        {SPORT_CATEGORY_OTHER, "id_sport_category_other"},         /**< 13' 其他类. */
    };

    static const SportType sportTypeArray[SPORT_TYPE_SIZE] = {
        {SPORT_TYPE_AEROBICS, "aerobics", 0},                                                 /* 健身操 */
        {SPORT_TYPE_ALPINE_SKIING, "alpine_skiing", 0},                                       /* 高山滑雪 */
        {SPORT_TYPE_ARCHERY, "archery", 0},                                                   /* 射箭 */
        {SPORT_TYPE_ATV, "atv", 0},                                                           /* 沙滩车 */
        {SPORT_TYPE_BADMINTON, "badminton", 0},                                               /* 羽毛球 */
        {SPORT_TYPE_BALLET, "ballet", 0},                                                     /* 芭蕾舞 */
        {SPORT_TYPE_BALLROOM_DANCE, "ballroom_dance", 0},                                     /* 交际舞 */
        {SPORT_TYPE_BASEBALL, "baseball", 0},                                                 /* 棒球 */
        {SPORT_TYPE_BASKETBALL, "basketball", 0},                                             /* 篮球 */
        {SPORT_TYPE_BATTLE_ROPE, "battle_rope", 0},                                           /* 战绳 */
        {SPORT_TYPE_BEACH_SOCCER, "beach_soccer", 0},                                         /* 沙滩足球 */
        {SPORT_TYPE_BEACH_VOLLEYBALL, "beach_volleyball", 0},                                 /* 沙滩排球 */
        {SPORT_TYPE_BELLY_DANCE, "belly_dance", 0},                                           /* 肚皮舞 */
        {SPORT_TYPE_BIATHLON, "biathlon", 0},                                                 /* 冬季两项 */
        {SPORT_TYPE_BILLIARDS, "billiards", 0},                                               /* 台球 */
        {SPORT_TYPE_BMX, "bmx", 0},                                                           /* 小轮车 */
        {SPORT_TYPE_BOARD_GAMES, "board_games", 0},                                           /* 桌游 */
        {SPORT_TYPE_BOULDERING, "bouldering", 0},                                             /* 抱石 */
        {SPORT_TYPE_BOWLING, "bowling", 0},                                                   /* 保龄球 */
        {SPORT_TYPE_BOXING, "boxing", 0},                                                     /* 拳击 */
        {SPORT_TYPE_BREAK_DANCING, "break_dancing", 0},                                       /* 霹雳舞 */
        {SPORT_TYPE_BREAKING, "breaking", 0},                                                 /* 街舞 */
        {SPORT_TYPE_BRIDGE, "bridge", 0},                                                     /* 桥牌 */
        {SPORT_TYPE_BUNGEE_JUMPING, "bungee_jumping", 0},                                     /* 蹦极 */
        {SPORT_TYPE_CHECKERS, "checkers", 0},                                                 /* 国际跳棋 */
        {SPORT_TYPE_CHESS, "chess", 0},                                                       /* 国际象棋 */
        {SPORT_TYPE_CLIMB_THE_STAIRS, "climb_the_stairs", 0},                                 /* 爬楼 */
        {SPORT_TYPE_CLIMBING_MACHINE, "climbing_machine", 0},                                 /* 爬楼机 */
        {SPORT_TYPE_CORE_TRAINING, "core_training", 0},                                       /* 核心训练 */
        {SPORT_TYPE_CRICKET, "cricket", 0},                                                   /* 板球 */
        {SPORT_TYPE_CROSS_COUNTRY_SKIING, "cross_country_skiing", 0},                         /* 越野滑雪 */
        {SPORT_TYPE_CROSS_TRAINING, "cross_training", 0},                                     /* 交叉训练 */
        {SPORT_TYPE_CURLING, "curling", 0},                                                   /* 冰壶 */
        {SPORT_TYPE_DANCE, "dance", 0},                                                       /* 舞蹈 */
        {SPORT_TYPE_DARTS, "darts", 0},                                                       /* 飞镖 */
        {SPORT_TYPE_DIRECTIONAL_CROSS_COUNTRY, "directional_cross_country", 0},               /* 定向越野 */
        {SPORT_TYPE_DODGE_BALL, "dodge_ball", 0},                                             /* 躲避球 */
        {SPORT_TYPE_DOWNHILL, "downhill", 0},                                                 /* 速降 */
        {SPORT_TYPE_DRAGON_BOAT, "dragon_boat", 0},                                           /* 龙舟 */
        {SPORT_TYPE_DRIFTING, "drifting", 0},                                                 /* 漂流 */
        {SPORT_TYPE_DRIVING, "driving", 0},                                                   /* 驾车 */
        {SPORT_TYPE_E_SPORTS, "e_sports", 0},                                                 /* 电子竞技 */
        {SPORT_TYPE_ELLIPTICAL_MACHINE, "elliptical_machine", 0},                             /* 椭圆机 */
        {SPORT_TYPE_EQUESTRIAN_SPORTS, "equestrian_sports", 0},                               /* 马术运动 */
        {SPORT_TYPE_FENCING, "fencing", 0},                                                   /* 击剑 */
        {SPORT_TYPE_FIGHTING_EXERCISE, "fighting_exercise", 0},                               /* 搏击操 */
        {SPORT_TYPE_FIN_SWIMMING, "fin_swimming", 0},                                         /* 蹼泳 */
        {SPORT_TYPE_FISHING, "fishing", 0},                                                   /* 钓鱼 */
        {SPORT_TYPE_FLEXIBILITY_TRAINING, "flexibility_training", 0},                         /* 柔韧训练 */
        {SPORT_TYPE_FLYING_A_KITE, "flying_a_kite", 0},                                       /* 放风筝 */
        {SPORT_TYPE_FOLK_DANCE, "folk_dance", 0},                                             /* 民族舞 */
        {SPORT_TYPE_FREE_TRAINING, "free_training", 0},                                       /* 自由训练 */
        {SPORT_TYPE_FRISBEE, "frisbee", 0},                                                   /* 飞盘 */
        {SPORT_TYPE_FUJIKI, "fujiki", 0},                                                     /* 藤球 */
        {SPORT_TYPE_GATEBALL, "gateball", 0},                                                 /* 门球 */
        {SPORT_TYPE_GO, "go", 0},                                                             /* 围棋 */
        {SPORT_TYPE_GOLF, "golf", 0},                                                         /* 高尔夫 */
        {SPORT_TYPE_GOLF_SWING, "golf_swing", 0},                                             /* 高尔夫挥杆 */
        {SPORT_TYPE_GROUND_THROW, "ground_throw", 0},                                         /* 地掷球 */
        {SPORT_TYPE_GROUP_GYMNASTICS, "group_gymnastics", 0},                                 /* 团体操 */
        {SPORT_TYPE_GYMNASTICS, "gymnastics", 0},                                             /* 体操 */
        {SPORT_TYPE_HANDBALL, "handball", 0},                                                 /* 手球 */
        {SPORT_TYPE_HIGH_INTENSITY_INTERVAL_TRAINING, "high_intensity_interval_training", 0}, /* 高强度间歇训练 */
        {SPORT_TYPE_HIP_HOP, "hip_hop", 0},                                                   /* 嘻哈舞 */
        {SPORT_TYPE_HOCKEY, "hockey", 0},                                                     /* 曲棍球 */
        {SPORT_TYPE_HORIZONTAL_BAR, "horizontal_bar", 0},                                     /* 单杠 */
        {SPORT_TYPE_HULA_HOOP, "hula_hoop", 0},                                               /* 呼啦圈 */
        {SPORT_TYPE_HUNTING, "hunting", 0},                                                   /* 打猎 */
        {SPORT_TYPE_ICE_HOCKEY, "ice_hockey", 0},                                             /* 冰球 */
        {SPORT_TYPE_INDOOR_FITNESS, "indoor_fitness", 0},                                     /* 室内健身 */
        {SPORT_TYPE_INDOOR_FOOTBALL, "indoor_football", 0},                                   /* 室内足球 */
        {SPORT_TYPE_INDOOR_RIDE, "indoor_ride", 0},                                           /* 室内骑行 */
        {SPORT_TYPE_INDOOR_SKATING, "indoor_skating", 0},                                     /* 室内滑冰 */
        {SPORT_TYPE_INDOOR_SURFING, "indoor_surfing", 0},                                     /* 室内冲浪 */
        {SPORT_TYPE_INDOOR_WALKING, "indoor_walking", 0},                                     /* 室内步行 */
        {SPORT_TYPE_JAI_LI_BALL, "jai_li_ball", 0},                                           /* 回力球 */
        {SPORT_TYPE_JAZZ_DANCE, "jazz_dance", 0},                                             /* 爵士舞 */
        {SPORT_TYPE_JUDO, "judo", 0},                                                         /* 柔道 */
        {SPORT_TYPE_JUJITSU, "jujitsu", 0},                                                   /* 柔术 */
        {SPORT_TYPE_KARATE, "karate", 0},                                                     /* 空手道 */
        {SPORT_TYPE_KAYAK, "kayak", 0},                                                       /* 皮划艇 */
        {SPORT_TYPE_KICKBOXING, "kickboxing", 0},                                             /* 自由搏击 */
        {SPORT_TYPE_KICKING_SHUTTLECOCK, "kicking_shuttlecock", 0},                           /* 踢毽子 */
        {SPORT_TYPE_KITE_SURFING, "kite_surfing", 0},                                         /* 风筝冲浪 */
        {SPORT_TYPE_LATIN_DANCE, "latin_dance", 0},                                           /* 拉丁舞 */
        {SPORT_TYPE_MARTIAL_ARTS, "martial_arts", 0},                                         /* 武术 */
        {SPORT_TYPE_MIXED_AEROBIC, "mixed_aerobic", 0},                                       /* 混合有氧 */
        {SPORT_TYPE_MOTORBOAT, "motorboat", 0},                                               /* 摩托艇 */
        {SPORT_TYPE_MOUNTAIN_BIKING, "mountain_biking", 0},                                   /* 山地骑行 */
        {SPORT_TYPE_MOUNTAINEERING, "mountaineering", 0},                                     /* 登山 */
        {SPORT_TYPE_MUAY_THAI, "muay_thai", 0},                                               /* 泰拳 */
        {SPORT_TYPE_MULTISPORT, "multisport", 0},                                             /* 复合运动 */
        {SPORT_TYPE_OFF_ROAD_MOTORCYCLE, "off_road_motorcycle", 0},                           /* 越野摩托 */
        {SPORT_TYPE_OPEN_WATER_SWIMMING, "open_water_swimming", 0},                           /* 公开水域游泳 */
        {SPORT_TYPE_ORIENTEERING, "orienteering", 0},                                         /* 定向滑雪 */
        {SPORT_TYPE_OUTDOOR_CYCLING, "outdoor_cycling", 0},                                   /* 户外骑行 */
        {SPORT_TYPE_OUTDOOR_HIKING, "outdoor_hiking", 0},                                     /* 户外徒步 */
        {SPORT_TYPE_OUTDOOR_ROWING, "outdoor_rowing", 0},                                     /* 户外划船 */
        {SPORT_TYPE_OUTDOOR_RUNNING, "outdoor_running", 0},                                   /* 户外跑 */
        {SPORT_TYPE_OUTDOOR_SKATING, "outdoor_skating", 0},                                   /* 户外滑冰 */
        {SPORT_TYPE_PADDLE_BOARD_SURFING, "paddle_board_surfing", 0},                         /* 桨板冲浪 */
        {SPORT_TYPE_PARALLEL_BARS, "parallel_bars", 0},                                       /* 双杠 */
        {SPORT_TYPE_PARASAILING, "parasailing", 0},                                           /* 滑翔伞 */
        {SPORT_TYPE_PARKOUR, "parkour", 0},                                                   /* 跑酷 */
        {SPORT_TYPE_PILATES, "pilates", 0},                                                   /* 普拉提 */
        {SPORT_TYPE_POLE_DANCING, "pole_dancing", 0},                                         /* 钢管舞 */
        {SPORT_TYPE_POLO, "polo", 0},                                                         /* 马球 */
        {SPORT_TYPE_RACE_WALKING, "race_walking", 0},                                         /* 竞走 */
        {SPORT_TYPE_ROCK_CLIMBING, "rock_climbing", 0},                                       /* 攀岩 */
        {SPORT_TYPE_ROLLER_SKATING, "roller_skating", 0},                                     /* 轮滑 */
        {SPORT_TYPE_ROWING, "rowing", 0},                                                     /* 赛艇 */
        {SPORT_TYPE_ROWING_MACHINE, "rowing_machine", 0},                                     /* 划船机 */
        {SPORT_TYPE_RUGBY, "rugby", 0},                                                       /* 橄榄球 */
        {SPORT_TYPE_SAILING, "sailing", 0},                                                   /* 帆船运动 */
        {SPORT_TYPE_SANDBAG_BALL, "sandbag_ball", 0},                                         /* 沙包球 */
        {SPORT_TYPE_SHOOT, "shoot", 0},                                                       /* 射击 */
        {SPORT_TYPE_SHUFFLEBOARD, "shuffleboard", 0},                                         /* 沙狐球 */
        {SPORT_TYPE_SHUTTLECOCK, "shuttlecock", 0},                                           /* 毽球 */
        {SPORT_TYPE_SKATEBOARD, "skateboard", 0},                                             /* 滑板 */
        {SPORT_TYPE_SKIPPING_ROPE, "skipping_rope", 0},                                       /* 跳绳 */
        {SPORT_TYPE_SKYDIVING, "skydiving", 0},                                               /* 跳伞 */
        {SPORT_TYPE_SLEIGH, "sleigh", 0},                                                     /* 雪橇 */
        {SPORT_TYPE_SNORKELING, "snorkeling", 0},                                             /* 浮潜 */
        {SPORT_TYPE_SNOWBOARDING_DOUBLE, "snowboarding_double", 0},                           /* 双板滑雪 */
        {SPORT_TYPE_SNOWBOARDING_SINGLE, "snowboarding_single", 0},                           /* 单板滑雪 */
        {SPORT_TYPE_SNOWMOBILE, "snowmobile", 0},                                             /* 雪地摩托 */
        {SPORT_TYPE_SNOWMOBILES, "snowmobiles", 0},                                           /* 雪车 */
        {SPORT_TYPE_SNOWSHOE_HIKING, "snowshoe_hiking", 0},                                   /* 雪鞋健行 */
        {SPORT_TYPE_SOCCER, "soccer", 0},                                                     /* 足球 */
        {SPORT_TYPE_SOFTBALL, "softball", 0},                                                 /* 垒球 */
        {SPORT_TYPE_SOMATOSENSORY_GAME, "somatosensory_game", 0},                             /* 体感游戏 */
        {SPORT_TYPE_SPINNING_BIKE, "spinning_bike", 0},                                       /* 动感单车 */
        {SPORT_TYPE_SQUARE_DANCE, "square_dance", 0},                                         /* 广场舞 */
        {SPORT_TYPE_SQUASH, "squash", 0},                                                     /* 壁球 */
        {SPORT_TYPE_STEP_TRAINING, "step_training", 0},                                       /* 踏步训练 */
        {SPORT_TYPE_STEPPER_MACHINE, "stepper_machine", 0},                                   /* 踏步机 */
        {SPORT_TYPE_STRENGTH_TRAINING, "strength_training", 0},                               /* 力量训练 */
        {SPORT_TYPE_STRETCH, "stretch", 0},                                                   /* 拉伸 */
        {SPORT_TYPE_SURFING, "surfing", 0},                                                   /* 冲浪 */
        {SPORT_TYPE_SWIMMING_POOL, "swimming_pool", 0},                                       /* 泳池游泳 */
        {SPORT_TYPE_SWING, "swing", 0},                                                       /* 秋千 */
        {SPORT_TYPE_SWORDSMANSHIP, "swordsmanship", 0},                                       /* 剑术 */
        {SPORT_TYPE_SYNCHRONIZED_SWIMMING, "synchronized_swimming", 0},                       /* 花样游泳 */
        {SPORT_TYPE_TABLE_FOOTBALL, "table_football", 0},                                     /* 桌上足球 */
        {SPORT_TYPE_TABLE_TENNIS, "table_tennis", 0},                                         /* 乒乓球 */
        {SPORT_TYPE_TAEKWONDO, "taekwondo", 0},                                               /* 跆拳道 */
        {SPORT_TYPE_TAI_CHI, "tai_chi", 0},                                                   /* 太极 */
        {SPORT_TYPE_TENNIS, "tennis", 0},                                                     /* 网球 */
        {SPORT_TYPE_TRAIL_RUNNING, "trail_running", 0},                                       /* 越野跑 */
        {SPORT_TYPE_TRAMPOLINE, "trampoline", 0},                                             /* 蹦床 */
        {SPORT_TYPE_TREADMILL, "treadmill", 0},                                               /* 跑步机（原室内跑） */
        {SPORT_TYPE_TRIATHLON, "triathlon", 0},                                               /* 铁人三项 */
        {SPORT_TYPE_TUG_OF_WAR, "tug_of_war", 0},                                             /* 拔河 */
        {SPORT_TYPE_VOLLEYBALL, "volleyball", 0},                                             /* 排球 */
        {SPORT_TYPE_WALKING, "walking", 0},                                                   /* 健走 */
        {SPORT_TYPE_WALKING_MACHINE, "walking_machine", 0},                                   /* 漫步机 */
        {SPORT_TYPE_WALL_BALL, "wall_ball", 0},                                               /* 墙球 */
        {SPORT_TYPE_WATER_POLO, "water_polo", 0},                                             /* 水球 */
        {SPORT_TYPE_WATER_SKIING, "water_skiing", 0},                                         /* 滑水 */
        {SPORT_TYPE_WRESTLING, "wrestling", 0},                                               /* 摔跤 */
        {SPORT_TYPE_YOGA, "yoga", 0},                                                         /* 瑜伽 */
        {SPORT_TYPE_ZUMBA, "zumba", 0},                                                       /* 尊巴 */
        {SPORT_TYPE_TAP_DANCE, "tap_dance", 0},                                               /* 踢踏舞 */
        {SPORT_TYPE_DISCO, "disco", 0},                                                       /* 迪斯科 */
        {SPORT_TYPE_FLOORBALL, "floor_ball", 0},                                              /* 地板球 */
        {SPORT_TYPE_MODERN_DANCE, "modern_dance", 0},                                         /* 现代舞 */
    };

    const char *sportType_getTypeIconId(int sportType);
    const char *sportType_getTypeIconSizeId(int sportType, int size);
    const char *sportType_getTypeNameChar(int sportType);
    const char *sportType_getCategoryNameChar(int sportCategory);
    const char *SportType_getTypeName(int sportType);

#ifdef __cplusplus
}
#endif

#endif