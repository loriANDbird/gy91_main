/******************************************************************************
创建者：
创建日期：2018.06.01
功能描述：
    1. 数据检测
    
*******************************************************************************/
#include "OpData.h"
#include "Config_ui.h"
#include "font.h"
#include <math.h>
#include "AimLib.h"


//////#define GY91_NEW


//瞄准镜
#define BIG_MZJ_HARF_LENGTH  	40	//瞄准镜边长的一半

#define BIG_MZJ_Y0  112//103//中心点纵坐标

//外围横线长宽
#define BIG_MZJ_WW_X_WIDTH		9
#define BIG_MZJ_WW_X_HEIGHT		6

//外围竖线长宽
#define BIG_MZJ_WW_Y_WIDTH  	6
#define BIG_MZJ_WW_Y_HEIGHT  	16

//十字线 横线 宽度和高度
#define BIG_MZJ_SZ_X_WIDTH		16
#define BIG_MZJ_SZ_X_HEIGHT		5

//十字线 竖线 宽度和高度
#define BIG_MZJ_SZ_Y_WIDTH		5
#define BIG_MZJ_SZ_Y_HEIGHT		16

#define BIG_MZJ_SZ_HARF_LENGTH	7	//十字线长度一半，不包括中心点
#define BIG_MZJ_SZ_HARF_WIDTH	2	//十字线宽度一半，不包括中心点

#define BOXINGLEN	21


//热力图
#define SAVEDATACNT (800)//
//热力图测试
const int AdcBuf[SAVEDATACNT] = {502,503,504,503,503,507,509,511,514,516,518,521,525,529,534,536,545,550,556,564,579,584,599,611,622,641,648,662,685,694,720,740,749,771,794,819,846,876,893,927,964,1004,1049,1098,1123,1204,1264,1296,1399,1474,1556,1600,1691,1789,1895,2009,2068,2197,2335,2487,2652,2832,2927,3130,3352,3470,3855,4138,4447,4785,5153,5558,6002,6488,7024,7310,7925,8596,8955,9332,10138,10569,11492,12498,13032,13586,14761,16023,16687,17372,18805,19553,21114,21925,23608,24479,25368,27195,28132,30047,32004,33994,36001,37007,39013,40011,41003,42963,43927,44878,46732,47632,48510,49365,50988,51754,53189,53855,54485,55076,56140,56612,57433,57780,58086,58564,58865,58983,58925,58688,58282,58018,57367,56984,56562,55611,55083,53927,53302,51960,51247,50507,48953,47313,46463,44712,43817,42908,41987,40120,38232,36330,34432,33489,31623,29794,28015,25459,23840,22301,20844,19471,18818,17583,17001,16442,15907,14907,14441,13578,13181,12807,12127,11821,11539,11279,10831,10481,10344,10233,10091,10059,10083,10138,10225,10492,10674,11135,11735,12087,12895,13841,14367,14926,16146,16806,17496,18970,19750,20555,22236,23989,25806,26731,28600,30483,31425,33295,35133,36033,37790,39478,40291,41853,42601,43326,44709,45998,46606,47745,48277,48780,49705,50518,50882,51219,51808,52282,52476,52771,52875,52991,53000,52927,52725,52577,52397,51945,51372,51041,50287,49866,49416,48425,47888,47321,46108,44791,43375,42632,41086,40283,38626,36909,36032,35144,33347,31531,29711,27908,26134,24403,23558,22730,21128,19610,18183,16852,16225,15050,14501,13486,13019,12581,11783,11424,10783,10501,10243,9797,9608,9296,9069,8986,8879,8856,8869,8904,9039,9136,9253,9552,9733,10161,10409,10680,10976,11645,12019,12419,13786,14300,15406,15998,17264,17938,18635,19358,20882,21678,23333,25064,25951,27757,29589,31428,32346,35064,35952,37694,39376,40983,43233,43936,45892,49086,51867,53217,53584,53691,53896,54097,54305,54413,54521,54628,54733,54935,55028,55112,55245,55320,55330,55317,55282,55131,54853,54659,54425,53834,53473,53070,52623,51604,51035,49787,48401,46899,45296,44463,42744,40964,38231,37314,36395,34564,32749,31851,30080,28350,27501,25845,24241,23462,21952,20511,18496,16674,14569,14100,12059,11705,11045,10436,9868,9334,8829,8584,8102,7864,7627,7155,6687,6454,5993,5542,5103,4684,4292,3935,3765,3451,3032,2906,2566,2367,2276,2109,1891,1763,1647,1493,1358,1277,1200,1099,1006,923,848,806,727,691,674,628,573,536,504,475,467,432,416,401,380,359,340,328,316,296,278,262,256,241,226,222,213,201,193,186,177,174,168,159,156,149,143,136,130,129,122,118,116,112,108,106,102,99,95,91,89,86,83,82,81,80,79,78,77,76,75,};
const int AdcBuf1[SAVEDATACNT] = {469,470,469,471,471,472,473,474,475,478,480,483,485,489,495,502,507,514,519,523,531,542,548,558,570,585,601,608,618,637,658,671,696,709,724,739,771,790,808,848,871,916,940,991,1017,1075,1104,1169,1203,1274,1311,1389,1431,1518,1563,1660,1760,1867,1982,2104,2168,2299,2439,2512,2587,2745,2828,3003,3095,3190,3290,3499,3608,3842,3964,4090,4221,4495,4640,4942,5261,5597,5947,6128,6312,6690,6884,7080,7482,7687,8101,8523,8953,9385,9603,10041,10479,10699,11140,11359,11796,12229,12444,12867,13074,13481,13877,14069,14438,14787,14954,15267,15554,15688,15812,16039,16139,16231,16388,16452,16556,16624,16643,16656,16638,16615,16508,16405,16344,16205,16045,15865,15668,15456,15341,15103,14847,14714,14435,14290,14140,13985,13664,13497,13151,12791,12420,12038,11844,11450,11055,10660,10464,10076,9697,9328,8973,8802,8472,8161,7868,7595,7466,7226,7005,6904,6717,6551,6406,6341,6227,6096,6033,6008,5976,5963,5964,5978,6011,6034,6093,6170,6215,6319,6439,6505,6646,6888,7064,7352,7453,7663,7998,8232,8597,8721,8973,9232,9365,9633,9905,10043,10463,10606,10750,10893,11186,11330,11622,11768,12058,12348,12632,12913,13183,13443,13692,13811,13924,14033,14139,14334,14422,14583,14655,14720,14780,14875,14911,14961,14981,14969,14952,14895,14809,14694,14555,14390,14204,13998,13773,13655,13408,13148,13015,12605,12321,11885,11735,11585,11280,10967,10651,10328,10166,10002,9672,9339,9007,8842,8677,8352,8032,7722,7571,7424,7280,7006,6875,6751,6514,6406,6303,6114,6029,5950,5812,5753,5699,5612,5579,5552,5516,5506,5524,5541,5565,5633,5783,5846,5914,6072,6158,6349,6453,6562,6794,6919,7179,7454,7597,7744,8047,8360,8521,8848,9015,9348,9683,9853,10192,10528,10698,11035,11370,11536,11864,12183,12340,12647,12944,13089,13370,13641,13899,14143,14368,14572,14756,14922,14998,15137,15257,15353,15424,15469,15488,15476,15436,15405,15323,15272,15215,15076,14910,14816,14496,14377,14254,13857,13575,13429,12979,12671,12516,12046,11732,11576,11259,10941,10781,10460,10134,9969,9636,9297,8954,8609,8262,7912,7738,7563,7218,6877,6543,6218,5904,5753,5460,5050,4921,4557,4329,4010,3910,3626,3450,3366,3202,3044,2897,2687,2554,2490,2366,2195,2141,2035,1935,1888,1794,1708,1624,1543,1465,1428,1355,1286,1221,1157,1097,1068,985,912,866,805,751,718,687,645,632,595,572,561,529,509,500,470,461,451,432,413,395,379,362,337,321,315,300,286,280,266,259,247,237,227,214,205,197,187,174,171,160,154,151,142,136,134,129,123,120,116,109,107,101,97,95,92,89,88,83,80,79,78,77,76,75,74,73,};
const int AdcBuf2[SAVEDATACNT] = {422,422,424,423,424,424,425,427,429,430,432,435,439,440,445,446,448,452,455,460,468,476,483,488,493,502,514,519,525,539,546,553,561,579,588,598,608,618,640,650,676,688,716,746,761,777,811,848,867,886,928,951,973,996,1046,1072,1126,1155,1183,1243,1273,1338,1405,1440,1511,1547,1622,1660,1737,1815,1855,1935,2056,2096,2177,2303,2345,2429,2512,2595,2721,2805,2848,2931,3015,3101,3187,3318,3403,3535,3579,3669,3760,3805,3896,3988,4080,4173,4263,4400,4492,4584,4629,4718,4805,4849,4932,5013,5053,5128,5232,5265,5356,5410,5435,5480,5518,5554,5584,5609,5620,5638,5650,5660,5664,5665,5664,5660,5645,5638,5621,5597,5584,5570,5519,5499,5458,5412,5387,5334,5307,5249,5188,5123,5090,5057,4952,4915,4840,4766,4729,4653,4578,4501,4425,4350,4313,4240,4168,4063,4028,3931,3868,3837,3777,3718,3690,3634,3609,3557,3532,3485,3462,3440,3399,3380,3345,3312,3298,3284,3263,3245,3239,3229,3226,3223,3225,3228,3239,3255,3264,3287,3314,3342,3377,3414,3433,3474,3518,3565,3614,3665,3717,3743,3797,3825,3884,3944,3974,4036,4099,4131,4195,4353,4416,4478,4539,4598,4655,4709,4763,4814,4836,4882,4924,4944,4982,5018,5033,5062,5099,5119,5135,5146,5152,5154,5157,5156,5148,5132,5125,5108,5080,5058,5033,5006,4991,4945,4910,4873,4814,4772,4750,4707,4634,4610,4534,4456,4401,4344,4286,4229,4170,4140,4080,4018,3957,3897,3807,3778,3719,3662,3607,3552,3473,3423,3398,3352,3309,3267,3246,3189,3154,3122,3079,3054,3043,3012,2996,2981,2959,2955,2953,2949,2948,2950,2954,2961,2967,2979,2992,3011,3032,3055,3068,3098,3145,3179,3233,3290,3309,3388,3485,3539,3610,3664,3683,3736,3790,3826,3885,3926,3947,4013,4060,4108,4158,4233,4259,4311,4391,4444,4495,4570,4644,4716,4738,4803,4863,4920,4971,5003,5044,5058,5093,5122,5139,5158,5172,5175,5179,5178,5175,5162,5147,5142,5112,5086,5067,5046,5011,4998,4970,4940,4906,4868,4827,4805,4735,4684,4630,4573,4481,4449,4385,4286,4217,4148,4045,4011,3942,3837,3768,3665,3562,3528,3425,3324,3258,3192,3093,3059,2962,2865,2771,2678,2589,2559,2471,2384,2328,2274,2192,2164,2054,2027,1972,1889,1863,1783,1729,1678,1626,1574,1548,1472,1423,1376,1329,1259,1193,1128,1087,1029,960,944,883,818,785,745,696,688,642,612,597,571,552,547,527,506,496,476,460,455,439,418,407,391,377,371,356,341,327,315,302,297,286,273,265,251,241,238,226,213,205,194,180,177,166,162,155,148,146,143,140,137,131,127,124,};

