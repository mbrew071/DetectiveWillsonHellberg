// All rights reserved hehe

#pragma once

UENUM(BlueprintType, Category="GameSettings|Graphics")
enum class EScreenResolutionEnum : uint8
{
	Custom UMETA(DisplayName = "Custom"),
	
	/*16:9*/
	Res_w16_h9_1280x720 UMETA(DisplayName = "16:9, 1280x720 (HD)"),
	Res_w16_h9_1366x768 UMETA(DisplayName = "16:9, 1366x768"),
	Res_w16_h9_1600x900 UMETA(DisplayName = "16:9, 1600x900 (HD+)"),
	Res_w16_h9_1920x1080 UMETA(DisplayName = "16:9, 1920x1080 (Full HD)"),
	Res_w16_h9_2560x1440 UMETA(DisplayName = "16:9, 2560x1440 (QHD)"),
	Res_w16_h9_3840x2160 UMETA(DisplayName = "16:9, 3840x2160 (4K UHD)"),
	Res_w16_h9_7680x4320 UMETA(DisplayName = "16:9, 7680x4320 (8K UHD)"),

	/*16:10*/
	Res_w16_h10_1280x800 UMETA(DisplayName = "16:10, 1280x800 (WXGA)"),
	Res_w16_h10_1440x900 UMETA(DisplayName = "16:10, 1440x900 (WXGA+)"),
	Res_w16_h10_1680x1050 UMETA(DisplayName = "16:10, 1680x1050 (WSXGA+)"),
	Res_w16_h10_1920x1200 UMETA(DisplayName = "16:10, 1920x1200 (WUXGA)"),
	Res_w16_h10_2560x1600 UMETA(DisplayName = "16:10, 2560x1600 (WQXGA)"),
	Res_w16_h10_3840x2400 UMETA(DisplayName = "16:10, 3840x2400 (WQUXGA)"),

	/*21:9*/
	Res_w21_h9_2560x1080 UMETA(DisplayName = "21:9, 2560x1080 (UFHD)"),
	Res_w21_h9_3440x1440 UMETA(DisplayName = "21:9, 3440x1440 (UWQHD)"),
	Res_w21_h9_5120x2160 UMETA(DisplayName = "21:9, 5120x2160 (UW5K)"),
	Res_w21_h9_6880x2880 UMETA(DisplayName = "21:9, 6880x2880"),

	/*32:9*/
	Res_w32_h9_3840x1080 UMETA(DisplayName = "32:9, 3840x1080 (DFHD)"),
	Res_w32_h9_5120x1440 UMETA(DisplayName = "32:9, 5120x1440 (DQHD)"),
	Res_w32_h9_7680x2160 UMETA(DisplayName = "32:9, 7680x2160 (DQUHD)"),

	/*4:3*/
	Res_4_3_640x480 UMETA(DisplayName = "4:3, 640x480 (VGA)"),
	Res_4_3_800x600 UMETA(DisplayName = "4:3, 800x600 (SVGA)"),
	Res_4_3_1024x768 UMETA(DisplayName = "4:3, 1024x768 (XGA)"),
	Res_4_3_1152x864 UMETA(DisplayName = "4:3, 1152x864"),
	Res_4_3_1280x960 UMETA(DisplayName = "4:3, 1280x960"),
	Res_4_3_1400x1050 UMETA(DisplayName = "4:3, 1400x1050 (SXGA+)"),
	Res_4_3_1600x1200 UMETA(DisplayName = "4:3, 1600x1200 (UXGA)"),

	/*24:10*/
	Res_w24_h10_3840x1600 UMETA(DisplayName = "24:10, 3840x1600"),
	Res_w24_h10_1920x800 UMETA(DisplayName = "24:10, 1920x800"),

	/*3:2*/
	Res_w3_h2_1440x960 UMETA(DisplayName = "3:2, 1440x960"),
	Res_w3_h2_2160x1440 UMETA(DisplayName = "3:2, 2160x1440"),
	Res_w3_h2_2400x1600 UMETA(DisplayName = "3:2, 2400x1600"),
	Res_w3_h2_3000x2000 UMETA(DisplayName = "3:2, 3000x2000"),
	Res_w3_h2_3240x2160 UMETA(DisplayName = "3:2, 3240x2160"),

//	/*Length*/
//	EnumLength UMETA(Hidden)
};