const int AdcBuf3[SAVEDATACNT] = {359,362,364,368,372,374,378,381,387,393,397,400,405,415,419,429,434,437,447,460,466,481,496,509,532,543,559,567,584,605,623,641,667,677,700,732,745,788,826,842,909,962,988,1011,1043,1061,1103,1142,1185,1235,1273,1314,1359,1407,1459,1548,1680,1830,1913,2001,2140,2339,2445,2666,2782,2962,3212,3472,3606,3953,4242,4536,4908,5503,5648,6268,6610,6894,7207,7567,7769,8228,8889,9179,10116,10946,11976,12335,13085,14095,14975,15935,16192,17286,17874,18490,19137,20530,21278,21664,23283,25009,25907,26822,27750,29615,30079,31918,32821,33711,35439,37055,37815,38543,40217,41400,42896,43918,44279,45212,45618,45876,46367,46605,46836,47060,47275,47477,47751,47909,48042,48143,48207,48231,48145,48030,47760,47514,47219,46485,45086,44558,43722,42543,41324,40083,39457,38829,37566,36607,35291,34617,33237,32533,31456,29986,29236,28476,26948,26189,25065,23597,22168,20792,19179,18573,17423,16089,15596,14281,13523,12675,12071,11262,11010,10411,9748,9540,9142,8668,8132,7725,7427,7040,6782,6694,6533,6388,6319,6220,6096,6038,5984,5882,5837,5779,5747,5722,5703,5685,5685,5699,5733,5793,5831,5874,5922,5977,6005,6132,6239,6354,6434,6519,6606,6786,6925,7023,7233,7342,7566,7682,7799,7973,8090,8206,8320,8428,8532,8628,8719,8883,8988,9049,9106,9240,9273,9351,9383,9389,9391,9377,9365,9348,9326,9312,9286,9219,9191,9110,9038,8999,8872,8757,8661,8561,8427,8372,8261,8114,7993,7808,7652,7588,7457,7324,7186,7007,6934,6786,6562,6487,6301,6150,5996,5797,5717,5475,5354,5276,5123,4975,4835,4667,4541,4480,4284,4185,4094,3950,3838,3802,3685,3594,3564,3494,3435,3372,3340,3307,3240,3190,3157,3092,3061,3031,3004,2978,2954,2922,2903,2870,2845,2836,2821,2811,2811,2815,2819,2821,2829,2834,2840,2849,2861,2874,2882,2895,2913,2924,2934,2956,2967,2980,2999,3012,3024,3049,3062,3074,3099,3117,3139,3150,3159,3179,3187,3198,3213,3218,3227,3237,3240,3242,3241,3237,3234,3232,3223,3213,3207,3197,3181,3173,3154,3132,3121,3090,3062,3035,2994,2962,2921,2874,2858,2811,2787,2754,2738,2701,2681,2650,2606,2560,2511,2431,2409,2350,2314,2270,2219,2153,2096,2052,2016,1966,1948,1922,1867,1847,1806,1735,1677,1631,1595,1554,1506,1491,1452,1422,1390,1374,1342,1324,1307,1269,1242,1204,1166,1122,1087,1057,1022,986,974,943,919,910,882,868,858,840,811,796,778,752,731,715,700,678,653,648,632,617,612,598,585,570,559,546,533,529,515,502,496,481,472,469,459,453,449,447,439,433,424,416,406,399,397,393,388,384,379,372,370,366,363,363,361,};
const int AdcBuf4[SAVEDATACNT] = {170,172,172,173,174,176,176,178,179,180,183,184,184,185,186,187,187,189,191,193,194,196,198,199,203,205,209,211,212,216,218,223,226,228,232,237,238,241,242,245,247,249,253,255,261,267,271,273,277,282,285,290,295,299,304,311,313,319,325,333,340,347,355,364,368,378,388,397,407,418,429,440,461,479,514,536,548,559,575,599,612,626,641,657,675,697,721,748,763,792,828,865,906,949,996,1044,1096,1150,1204,1288,1344,1400,1456,1564,1618,1697,1751,1803,1857,1911,2023,2081,2140,2202,2264,2329,2429,2495,2563,2634,2706,2777,2850,2922,2994,3067,3138,3208,3275,3340,3436,3495,3551,3603,3702,3784,3802,3838,3874,3907,3938,3966,3993,4018,4040,4060,4078,4092,4110,4118,4122,4122,4119,4113,4104,4095,4081,4057,4040,4020,3998,3973,3946,3917,3886,3853,3816,3776,3715,3669,3621,3570,3517,3462,3405,3347,3287,3227,3166,3105,3012,2951,2890,2831,2773,2715,2657,2599,2544,2462,2409,2356,2305,2254,2205,2157,2109,2062,2017,1974,1932,1893,1834,1799,1764,1732,1702,1673,1619,1584,1542,1523,1505,1487,1452,1436,1414,1399,1385,1370,1357,1343,1331,1318,1305,1294,1282,1272,1262,1252,1238,1229,1220,1212,1205,1199,1193,1188,1183,1177,1173,1170,1167,1164,1160,1156,1153,1150,1146,1143,1138,1135,1131,1125,1120,1115,1110,1105,1100,1094,1088,1081,1078,1071,1063,1054,1045,1035,1026,1016,1004,993,981,969,958,946,933,921,908,891,879,868,856,845,835,824,804,789,781,773,756,742,735,725,718,706,701,694,681,676,665,657,648,642,637,628,625,621,616,613,606,602,598,593,589,584,579,574,569,564,560,555,547,542,533,524,520,515,509,505,500,497,495,491,488,485,481,478,475,473,469,468,465,464,461,459,457,454,452,450,446,444,442,440,438,434,432,429,425,423,419,417,413,410,406,404,401,398,394,392,390,388,386,380,379,378,376,372,370,368,366,364,358,356,352,350,348,346,341,339,335,331,325,323,321,319,317,315,313,310,307,305,304,300,297,294,290,286,285,282,278,276,273,272,270,268,266,264,263,261,259,258,256,254,252,250,248,247,245,243,243,239,237,235,233,232,230,229,227,225,223,222,220,219,217,215,214,212,209,207,204,202,200,198,198,197,196,194,194,194,191,191,190,189,187,186,184,183,181,181,181,181,180,180,179,179,177,177,175,};
const int AdcBuf5[SAVEDATACNT] = {153,153,153,153,153,153,153,153,153,153,153,153,153,153,153,153,153,153,153,153,153,153,154,155,155,156,156,156,156,157,157,157,157,158,160,161,161,162,163,163,164,164,164,165,165,165,167,168,170,171,172,172,173,174,174,174,176,178,178,180,181,183,184,184,184,184,185,185,186,187,187,189,189,189,190,193,195,196,197,199,201,201,203,205,208,210,211,212,214,215,217,218,220,222,224,228,230,233,235,239,242,245,247,251,256,258,260,263,266,269,271,274,276,279,281,282,285,286,289,292,296,299,302,305,308,313,317,320,326,330,335,338,342,346,349,353,358,362,366,376,380,384,388,392,397,405,407,414,418,421,424,426,434,437,440,443,449,453,456,460,463,467,472,476,478,483,487,491,497,499,505,509,511,513,515,519,521,523,525,528,531,533,535,538,540,543,545,548,550,553,555,559,562,564,567,569,573,575,579,582,585,589,594,597,602,606,611,617,622,624,630,636,642,649,656,662,669,677,684,692,699,711,720,729,738,747,757,766,775,784,795,804,809,820,830,841,851,860,869,878,888,898,907,915,924,932,940,947,959,966,973,985,989,994,998,1004,1011,1013,1015,1017,1017,1017,1018,1019,1018,1017,1015,1014,1011,1009,1005,1002,999,995,992,987,980,974,967,959,950,940,930,921,909,897,891,879,866,855,842,829,817,805,792,780,768,755,742,730,718,706,694,676,665,654,643,633,623,613,604,595,586,574,567,560,553,546,539,532,527,522,515,513,508,503,499,495,490,485,482,478,475,471,467,465,463,460,457,454,450,448,445,443,439,437,435,432,430,426,425,424,419,415,413,412,407,405,403,399,397,394,392,392,389,388,384,381,379,377,375,372,369,366,363,362,360,358,354,352,350,348,346,342,340,338,336,334,332,328,326,324,320,317,313,310,307,304,301,297,293,291,289,284,280,278,276,271,266,264,263,262,258,256,253,251,248,245,243,240,236,235,233,231,229,228,226,226,225,223,222,221,217,216,215,212,211,211,208,208,205,204,204,202,201,198,196,195,194,194,193,193,191,191,189,188,186,186,185,185,184,182,181,179,177,177,177,175,175,173,172,170,169,167,167,165,161,161,161,};

//数据存储缓存
COVER_BUFFER ResBuf_CH1;
COVER_BUFFER ResBuf_CH2;
COVER_BUFFER ResBuf_CH3;

//数据缓存指针
COVER_BUFFER* ResBuf[3] = {
	&ResBuf_CH1,
	&ResBuf_CH2,
	&ResBuf_CH3
};

//ADC原始值
int g_iCh1AdcValOrig;
int g_iCh2AdcValOrig;
int g_iCh3AdcValOrig;
int g_iCh4AdcValOrig;
int g_iCh5AdcValOrig;
int g_iCh6AdcValOrig;
//ADC值
int g_iCh1AdcVal;
int g_iCh2AdcVal;
int g_iCh3AdcVal;
int g_iCh4AdcVal;
int g_iCh5AdcVal;
int g_iCh6AdcVal;
//ADC最大值
int g_iCh1AdcValMax;
int g_iCh2AdcValMax;
int g_iCh3AdcValMax;
int g_iCh4AdcValMax;
int g_iCh5AdcValMax;
int g_iCh6AdcValMax;
//ADC最小值
int g_iCh1AdcValMin;
int g_iCh2AdcValMin;
int g_iCh3AdcValMin;
int g_iCh4AdcValMin;
int g_iCh5AdcValMin;
int g_iCh6AdcValMin;

//SAMPLEVALADC g_SampleVal;
//位移值
unsigned int g_iCarDist = 0;
unsigned int g_iCarDistLast = 0;

unsigned int g_iCarCount = 0;
unsigned int g_iCarCountLast = 0;
short g_iCarDir = 0;
int g_iCarCountSum = 0;

unsigned int g_iCarMaxDist = 0;
unsigned int g_iCarLastDist = 0;
unsigned int g_iCarLastCount = 0;
unsigned int g_iCarCountZero = 0;

//瞄准镜坐标
static struct MZJZuoBiao{
	short sx;
	short sy;
	short width;
	short height;
}g_iMiaoZhunJingZB[15],g_iMiaoZhunJingLastZB[15];

//波形曲线纵坐标数据
const short g_BoXing_Y_Buf[BOXINGLEN] = {3,9,16,22,28,34,39,44,49,53,57,60,63,65,67,69,71,72,73,74,74};
//const short g_BoXing_Y_Buf[BOXINGLEN] = {4,12,20,28,36,43,50,56,62,67,72,76,80,83,86,88,90,92,93,94,94};
ADC_DATA_STR g_AdcData;
    
//================================================================//
//功能：获取各个通道信号量
//参数：NULL
//返回：ADC_RES_STR
//================================================================//
ADC_DATA_STR GetSignalData(void)
{
    ADC_DATA_STR AdcData;
    
    //原始数据
    AdcData.iCh1 = g_iCh1AdcVal;
    AdcData.iCh2 = g_iCh2AdcVal;
    AdcData.iCh3 = g_iCh3AdcVal;
    AdcData.iCh4 = g_iCh4AdcVal;
    AdcData.iCh5 = g_iCh5AdcVal;
    AdcData.iCh6 = g_iCh6AdcVal;

    return AdcData;
}
//================================================================//
//功能：按瞄准框位置刷新中心线
//参数：NULL
//返回：NULL
//================================================================//
void BrushZXX(int AimState)
{
	u16 x = 239;
	u16 ytop = 32+1;//开始坐标
	u16 ybot = 192-1;//结束坐标
    u16 color = 0;
    
    if(AimState != AIM){
        color = GRAY;
    }
    else{
        color = RED;
    }
    
	LCD_FillRect(x-2,ytop,5,BIG_MZJ_Y0-BIG_MZJ_HARF_LENGTH-ytop,color);
	LCD_FillRect(x-2,BIG_MZJ_Y0-BIG_MZJ_HARF_LENGTH+BIG_MZJ_SZ_Y_HEIGHT,5,BIG_MZJ_HARF_LENGTH-BIG_MZJ_SZ_Y_HEIGHT-BIG_MZJ_SZ_HARF_LENGTH,color);
	LCD_FillRect(x-2,BIG_MZJ_Y0+7+1,5,BIG_MZJ_HARF_LENGTH-BIG_MZJ_SZ_Y_HEIGHT-BIG_MZJ_SZ_HARF_LENGTH,color);
	LCD_FillRect(x-2,BIG_MZJ_Y0+BIG_MZJ_HARF_LENGTH+1,5,ybot-BIG_MZJ_Y0-1-BIG_MZJ_HARF_LENGTH,color);
}

//================================================================//
//功能：计算瞄准镜位置
//参数：左右通道信号量
//返回：NULL
//================================================================//
short GetMZJSite(int iLeftSignal,int iRightSignal,int iMzjLastSite)
{
    //计算瞄准镜 位置
//	int fbuf1 = iLeftSignal;
//	int fbuf2 = iRightSignal;
//    unsigned char MAXSITE = 159;
//    float GAIN = 4.0;
    short Res_MzjSite;
    
    static int LastMzjSite = 0;
    
    Res_MzjSite = iRightSignal - iLeftSignal;
	Res_MzjSite = (Res_MzjSite + LastMzjSite)/2;//滤波
	LastMzjSite = Res_MzjSite;

//	fbuf1 = g_fPingFangCha / (float)GAIN;//g_iSmallMzjLastSite
//	Res_MzjSite = (int)(fbuf1);
	
//	if((iLeftSignal + iRightSignal) > 110){
//        Res_MzjSite = (Res_MzjSite>> 2) + (iMzjLastSite - (iMzjLastSite >> 2));

//		if((Res_MzjSite <=5)&&(Res_MzjSite>=-5)){
//			Res_MzjSite = 0;
//		}
//	}
    
    return Res_MzjSite;
}

//================================================================//
//功能：刷新瞄准镜定位框
//参数：位置和颜色
//返回：NULL
//================================================================//
//static 
void BrushMZJ(short iSite,char AimState)
{
	u8 i;
	short x0 = 239;//中心位置
	static int iSiteLast = 238;
    short int iStep;
    u16 color = 0;
	
	u16 bkCol;
	//深色背景
	if(g_SysPara.iTheme == THEME_DARK){
		bkCol = BLACK;
	}else{
		bkCol = WHITE;
	}

//	iSite *= 10;
	iSite += x0;
	if(iSite > 449) iSite = 449;
	if(iSite < 60) iSite = 60;
	if(iSite == iSiteLast)return;
    
    if(abs(iSite-iSiteLast) > 5){
        if(iSite > iSiteLast){
            iSite = iSiteLast + 5;
        }
        else if(iSite < iSiteLast){
            iSite = iSiteLast - 5;
        }
    }
    
    if(AimState != AIM){
        color = DGREEN;
    }
    else{
        color = BLUE;
    }

	memcpy(g_iMiaoZhunJingLastZB,g_iMiaoZhunJingZB,sizeof(g_iMiaoZhunJingZB));
	
    //计算需要绘制的瞄准框位移
	g_iMiaoZhunJingZB[0].sx = iSite - BIG_MZJ_HARF_LENGTH;//左上 竖
	g_iMiaoZhunJingZB[0].sy = BIG_MZJ_Y0 - BIG_MZJ_HARF_LENGTH;
	g_iMiaoZhunJingZB[0].width = BIG_MZJ_WW_Y_WIDTH;
	g_iMiaoZhunJingZB[0].height = BIG_MZJ_WW_Y_HEIGHT;
	
	g_iMiaoZhunJingZB[1].sx = iSite - BIG_MZJ_HARF_LENGTH + BIG_MZJ_WW_Y_WIDTH;//左上 横
	g_iMiaoZhunJingZB[1].sy = BIG_MZJ_Y0 - BIG_MZJ_HARF_LENGTH;
	g_iMiaoZhunJingZB[1].width = BIG_MZJ_WW_X_WIDTH;
	g_iMiaoZhunJingZB[1].height = BIG_MZJ_WW_X_HEIGHT;
	
	g_iMiaoZhunJingZB[2].sx = iSite - BIG_MZJ_HARF_LENGTH;//左下 竖
	g_iMiaoZhunJingZB[2].sy = BIG_MZJ_Y0 + BIG_MZJ_HARF_LENGTH - BIG_MZJ_WW_Y_HEIGHT+1;
	g_iMiaoZhunJingZB[2].width = BIG_MZJ_WW_Y_WIDTH;
	g_iMiaoZhunJingZB[2].height = BIG_MZJ_WW_Y_HEIGHT;
	
	g_iMiaoZhunJingZB[3].sx = iSite - BIG_MZJ_HARF_LENGTH + BIG_MZJ_WW_Y_WIDTH;//左下 横
	g_iMiaoZhunJingZB[3].sy = BIG_MZJ_Y0 + BIG_MZJ_HARF_LENGTH - BIG_MZJ_WW_X_HEIGHT+1;
	g_iMiaoZhunJingZB[3].width = BIG_MZJ_WW_X_WIDTH;
	g_iMiaoZhunJingZB[3].height = BIG_MZJ_WW_X_HEIGHT;
	
	g_iMiaoZhunJingZB[4].sx = iSite + BIG_MZJ_HARF_LENGTH - BIG_MZJ_WW_Y_WIDTH;//右上 竖
	g_iMiaoZhunJingZB[4].sy = BIG_MZJ_Y0 - BIG_MZJ_HARF_LENGTH;
	g_iMiaoZhunJingZB[4].width = BIG_MZJ_WW_Y_WIDTH;
	g_iMiaoZhunJingZB[4].height = BIG_MZJ_WW_Y_HEIGHT;
	
	g_iMiaoZhunJingZB[5].sx = iSite + BIG_MZJ_HARF_LENGTH - BIG_MZJ_WW_Y_WIDTH - BIG_MZJ_WW_X_WIDTH;//右上 横
	g_iMiaoZhunJingZB[5].sy = BIG_MZJ_Y0 - BIG_MZJ_HARF_LENGTH;
	g_iMiaoZhunJingZB[5].width = BIG_MZJ_WW_X_WIDTH;
	g_iMiaoZhunJingZB[5].height = BIG_MZJ_WW_X_HEIGHT;
	
	g_iMiaoZhunJingZB[6].sx = iSite + BIG_MZJ_HARF_LENGTH - BIG_MZJ_WW_Y_WIDTH;//右下 竖
	g_iMiaoZhunJingZB[6].sy = BIG_MZJ_Y0 + BIG_MZJ_HARF_LENGTH - BIG_MZJ_WW_Y_HEIGHT+1;
	g_iMiaoZhunJingZB[6].width = BIG_MZJ_WW_Y_WIDTH;
	g_iMiaoZhunJingZB[6].height = BIG_MZJ_WW_Y_HEIGHT;
	
	g_iMiaoZhunJingZB[7].sx = iSite + BIG_MZJ_HARF_LENGTH - BIG_MZJ_WW_Y_WIDTH - BIG_MZJ_WW_X_WIDTH;//右下 横
	g_iMiaoZhunJingZB[7].sy = BIG_MZJ_Y0 + BIG_MZJ_HARF_LENGTH - BIG_MZJ_WW_X_HEIGHT+1;
	g_iMiaoZhunJingZB[7].width = BIG_MZJ_WW_X_WIDTH;
	g_iMiaoZhunJingZB[7].height = BIG_MZJ_WW_X_HEIGHT;
	
	g_iMiaoZhunJingZB[8].sx = iSite - BIG_MZJ_SZ_HARF_WIDTH;//中上
	g_iMiaoZhunJingZB[8].sy = BIG_MZJ_Y0 - BIG_MZJ_HARF_LENGTH;
	g_iMiaoZhunJingZB[8].width = BIG_MZJ_SZ_Y_WIDTH;
	g_iMiaoZhunJingZB[8].height = BIG_MZJ_SZ_Y_HEIGHT;
	
	g_iMiaoZhunJingZB[9].sx = iSite - BIG_MZJ_SZ_HARF_WIDTH;//中下
	g_iMiaoZhunJingZB[9].sy = BIG_MZJ_Y0 + BIG_MZJ_HARF_LENGTH - BIG_MZJ_SZ_Y_HEIGHT+1;
	g_iMiaoZhunJingZB[9].width = BIG_MZJ_SZ_Y_WIDTH;
	g_iMiaoZhunJingZB[9].height = BIG_MZJ_SZ_Y_HEIGHT;
	
	g_iMiaoZhunJingZB[10].sx = iSite - BIG_MZJ_HARF_LENGTH;//中左
	g_iMiaoZhunJingZB[10].sy = BIG_MZJ_Y0 - BIG_MZJ_SZ_HARF_WIDTH;
	g_iMiaoZhunJingZB[10].width = BIG_MZJ_SZ_X_WIDTH;
	g_iMiaoZhunJingZB[10].height = BIG_MZJ_SZ_X_HEIGHT;
	
	g_iMiaoZhunJingZB[11].sx = iSite + BIG_MZJ_HARF_LENGTH - BIG_MZJ_SZ_X_WIDTH;//中右
	g_iMiaoZhunJingZB[11].sy = BIG_MZJ_Y0 - BIG_MZJ_SZ_HARF_WIDTH;
	g_iMiaoZhunJingZB[11].width = BIG_MZJ_SZ_X_WIDTH;
	g_iMiaoZhunJingZB[11].height = BIG_MZJ_SZ_X_HEIGHT;
	
	g_iMiaoZhunJingZB[12].sx = iSite - BIG_MZJ_SZ_HARF_WIDTH;//中竖
	g_iMiaoZhunJingZB[12].sy = BIG_MZJ_Y0 - BIG_MZJ_SZ_HARF_LENGTH;
	g_iMiaoZhunJingZB[12].width = BIG_MZJ_SZ_Y_WIDTH;
	g_iMiaoZhunJingZB[12].height = BIG_MZJ_SZ_HARF_LENGTH * 2 + 1;
	
	g_iMiaoZhunJingZB[13].sx = iSite - BIG_MZJ_SZ_HARF_LENGTH;//中横
	g_iMiaoZhunJingZB[13].sy = BIG_MZJ_Y0 - BIG_MZJ_SZ_HARF_WIDTH;
	g_iMiaoZhunJingZB[13].width = BIG_MZJ_SZ_HARF_LENGTH * 2 + 1;
	g_iMiaoZhunJingZB[13].height = BIG_MZJ_SZ_X_HEIGHT;
	
    //计算需要擦除的瞄准框位移
    iStep = iSite - iSiteLast;
	if(iStep > 0){//瞄准镜向右移动
		for(i=0;i<14;i++){
            if(i==1){
                g_iMiaoZhunJingLastZB[1].sx = g_iMiaoZhunJingZB[0].sx + g_iMiaoZhunJingZB[0].width;
            }
            else if(i==3){
                g_iMiaoZhunJingLastZB[3].sx = g_iMiaoZhunJingZB[2].sx + g_iMiaoZhunJingZB[2].width;
            }
            if(abs(iStep) < g_iMiaoZhunJingZB[i].width){
                g_iMiaoZhunJingLastZB[i].width = iStep;
            }
		}
	}
	else{//瞄准镜向左移动
		for(i=0;i<14;i++){
            if(i==5){
                if(abs(iStep) < g_iMiaoZhunJingZB[i].width){
                    g_iMiaoZhunJingLastZB[5].width = g_iMiaoZhunJingZB[4].sx - g_iMiaoZhunJingLastZB[5].sx;
                }
            }
            else if(i==7){
                if(abs(iStep) < g_iMiaoZhunJingZB[i].width){
                    g_iMiaoZhunJingLastZB[7].width = g_iMiaoZhunJingZB[6].sx - g_iMiaoZhunJingLastZB[7].sx;
                }
            }
//            if(abs(iStep) < g_iMiaoZhunJingZB[i].width){
//                g_iMiaoZhunJingLastZB[i].width = iStep;
//            }
		}
	}
    
	for(i=0;i<14;i++){
		LCD_FillRectMZJ(g_iMiaoZhunJingLastZB[i].sx,g_iMiaoZhunJingLastZB[i].sy,g_iMiaoZhunJingLastZB[i].width,g_iMiaoZhunJingLastZB[i].height,bkCol);
		LCD_FillRectMZJ(g_iMiaoZhunJingZB[i].sx,g_iMiaoZhunJingZB[i].sy,g_iMiaoZhunJingZB[i].width,g_iMiaoZhunJingZB[i].height,color);
	}
    
    iSiteLast = iSite;
}


//================================================================//
//功能：计算瞄准框位移并刷新
//参数：NULL
//返回：NULL
//================================================================//
char UpdateMZK(unsigned char ucChannel)
{
    short iCurSite = 0;
    unsigned char ucAimState = FAR;
    int iLeftSignal,iRightSignal,uibuf1,uibuf2;
    float fLeftSignal,fRightSignal;
    
    static signed char iLastSite = 0;
    static unsigned char ucLastAimState = FAR;
//    static signed char cBrushCnt = 0;
    
//    char hBuf[18];
    static float fbuf1=0,fbuf2=0,g_fPingFangCha=0,g_fLastPingFangCha=0,mm=0;
    
    //获取各个通道信号量
    g_AdcData = GetSignalData();
    
    //选取工作通道信号
    if(ucChannel == 0){
        iLeftSignal = g_AdcData.iCh1;
        iRightSignal = g_AdcData.iCh2;
    }
    else if(ucChannel == 1){
        iLeftSignal = g_AdcData.iCh3;
        iRightSignal = g_AdcData.iCh4;
    }
    else if(ucChannel == 2){
        iLeftSignal = g_AdcData.iCh5;
        iRightSignal = g_AdcData.iCh6;
    }
    
    if((iLeftSignal<8)||(iRightSignal<8)){
        iCurSite = 0;
    }
    else{
        //计算 对数操作
        uibuf1 = (iLeftSignal & 0x1FFFF)+12;
        uibuf2 = (iRightSignal & 0x1FFFF)+12;
        
        fLeftSignal = 71 - ((float)20.0 * log10(uibuf1) - 24.7);
        fRightSignal = 71 - ((float)20.0 * log10(uibuf2) - 24.7);

        //计算瞄准镜 位置
        fbuf1 = fLeftSignal;
        fbuf2 = fRightSignal;

        g_fPingFangCha = (fbuf1 * fbuf1 - fbuf2 * fbuf2)*2;

        g_fPingFangCha = (float)g_fPingFangCha*0.5 + (float)g_fLastPingFangCha*0.5;//去除低通滤波
        g_fLastPingFangCha = g_fPingFangCha;

        mm = 4.0;

        iCurSite = (int)(g_fPingFangCha / (float)mm);//g_iSmallMzjLastSite
    }
    
//    sprintf(hBuf,"%3d,%3d,%3d,%.1f,%.1f,%.1f      ",iCurSite,iLeftSignal,iRightSignal,fbuf1,fbuf2,g_fPingFangCha);
//    LCD_SetColor(BLUE,BLACK);
//    LCD_ShowString(ASC24x12,0,166,0,hBuf);
 
    if(iCurSite < -177){
        iCurSite = -177;
        ucAimState = FAR;
    }
    else if(iCurSite > 177){
        iCurSite = 177;
        ucAimState = FAR;
    }
    else{
        if((abs(iCurSite) < 5)&&(iLeftSignal > 5)&&(iRightSignal > 5)){
            ucAimState = AIM;
        }
        else{
            ucAimState = FAR;
        }
    }

//    BrushMZJ(iCurSite,DGREEN);

    //根据位移刷新中心线
    if(ucAimState != ucLastAimState){
        ucLastAimState = ucAimState;
        BrushZXX(ucAimState);
    }

    //根据位移和显示状态刷新瞄准框
    if(iCurSite != iLastSite){
        iLastSite = iCurSite;
        
        BrushMZJ(iCurSite,ucAimState);
//        cBrushCnt = 1;
    }
    BrushMZJ(iCurSite,ucAimState);
    
    return ucAimState;
}


//================================================================//
//功能：刷新位移刻度线
//参数：page:页码(1-25)
//返回：NULL
//================================================================//
void BrushKeDu(u8 page)
{
/*	u8 i;
	u16 dist;
	u16 distMin = (page-1)*g_GlobalPara.sDispScreenWide;
	LCD_SetColor(WHITE,BLACK);
//	LCD_ShowNnNums(ASC12x6,60-2,272,0,2,page);
	LCD_FillRect(60,300,420,12,WHITE);//清除刻度
	for(i=0;i<=40;i++){
		dist = distMin + i*10;
		if(i%5 == 0){
			LCD_FillRect(70+i*10,312,1,8,BLACK);//长刻度
			if(i%10 == 0){
				if(dist == 0)LCD_ShowNnNums(ASC12x6,70-2,300,0,1,0);
				else if(dist < 1000)LCD_ShowNnNums(ASC12x6,70+i*10-8,300,0,3,dist);
				else if(dist < 10000){
					if(i == 0){
						LCD_ShowNnNums(ASC12x6,70+i*10-9,300,0,4,dist);
					}else if(i == 40){
						LCD_ShowNnNums(ASC12x6,70+i*10-14,300,0,4,dist);
					}else {
						LCD_ShowNnNums(ASC12x6,70+i*10-11,300,0,4,dist);
					}
				}
				else if(dist == 10000)LCD_ShowNnNums(ASC12x6,70+i*10-20,300,0,5,dist);
				else break;
			}
		}else{
			LCD_FillRect(70+i*10,316,1,4,BLACK);//短刻度
		}
	}
	*/
}

//================================================================//
//功能：显示厚度数据
//参数：厚度值
//返回：NULL
//================================================================//
void DispHouduRes(short houdu)
{
	char hBuf[8];
	sprintf(hBuf,"%3d",houdu);
	if((g_SysPara.SampleDispMode == TIAO_POU)||(g_SysPara.SampleDispMode == TIAO_BO)){
		LCD_SetColor(BLACK,WHITE);
	}else{
		LCD_SetColor(WHITE,BLACK);
	}
	
	LCD_ShowString(ASC24x12,384,166,0,hBuf);
	LCD_ShowString(ASC16x8,424,174,0,"m");
	LCD_ShowString(ASC16x8,434,174,0,"m");
}
//================================================================//
//功能：显示测点计数
//参数：计数值
//返回：NULL
//================================================================//
void DispSaveDataCnt(short cnt)
{
	char cBuf[8];
	sprintf(cBuf,"%-4d",cnt);
	LCD_SetColor(LCD_ReadPoint(60,1),WHITE);
	LCD_ShowString(ASC20x10,438,7,0,cBuf);
}
//================================================================//
//功能：存储测点数据
//参数：计数值
//返回：NULL
//================================================================//
void SaveDotData(u8 ch,u8 dist,u8 houdu)
{
	u16 cnt;
	cnt = ResBuf[ch]->AllRebarCnt;
	
	ResBuf[ch]->RebarSite[cnt] = dist;
	ResBuf[ch]->RebarCover[cnt] = houdu;
	
	ResBuf[ch]->AllRebarCnt += 1;
}
//================================================================//
//功能：清除位移数据
//参数：位移值
//返回：NULL
//================================================================//
void ClearCarDist(void)
{
//	char dBuf[8];
	
	g_iCarDist = 0;
	g_iCarCountSum = 0;
	
//	sprintf(dBuf,"%5d",g_iCarDist);
//	LCD_SetColor(WHITE,BLACK);
//	LCD_ShowString(ASC20x10,330,7,0,dBuf);
//	LCD_ShowString(ASC20x10,381,7,0,"m");
//	LCD_ShowString(ASC20x10,391,7,0,"m");
}
//================================================================//
//功能：显示位移数据
//参数：位移值
//返回：NULL
//================================================================//
void DispCarDist(short dist)
{
	char dBuf[8];
	sprintf(dBuf,"%5d",dist);
	LCD_SetColor(LCD_ReadPoint(60,1),WHITE);
	LCD_ShowString(ASC20x10,330,7,0,dBuf);
	LCD_ShowString(ASC20x10,381,7,0,"m");
	LCD_ShowString(ASC20x10,391,7,0,"m");
}
//================================================================//
//功能：刷新小车图标
//参数：小车位移
//返回：NULL
//================================================================//
void BrushCar(short carSite)
{
	u16 pixSite,pixSiteLast;
	static u16 carSiteLast = 0;
	u16 colbk;
	
	if(carSite < 0)carSite = 0;
	if(g_SysPara.iTheme == THEME_DARK){
		colbk = BLACK;
	}else{
		colbk = WHITE;
	}
	
	pixSite = (carSite%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
	pixSiteLast = (carSiteLast%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
	
	if(pixSite != pixSiteLast){
		LCD_FillRect(70+pixSiteLast-5,296+4,11,4,colbk);
		LCD_FillRect(70+pixSite-5,296+4,11,4,BLUE);
	}
	
	carSiteLast = carSite;
}
//================================================================//
//功能：刷新通道选择
//参数：chNum:线圈通道(0-2),mode:0显示上中下，1不显示上中下
//返回：NULL
//================================================================//
void BrushChannelSel(u8 chNum,u8 mode)
{
	u8 i;
	u16 color;
	char *ChName[3] = {"上","中","下"};
	
//	if(mode == 0){
//		LCD_FillRect(449,33,1,141,GRAY);//竖
//	}else{
//		LCD_FillRect(471,33,1,141,GRAY);//纵
//	}

	for(i=0;i<3;i++){
		if(i == g_DetectPara.Channel){
			//深色背景
			if(g_SysPara.iTheme == THEME_DARK){
				color = GOLD;
				LCD_SetColor(GOLD,BLACK);
			}else{
				color = GRAY;
				LCD_SetColor(GRAY,BLACK);
			}
		}else{
			//深色背景
			if(g_SysPara.iTheme == THEME_DARK){
				color = GRAY;
				LCD_SetColor(GRAY,WHITE);
			}else{
				color = LGRAY;
				LCD_SetColor(LGRAY,BLACK);
			}
		}
		if(mode == 0){
			LCD_ShowText_Center(Font16,450,34+i*53,28,50,0,ChName[i]);
		}else{
			LCD_FillRect(472,34+i*53,6,50,color);
		}
	}
}
//================================================================//
//功能：画通道横线
//参数：chNum:线圈通道(0-2)
//返回：NULL
//================================================================//
void DispChannelLine(u8 chNum)
{
	u8 i;
	for(i=0;i<3;i++){
		if(i==chNum){
			//深色背景
			if(g_SysPara.iTheme == THEME_DARK){
				LCD_FillRect(60,59+i*53,411,1,LGRAY);
			}else{
				LCD_FillRect(60,59+i*53,411,1,DGRAY);
			}
		}else{
//			LCD_FillRect(60,59+i*45,411,1,WHITE);
		}
	}
}
//================================================================//
//功能：删除道横线
//参数：chNum:线圈通道(0-2)
//返回：NULL
//================================================================//
void DeleteChannelLine(u8 chNum)
{
	u8 i;
	for(i=0;i<3;i++){
		if(i==chNum){
			//深色背景
			if(g_SysPara.iTheme == THEME_DARK){
				LCD_FillRect(60,59+i*53,411,1,BLACK);
			}else{
				LCD_FillRect(60,59+i*53,411,1,WHITE);
			}
		}else{
//			LCD_FillRect(60,59+i*45,411,1,WHITE);
		}
	}
}
//================================================================//
//功能：颜色转换(蓝色到黑色过度)
//参数：厚度值
//返回：NULL
//================================================================//
u16 ColorConverter(u16 ref)
{
	u16 R = 0,G = 0,B = 0,color;

	if(ref > 240){
		ref = 240;
	}
	if(ref < 100){
		R = 31 - (u8)(31.0f*(float)ref/100);//R,G=2R
	}
	if((ref >= 60)&&(ref < 160)){
		B = (u8)(31.0f*(float)(ref-60)/100);//R,G=2R
	}
	if((ref > 0)&&(ref < 80)){
		G = (u8)(16.0f*(float)(ref-0)/80);//R,G=2R
	}
	if((ref >= 80)&&(ref < 120)){
		G = 16 - (u8)(16.0f*(float)(ref-80)/40);//R,G=2R
	}
	if(ref > 120){
		B = 16 - (u8)(16.0f*(float)(ref-120)/360);//R,G=2R
	}
	
	color = (R<<11)+(G<<5)+B;
	return color;
}
//================================================================//
//功能：清除曲线区域
//参数：void
//返回：NULL
//================================================================//
void ClearCurvePage(void)
{
	if(g_SysPara.iTheme == THEME_DARK){
		LCD_FillRect(60,200,420,98,BLACK);//清空
		LCD_FillRect(60,299,420,21,BLACK);//清空
		LCD_FillRect(60,298,420,1,WHITE);//横
	}else{
		LCD_FillRect(60,200,420,98,WHITE);//清空
		LCD_FillRect(60,299,420,21,WHITE);//清空
		LCD_FillRect(60,298,420,1,BLACK);//横
	}
}

//================================================================//
//功能：刷新剖面图标
//参数：小车位移：1mm->1pix
//返回：NULL
//================================================================//
void BrushPouMian(u8 carPage,short rebarSite,short rebarSiteLast,short houDu)
{	//剖面显示区域：sx=70,sy=210+20,ex=470,ey=284+4;(圆点中心坐标，圆点直径13)
	//厚度范围：160mm
	short x,xl,y,xc;
	u8 pmPage,pmPageLast;
	short pixSite,pixSiteLast;
	char tempBuf[8];
	u8 samePageFlag = 0;
	
	u16 colorBk,colorHd,colorJj,colorLn;
	if(g_SysPara.iTheme == THEME_DARK){
		colorBk = BLACK;
		colorHd = GOLD;
		colorJj = BLUE;
		colorLn = LGRAY;
	}else{
		colorBk = WHITE;
		colorHd = BLACK;
		colorJj = BLACK;
		colorLn = BLACK;
	}
	
	pmPage = rebarSite/g_GlobalPara.sDispScreenWide+1;
	pmPageLast = rebarSiteLast/g_GlobalPara.sDispScreenWide+1;
	
	if(pmPage != carPage)return;
	
	if(pmPage == pmPageLast){//在同一页
		samePageFlag = 1;
	}else{
		samePageFlag = 0;
		ClearCurvePage();//翻页清屏
	}
	
	pixSite = (rebarSite%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
	pixSiteLast = (rebarSiteLast%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
	
	x = 70 + pixSite;
	y = 210+20 + (short)((float)houDu/HOUDUMAX*(70-20));
	if(samePageFlag == 1){
		xl = 70 + pixSiteLast;
	}else{
		xl = 70;
	}
	if(rebarSiteLast == 0){
		LCD_DrawLine(xl,288-6,xl,298,colorLn);//开头竖线
		LCD_SetColor(colorBk,colorLn);
		LCD_ShowString(ASC12x6,70-2,306,1,"0");
	}
	
	LCD_SetColor(colorBk,RED);
	LCD_ShowChinese(Font14,x-6,y-6,0,(u8*)"點");
//	LCD_DrawLine(x,210-6,x,y-6,LGRAY);//點上线
	LCD_DrawLine(x,y+7+1,x,297,colorLn);//點下线
	
	//显示厚度值
	sprintf(tempBuf,"%d",houDu);
	xc = x - strlen(tempBuf)*8/2;//8是字体宽度16x8
	if(xc < 60)xc = 60;
	LCD_SetColor(colorBk,colorHd);
	LCD_ShowString(ASC16x8,xc,y-6-16,1,tempBuf);//厚度值

	//显示位移
	sprintf(tempBuf,"%d",rebarSite);
	xc = x - strlen(tempBuf)*6/2 + 1;//6是字体宽度12x6
	if(xc < 60)xc = 60;
	LCD_SetColor(colorBk,colorLn);
	LCD_ShowString(ASC12x6,xc,306,1,tempBuf);
	
	//显示距离
	LCD_DrawLine(xl,288,x,288,colorLn);//距离线
//	if((xl != 70)||(rebarSiteLast == 0)){
//		LCD_DrawPoint(xl+2,288-1,RED);//箭头
//		LCD_DrawPoint(xl+2,288+1,RED);//箭头
//		LCD_DrawPoint(xl+3,288-2,RED);//箭头
//		LCD_DrawPoint(xl+3,288+2,RED);//箭头
//	}
//	LCD_DrawPoint(x-2,288-1,RED);//箭头
//	LCD_DrawPoint(x-2,288+1,RED);//箭头
//	LCD_DrawPoint(x-3,288-2,RED);//箭头
//	LCD_DrawPoint(x-3,288+2,RED);//箭头
	sprintf(tempBuf,"%d",rebarSite-rebarSiteLast);
	xc = xl + (x-xl)/2 - strlen(tempBuf)*8/2 + 1;//6是字体宽度12x6
	if(xc < 60)xc = 60;
	LCD_SetColor(colorBk,colorJj);
	LCD_ShowString(ASC12x6,xc,288-16+2,1,tempBuf);
}

//================================================================//
//功能：刷新整页剖面图标
//参数：每页小车开始位移：1mm->1pix，页宽度400
//返回：NULL
//================================================================//
void BrushPouMianPage(short startSite,COVER_BUFFER* dataBuf)
{	//剖面显示区域：sx=70,sy=210+20,ex=470,ey=284+4;(圆点中心坐标，圆点直径13)
	//厚度范围：160mm
	short i,dotCnt = 0;
	short siteCur,siteLast,sSite,eSite,siteDif;
//	short siteDif;
	short x,xl,y,xc;
	short pixSite,pixSiteLast;
	char tempBuf[8];
	
	u16 colorBk,colorHd,colorJj,colorLn;
	if(g_SysPara.iTheme == THEME_DARK){
		colorBk = BLACK;
		colorHd = GOLD;
		colorJj = BLUE;
		colorLn = LGRAY;
	}else{
		colorBk = WHITE;
		colorHd = BLACK;
		colorJj = BLACK;
		colorLn = BLACK;
	}
	
	sSite = startSite;
	if(startSite%g_GlobalPara.sDispScreenWide != 0){
		sSite = startSite-(startSite%g_GlobalPara.sDispScreenWide);
	}
	eSite = sSite + g_GlobalPara.sDispScreenWide;
	siteLast = sSite;
	pixSiteLast = 0;
	if((sSite == 0)&&(dataBuf->AllRebarCnt > 0)){
		LCD_DrawLine(70,288-6,70,298,colorLn);//开头竖线
		LCD_SetColor(colorBk,colorLn);
		LCD_ShowString(ASC12x6,70-2,306,1,"0");
	}
	
	for(i=0;i<dataBuf->AllRebarCnt;i++){
		if((dataBuf->RebarSite[i] >= sSite)&&(dataBuf->RebarSite[i] < eSite)){
			dotCnt += 1;
			if(dotCnt == 1){//找到本页第一个点
				if(i > 0){
					siteLast = dataBuf->RebarSite[i-1];
				}else{
					siteLast = 0;
				}
			}
			siteCur = dataBuf->RebarSite[i];
			siteDif = siteCur - siteLast;
			pixSite = (siteCur%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
			x = 70 + pixSite;
			y = 210+20 + (short)((float)dataBuf->RebarCover[i]/HOUDUMAX*(70-20));
			xl = 70 + pixSiteLast;
			
			siteLast = siteCur;
			pixSiteLast = pixSite;
		}else if(dataBuf->RebarSite[i] >= eSite){//下页第一个
//			dotCnt += 1;
			if(dotCnt == 0){//本页无测点
				if(i > 0){
					siteLast = dataBuf->RebarSite[i-1];
				}else{
					siteLast = 0;
				}
			}
			siteCur = dataBuf->RebarSite[i];
			siteDif = siteCur - siteLast;
			pixSite = g_GlobalPara.sDispScreenWide-1;
			x = 70 + pixSite;
			y = 210+20 + (short)((float)dataBuf->RebarCover[i]/HOUDUMAX*(70-20));
			xl = 70 + pixSiteLast;
			
			//显示距离
			LCD_DrawLine(xl,288,x,288,colorLn);//距离线
//			if((xl != 70)||(sSite == 0)){
//				LCD_DrawPoint(xl+2,288-1,RED);//箭头
//				LCD_DrawPoint(xl+2,288+1,RED);//箭头
//				LCD_DrawPoint(xl+3,288-2,RED);//箭头
//				LCD_DrawPoint(xl+3,288+2,RED);//箭头
//			}
			sprintf(tempBuf,"%d",siteDif);
			xc = xl + siteDif/2 - strlen(tempBuf)*8/2 + 1;//6是字体宽度12x6
			if(xc < 60)xc = 60;
			if(xc > 470)break;
			LCD_SetColor(colorBk,colorJj);
			LCD_ShowString(ASC12x6,xc,288-16+2,1,tempBuf);
			break;
		}else{
			continue;
		}
		
		LCD_SetColor(colorBk,RED);
		LCD_ShowChinese(Font14,x-6,y-6,0,(u8*)"點");
//		LCD_DrawLine(x,210-6,x,y-6,LGRAY);//點上线
		LCD_DrawLine(x,y+7+1,x,297,colorLn);//點下线
		
		//显示厚度值
		sprintf(tempBuf,"%d",dataBuf->RebarCover[i]);
		xc = x - strlen(tempBuf)*8/2;//8是字体宽度16x8
		if(xc < 60)xc = 60;
		LCD_SetColor(colorBk,colorHd);
		LCD_ShowString(ASC16x8,xc,y-6-16,1,tempBuf);//厚度值
		
		//显示位移
		sprintf(tempBuf,"%d",dataBuf->RebarSite[i]);
		xc = x - strlen(tempBuf)*6/2 + 1;//6是字体宽度12x6
		if(xc < 60)xc = 60;
		LCD_SetColor(colorBk,colorLn);
		LCD_ShowString(ASC12x6,xc,306,1,tempBuf);
		
		//显示距离
		LCD_DrawLine(xl,288,x,288,colorLn);//距离线
//		if((xl != 70)||(sSite == 0)){
//			LCD_DrawPoint(xl+2,288-1,RED);//箭头
//			LCD_DrawPoint(xl+2,288+1,RED);//箭头
//			LCD_DrawPoint(xl+3,288-2,RED);//箭头
//			LCD_DrawPoint(xl+3,288+2,RED);//箭头
//		}
//		LCD_DrawPoint(x-2,288-1,RED);//箭头
//		LCD_DrawPoint(x-2,288+1,RED);//箭头
//		LCD_DrawPoint(x-3,288-2,RED);//箭头
//		LCD_DrawPoint(x-3,288+2,RED);//箭头
		sprintf(tempBuf,"%d",siteDif);
		xc = xl + (x-xl)/2 - strlen(tempBuf)*8/2 + 1;//6是字体宽度12x6
		if(xc < 60)xc = 60;
		if(xc > 470)break;
		LCD_SetColor(colorBk,colorJj);
		LCD_ShowString(ASC12x6,xc,288-16+2,1,tempBuf);
	}
}
//================================================================//
//功能：刷新波形图标
//参数：小车位移1mm->1pix
//返回：NULL
//================================================================//
void BrushBoXing(u8 carPage,short rebarSite,short rebarSiteLast,short houDu)
{
	//波形显示区域：sx=60,sy=204+20,ex=460,ey=293;	波形高度：90+4-20,42
	//公式：y = -0.1148xx + 6.42857x
	//公式：y = -0.15625xx + 7.5x
	//厚度范围：160mm
	short i;
	short yBot = 293+4;
	
	short x1,y1,x2,y2;
	short x0,y0,xc,x0l;
	
	u8 bxPage,bxPageLast;
	short pixSite,pixSiteLast;
	char tempBuf[8];
	u8 samePageFlag = 0;
	
	u16 colorBk,colorHd,colorJj,colorLn;
	if(g_SysPara.iTheme == THEME_DARK){
		colorBk = BLACK;
		colorHd = GOLD;
		colorJj = BLUE;
		colorLn = LGRAY;
	}else{
		colorBk = WHITE;
		colorHd = BLACK;
		colorJj = BLACK;
		colorLn = BLACK;
	}
	
	bxPage = rebarSite/g_GlobalPara.sDispScreenWide+1;
	bxPageLast = rebarSiteLast/g_GlobalPara.sDispScreenWide+1;
	
	if(bxPage != carPage)return;
	
	if(bxPage==bxPageLast){//在同一页
		samePageFlag = 1;
	}else{
		samePageFlag = 0;
		ClearCurvePage();//翻页清屏
	}
	
	pixSite = (rebarSite%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
	pixSiteLast = (rebarSiteLast%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
	
	x0 = 70 + pixSite;
//	y0 = 94 - (float)houDu/houduMax*94;//高度差
	y0 = (float)houDu/HOUDUMAX*(94-20);//高度差
	if(samePageFlag == 1){
		x0l = 70 + pixSiteLast;
	}else{
		x0l = 70;
	}
	if(rebarSiteLast == 0){
		LCD_DrawLine(x0l,288-6,x0l,298,RED);//开头竖线
		LCD_SetColor(colorBk,colorLn);
		LCD_ShowString(ASC12x6,70-2,306,1,"0");
	}
	//	float y;
//	for(i=0;i<40;i++){
//		y = -0.225*i*i + 9.0f*i;
//		sprintf((char*)g_ucUart2SendBuf,"%d,%.3f \r\n",i,y);
//		USART2_DMASendData(strlen((const char*)g_ucUart2SendBuf));
//		Sleep_ms(10);
//	}
	for(i=0;i<BOXINGLEN;i++){
		x1 = x0-i;
		y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1-i]-y0)+1;
		x2 = x1;
		y2 = yBot;
		if(x1 < 60)break;
		if(y1 > yBot)break;
		if(i == 0)LCD_DrawLine(x1,y1,x2,y2,RED);
		else LCD_DrawLine(x1,y1,x2,y2,GOLD);
	}
	for(i=0;i<BOXINGLEN;i++){
		x1 = x0+i+1;
		y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1-i]-y0)+1;
		x2 = x1;
		y2 = yBot;
		if(x1 > 479)break;
		if(y1 > yBot)break;
		if(i == 0)LCD_DrawLine(x1,y1,x2,y2,RED);
		else LCD_DrawLine(x1,y1,x2,y2,GOLD);
	}
	
	//显示厚度值
	sprintf(tempBuf,"%d",houDu);
	xc = x0 - strlen(tempBuf)*8/2;//8是字体宽度16x8
	if(xc < 60)xc = 60;
	y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1]-y0)+1-16;//188+y0;//
	LCD_SetColor(colorBk,colorHd);
	LCD_ShowString(ASC16x8,xc,y1,1,tempBuf);//厚度值
	
	//显示位移
	sprintf(tempBuf,"%d",rebarSite);
	xc = x0 - strlen(tempBuf)*6/2 + 1;//6是字体宽度12x6
	if(xc < 60)xc = 60;
	LCD_SetColor(colorBk,colorLn);
	LCD_ShowString(ASC12x6,xc,306,1,tempBuf);
	
	//显示距离
	LCD_DrawLine(x0l,288,x0,288,colorLn);//距离线
//	if((x0l == 70)&&(rebarSiteLast == 0)){
//		LCD_DrawPoint(x0l+2,288-1,RED);//箭头
//		LCD_DrawPoint(x0l+2,288+1,RED);//箭头
//		LCD_DrawPoint(x0l+3,288-2,RED);//箭头
//		LCD_DrawPoint(x0l+3,288+2,RED);//箭头
//	}
//	if(x0l != 70){
//		LCD_DrawPoint(x0l+3,288-1,RED);//箭头
//		LCD_DrawPoint(x0l+3,288+1,RED);//箭头
//		LCD_DrawPoint(x0l+4,288-2,RED);//箭头
//		LCD_DrawPoint(x0l+4,288+2,RED);//箭头
//	}
//	LCD_DrawPoint(x0-2,288-1,RED);//箭头
//	LCD_DrawPoint(x0-2,288+1,RED);//箭头
//	LCD_DrawPoint(x0-3,288-2,RED);//箭头
//	LCD_DrawPoint(x0-3,288+2,RED);//箭头
	sprintf(tempBuf,"%d",rebarSite-rebarSiteLast);
	xc = x0l + (x0-x0l)/2 - strlen(tempBuf)*8/2 + 1;//6是字体宽度12x6
	if(xc < 60)xc = 60;
	LCD_SetColor(colorBk,colorJj);
	LCD_ShowString(ASC12x6,xc,288-16+2,1,tempBuf);
}

//================================================================//
//功能：刷新波形图标
//参数：小车位移1mm->1pix
//返回：NULL
//================================================================//
void BrushBoXingPage(short startSite,COVER_BUFFER* dataBuf)
{	//波形显示区域：sx=60,sy=204,ex=460,ey=293;	波形高度：90+4-20,42
	//公式：y = -0.1148xx + 6.42857x
	//公式：y = -0.15625xx + 7.5x
	//厚度范围：160mm
	short i,j,dotCnt = 0;
	short yBot = 293+4;
	short siteCur,siteLast,sSite,eSite,siteDif;
//	short siteDif;
	short x1,y1,x2,y2;
	short x0,y0,xc,x0l;
	short pixSite,pixSiteLast;
	char tempBuf[8];
	
	u16 colorBk,colorHd,colorJj,colorLn;
	if(g_SysPara.iTheme == THEME_DARK){
		colorBk = BLACK;
		colorHd = GOLD;
		colorJj = BLUE;
		colorLn = LGRAY;
	}else{
		colorBk = WHITE;
		colorHd = BLACK;
		colorJj = BLACK;
		colorLn = BLACK;
	}
	
	sSite = startSite;
	eSite = sSite + g_GlobalPara.sDispScreenWide;
	if(startSite%g_GlobalPara.sDispScreenWide != 0){
		sSite = startSite-(startSite%g_GlobalPara.sDispScreenWide);
	}
	siteLast = sSite;
	pixSiteLast = 0;
	if((sSite == 0)&&(dataBuf->AllRebarCnt > 0)){
		LCD_DrawLine(70,288-6,70,298,RED);//开头竖线
		LCD_SetColor(colorBk,colorLn);
		LCD_ShowString(ASC12x6,70-2,306,1,"0");
	}
	
	for(i=0;i<dataBuf->AllRebarCnt;i++){
		if((dataBuf->RebarSite[i] >= sSite)&&(dataBuf->RebarSite[i] < eSite)){
			dotCnt += 1;
			if(dotCnt == 1){//找到本页第一个点
				if(i > 0){
					siteLast = dataBuf->RebarSite[i-1];
				}else{
					siteLast = 0;
				}
			}
			siteCur = dataBuf->RebarSite[i];
			siteDif = siteCur - siteLast;
			pixSite = (siteCur%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
			x0 = 70 + pixSite;
//			y0 = 94 - (float)dataBuf->RebarCover[i]/houduMax*94;
			y0 = (float)dataBuf->RebarCover[i]/HOUDUMAX*(94-20);
			x0l = 70 + pixSiteLast;
			
			siteLast = siteCur;
			pixSiteLast = pixSite;
		}else if(dataBuf->RebarSite[i] >= eSite){//下页第一个
//			dotCnt += 1;
			if(dotCnt == 0){//本页无测点
				if(i > 0){
					siteLast = dataBuf->RebarSite[i-1];
				}else{
					siteLast = 0;
				}
			}
			siteCur = dataBuf->RebarSite[i];
			siteDif = siteCur - siteLast;
			pixSite = g_GlobalPara.sDispScreenWide-1;
			x0 = 70 + pixSite;
//			y0 = 94 - (float)dataBuf->RebarCover[i]/houduMax*94;
			y0 = (float)dataBuf->RebarCover[i]/HOUDUMAX*(94-20);
			x0l = 70 + pixSiteLast;
			
			//显示距离
			LCD_DrawLine(x0l,288,x0,288,colorLn);//距离线
//			if((x0l == 70)&&(sSite == 0)){
//				LCD_DrawPoint(x0l+2,288-1,RED);//箭头
//				LCD_DrawPoint(x0l+2,288+1,RED);//箭头
//				LCD_DrawPoint(x0l+3,288-2,RED);//箭头
//				LCD_DrawPoint(x0l+3,288+2,RED);//箭头
//			}
//			if(x0l != 70){
//				LCD_DrawPoint(x0l+3,288-1,RED);//箭头
//				LCD_DrawPoint(x0l+3,288+1,RED);//箭头
//				LCD_DrawPoint(x0l+4,288-2,RED);//箭头
//				LCD_DrawPoint(x0l+4,288+2,RED);//箭头
//			}
			sprintf(tempBuf,"%d",siteDif);
			xc = x0l + siteDif/2 - strlen(tempBuf)*8/2 + 1;//6是字体宽度12x6
			if(xc < 60)xc = 60;
			if(xc > 470)break;
			LCD_SetColor(colorBk,colorJj);
			LCD_ShowString(ASC12x6,xc,288-16+2,1,tempBuf);
			break;
		}else{
			continue;
		}
		
		for(j=0;j<BOXINGLEN;j++){
			x1 = x0-j;
			y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1-j]-y0)+1;
			x2 = x1;
			y2 = yBot;
			if(x1 < 60)break;
			if(y1 > yBot)break;
			if(j == 0)LCD_DrawLine(x1,y1,x2,y2,RED);
			else LCD_DrawLine(x1,y1,x2,y2,GOLD);
		}
		for(j=0;j<BOXINGLEN;j++){
			x1 = x0+j+1;
			y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1-j]-y0)+1;
			x2 = x1;
			y2 = yBot;
			if(x1 > 479)break;
			if(y1 > yBot)break;
			if(j == 0)LCD_DrawLine(x1,y1,x2,y2,RED);
			else LCD_DrawLine(x1,y1,x2,y2,GOLD);
		}
		
		//显示厚度值
		sprintf(tempBuf,"%d",dataBuf->RebarCover[i]);
		xc = x0 - strlen(tempBuf)*8/2;//8是字体宽度16x8
		if(xc < 60)xc = 60;
		y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1]-y0)+1-16;//188+y0;//
		LCD_SetColor(colorBk,colorHd);
		LCD_ShowString(ASC16x8,xc,y1,1,tempBuf);//厚度值
		
		//显示位移
		sprintf(tempBuf,"%d",dataBuf->RebarSite[i]);
		xc = x0 - strlen(tempBuf)*6/2 + 1;//6是字体宽度12x6
		if(xc < 60)xc = 60;
		LCD_SetColor(colorBk,colorLn);
		LCD_ShowString(ASC12x6,xc,306,1,tempBuf);
		
		//显示距离
		LCD_DrawLine(x0l,288,x0,288,colorLn);//距离线
//		if((x0l == 70)&&(sSite == 0)){
//			LCD_DrawPoint(x0l+2,288-1,RED);//箭头
//			LCD_DrawPoint(x0l+2,288+1,RED);//箭头
//			LCD_DrawPoint(x0l+3,288-2,RED);//箭头
//			LCD_DrawPoint(x0l+3,288+2,RED);//箭头
//		}
//		if(x0l != 70){
//			LCD_DrawPoint(x0l+3,288-1,RED);//箭头
//			LCD_DrawPoint(x0l+3,288+1,RED);//箭头
//			LCD_DrawPoint(x0l+4,288-2,RED);//箭头
//			LCD_DrawPoint(x0l+4,288+2,RED);//箭头
//		}
//		LCD_DrawPoint(x0-2,288-1,RED);//箭头
//		LCD_DrawPoint(x0-2,288+1,RED);//箭头
//		LCD_DrawPoint(x0-3,288-2,RED);//箭头
//		LCD_DrawPoint(x0-3,288+2,RED);//箭头
		sprintf(tempBuf,"%d",siteDif);
		xc = x0l + (x0-x0l)/2 - strlen(tempBuf)*8/2 + 1;//6是字体宽度12x6
		if(xc < 60)xc = 60;
		if(xc > 470)break;
		LCD_SetColor(colorBk,colorJj);
		LCD_ShowString(ASC12x6,xc,288-16+2,1,tempBuf);
	}
}
//================================================================//
//功能：清空条形图
//参数：chNum:线圈通道(0/1/2(上中下))
//返回：NULL
//================================================================//
void ClearTiaoxingPage(u8 chNum)
{
	LCD_FillRect(60,35+chNum*52,411,52,BLACK);
}
//================================================================//
//功能：刷新条形图标
//参数：chNum:线圈通道(0/1/2(上中下))，rebarSite:钢筋位移1mm->1pix
//返回：NULL
//================================================================//
void BrushTiaoXing(u8 chNum,short rebarSite,short houdu)
{
	//条形显示区域：sx=0,sy=35,ex=479,ey=188;
	//条形宽度、高度：7,45
	u16 width;
	u16 height = 50;
	u16 x0,y0;
	u16 pixSite;
	u16 color;
	
	if(houdu > HOUDUMAX)houdu = HOUDUMAX;
	
	color = ColorConverter(houdu);
	
	pixSite = (rebarSite%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
	width = (HOUDUMAX-houdu)/30 + 2;
	if(width < 4)width = 4;
	
	x0 = 70 + pixSite - width/2;
	y0 = 32+3 + chNum*52;
//	if(chNum == 1)y0 = 34;
//	if(chNum == 2)y0 = 77;
//	if(chNum == 3)y0 = 120;
	if(x0 > 470-width)x0 = 470-width;
	LCD_FillRect(x0,y0,width,height,color);
}
//================================================================//
//功能：刷新条形图标页
//参数：chNum:线圈通道(0/1/2(上中下))，page:页码
//返回：NULL
//================================================================//
void BrushTiaoXingPage(u8 page,u8 chNum)
{
	//条形显示区域：sx=0,sy=35,ex=479,ey=188;
	//条形宽度、高度：7,45
	short i;
	short sSite,eSite;
	
	sSite = (page-1)*g_GlobalPara.sDispScreenWide;
	eSite = sSite + g_GlobalPara.sDispScreenWide;
//	ClearTiaoxingPage(chNum);
	for(i=0;i<ResBuf[chNum]->AllRebarCnt;i++){
		if((ResBuf[chNum]->RebarSite[i] >= sSite)&&(ResBuf[chNum]->RebarSite[i] < eSite)){	
			BrushTiaoXing(chNum,ResBuf[chNum]->RebarSite[i],ResBuf[chNum]->RebarCover[i]);
		}
	}
}

//================================================================//
//功能：刷新图形显示页面
//参数：page(1.2.3...),chNum:线圈通道(0/1/2(上中下))
//返回：NULL
//================================================================//
void BrushCurvePage(u8 page,u8 chNum,u8 dispMode)
{
	short SiteS;
	SiteS = (page-1)*g_GlobalPara.sDispScreenWide;
//	if((dispMode == TIAO_POU)||(dispMode == TIAO_BO)){//条形
//		BrushTiaoXingPage(page,chNum);
//	}
	if((dispMode == MIAO_POU)||(dispMode == RELI_POU)||(dispMode == TIAO_POU)){//剖面
		ClearCurvePage();//翻页清屏
		BrushPouMianPage(SiteS,ResBuf[chNum]);
	}
	if((dispMode == MIAO_BO)||(dispMode == RELI_BO)||(dispMode == TIAO_BO)){//波形
		ClearCurvePage();//翻页清屏
		BrushBoXingPage(SiteS,ResBuf[chNum]);
	}
}
//================================================================//
//功能：删除剖面图标
//参数：小车位移：1mm->1pix
//返回：NULL
//================================================================//
void DeletePouMian(COVER_BUFFER* dataBuf)
{	//剖面显示区域：sx=70,sy=210,ex=470,ey=284+4;(圆点中心坐标，圆点直径13)
	//厚度范围：160mm
	short x,xl,y,xc;
	short delDotNum;
	u8 pmPage,pmPageLast;
	u8 samePageFlag = 0;
	short pixSite,pixSiteLast;
	short rebarSite,rebarSiteLast;
	char tempBuf[8];
	
	delDotNum = dataBuf->AllRebarCnt;
//	if(delDotNum < 1)return;
	
	if(delDotNum > 0){
		rebarSite = dataBuf->RebarSite[delDotNum];
		rebarSiteLast = dataBuf->RebarSite[delDotNum-1];
		
		pmPage = rebarSite/g_GlobalPara.sDispScreenWide+1;
		pmPageLast = rebarSiteLast/g_GlobalPara.sDispScreenWide+1;
		
		if(pmPage == pmPageLast){//在同一页
			samePageFlag = 1;
		}else{
			samePageFlag = 0;
		}
		
		pixSite = (rebarSite%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
		pixSiteLast = (rebarSiteLast%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
		
		x = 70 + pixSite;
		y = 210+20 + (short)((float)dataBuf->RebarCover[delDotNum]/HOUDUMAX*(70-20));
		if(samePageFlag == 1){
			xl = 70 + pixSiteLast;
//			yl = 210 + (short)((float)dataBuf->RebarCover[delDotNum-1]/HOUDUMAX*(68+2));
		}else{
			xl = 70;
		}
	}else{
		rebarSite = dataBuf->RebarSite[delDotNum];
		rebarSiteLast = 0;
		
		pixSite = (rebarSite%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
		pixSiteLast = (rebarSiteLast%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
		
		x = 70 + pixSite;
		y = 210+20 + (short)((float)dataBuf->RebarCover[delDotNum]/HOUDUMAX*(70-20));
		xl = 70;
		
		LCD_DrawLine(xl,288-6,xl,298-1,WHITE);//删除开头竖线
		LCD_SetColor(WHITE,WHITE);
		LCD_ShowString(ASC12x6,70-2,306,1,"0");
	}
	
	LCD_SetColor(WHITE,WHITE);
	LCD_ShowChinese(Font14,x-6,y-6,0,(u8*)"點");
//	LCD_DrawLine(x,210-6,x,y-6,LGRAY);//點上线
	LCD_DrawLine(x,y+7+1,x,297,WHITE);//點下线
	
	//显示厚度值
	sprintf(tempBuf,"%d",dataBuf->RebarCover[delDotNum]);
	xc = x - strlen(tempBuf)*8/2;//8是字体宽度16x8
	if(xc < 60)xc = 60;
	LCD_SetColor(WHITE,WHITE);
	LCD_ShowString(ASC16x8,xc,y-6-16,1,tempBuf);//厚度值

	//显示位移
	sprintf(tempBuf,"%d",rebarSite);
	xc = x - strlen(tempBuf)*6/2 + 1;//6是字体宽度12x6
	if(xc < 60)xc = 60;
	LCD_SetColor(WHITE,WHITE);
	LCD_ShowString(ASC12x6,xc,306,1,tempBuf);
	
	//显示距离
	if(delDotNum > 0){
		LCD_DrawLine(xl+1,288,x,288,WHITE);//距离线
		if(samePageFlag == 1){//在同一页
			LCD_DrawPoint(xl+2,288-1,WHITE);//箭头
			LCD_DrawPoint(xl+2,288+1,WHITE);//箭头
			LCD_DrawPoint(xl+3,288-2,WHITE);//箭头
			LCD_DrawPoint(xl+3,288+2,WHITE);//箭头
		}
	}else{//只有一个构件
		LCD_DrawLine(xl,288,x,288,WHITE);//距离线
		LCD_DrawPoint(xl+2,288-1,WHITE);//箭头
		LCD_DrawPoint(xl+2,288+1,WHITE);//箭头
		LCD_DrawPoint(xl+3,288-2,WHITE);//箭头
		LCD_DrawPoint(xl+3,288+2,WHITE);//箭头
	}
	LCD_DrawPoint(x-2,288-1,WHITE);//箭头
	LCD_DrawPoint(x-2,288+1,WHITE);//箭头
	LCD_DrawPoint(x-3,288-2,WHITE);//箭头
	LCD_DrawPoint(x-3,288+2,WHITE);//箭头
	sprintf(tempBuf,"%d",rebarSite-rebarSiteLast);
	xc = xl + (x-xl)/2 - strlen(tempBuf)*8/2 + 1;//6是字体宽度12x6
	if(xc < 60)xc = 60;
	LCD_SetColor(WHITE,WHITE);
	LCD_ShowString(ASC12x6,xc,288-16+2,1,tempBuf);
}
//================================================================//
//功能：删除波形图标
//参数：小车位移1mm->1pix
//返回：NULL
//================================================================//
void DeleteBoXing(COVER_BUFFER* dataBuf)
{
	//波形显示区域：sx=60,sy=204,ex=460,ey=293;	波形高度：90+4,42
	//公式：y = -0.1148xx + 6.42857x
	//公式：y = -0.15625xx + 7.5x
	//厚度范围：160mm
	short i;
	short yBot = 293+4;

	short x1,y1,x2,y2;
	short x0,y0,xc,x0l,y0l;
	short delDotNum;
	
	u8 bxPage,bxPageLast;
	short pixSite,pixSiteLast;
	short rebarSite,rebarSiteLast;
	char tempBuf[8];
	u8 samePageFlag = 0;
	
	delDotNum = dataBuf->AllRebarCnt;
//	if(delDotNum < 1)return;
	
	if(delDotNum > 0){
		rebarSite = dataBuf->RebarSite[delDotNum];
		rebarSiteLast = dataBuf->RebarSite[delDotNum-1];
		
		bxPage = rebarSite/g_GlobalPara.sDispScreenWide+1;
		bxPageLast = rebarSiteLast/g_GlobalPara.sDispScreenWide+1;
		
		if(bxPage==bxPageLast){//在同一页
			samePageFlag = 1;
		}else{
			samePageFlag = 0;
		}
		
		pixSite = (rebarSite%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
		pixSiteLast = (rebarSiteLast%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
		
		x0 = 70 + pixSite;
		y0 = (float)dataBuf->RebarCover[delDotNum]/HOUDUMAX*(94-20);//高度差
		if(samePageFlag == 1){
			x0l = 70 + pixSiteLast;
			y0l = (float)dataBuf->RebarCover[delDotNum-1]/HOUDUMAX*(94-20);//高度差
		}else{
			x0l = 70;
		}
	}else{
		rebarSite = dataBuf->RebarSite[delDotNum];
		rebarSiteLast = 0;
		
		pixSite = (rebarSite%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
		pixSiteLast = (rebarSiteLast%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
		
		x0 = 70 + pixSite;
		y0 = (float)dataBuf->RebarCover[delDotNum]/HOUDUMAX*(94-20);//高度差
		x0l = 70;
		
		LCD_DrawLine(x0l,288-6,x0l,298-1,WHITE);//开头竖线
		LCD_SetColor(WHITE,WHITE);
		LCD_ShowString(ASC12x6,70-2,306,1,"0");
	}
	
	for(i=0;i<BOXINGLEN;i++){
		x1 = x0-i;
		y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1-i]-y0)+1;
		x2 = x1;
		y2 = yBot;
		if(x1 < 60)break;
		if(y1 > yBot)break;
		if(i == 0)LCD_DrawLine(x1,y1,x2,y2,WHITE);
		else LCD_DrawLine(x1,y1,x2,y2,WHITE);
	}
	for(i=0;i<BOXINGLEN;i++){
		x1 = x0+i+1;
		y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1-i]-y0)+1;
		x2 = x1;
		y2 = yBot;
		if(x1 > 479)break;
		if(y1 > yBot)break;
		if(i == 0)LCD_DrawLine(x1,y1,x2,y2,WHITE);
		else LCD_DrawLine(x1,y1,x2,y2,WHITE);
	}
	
	//显示厚度值
	sprintf(tempBuf,"%d",dataBuf->RebarCover[delDotNum]);
	xc = x0 - strlen(tempBuf)*8/2;//8是字体宽度16x8
	if(xc < 60)xc = 60;
	y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1]-y0)+1-16;//188+y0;//
	LCD_SetColor(WHITE,WHITE);
	LCD_ShowString(ASC16x8,xc,y1,1,tempBuf);//厚度值
	
	//显示位移
	sprintf(tempBuf,"%d",rebarSite);
	xc = x0 - strlen(tempBuf)*6/2 + 1;//6是字体宽度12x6
	if(xc < 60)xc = 60;
	LCD_SetColor(WHITE,WHITE);
	LCD_ShowString(ASC12x6,xc,306,1,tempBuf);
	
	//显示距离
	sprintf(tempBuf,"%d",rebarSite-rebarSiteLast);
	xc = x0l + (x0-x0l)/2 - strlen(tempBuf)*8/2 + 1;//6是字体宽度12x6
	if(xc < 60)xc = 60;
	LCD_SetColor(WHITE,WHITE);
	LCD_ShowString(ASC12x6,xc,288-16+2,1,tempBuf);
	
	LCD_DrawLine(x0l,288,x0,288,WHITE);//距离线
	if(delDotNum > 0){
		if(samePageFlag == 1){
			LCD_DrawPoint(x0l+2,288-1,WHITE);//箭头
			LCD_DrawPoint(x0l+2,288+1,WHITE);//箭头
			LCD_DrawPoint(x0l+3,288-2,WHITE);//箭头
			LCD_DrawPoint(x0l+3,288+2,WHITE);//箭头
			
			//补画前一波形
			for(i=0;i<BOXINGLEN;i++){
				x1 = x0l+i+1;
				y1 = yBot-(g_BoXing_Y_Buf[BOXINGLEN-1-i]-y0l)+1;
				x2 = x1;
				y2 = yBot;
				if(x1 > 479)break;
				if(y1 > yBot)break;
				if(i == 0){
					LCD_DrawLine(x1-1,y1,x2-1,y2,RED);
					LCD_DrawLine(x1,y1,x2,y2,RED);
				}
				else LCD_DrawLine(x1,y1,x2,y2,GOLD);
			}
		}
	}else{
		LCD_DrawPoint(x0l+2,288-1,WHITE);//箭头
		LCD_DrawPoint(x0l+2,288+1,WHITE);//箭头
		LCD_DrawPoint(x0l+3,288-2,WHITE);//箭头
		LCD_DrawPoint(x0l+3,288+2,WHITE);//箭头
	}
	LCD_DrawPoint(x0-2,288-1,WHITE);//箭头
	LCD_DrawPoint(x0-2,288+1,WHITE);//箭头
	LCD_DrawPoint(x0-3,288-2,WHITE);//箭头
	LCD_DrawPoint(x0-3,288+2,WHITE);//箭头
}
//================================================================//
//功能：删除条形图标
//参数：chNum:线圈通道(0/1/2(上中下))
//返回：NULL
//================================================================//
void DeleteTiaoXing(u8 chNum,COVER_BUFFER* dataBuf)
{
	//条形显示区域：sx=0,sy=35,ex=479,ey=188;
	//条形宽度、高度：7,45
	u16 width;
	u16 height = 50;
	u16 x0,y0;
	u16 pixSite;
	u16 color;
	u16 delDotNum;
	
	color = BLACK;
	delDotNum = dataBuf->AllRebarCnt;
	
	pixSite = (dataBuf->RebarSite[delDotNum]%g_GlobalPara.sDispScreenWide)/(g_GlobalPara.sDispScreenWide/400);
	width = (HOUDUMAX-dataBuf->RebarCover[delDotNum])/30 + 2;
	if(width < 4)width = 4;
	
	x0 = 70 + pixSite - width/2;
	y0 = 35 + chNum*52;
//	if(chNum == 1)y0 = 34;
//	if(chNum == 2)y0 = 77;
//	if(chNum == 3)y0 = 120;
	if(x0 > 470-width)x0 = 470-width;
	LCD_FillRect(x0,y0,width,height,color);
}
//================================================================//
//功能：小车后退删图形
//参数：小车位移：1mm->1pix
//返回：NULL
//================================================================//
void DeleteCurveOne(u8 chNum,short carSite,u8 dispMode)
{	//剖面显示区域：sx=70,sy=210,ex=470,ey=284+4;(圆点中心坐标，圆点直径13)
	//厚度范围：160mm
	u8 i;
	short saveDataCnt;
	u8 carPage,rebarPage;
	carPage = carSite/g_GlobalPara.sDispScreenWide + 1;
	for(i=0;i<3;i++){
		saveDataCnt = ResBuf[i]->AllRebarCnt;
		if(saveDataCnt < 1)return;
		if(carSite >= ResBuf[i]->RebarSite[saveDataCnt-1]){
			continue;
		}else{
			rebarPage = ResBuf[i]->RebarSite[saveDataCnt-1]/g_GlobalPara.sDispScreenWide + 1;
			if(rebarPage != carPage)continue;
			ResBuf[i]->AllRebarCnt -= 1;
			if((dispMode == TIAO_POU)||(dispMode == TIAO_BO)){//条形
				DeleteTiaoXing(i,ResBuf[i]);
				if(i == chNum){
					DispChannelLine(chNum);
				}
			}
			if((dispMode == MIAO_POU)||(dispMode == RELI_POU)||(dispMode == TIAO_POU)){//剖面
				if(i == chNum){
					DeletePouMian(ResBuf[chNum]);
				}
			}
			if((dispMode == MIAO_BO)||(dispMode == RELI_BO)||(dispMode == TIAO_BO)){//波形
				if(i == chNum){
					DeleteBoXing(ResBuf[chNum]);
				}
			}
		}
	}
}
//================================================================//
//功能：颜色转换(蓝色到黑色过度)
//参数：信号量 400
//返回：NULL
//================================================================//
u16 ColorConverter2(u16 ref)
{
	u16 R = 0,G = 0,B = 0,color;
	
	if(ref > 400){
		ref = 400;
	}
	
	if(ref < 100){
		G = (u8)(4.0f*(float)ref/100);
		B = (u8)(16.0f*(float)ref/100);//R,G=2R
		
	}else if(ref < 200){
		G = 4 + (u8)(12.0f*(float)(ref-100)/100);
		B = 16 - (u8)(16.0f*(float)(ref-100)/100);
	}else if(ref < 300){
		G = 16 - (u8)(16.0f*(float)(ref-200)/100);
	}else if(ref < 400){
	}else{
		R = 31;
	}
	if((ref >= 200)&&(ref < 400)){
		R = (u8)(31.0f*(float)(ref-200)/200);
	}
	color = (R<<11)+(G<<5)+B;
	return color;
}
//================================================================//
//功能：刷新条形图标
//参数：page:页码，gain:增益。小车位移1mm->1pix
//返回：NULL
//================================================================//1079
void BrushHeatMap(u8 page,u8 gain)
{
	//热力显示区域：sx=60,sy=40,ex=479,ey=157;
	//颜色渐变范围：RGB565,B=0x1F
	//信号范围：0-400
	u16 i,x,y,yLen = 50;
	u16 color = 0;
	int adcTemp;
	int adcMax1 = 0,adcMax2 = 0,adcMax3 = 0;
	int adcMin1 = 0xfffffff,adcMin2 = 0xfffffff,adcMin3 = 0xfffffff;
	int adcDif1,adcDif2,adcDif3;
	float toOne;
	
	int indexStep = g_GlobalPara.sDispScreenWide/400;
	int indexMin = (page-1)*g_GlobalPara.sDispScreenWide;
	int indexMax = indexMin + g_GlobalPara.sDispScreenWide-1;
    int indexSumBuf = 0;
    
    int iCh1Sum[810] = {0};
    int iCh2Sum[810] = {0};
    int iCh3Sum[810] = {0};
    
    int iCh1SumBuf = 0;
    int iCh2SumBuf = 0;
    int iCh3SumBuf = 0;
    
    int iCh1SumStart;
    int iCh2SumStart;
    int iCh3SumStart;
    
    
    if(g_GlobalPara.ucFromPageNum == UI_BROWSE10){
        SourceData.iCH1Start = FlieDataHead.iCH1Start;
        SourceData.iCH2Start = FlieDataHead.iCH2Start;
        SourceData.iCH3Start = FlieDataHead.iCH3Start;
        SourceData.iCH4Start = FlieDataHead.iCH4Start;
        SourceData.iCH5Start = FlieDataHead.iCH5Start;
        SourceData.iCH6Start = FlieDataHead.iCH6Start;
    }
    
    iCh1SumStart = SourceData.iCH1Start + SourceData.iCH2Start;
    iCh2SumStart = SourceData.iCH3Start + SourceData.iCH4Start;
    iCh3SumStart = SourceData.iCH5Start + SourceData.iCH6Start;
    
    
    
    FlieDataHead.iCH1Start = SourceData.iCH1Start;
    
//    int iCh1[500] = {0};
//    int iCh2[500] = {0};
//    int iCh3[500] = {0};
//    int iCh4[500] = {0};
//    int iCh5[500] = {0};
//    int iCh6[500] = {0};
    
//    iCh1[0] = SourceData.iCH1Start;
//    iCh2[0] = SourceData.iCH2Start;
//    iCh3[0] = SourceData.iCH3Start;
//    iCh4[0] = SourceData.iCH4Start;
//    iCh5[0] = SourceData.iCH5Start;
//    iCh6[0] = SourceData.iCH6Start;

//    if(indexMin == 0){
//        indexMin = 1;
//    }
    
    if(indexMax > g_iCarDist){
        indexMax = g_iCarDist;
    }
    
    for(i=0;i<=g_iCarDist;i++){
//        iCh1[i] = iCh1[i-1];
//        iCh2[i] = iCh2[i-1];
//        iCh3[i] = iCh3[i-1];
//        iCh4[i] = iCh4[i-1];
//        iCh5[i] = iCh5[i-1];
//        iCh6[i] = iCh6[i-1];
//        
//        iCh1[i] += SourceData.iCH1[i];
//        iCh2[i] += SourceData.iCH2[i];
//        iCh3[i] += SourceData.iCH3[i];
//        iCh4[i] += SourceData.iCH4[i];
//        iCh5[i] += SourceData.iCH5[i];
//        iCh6[i] += SourceData.iCH6[i];
        if(i==0){
            iCh1SumBuf = iCh1SumStart;
            iCh2SumBuf = iCh2SumStart;
            iCh3SumBuf = iCh3SumStart;
        }

        iCh1SumBuf += SourceData.iCH1[i];
        iCh1SumBuf += SourceData.iCH2[i];
        iCh2SumBuf += SourceData.iCH3[i];
        iCh2SumBuf += SourceData.iCH4[i];
        iCh3SumBuf += SourceData.iCH5[i];
        iCh3SumBuf += SourceData.iCH6[i];
        
        indexSumBuf = i-indexMin;
        if((i>=indexMin)&&(i<=indexMax)){
            iCh1Sum[indexSumBuf] = iCh1SumBuf;
            iCh2Sum[indexSumBuf] = iCh2SumBuf;
            iCh3Sum[indexSumBuf] = iCh3SumBuf;
        }

        if(g_iCarDist > 2299){
            g_iCarDist = 2299;
        }
    }

//    int iCh1Sub[2300] = {0};
//    int iCh2Sub[2300] = {0};
//    int iCh3Sub[2300] = {0};
    
//	if(indexMax >= SAVEDATACNT)indexMax = SAVEDATACNT-1;
	
	//测试
//	if(page == 1){
//		memcpy(iCh1Sum,AdcBuf,sizeof(AdcBuf));
//		memcpy(iCh2Sum,AdcBuf1,sizeof(AdcBuf1));
//		memcpy(iCh3Sum,AdcBuf2,sizeof(AdcBuf2));
//	}else{
//		memcpy(iCh1Sum,AdcBuf3,sizeof(AdcBuf3));
//		memcpy(iCh2Sum,AdcBuf4,sizeof(AdcBuf4));
//		memcpy(iCh3Sum,AdcBuf5,sizeof(AdcBuf5));
//	}
    
     for(i=indexMin;i<=indexMax;i++){
		if(iCh1Sum[i-indexMin] > adcMax1){
			adcMax1 = iCh1Sum[i-indexMin];
		}
		if(iCh2Sum[i-indexMin] > adcMax2){
			adcMax2 = iCh2Sum[i-indexMin];
		}
		if(iCh3Sum[i-indexMin] > adcMax3){
			adcMax3 = iCh3Sum[i-indexMin];
		}
		if(iCh1Sum[i-indexMin] < adcMin1){
			adcMin1 = iCh1Sum[i-indexMin];
		}
		if(iCh2Sum[i-indexMin] < adcMin2){
			adcMin2 = iCh2Sum[i-indexMin];
		}
		if(iCh3Sum[i-indexMin] < adcMin3){
			adcMin3 = iCh3Sum[i-indexMin];
		}
	}
	adcDif1 = adcMax1 - adcMin1;
	adcDif2 = adcMax2 - adcMin2;
	adcDif3 = adcMax3 - adcMin3;
    //end

	//深色背景
//	if(g_SysPara.iTheme == THEME_DARK){
//		LCD_FillRect(60,35,5,154,BLACK);
//		LCD_FillRect(465,35,5,154,BLACK);
//	}else{
//		LCD_FillRect(60,35,5,154,WHITE);
//		LCD_FillRect(465,35,5,154,WHITE);
//	}

	y = 35;
	for(i=0;i<400;i++){
		if(((indexMin + i*indexStep) > indexMax)||((indexMin + i*indexStep) > indexMax)){
			adcTemp = 0;
		}
        else{
			toOne = (float)(iCh1Sum[i*indexStep]-adcMin1)/adcDif1;//归一化
			adcTemp = toOne*(gain+1)*50;//最大值400，gain:0-7，50是倍数
		}
		x = 65 + i;
		color = ColorConverter2(adcTemp);
//		if(x > 395){y = 56;yLen = 23;}//避开按钮
		LCD_FillRect(x,y,1,yLen,color);
	}
	y = 87;
	for(i=0;i<400;i++){
		if(((indexMin + i*indexStep) > indexMax)||((indexMin + i*indexStep) > indexMax)){
			adcTemp = 0;
		}
        else{
			toOne = (float)(iCh2Sum[i*indexStep]-adcMin2)/adcDif2;//归一化
			adcTemp = toOne*(gain+1)*50;
		}
		x = 65 + i;
		color = ColorConverter2(adcTemp);
		LCD_FillRect(x,y,1,yLen,color);
	}
	y = 139;
	for(i=0;i<400;i++){
		if(((indexMin + i*indexStep) > indexMax)||((indexMin + i*indexStep) > indexMax)){
			adcTemp = 0;
		}
        else{
			toOne = (float)(iCh3Sum[i*indexStep]-adcMin3)/adcDif3;//归一化
			adcTemp = toOne*(gain+1)*50;
		}
		x = 65 + i;
		color = ColorConverter2(adcTemp);
		LCD_FillRect(x,y,1,yLen,color);
	}
//	LCD_FillRect(465,35,1,154,RED);//测试边界
}

//***************************************************************************************//
int Get_CarJourney(int carCount)
{
	int carDist;
	int carCountIncr = 0;
	
	carCountIncr = carCount - g_iCarCountLast;
	if(abs(carCountIncr) > 1000){//过零点
		if((carCount < 1000)&&(g_iCarCountLast > 64535)){//前进
			carCountIncr = carCount + 65535 - g_iCarCountLast;
		}
		if((carCount > 64535)&&(g_iCarCountLast < 1000)){//后退
			carCountIncr = carCount - 65535 + (0 - g_iCarCountLast);
		}
	}
	if(carCountIncr > 0){//前进
		g_iCarDir = 1;
	}else if(carCountIncr < 0){//后退
		g_iCarDir = -1;
	}else{
		g_iCarDir = 0;
	}
	g_iCarCountLast = carCount;
	g_iCarCountSum += carCountIncr;
	if(g_iCarCountSum < 0)g_iCarCountSum = 0;
    
#ifdef GY91_NEW
    carDist = g_iCarCountSum*(135.02f/200);
#else
    carDist = g_iCarCountSum*(120.38f/200);
#endif
    
	if(carDist < 0)carDist = 0;
	return carDist;
}

u8 Get_Adc_Data(void)
{
	char dataBuf[10];
	if(g_ucUart2RecValid == 1){
		g_ucUart2RecValid = 0;

		g_iCh1AdcValOrig = g_ucUart2RecBuf[4];
		g_iCh1AdcValOrig = (g_iCh1AdcValOrig << 8);
		g_iCh1AdcValOrig += g_ucUart2RecBuf[5];
		g_iCh1AdcValOrig = (g_iCh1AdcValOrig << 8);
		g_iCh1AdcValOrig += g_ucUart2RecBuf[6];

		g_iCh2AdcValOrig = g_ucUart2RecBuf[1];
		g_iCh2AdcValOrig = (g_iCh2AdcValOrig << 8);
		g_iCh2AdcValOrig += g_ucUart2RecBuf[2];
		g_iCh2AdcValOrig = (g_iCh2AdcValOrig << 8);
		g_iCh2AdcValOrig += g_ucUart2RecBuf[3];
		
		g_iCh3AdcValOrig = g_ucUart2RecBuf[10];
		g_iCh3AdcValOrig = (g_iCh3AdcValOrig << 8);
		g_iCh3AdcValOrig += g_ucUart2RecBuf[11];
		g_iCh3AdcValOrig = (g_iCh3AdcValOrig << 8);
		g_iCh3AdcValOrig += g_ucUart2RecBuf[12];
		
		g_iCh4AdcValOrig = g_ucUart2RecBuf[7];
		g_iCh4AdcValOrig = (g_iCh4AdcValOrig << 8);
		g_iCh4AdcValOrig += g_ucUart2RecBuf[8];
		g_iCh4AdcValOrig = (g_iCh4AdcValOrig << 8);
		g_iCh4AdcValOrig += g_ucUart2RecBuf[9];
		
		g_iCh5AdcValOrig = g_ucUart2RecBuf[16];
		g_iCh5AdcValOrig = (g_iCh5AdcValOrig << 8);
		g_iCh5AdcValOrig += g_ucUart2RecBuf[17];
		g_iCh5AdcValOrig = (g_iCh5AdcValOrig << 8);
		g_iCh5AdcValOrig += g_ucUart2RecBuf[18];
		
		g_iCh6AdcValOrig = g_ucUart2RecBuf[13];
		g_iCh6AdcValOrig = (g_iCh6AdcValOrig << 8);
		g_iCh6AdcValOrig += g_ucUart2RecBuf[14];
		g_iCh6AdcValOrig = (g_iCh6AdcValOrig << 8);
		g_iCh6AdcValOrig += g_ucUart2RecBuf[15];
		
		g_iCarCount = g_ucUart2RecBuf[19];
		g_iCarCount = (g_iCarCount << 8);
		g_iCarCount += g_ucUart2RecBuf[20];
		g_iCarDist = Get_CarJourney(g_iCarCount);
        
        //判断运动方向
//        if(g_iCarLastCount != g_iCarCount){
//            if((g_iCarCount - g_iCarLastCount) > 60000){
//                g_iCarDir = 0;
//            }
//            else if(g_iCarCount < g_iCarLastCount){
//                g_iCarDir = 0;
//            }
//            else if(g_iCarCount > g_iCarLastCount){
//                g_iCarDir = 1;
//            }
//            
//            g_iCarLastCount = g_iCarCount;
//            
//        
//            //调整位移起始点
//            if((g_iCarDist == 0)&&(g_iCarDir == 0)){
//                g_iCarCountZero = g_iCarCount;
//            }

//            //计算位移
//            if(g_iCarCount < g_iCarCountZero){
//                g_iCarCount += 0xFFFF;
//            }
//            g_iCarDist = (g_iCarCount - g_iCarCountZero)*(120.38f/200);
//            
//            //更新最大位移
//            if(g_iCarMaxDist < g_iCarDist){
//                g_iCarMaxDist = g_iCarDist;
//            }
//        }

//        if(g_iCarLastCount != g_iCarCount){
//            //更新最大位移
//            if(g_iCarMaxDist < g_iCarDist){
//                g_iCarMaxDist = g_iCarDist;
//            }
//        }
		
		g_iCh1AdcVal = g_iCh1AdcValOrig;// - g_SysPara.uiCh1Zero;
		g_iCh2AdcVal = g_iCh2AdcValOrig;// - g_SysPara.uiCh2Zero;
		g_iCh3AdcVal = g_iCh3AdcValOrig;// - g_SysPara.uiCh3Zero;
		g_iCh4AdcVal = g_iCh4AdcValOrig;// - g_SysPara.uiCh4Zero;
		g_iCh5AdcVal = g_iCh5AdcValOrig;// - g_SysPara.uiCh5Zero;
		g_iCh6AdcVal = g_iCh6AdcValOrig;// - g_SysPara.uiCh6Zero;
		
//////		g_SampleVal.AdcVal_Ch1 = (g_iCh1AdcVal+g_iCh2AdcVal)/2;
//////		g_SampleVal.AdcVal_Ch2 = (g_iCh3AdcVal+g_iCh4AdcVal)/2;
//////		g_SampleVal.AdcVal_Ch3 = (g_iCh5AdcVal+g_iCh6AdcVal)/2;
//////		g_SampleVal.CarCnt = g_iCarCount;
		
//////		if(g_iCh1AdcVal > g_iCh1AdcValMax)g_iCh1AdcValMax = g_iCh1AdcVal;
//////		if(g_iCh2AdcVal > g_iCh2AdcValMax)g_iCh2AdcValMax = g_iCh2AdcVal;
//////		if(g_iCh3AdcVal > g_iCh3AdcValMax)g_iCh3AdcValMax = g_iCh3AdcVal;
//////		if(g_iCh4AdcVal > g_iCh4AdcValMax)g_iCh4AdcValMax = g_iCh4AdcVal;
//////		if(g_iCh5AdcVal > g_iCh5AdcValMax)g_iCh5AdcValMax = g_iCh5AdcVal;
//////		if(g_iCh6AdcVal > g_iCh6AdcValMax)g_iCh6AdcValMax = g_iCh6AdcVal;
//////		
//////		if(g_iCh1AdcVal < g_iCh1AdcValMin)g_iCh1AdcValMin = g_iCh1AdcVal;
//////		if(g_iCh2AdcVal < g_iCh2AdcValMin)g_iCh2AdcValMin = g_iCh2AdcVal;
//////		if(g_iCh3AdcVal < g_iCh3AdcValMin)g_iCh3AdcValMin = g_iCh3AdcVal;
//////		if(g_iCh4AdcVal < g_iCh4AdcValMin)g_iCh4AdcValMin = g_iCh4AdcVal;
//////		if(g_iCh5AdcVal < g_iCh5AdcValMin)g_iCh5AdcValMin = g_iCh5AdcVal;
//////		if(g_iCh6AdcVal < g_iCh6AdcValMin)g_iCh6AdcValMin = g_iCh6AdcVal;
		
		
		//显示
//		if(g_bDispAdcValFlag == 1){
//			g_bDispAdcValFlag = 0;

//        LCD_SetColor(WHITE,BLACK);
//		sprintf(dataBuf,"Car:%-5d",g_iCarDist);
//		LCD_ShowString(ASC16x8,290,8,0,dataBuf);

//		BrushMZJ(GetMZJSite(1),DGREEN);
		
        
		LCD_SetColor(WHITE,ENROD);
		sprintf(dataBuf,"1:%-6d",g_iCh1AdcVal);
		LCD_ShowString(ASC16x8,20, 60, 0,dataBuf);
		sprintf(dataBuf,"2:%-6d",g_iCh2AdcVal);
		LCD_ShowString(ASC16x8,140,60, 0,dataBuf);
		sprintf(dataBuf,"3:%-6d",g_iCh3AdcVal);
		LCD_ShowString(ASC16x8,20, 100,0,dataBuf);
		sprintf(dataBuf,"4:%-6d",g_iCh4AdcVal);
		LCD_ShowString(ASC16x8,140,100,0,dataBuf);
//////		sprintf(dataBuf,"5:%-6d",g_iCh5AdcVal);
//////		LCD_ShowString(ASC16x8,20, 140,0,dataBuf);
//////		sprintf(dataBuf,"6:%-6d",g_iCh6AdcVal);
//////		LCD_ShowString(ASC16x8,140,140,0,dataBuf);
//////		
//////		LCD_SetColor(WHITE,BLUE);
//////		sprintf(dataBuf,"1:%-6d",g_iCh1AdcValOrig);
//////		LCD_ShowString(ASC16x8,20, 180,0,dataBuf);
//////		sprintf(dataBuf,"2:%-6d",g_iCh2AdcValOrig);
//////		LCD_ShowString(ASC16x8,140,180,0,dataBuf);
//////		sprintf(dataBuf,"3:%-6d",g_iCh3AdcValOrig);
//////		LCD_ShowString(ASC16x8,20, 220,0,dataBuf);
//////		sprintf(dataBuf,"4:%-6d",g_iCh4AdcValOrig);
//////		LCD_ShowString(ASC16x8,140,220,0,dataBuf);
//////		sprintf(dataBuf,"5:%-6d",g_iCh5AdcValOrig);
//////		LCD_ShowString(ASC16x8,20, 260,0,dataBuf);
//////		sprintf(dataBuf,"6:%-6d",g_iCh6AdcValOrig);
//////		LCD_ShowString(ASC16x8,140,260,0,dataBuf);
//////		
//////		LCD_SetColor(WHITE,ENROD);
//////		sprintf(dataBuf,"1:%-6d",g_iCh1AdcValMax);
//////		LCD_ShowString(ASC16x8,260,60, 0,dataBuf);
//////		sprintf(dataBuf,"2:%-6d",g_iCh2AdcValMax);
//////		LCD_ShowString(ASC16x8,360,60, 0,dataBuf);
//////		sprintf(dataBuf,"3:%-6d",g_iCh3AdcValMax);
//////		LCD_ShowString(ASC16x8,260,100,0,dataBuf);
//////		sprintf(dataBuf,"4:%-6d",g_iCh4AdcValMax);
//////		LCD_ShowString(ASC16x8,360,100,0,dataBuf);
//////		sprintf(dataBuf,"5:%-6d",g_iCh5AdcValMax);
//////		LCD_ShowString(ASC16x8,260,140,0,dataBuf);
//////		sprintf(dataBuf,"6:%-6d",g_iCh6AdcValMax);
//////		LCD_ShowString(ASC16x8,360,140,0,dataBuf);
//////		
//////		LCD_SetColor(WHITE,BLUE);
//////		sprintf(dataBuf,"1:%-6d",g_iCh1AdcValMin);
//////		LCD_ShowString(ASC16x8,260,180,0,dataBuf);
//////		sprintf(dataBuf,"2:%-6d",g_iCh2AdcValMin);
//////		LCD_ShowString(ASC16x8,360,180,0,dataBuf);
//////		sprintf(dataBuf,"3:%-6d",g_iCh3AdcValMin);
//////		LCD_ShowString(ASC16x8,260,220,0,dataBuf);
//////		sprintf(dataBuf,"4:%-6d",g_iCh4AdcValMin);
//////		LCD_ShowString(ASC16x8,360,220,0,dataBuf);
//////		sprintf(dataBuf,"5:%-6d",g_iCh5AdcValMin);
//////		LCD_ShowString(ASC16x8,260,260,0,dataBuf);
//////		sprintf(dataBuf,"6:%-6d",g_iCh6AdcValMin);
//////		LCD_ShowString(ASC16x8,360,260,0,dataBuf);
//		}
		return 1;
	}
	
	return 0;
}


//ADC原始数据
u8 Get_Adc_DataOrig(void)
{

	if(g_ucUart2RecValid == 1){
		g_ucUart2RecValid = 0;
		
		g_iCh1AdcValOrig = g_ucUart2RecBuf[4];
		g_iCh1AdcValOrig = (g_iCh1AdcValOrig << 8);
		g_iCh1AdcValOrig += g_ucUart2RecBuf[5];
		g_iCh1AdcValOrig = (g_iCh1AdcValOrig << 8);
		g_iCh1AdcValOrig += g_ucUart2RecBuf[6];
		
		g_iCh2AdcValOrig = g_ucUart2RecBuf[1];
		g_iCh2AdcValOrig = (g_iCh2AdcValOrig << 8);
		g_iCh2AdcValOrig += g_ucUart2RecBuf[2];
		g_iCh2AdcValOrig = (g_iCh2AdcValOrig << 8);
		g_iCh2AdcValOrig += g_ucUart2RecBuf[3];
		
		g_iCh3AdcValOrig = g_ucUart2RecBuf[10];
		g_iCh3AdcValOrig = (g_iCh3AdcValOrig << 8);
		g_iCh3AdcValOrig += g_ucUart2RecBuf[11];
		g_iCh3AdcValOrig = (g_iCh3AdcValOrig << 8);
		g_iCh3AdcValOrig += g_ucUart2RecBuf[12];
		
		g_iCh4AdcValOrig = g_ucUart2RecBuf[7];
		g_iCh4AdcValOrig = (g_iCh4AdcValOrig << 8);
		g_iCh4AdcValOrig += g_ucUart2RecBuf[8];
		g_iCh4AdcValOrig = (g_iCh4AdcValOrig << 8);
		g_iCh4AdcValOrig += g_ucUart2RecBuf[9];
		
		g_iCh5AdcValOrig = g_ucUart2RecBuf[16];
		g_iCh5AdcValOrig = (g_iCh5AdcValOrig << 8);
		g_iCh5AdcValOrig += g_ucUart2RecBuf[17];
		g_iCh5AdcValOrig = (g_iCh5AdcValOrig << 8);
		g_iCh5AdcValOrig += g_ucUart2RecBuf[18];
		
		g_iCh6AdcValOrig = g_ucUart2RecBuf[13];
		g_iCh6AdcValOrig = (g_iCh6AdcValOrig << 8);
		g_iCh6AdcValOrig += g_ucUart2RecBuf[14];
		g_iCh6AdcValOrig = (g_iCh6AdcValOrig << 8);
		g_iCh6AdcValOrig += g_ucUart2RecBuf[15];
		
		g_iCarCount = g_ucUart2RecBuf[19];
		g_iCarCount = (g_iCarCount << 8);
		g_iCarCount += g_ucUart2RecBuf[20];
		
		return 1;
	}
	return 0;
}

