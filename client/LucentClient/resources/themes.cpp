#include "themes.h"
#include <oxorany/oxorany_include.h>

void LoadImGuiStyle(const MenuTheme& theme) {
	ImGuiStyle& style = ImGui::GetStyle();
	ImVec4* colors = style.Colors;

	// Color palette from hex
	ImVec4 col_bg_dark = theme.bgDark;
	ImVec4 col_bg_med = theme.bgMed;
	ImVec4 col_accent = theme.accent;
	ImVec4 col_text = theme.text;

	// Text
	colors[ImGuiCol_Text] = col_text;
	colors[ImGuiCol_TextDisabled] = ImVec4(col_text.x, col_text.y, col_text.z, 0.5f);

	// Backgrounds
	colors[ImGuiCol_WindowBg] = col_bg_dark;
	colors[ImGuiCol_ChildBg] = col_bg_dark * ImVec4(1.3f, 1.3f, 1.3f, 1.3f);
	colors[ImGuiCol_PopupBg] = ImVec4(col_bg_dark.x, col_bg_dark.y, col_bg_dark.z, 0.98f);

	// Borders
	colors[ImGuiCol_Border] = col_bg_dark * ImVec4(1.6f, 1.6f, 1.6f, 1.6f);
	colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

	// Frames
	colors[ImGuiCol_FrameBg] = col_bg_med;
	colors[ImGuiCol_FrameBgHovered] = col_accent;
	colors[ImGuiCol_FrameBgActive] = col_accent;

	// Titles
	colors[ImGuiCol_TitleBg] = col_bg_med;
	colors[ImGuiCol_TitleBgActive] = col_accent;
	colors[ImGuiCol_TitleBgCollapsed] = col_bg_dark;

	// Menus & Tabs
	colors[ImGuiCol_MenuBarBg] = col_bg_med;
	colors[ImGuiCol_Tab] = col_bg_med;
	colors[ImGuiCol_TabHovered] = col_accent;
	colors[ImGuiCol_TabActive] = col_accent;
	colors[ImGuiCol_TabUnfocused] = col_bg_dark;
	colors[ImGuiCol_TabUnfocusedActive] = col_bg_med;

	// Scrollbars
	colors[ImGuiCol_ScrollbarBg] = col_bg_dark;
	colors[ImGuiCol_ScrollbarGrab] = col_accent;
	colors[ImGuiCol_ScrollbarGrabHovered] = col_text;
	colors[ImGuiCol_ScrollbarGrabActive] = col_text;

	// Buttons
	colors[ImGuiCol_Button] = col_bg_med;
	colors[ImGuiCol_ButtonHovered] = col_accent;
	colors[ImGuiCol_ButtonActive] = col_accent;

	// Headers (used for Tree, CollapsingHeader)
	colors[ImGuiCol_Header] = col_bg_med;
	colors[ImGuiCol_HeaderHovered] = col_accent;
	colors[ImGuiCol_HeaderActive] = col_accent;

	// Sliders, checks, etc.
	colors[ImGuiCol_CheckMark] = col_text;
	colors[ImGuiCol_SliderGrab] = col_text;
	colors[ImGuiCol_SliderGrabActive] = col_bg_med;
	colors[ImGuiCol_ResizeGrip] = col_accent;
	colors[ImGuiCol_ResizeGripHovered] = col_text;
	colors[ImGuiCol_ResizeGripActive] = col_text;

	// Tables
	colors[ImGuiCol_TableHeaderBg] = col_bg_med;
	colors[ImGuiCol_TableBorderStrong] = col_accent;
	colors[ImGuiCol_TableBorderLight] = ImVec4(col_accent.x, col_accent.y, col_accent.z, 0.4f);
	colors[ImGuiCol_TableRowBg] = ImVec4(0, 0, 0, 0.0f);
	colors[ImGuiCol_TableRowBgAlt] = ImVec4(1, 1, 1, 0.03f);

	// Misc
	colors[ImGuiCol_TextSelectedBg] = ImVec4(col_text.x, col_text.y, col_text.z, 0.2f);
	colors[ImGuiCol_DragDropTarget] = ImVec4(1, 1, 0, 0.9f);
	colors[ImGuiCol_NavHighlight] = col_accent;
	colors[ImGuiCol_NavWindowingHighlight] = col_text;
	colors[ImGuiCol_NavWindowingDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.2f);
	colors[ImGuiCol_ModalWindowDimBg] = ImVec4(0.2f, 0.2f, 0.2f, 0.35f);

	// Optional: tuning style
	style.WindowRounding = 6.0f;
	style.ItemSpacing = ImVec2(4.f, 4.f);
	style.FrameRounding = 4.0f;
	style.ScrollbarRounding = 4.0f;
	style.GrabRounding = 4.0f;
	style.TabRounding = 4.0f;
}

MenuTheme allThemes[150]{
	{
		"black_grey_brown_night_dark_vintage_winter",
		ImVec4(0.216f, 0.208f, 0.243f, 1.000f),
		ImVec4(0.267f, 0.267f, 0.306f, 1.000f),
		ImVec4(0.443f, 0.353f, 0.353f, 1.000f),
		ImVec4(0.827f, 0.855f, 0.851f, 1.000f)
	},
	{
		"navy_maroon_purple_red_orange_dark_space_gradient_halloween",
		ImVec4(0.051f, 0.067f, 0.392f, 1.000f),
		ImVec4(0.392f, 0.051f, 0.373f, 1.000f),
		ImVec4(0.918f, 0.133f, 0.392f, 1.000f),
		ImVec4(0.969f, 0.553f, 0.376f, 1.000f)
	},
	{
		"purple_maroon_red_orange_yellow_space_retro_kids_gradient_dark_halloween",
		ImVec4(0.392f, 0.051f, 0.373f, 1.000f),
		ImVec4(0.694f, 0.173f, 0.000f, 1.000f),
		ImVec4(0.922f, 0.357f, 0.000f, 1.000f),
		ImVec4(1.000f, 0.800f, 0.000f, 1.000f)
	},
	{
		"teal_blue_navy_dark_night_sea_cold",
		ImVec4(0.471f, 0.725f, 0.710f, 1.000f),
		ImVec4(0.059f, 0.510f, 0.549f, 1.000f),
		ImVec4(0.024f, 0.314f, 0.518f, 1.000f),
		ImVec4(0.196f, 0.039f, 0.420f, 1.000f)
	},
	{
		"purple_yellow_night_space_dark",
		ImVec4(0.035f, 0.000f, 0.251f, 1.000f),
		ImVec4(0.278f, 0.075f, 0.588f, 1.000f),
		ImVec4(0.694f, 0.231f, 1.000f, 1.000f),
		ImVec4(1.000f, 0.800f, 0.000f, 1.000f)
	},
	{
		"maroon_purple_red_sage_space_dark_christmas_halloween",
		ImVec4(0.318f, 0.114f, 0.263f, 1.000f),
		ImVec4(0.565f, 0.118f, 0.243f, 1.000f),
		ImVec4(0.863f, 0.145f, 0.145f, 1.000f),
		ImVec4(0.608f, 0.753f, 0.612f, 1.000f)
	},
	{
		"maroon_pink_space_warm_dark_halloween",
		ImVec4(0.227f, 0.020f, 0.098f, 1.000f),
		ImVec4(0.404f, 0.051f, 0.184f, 1.000f),
		ImVec4(0.647f, 0.220f, 0.376f, 1.000f),
		ImVec4(0.937f, 0.533f, 0.678f, 1.000f)
	},
	{
		"black_grey_beige_night_dark_halloween",
		ImVec4(0.133f, 0.157f, 0.192f, 1.000f),
		ImVec4(0.224f, 0.243f, 0.275f, 1.000f),
		ImVec4(0.580f, 0.537f, 0.475f, 1.000f),
		ImVec4(0.875f, 0.816f, 0.722f, 1.000f)
	},
	{
		"red_maroon_purple_black_dark_gradient_space_halloween",
		ImVec4(0.969f, 0.216f, 0.310f, 1.000f),
		ImVec4(0.533f, 0.188f, 0.306f, 1.000f),
		ImVec4(0.322f, 0.145f, 0.275f, 1.000f),
		ImVec4(0.173f, 0.173f, 0.173f, 1.000f)
	},
	{
		"black_purple_brown_orange_warm_night_dark_halloween",
		ImVec4(0.129f, 0.059f, 0.216f, 1.000f),
		ImVec4(0.310f, 0.110f, 0.318f, 1.000f),
		ImVec4(0.647f, 0.357f, 0.294f, 1.000f),
		ImVec4(0.863f, 0.627f, 0.427f, 1.000f)
	},
	{
		"black_green_dark_gradient",
		ImVec4(0.094f, 0.137f, 0.059f, 1.000f),
		ImVec4(0.153f, 0.224f, 0.110f, 1.000f),
		ImVec4(0.145f, 0.373f, 0.220f, 1.000f),
		ImVec4(0.122f, 0.490f, 0.325f, 1.000f)
	},
	{
		"black_green_brown_grey_earth_night_dark_vintage_winter",
		ImVec4(0.173f, 0.224f, 0.188f, 1.000f),
		ImVec4(0.247f, 0.310f, 0.267f, 1.000f),
		ImVec4(0.635f, 0.482f, 0.361f, 1.000f),
		ImVec4(0.863f, 0.843f, 0.788f, 1.000f)
	},
	{
		"black_maroon_red_orange_dark_warm_gradient_space_halloween",
		ImVec4(0.035f, 0.071f, 0.173f, 1.000f),
		ImVec4(0.529f, 0.137f, 0.255f, 1.000f),
		ImVec4(0.745f, 0.192f, 0.267f, 1.000f),
		ImVec4(0.882f, 0.459f, 0.392f, 1.000f)
	},
	{
		"black_maroon_red_grey_space_dark_halloween",
		ImVec4(0.114f, 0.086f, 0.086f, 1.000f),
		ImVec4(0.557f, 0.086f, 0.086f, 1.000f),
		ImVec4(0.847f, 0.251f, 0.251f, 1.000f),
		ImVec4(0.933f, 0.933f, 0.933f, 1.000f)
	},
	{
		"navy_purple_red_orange_space_dark_halloween",
		ImVec4(0.165f, 0.000f, 0.306f, 1.000f),
		ImVec4(0.314f, 0.000f, 0.451f, 1.000f),
		ImVec4(0.776f, 0.137f, 0.000f, 1.000f),
		ImVec4(0.945f, 0.290f, 0.000f, 1.000f)
	},
	{
		"black_navy_maroon_peach_night_dark_halloween",
		ImVec4(0.263f, 0.180f, 0.329f, 1.000f),
		ImVec4(0.294f, 0.263f, 0.463f, 1.000f),
		ImVec4(0.682f, 0.267f, 0.353f, 1.000f),
		ImVec4(0.910f, 0.737f, 0.725f, 1.000f)
	},
	{
		"black_maroon_purple_pink_dark_night_gradient_halloween",
		ImVec4(0.102f, 0.102f, 0.114f, 1.000f),
		ImVec4(0.231f, 0.110f, 0.196f, 1.000f),
		ImVec4(0.416f, 0.118f, 0.333f, 1.000f),
		ImVec4(0.651f, 0.302f, 0.475f, 1.000f)
	},
	{
		"navy_teal_yellow_night_winter_dark_cold_wedding",
		ImVec4(0.000f, 0.043f, 0.345f, 1.000f),
		ImVec4(0.000f, 0.192f, 0.380f, 1.000f),
		ImVec4(0.000f, 0.416f, 0.404f, 1.000f),
		ImVec4(1.000f, 0.957f, 0.718f, 1.000f)
	},
	{
		"orange_blue_navy_black_dark_space_halloween",
		ImVec4(1.000f, 0.396f, 0.000f, 1.000f),
		ImVec4(0.118f, 0.243f, 0.384f, 1.000f),
		ImVec4(0.043f, 0.098f, 0.173f, 1.000f),
		ImVec4(0.000f, 0.000f, 0.000f, 1.000f)
	},
	{
		"black_grey_teal_beige_night_dark_winter",
		ImVec4(0.094f, 0.110f, 0.078f, 1.000f),
		ImVec4(0.235f, 0.239f, 0.216f, 1.000f),
		ImVec4(0.412f, 0.459f, 0.396f, 1.000f),
		ImVec4(0.925f, 0.875f, 0.800f, 1.000f)
	},
	{
		"black_purple_night_space_dark_gradient_halloween",
		ImVec4(0.180f, 0.027f, 0.247f, 1.000f),
		ImVec4(0.478f, 0.110f, 0.675f, 1.000f),
		ImVec4(0.678f, 0.286f, 0.882f, 1.000f),
		ImVec4(0.922f, 0.827f, 0.973f, 1.000f)
	},
	{
		"black_sage_grey_beige_night_dark_gradient_halloween",
		ImVec4(0.118f, 0.125f, 0.118f, 1.000f),
		ImVec4(0.235f, 0.239f, 0.216f, 1.000f),
		ImVec4(0.412f, 0.459f, 0.396f, 1.000f),
		ImVec4(0.925f, 0.875f, 0.800f, 1.000f)
	},
	{
		"purple_maroon_red_orange_warm_dark_night",
		ImVec4(0.322f, 0.133f, 0.345f, 1.000f),
		ImVec4(0.549f, 0.188f, 0.380f, 1.000f),
		ImVec4(0.776f, 0.235f, 0.318f, 1.000f),
		ImVec4(0.851f, 0.373f, 0.349f, 1.000f)
	},
	{
		"navy_purple_red_pink_orange_retro_dark_night_space",
		ImVec4(0.094f, 0.004f, 0.380f, 1.000f),
		ImVec4(0.310f, 0.090f, 0.529f, 1.000f),
		ImVec4(0.922f, 0.212f, 0.471f, 1.000f),
		ImVec4(0.984f, 0.467f, 0.235f, 1.000f)
	},
	{
		"black_navy_blue_beige_retro_dark_gradient",
		ImVec4(0.008f, 0.082f, 0.149f, 1.000f),
		ImVec4(0.012f, 0.204f, 0.431f, 1.000f),
		ImVec4(0.431f, 0.675f, 0.855f, 1.000f),
		ImVec4(0.886f, 0.886f, 0.714f, 1.000f)
	},
	{
		"black_sage_beige_dark_gradient_nature_winter_food",
		ImVec4(0.102f, 0.212f, 0.212f, 1.000f),
		ImVec4(0.251f, 0.325f, 0.298f, 1.000f),
		ImVec4(0.404f, 0.490f, 0.416f, 1.000f),
		ImVec4(0.839f, 0.741f, 0.596f, 1.000f)
	},
	{
		"black_navy_purple_space_night_dark_gradient",
		ImVec4(0.090f, 0.082f, 0.231f, 1.000f),
		ImVec4(0.180f, 0.137f, 0.424f, 1.000f),
		ImVec4(0.263f, 0.239f, 0.545f, 1.000f),
		ImVec4(0.784f, 0.675f, 0.839f, 1.000f)
	},
	{
		"navy_blue_sea_dark_gradient_cold",
		ImVec4(0.251f, 0.180f, 0.478f, 1.000f),
		ImVec4(0.298f, 0.231f, 0.812f, 1.000f),
		ImVec4(0.294f, 0.439f, 0.961f, 1.000f),
		ImVec4(0.239f, 0.761f, 0.925f, 1.000f)
	},
	{
		"navy_teal_dark_sea_gradient_cold_winter",
		ImVec4(0.141f, 0.027f, 0.314f, 1.000f),
		ImVec4(0.204f, 0.298f, 0.392f, 1.000f),
		ImVec4(0.341f, 0.482f, 0.553f, 1.000f),
		ImVec4(0.341f, 0.651f, 0.631f, 1.000f)
	},
	{
		"purple_grey_orange_vintage_dark_halloween",
		ImVec4(0.196f, 0.004f, 0.184f, 1.000f),
		ImVec4(0.322f, 0.298f, 0.259f, 1.000f),
		ImVec4(0.886f, 0.875f, 0.816f, 1.000f),
		ImVec4(0.976f, 0.451f, 0.000f, 1.000f)
	},
	{
		"purple_maroon_peach_dark_warm_night_halloween",
		ImVec4(0.286f, 0.141f, 0.243f, 1.000f),
		ImVec4(0.439f, 0.259f, 0.392f, 1.000f),
		ImVec4(0.733f, 0.518f, 0.576f, 1.000f),
		ImVec4(0.859f, 0.686f, 0.627f, 1.000f)
	},
	{
		"black_brown_maroon_orange_dark_fall_halloween_night",
		ImVec4(0.047f, 0.047f, 0.047f, 1.000f),
		ImVec4(0.282f, 0.118f, 0.078f, 1.000f),
		ImVec4(0.608f, 0.224f, 0.133f, 1.000f),
		ImVec4(0.949f, 0.380f, 0.247f, 1.000f)
	},
	{
		"red_maroon_navy_dark_neon_gradient_warm_space",
		ImVec4(1.000f, 0.125f, 0.306f, 1.000f),
		ImVec4(0.627f, 0.082f, 0.243f, 1.000f),
		ImVec4(0.365f, 0.055f, 0.255f, 1.000f),
		ImVec4(0.000f, 0.133f, 0.302f, 1.000f)
	},
	{
		"purple_brown_yellow_retro_gradient_dark_night_vintage_coffee_halloween",
		ImVec4(0.263f, 0.039f, 0.365f, 1.000f),
		ImVec4(0.373f, 0.216f, 0.294f, 1.000f),
		ImVec4(0.549f, 0.416f, 0.365f, 1.000f),
		ImVec4(0.933f, 0.894f, 0.694f, 1.000f)
	},
	{
		"black_teal_grey_vintage_dark_wedding_winter",
		ImVec4(0.133f, 0.157f, 0.192f, 1.000f),
		ImVec4(0.192f, 0.212f, 0.247f, 1.000f),
		ImVec4(0.463f, 0.671f, 0.682f, 1.000f),
		ImVec4(0.933f, 0.933f, 0.933f, 1.000f)
	},
	{
		"black_navy_sage_dark_cold_winter_night_halloween",
		ImVec4(0.208f, 0.216f, 0.294f, 1.000f),
		ImVec4(0.204f, 0.286f, 0.333f, 1.000f),
		ImVec4(0.314f, 0.447f, 0.482f, 1.000f),
		ImVec4(0.471f, 0.627f, 0.514f, 1.000f)
	},
	{
		"navy_blue_dark_cold_night_space_gradient_halloween",
		ImVec4(0.027f, 0.059f, 0.169f, 1.000f),
		ImVec4(0.106f, 0.102f, 0.333f, 1.000f),
		ImVec4(0.325f, 0.361f, 0.569f, 1.000f),
		ImVec4(0.573f, 0.565f, 0.765f, 1.000f)
	},
	{
		"black_purple_maroon_dark_night_space_halloween",
		ImVec4(0.012f, 0.024f, 0.216f, 1.000f),
		ImVec4(0.235f, 0.027f, 0.325f, 1.000f),
		ImVec4(0.447f, 0.016f, 0.333f, 1.000f),
		ImVec4(0.569f, 0.039f, 0.404f, 1.000f)
	},
	{
		"black_brown_peach_vintage_dark_night_gradient_warm_skin_coffee",
		ImVec4(0.243f, 0.196f, 0.196f, 1.000f),
		ImVec4(0.314f, 0.235f, 0.235f, 1.000f),
		ImVec4(0.494f, 0.388f, 0.388f, 1.000f),
		ImVec4(0.659f, 0.486f, 0.486f, 1.000f)
	},
	{
		"black_navy_orange_dark_gradient_night_winter_cold_halloween_vintage",
		ImVec4(0.176f, 0.196f, 0.314f, 1.000f),
		ImVec4(0.259f, 0.278f, 0.412f, 1.000f),
		ImVec4(0.439f, 0.467f, 0.631f, 1.000f),
		ImVec4(0.965f, 0.694f, 0.478f, 1.000f)
	},
	{
		"black_teal_sage_night_dark_sea_winter",
		ImVec4(0.035f, 0.149f, 0.208f, 1.000f),
		ImVec4(0.106f, 0.259f, 0.259f, 1.000f),
		ImVec4(0.361f, 0.514f, 0.455f, 1.000f),
		ImVec4(0.620f, 0.784f, 0.725f, 1.000f)
	},
	{
		"black_maroon_orange_green_dark_winter_christmas_halloween",
		ImVec4(0.098f, 0.098f, 0.098f, 1.000f),
		ImVec4(0.459f, 0.055f, 0.129f, 1.000f),
		ImVec4(0.890f, 0.396f, 0.114f, 1.000f),
		ImVec4(0.745f, 0.843f, 0.329f, 1.000f)
	},
	{
		"black_maroon_red_orange_dark_warm_gradient_night_halloween",
		ImVec4(0.133f, 0.035f, 0.173f, 1.000f),
		ImVec4(0.529f, 0.137f, 0.255f, 1.000f),
		ImVec4(0.745f, 0.192f, 0.267f, 1.000f),
		ImVec4(0.941f, 0.349f, 0.255f, 1.000f)
	},
	{
		"navy_blue_beige_dark_winter_cold_night_sea",
		ImVec4(0.212f, 0.188f, 0.384f, 1.000f),
		ImVec4(0.263f, 0.333f, 0.522f, 1.000f),
		ImVec4(0.506f, 0.561f, 0.706f, 1.000f),
		ImVec4(0.961f, 0.910f, 0.780f, 1.000f)
	},
	{
		"purple_maroon_red_pink_dark_space_warm",
		ImVec4(0.380f, 0.047f, 0.624f, 1.000f),
		ImVec4(0.580f, 0.043f, 0.573f, 1.000f),
		ImVec4(0.855f, 0.047f, 0.506f, 1.000f),
		ImVec4(0.914f, 0.341f, 0.576f, 1.000f)
	},
	{
		"black_teal_night_space_dark_halloween_cold",
		ImVec4(0.059f, 0.059f, 0.059f, 1.000f),
		ImVec4(0.137f, 0.176f, 0.247f, 1.000f),
		ImVec4(0.000f, 0.357f, 0.255f, 1.000f),
		ImVec4(0.000f, 0.506f, 0.439f, 1.000f)
	},
	{
		"purple_maroon_orange_night_space_dark_gradient_warm_skin_gold_halloween",
		ImVec4(0.271f, 0.098f, 0.322f, 1.000f),
		ImVec4(0.400f, 0.145f, 0.286f, 1.000f),
		ImVec4(0.682f, 0.267f, 0.353f, 1.000f),
		ImVec4(0.953f, 0.624f, 0.353f, 1.000f)
	},
	{
		"black_teal_night_winter_dark_cold_sea",
		ImVec4(0.016f, 0.051f, 0.071f, 1.000f),
		ImVec4(0.094f, 0.239f, 0.239f, 1.000f),
		ImVec4(0.361f, 0.514f, 0.455f, 1.000f),
		ImVec4(0.576f, 0.694f, 0.651f, 1.000f)
	},
	{
		"grey_beige_gradient_dark_halloween_night",
		ImVec4(0.208f, 0.184f, 0.267f, 1.000f),
		ImVec4(0.361f, 0.329f, 0.439f, 1.000f),
		ImVec4(0.725f, 0.706f, 0.780f, 1.000f),
		ImVec4(0.980f, 0.941f, 0.902f, 1.000f)
	},
	{
		"peach_purple_navy_black_dark_gradient_night_vintage_skin",
		ImVec4(0.882f, 0.596f, 0.596f, 1.000f),
		ImVec4(0.635f, 0.404f, 0.541f, 1.000f),
		ImVec4(0.302f, 0.235f, 0.467f, 1.000f),
		ImVec4(0.247f, 0.114f, 0.220f, 1.000f)
	},
	{
		"navy_purple_blue_dark_night_sea_winter",
		ImVec4(0.208f, 0.082f, 0.365f, 1.000f),
		ImVec4(0.318f, 0.169f, 0.506f, 1.000f),
		ImVec4(0.267f, 0.467f, 0.808f, 1.000f),
		ImVec4(0.549f, 0.671f, 1.000f, 1.000f)
	},
	{
		"black_peach_beige_vintage_dark_warm_night_halloween",
		ImVec4(0.200f, 0.114f, 0.173f, 1.000f),
		ImVec4(0.247f, 0.180f, 0.243f, 1.000f),
		ImVec4(0.655f, 0.510f, 0.584f, 1.000f),
		ImVec4(0.937f, 0.882f, 0.820f, 1.000f)
	},
	{
		"navy_peach_halloween_dark_vintage_night",
		ImVec4(0.176f, 0.263f, 0.337f, 1.000f),
		ImVec4(0.263f, 0.357f, 0.400f, 1.000f),
		ImVec4(0.655f, 0.435f, 0.435f, 1.000f),
		ImVec4(0.918f, 0.698f, 0.627f, 1.000f)
	},
	{
		"navy_blue_teal_dark_night_space",
		ImVec4(0.055f, 0.161f, 0.329f, 1.000f),
		ImVec4(0.122f, 0.431f, 0.549f, 1.000f),
		ImVec4(0.180f, 0.541f, 0.600f, 1.000f),
		ImVec4(0.518f, 0.655f, 0.631f, 1.000f)
	},
	{
		"teal_black_brown_red_vintage_dark_wedding_halloween_night",
		ImVec4(0.067f, 0.427f, 0.431f, 1.000f),
		ImVec4(0.196f, 0.118f, 0.118f, 1.000f),
		ImVec4(0.306f, 0.212f, 0.212f, 1.000f),
		ImVec4(0.804f, 0.094f, 0.094f, 1.000f)
	},
	{
		"navy_blue_grey_dark_night_sea_winter_cold",
		ImVec4(0.153f, 0.216f, 0.302f, 1.000f),
		ImVec4(0.322f, 0.427f, 0.510f, 1.000f),
		ImVec4(0.616f, 0.698f, 0.749f, 1.000f),
		ImVec4(0.867f, 0.902f, 0.929f, 1.000f)
	},
	{
		"navy_blue_purple_space_dark_gradient_halloween",
		ImVec4(0.047f, 0.075f, 0.310f, 1.000f),
		ImVec4(0.114f, 0.149f, 0.490f, 1.000f),
		ImVec4(0.361f, 0.275f, 0.612f, 1.000f),
		ImVec4(0.831f, 0.678f, 0.988f, 1.000f)
	},
	{
		"maroon_purple_brown_grey_night_dark_halloween_skin",
		ImVec4(0.373f, 0.149f, 0.290f, 1.000f),
		ImVec4(0.392f, 0.227f, 0.420f, 1.000f),
		ImVec4(0.584f, 0.467f, 0.467f, 1.000f),
		ImVec4(0.690f, 0.643f, 0.643f, 1.000f)
	},
	{
		"navy_red_dark_space",
		ImVec4(0.027f, 0.039f, 0.322f, 1.000f),
		ImVec4(0.824f, 0.075f, 0.071f, 1.000f),
		ImVec4(0.929f, 0.169f, 0.165f, 1.000f),
		ImVec4(0.945f, 0.353f, 0.349f, 1.000f)
	},
	{
		"black_grey_beige_night_dark",
		ImVec4(0.224f, 0.212f, 0.275f, 1.000f),
		ImVec4(0.310f, 0.271f, 0.341f, 1.000f),
		ImVec4(0.427f, 0.365f, 0.431f, 1.000f),
		ImVec4(0.957f, 0.933f, 0.878f, 1.000f)
	},
	{
		"navy_blue_sea_night_dark_winter",
		ImVec4(0.043f, 0.141f, 0.278f, 1.000f),
		ImVec4(0.098f, 0.216f, 0.427f, 1.000f),
		ImVec4(0.341f, 0.424f, 0.737f, 1.000f),
		ImVec4(0.647f, 0.843f, 0.910f, 1.000f)
	},
	{
		"navy_purple_maroon_peach_space_night_dark_gradient_halloween",
		ImVec4(0.216f, 0.188f, 0.420f, 1.000f),
		ImVec4(0.400f, 0.204f, 0.498f, 1.000f),
		ImVec4(0.620f, 0.278f, 0.518f, 1.000f),
		ImVec4(0.824f, 0.463f, 0.522f, 1.000f)
	},
	{
		"black_teal_mint_sea_night_dark_gradient",
		ImVec4(0.173f, 0.200f, 0.200f, 1.000f),
		ImVec4(0.180f, 0.310f, 0.310f, 1.000f),
		ImVec4(0.055f, 0.514f, 0.533f, 1.000f),
		ImVec4(0.796f, 0.894f, 0.871f, 1.000f)
	},
	{
		"navy_purple_blue_teal_night_dark_gradient_cold_sea",
		ImVec4(0.227f, 0.063f, 0.471f, 1.000f),
		ImVec4(0.306f, 0.192f, 0.667f, 1.000f),
		ImVec4(0.184f, 0.345f, 0.804f, 1.000f),
		ImVec4(0.216f, 0.584f, 0.741f, 1.000f)
	},
	{
		"navy_black_dark_cold_night_space_winter",
		ImVec4(0.388f, 0.349f, 0.522f, 1.000f),
		ImVec4(0.267f, 0.235f, 0.408f, 1.000f),
		ImVec4(0.224f, 0.188f, 0.325f, 1.000f),
		ImVec4(0.094f, 0.071f, 0.169f, 1.000f)
	},
	{
		"black_purple_blue_mint_winter_sea_space_dark_gradient",
		ImVec4(0.012f, 0.000f, 0.110f, 1.000f),
		ImVec4(0.188f, 0.118f, 0.404f, 1.000f),
		ImVec4(0.357f, 0.561f, 0.725f, 1.000f),
		ImVec4(0.714f, 0.918f, 0.855f, 1.000f)
	},
	{
		"navy_blue_green_sea_dark_night_cold",
		ImVec4(0.075f, 0.000f, 0.353f, 1.000f),
		ImVec4(0.000f, 0.200f, 0.486f, 1.000f),
		ImVec4(0.110f, 0.510f, 0.678f, 1.000f),
		ImVec4(0.012f, 0.788f, 0.533f, 1.000f)
	},
	{
		"navy_blue_dark_sea_cold",
		ImVec4(0.039f, 0.149f, 0.278f, 1.000f),
		ImVec4(0.078f, 0.259f, 0.447f, 1.000f),
		ImVec4(0.125f, 0.322f, 0.584f, 1.000f),
		ImVec4(0.173f, 0.455f, 0.702f, 1.000f)
	},
	{
		"black_brown_beige_dark_night_coffee_halloween_warm",
		ImVec4(0.102f, 0.071f, 0.043f, 1.000f),
		ImVec4(0.235f, 0.165f, 0.129f, 1.000f),
		ImVec4(0.835f, 0.808f, 0.639f, 1.000f),
		ImVec4(0.898f, 0.898f, 0.796f, 1.000f)
	},
	{
		"blue_navy_yellow_purple_dark_warm_winter_night_space_sky",
		ImVec4(0.271f, 0.235f, 0.404f, 1.000f),
		ImVec4(0.427f, 0.404f, 0.894f, 1.000f),
		ImVec4(0.275f, 0.761f, 0.796f, 1.000f),
		ImVec4(0.949f, 0.969f, 0.631f, 1.000f)
	},
	{
		"black_purple_gradient_night_dark_space",
		ImVec4(0.176f, 0.012f, 0.231f, 1.000f),
		ImVec4(0.506f, 0.047f, 0.659f, 1.000f),
		ImVec4(0.757f, 0.278f, 0.914f, 1.000f),
		ImVec4(0.898f, 0.722f, 0.957f, 1.000f)
	},
	{
		"navy_purple_beige_dark_night",
		ImVec4(0.000f, 0.000f, 0.361f, 1.000f),
		ImVec4(0.231f, 0.094f, 0.373f, 1.000f),
		ImVec4(0.753f, 0.376f, 0.631f, 1.000f),
		ImVec4(0.941f, 0.792f, 0.639f, 1.000f)
	},
	{
		"black_brown_beige_dark_night",
		ImVec4(0.000f, 0.000f, 0.000f, 1.000f),
		ImVec4(0.157f, 0.165f, 0.227f, 1.000f),
		ImVec4(0.451f, 0.373f, 0.196f, 1.000f),
		ImVec4(0.776f, 0.592f, 0.286f, 1.000f)
	},
	{
		"black_navy_grey_dark_winter_night",
		ImVec4(0.251f, 0.259f, 0.345f, 1.000f),
		ImVec4(0.278f, 0.306f, 0.408f, 1.000f),
		ImVec4(0.314f, 0.341f, 0.478f, 1.000f),
		ImVec4(0.420f, 0.447f, 0.557f, 1.000f)
	},
	{
		"navy_purple_blue_night_space_dark_cold_gradient",
		ImVec4(0.247f, 0.231f, 0.424f, 1.000f),
		ImVec4(0.384f, 0.310f, 0.510f, 1.000f),
		ImVec4(0.624f, 0.451f, 0.671f, 1.000f),
		ImVec4(0.639f, 0.780f, 0.839f, 1.000f)
	},
	{
		"black_navy_purple_pink_space_dark",
		ImVec4(0.000f, 0.000f, 0.000f, 1.000f),
		ImVec4(0.082f, 0.000f, 0.314f, 1.000f),
		ImVec4(0.247f, 0.000f, 0.443f, 1.000f),
		ImVec4(0.984f, 0.145f, 0.463f, 1.000f)
	},
	{
		"navy_brown_sage_grey_dark_night_vintage_halloween_sea",
		ImVec4(0.094f, 0.153f, 0.278f, 1.000f),
		ImVec4(0.337f, 0.169f, 0.031f, 1.000f),
		ImVec4(0.392f, 0.494f, 0.408f, 1.000f),
		ImVec4(0.847f, 0.847f, 0.847f, 1.000f)
	},
	{
		"maroon_purple_pink_wedding_dark",
		ImVec4(0.298f, 0.000f, 0.200f, 1.000f),
		ImVec4(0.475f, 0.008f, 0.322f, 1.000f),
		ImVec4(0.686f, 0.004f, 0.443f, 1.000f),
		ImVec4(0.910f, 0.059f, 0.533f, 1.000f)
	},
	{
		"navy_purple_red_dark_space",
		ImVec4(0.086f, 0.129f, 0.243f, 1.000f),
		ImVec4(0.059f, 0.204f, 0.376f, 1.000f),
		ImVec4(0.325f, 0.204f, 0.514f, 1.000f),
		ImVec4(0.914f, 0.271f, 0.376f, 1.000f)
	},
	{
		"brown_dark_coffee_warm_earth_vintage_halloween",
		ImVec4(0.278f, 0.176f, 0.176f, 1.000f),
		ImVec4(0.333f, 0.224f, 0.224f, 1.000f),
		ImVec4(0.439f, 0.310f, 0.310f, 1.000f),
		ImVec4(0.655f, 0.475f, 0.475f, 1.000f)
	},
	{
		"dark_black_navy_teal_cold_night_gradient_sea",
		ImVec4(0.173f, 0.200f, 0.200f, 1.000f),
		ImVec4(0.224f, 0.357f, 0.392f, 1.000f),
		ImVec4(0.647f, 0.788f, 0.792f, 1.000f),
		ImVec4(0.906f, 0.965f, 0.949f, 1.000f)
	},
	{
		"navy_brown_earth_warm_nature_fall_coffee_dark_vintage_night",
		ImVec4(0.173f, 0.212f, 0.224f, 1.000f),
		ImVec4(0.247f, 0.306f, 0.310f, 1.000f),
		ImVec4(0.635f, 0.482f, 0.361f, 1.000f),
		ImVec4(0.863f, 0.843f, 0.788f, 1.000f)
	},
	{
		"black_maroon_red_orange_dark_warm_sunset_halloween_skin",
		ImVec4(0.298f, 0.227f, 0.318f, 1.000f),
		ImVec4(0.467f, 0.263f, 0.376f, 1.000f),
		ImVec4(0.698f, 0.314f, 0.408f, 1.000f),
		ImVec4(0.906f, 0.671f, 0.475f, 1.000f)
	},
	{
		"black_navy_purple_beige_night_dark_vintage",
		ImVec4(0.106f, 0.141f, 0.188f, 1.000f),
		ImVec4(0.318f, 0.333f, 0.494f, 1.000f),
		ImVec4(0.506f, 0.404f, 0.592f, 1.000f),
		ImVec4(0.839f, 0.835f, 0.659f, 1.000f)
	},
	{
		"navy_purple_night_dark_halloween_cold",
		ImVec4(0.216f, 0.106f, 0.345f, 1.000f),
		ImVec4(0.298f, 0.208f, 0.459f, 1.000f),
		ImVec4(0.357f, 0.294f, 0.541f, 1.000f),
		ImVec4(0.471f, 0.345f, 0.651f, 1.000f)
	},
	{
		"black_purple_pink_neon_space_dark_sky",
		ImVec4(0.180f, 0.008f, 0.286f, 1.000f),
		ImVec4(0.341f, 0.039f, 0.341f, 1.000f),
		ImVec4(0.663f, 0.063f, 0.475f, 1.000f),
		ImVec4(0.973f, 0.024f, 0.800f, 1.000f)
	},
	{
		"pink_black_grey_retro_dark_space",
		ImVec4(0.969f, 0.239f, 0.576f, 1.000f),
		ImVec4(0.086f, 0.000f, 0.231f, 1.000f),
		ImVec4(0.255f, 0.247f, 0.259f, 1.000f),
		ImVec4(0.498f, 0.518f, 0.529f, 1.000f)
	},
	{
		"purple_red_yellow_dark_night_warm_gradient",
		ImVec4(0.275f, 0.141f, 0.298f, 1.000f),
		ImVec4(0.443f, 0.169f, 0.459f, 1.000f),
		ImVec4(0.780f, 0.294f, 0.314f, 1.000f),
		ImVec4(0.831f, 0.608f, 0.329f, 1.000f)
	},
	{
		"black_purple_pink_space_dark",
		ImVec4(0.094f, 0.039f, 0.039f, 1.000f),
		ImVec4(0.443f, 0.102f, 0.459f, 1.000f),
		ImVec4(0.945f, 0.000f, 0.525f, 1.000f),
		ImVec4(0.961f, 0.510f, 0.655f, 1.000f)
	},
	{
		"black_navy_purple_pink_space_dark_halloween",
		ImVec4(0.102f, 0.102f, 0.251f, 1.000f),
		ImVec4(0.153f, 0.000f, 0.510f, 1.000f),
		ImVec4(0.478f, 0.043f, 0.753f, 1.000f),
		ImVec4(0.980f, 0.345f, 0.714f, 1.000f)
	},
	{
		"maroon_purple_dark_night_vintage_skin_halloween",
		ImVec4(0.298f, 0.000f, 0.153f, 1.000f),
		ImVec4(0.341f, 0.020f, 0.188f, 1.000f),
		ImVec4(0.459f, 0.020f, 0.314f, 1.000f),
		ImVec4(0.596f, 0.059f, 0.353f, 1.000f)
	},
	{
		"black_maroon_green_grey_dark_vintage_food_winter_halloween",
		ImVec4(0.059f, 0.055f, 0.055f, 1.000f),
		ImVec4(0.329f, 0.071f, 0.071f, 1.000f),
		ImVec4(0.545f, 0.604f, 0.275f, 1.000f),
		ImVec4(0.933f, 0.933f, 0.933f, 1.000f)
	},
	{
		"black_navy_red_beige_dark_halloween_winter_vintage",
		ImVec4(0.098f, 0.098f, 0.098f, 1.000f),
		ImVec4(0.176f, 0.259f, 0.388f, 1.000f),
		ImVec4(0.784f, 0.294f, 0.192f, 1.000f),
		ImVec4(0.925f, 0.859f, 0.729f, 1.000f)
	},
	{
		"black_navy_orange_dark_night_winter_cold",
		ImVec4(0.016f, 0.110f, 0.196f, 1.000f),
		ImVec4(0.016f, 0.161f, 0.227f, 1.000f),
		ImVec4(0.024f, 0.275f, 0.388f, 1.000f),
		ImVec4(0.925f, 0.702f, 0.396f, 1.000f)
	},
	{
		"black_maroon_red_orange_brown_dark_night_halloween_coffee_warm",
		ImVec4(0.016f, 0.012f, 0.012f, 1.000f),
		ImVec4(0.275f, 0.067f, 0.067f, 1.000f),
		ImVec4(0.631f, 0.200f, 0.200f, 1.000f),
		ImVec4(0.702f, 0.329f, 0.118f, 1.000f)
	},
	{
		"purple_navy_black_dark_space",
		ImVec4(0.604f, 0.024f, 0.502f, 1.000f),
		ImVec4(0.475f, 0.004f, 0.549f, 1.000f),
		ImVec4(0.298f, 0.000f, 0.439f, 1.000f),
		ImVec4(0.086f, 0.000f, 0.251f, 1.000f)
	},
	{
		"black_navy_purple_peach_night_space_dark_halloween_gradient",
		ImVec4(0.122f, 0.114f, 0.212f, 1.000f),
		ImVec4(0.247f, 0.200f, 0.318f, 1.000f),
		ImVec4(0.525f, 0.282f, 0.475f, 1.000f),
		ImVec4(0.914f, 0.651f, 0.651f, 1.000f)
	},
	{
		"black_purple_night_dark",
		ImVec4(0.000f, 0.000f, 0.000f, 1.000f),
		ImVec4(0.243f, 0.024f, 0.373f, 1.000f),
		ImVec4(0.439f, 0.043f, 0.592f, 1.000f),
		ImVec4(0.557f, 0.020f, 0.761f, 1.000f)
	},
	{
		"black_green_dark",
		ImVec4(0.098f, 0.102f, 0.098f, 1.000f),
		ImVec4(0.118f, 0.318f, 0.157f, 1.000f),
		ImVec4(0.306f, 0.624f, 0.239f, 1.000f),
		ImVec4(0.847f, 0.914f, 0.659f, 1.000f)
	},
	{
		"black_maroon_red_dark_night_space_halloween",
		ImVec4(0.000f, 0.000f, 0.000f, 1.000f),
		ImVec4(0.239f, 0.000f, 0.000f, 1.000f),
		ImVec4(0.584f, 0.004f, 0.004f, 1.000f),
		ImVec4(1.000f, 0.000f, 0.000f, 1.000f)
	},
	{
		"navy_purple_night_dark_cold_gradient",
		ImVec4(0.110f, 0.047f, 0.357f, 1.000f),
		ImVec4(0.239f, 0.173f, 0.553f, 1.000f),
		ImVec4(0.569f, 0.420f, 0.749f, 1.000f),
		ImVec4(0.788f, 0.588f, 0.800f, 1.000f)
	},
	{
		"black_navy_purple_halloween_dark_night",
		ImVec4(0.000f, 0.000f, 0.000f, 1.000f),
		ImVec4(0.082f, 0.000f, 0.314f, 1.000f),
		ImVec4(0.247f, 0.000f, 0.443f, 1.000f),
		ImVec4(0.380f, 0.000f, 0.580f, 1.000f)
	},
	{
		"navy_purple_yellow_dark_space_night",
		ImVec4(0.165f, 0.035f, 0.267f, 1.000f),
		ImVec4(0.231f, 0.094f, 0.373f, 1.000f),
		ImVec4(0.631f, 0.145f, 0.408f, 1.000f),
		ImVec4(0.996f, 0.761f, 0.376f, 1.000f)
	},
	{
		"maroon_red_night_dark_warm_halloween",
		ImVec4(0.259f, 0.020f, 0.086f, 1.000f),
		ImVec4(0.490f, 0.098f, 0.208f, 1.000f),
		ImVec4(0.706f, 0.169f, 0.318f, 1.000f),
		ImVec4(0.902f, 0.243f, 0.427f, 1.000f)
	},
	{
		"black_navy_blue_dark_night_cold",
		ImVec4(0.149f, 0.110f, 0.173f, 1.000f),
		ImVec4(0.243f, 0.173f, 0.255f, 1.000f),
		ImVec4(0.361f, 0.322f, 0.498f, 1.000f),
		ImVec4(0.431f, 0.522f, 0.698f, 1.000f)
	},
	{
		"black_brown_orange_beige_coffee_warm_skin_dark_earth_fall",
		ImVec4(0.176f, 0.141f, 0.141f, 1.000f),
		ImVec4(0.361f, 0.239f, 0.180f, 1.000f),
		ImVec4(0.722f, 0.361f, 0.220f, 1.000f),
		ImVec4(0.878f, 0.753f, 0.592f, 1.000f)
	},
	{
		"black_navy_orange_space_dark",
		ImVec4(0.031f, 0.125f, 0.196f, 1.000f),
		ImVec4(0.173f, 0.224f, 0.294f, 1.000f),
		ImVec4(0.200f, 0.278f, 0.337f, 1.000f),
		ImVec4(1.000f, 0.298f, 0.161f, 1.000f)
	},
	{
		"navy_grey_white_night_winter_dark",
		ImVec4(0.059f, 0.016f, 0.298f, 1.000f),
		ImVec4(0.078f, 0.118f, 0.380f, 1.000f),
		ImVec4(0.471f, 0.478f, 0.569f, 1.000f),
		ImVec4(0.933f, 0.933f, 0.933f, 1.000f)
	},
	{
		"brown_black_grey_night_dark_vintage",
		ImVec4(0.212f, 0.133f, 0.133f, 1.000f),
		ImVec4(0.090f, 0.063f, 0.063f, 1.000f),
		ImVec4(0.259f, 0.247f, 0.243f, 1.000f),
		ImVec4(0.169f, 0.169f, 0.169f, 1.000f)
	},
	{
		"black_grey_red_dark_night",
		ImVec4(0.090f, 0.090f, 0.090f, 1.000f),
		ImVec4(0.267f, 0.267f, 0.267f, 1.000f),
		ImVec4(0.855f, 0.000f, 0.216f, 1.000f),
		ImVec4(0.929f, 0.929f, 0.929f, 1.000f)
	},
	{
		"vintage_black_green_orange_dark_nature",
		ImVec4(0.086f, 0.086f, 0.086f, 1.000f),
		ImVec4(0.204f, 0.404f, 0.318f, 1.000f),
		ImVec4(0.784f, 0.294f, 0.192f, 1.000f),
		ImVec4(0.925f, 0.859f, 0.729f, 1.000f)
	},
	{
		"maroon_navy_blue_dark_night_winter",
		ImVec4(0.482f, 0.067f, 0.227f, 1.000f),
		ImVec4(0.082f, 0.055f, 0.337f, 1.000f),
		ImVec4(0.082f, 0.592f, 0.733f, 1.000f),
		ImVec4(0.561f, 0.839f, 0.882f, 1.000f)
	},
	{
		"orange_black_skin_gold_dark_fall_halloween_sunset_night_warm_coffee",
		ImVec4(0.933f, 0.718f, 0.420f, 1.000f),
		ImVec4(0.886f, 0.439f, 0.227f, 1.000f),
		ImVec4(0.612f, 0.239f, 0.329f, 1.000f),
		ImVec4(0.192f, 0.043f, 0.043f, 1.000f)
	},
	{
		"black_purple_grey_dark_space_night_vintage_halloween",
		ImVec4(0.082f, 0.082f, 0.082f, 1.000f),
		ImVec4(0.188f, 0.106f, 0.247f, 1.000f),
		ImVec4(0.235f, 0.255f, 0.361f, 1.000f),
		ImVec4(0.706f, 0.647f, 0.647f, 1.000f)
	},
	{
		"purple_blue_retro_night_cold_dark",
		ImVec4(0.208f, 0.122f, 0.224f, 1.000f),
		ImVec4(0.447f, 0.416f, 0.584f, 1.000f),
		ImVec4(0.443f, 0.624f, 0.690f, 1.000f),
		ImVec4(0.627f, 0.757f, 0.722f, 1.000f)
	},
	{
		"black_orange_warm_vintage_dark_coffee_earth_night",
		ImVec4(0.173f, 0.024f, 0.122f, 1.000f),
		ImVec4(0.216f, 0.251f, 0.271f, 1.000f),
		ImVec4(0.847f, 0.573f, 0.086f, 1.000f),
		ImVec4(0.882f, 0.847f, 0.624f, 1.000f)
	},
	{
		"grey_navy_red_space_cold_dark",
		ImVec4(0.867f, 0.867f, 0.867f, 1.000f),
		ImVec4(0.133f, 0.157f, 0.192f, 1.000f),
		ImVec4(0.188f, 0.278f, 0.369f, 1.000f),
		ImVec4(0.941f, 0.329f, 0.329f, 1.000f)
	},
	{
		"navy_grey_peach_wedding_vintage_night_dark",
		ImVec4(0.039f, 0.016f, 0.235f, 1.000f),
		ImVec4(0.012f, 0.314f, 0.435f, 1.000f),
		ImVec4(0.733f, 0.733f, 0.733f, 1.000f),
		ImVec4(1.000f, 0.890f, 0.847f, 1.000f)
	},
	{
		"black_yellow_grey_space_night_dark_neon",
		ImVec4(0.133f, 0.157f, 0.192f, 1.000f),
		ImVec4(0.224f, 0.243f, 0.275f, 1.000f),
		ImVec4(1.000f, 0.827f, 0.412f, 1.000f),
		ImVec4(0.933f, 0.933f, 0.933f, 1.000f)
	},
	{
		"black_grey_yellow_night_dark",
		ImVec4(0.224f, 0.192f, 0.114f, 1.000f),
		ImVec4(0.494f, 0.455f, 0.455f, 1.000f),
		ImVec4(0.769f, 0.714f, 0.714f, 1.000f),
		ImVec4(1.000f, 0.867f, 0.576f, 1.000f)
	},
	{
		"purple_black_dark_neon_night_space_cold",
		ImVec4(0.000f, 0.000f, 0.000f, 1.000f),
		ImVec4(0.322f, 0.020f, 0.482f, 1.000f),
		ImVec4(0.537f, 0.173f, 0.863f, 1.000f),
		ImVec4(0.737f, 0.435f, 0.945f, 1.000f)
	},
	{
		"black_brown_orange_warm_halloween_skin_dark_coffee_night",
		ImVec4(0.196f, 0.122f, 0.157f, 1.000f),
		ImVec4(0.451f, 0.251f, 0.275f, 1.000f),
		ImVec4(0.627f, 0.325f, 0.267f, 1.000f),
		ImVec4(0.906f, 0.620f, 0.310f, 1.000f)
	},
	{
		"pink_navy_black_cold_dark_wedding_night_vintage",
		ImVec4(0.957f, 0.671f, 0.769f, 1.000f),
		ImVec4(0.349f, 0.357f, 0.514f, 1.000f),
		ImVec4(0.200f, 0.204f, 0.337f, 1.000f),
		ImVec4(0.024f, 0.035f, 0.188f, 1.000f)
	},
	{
		"black_purple_beige_wedding_night_space_dark_gradient",
		ImVec4(0.000f, 0.000f, 0.000f, 1.000f),
		ImVec4(0.416f, 0.035f, 0.490f, 1.000f),
		ImVec4(0.753f, 0.376f, 0.631f, 1.000f),
		ImVec4(0.945f, 0.831f, 0.831f, 1.000f)
	},
	{
		"navy_blue_purple_teal_retro_space_kids_dark",
		ImVec4(0.082f, 0.016f, 0.522f, 1.000f),
		ImVec4(0.349f, 0.035f, 0.584f, 1.000f),
		ImVec4(0.776f, 0.165f, 0.533f, 1.000f),
		ImVec4(0.012f, 0.769f, 0.631f, 1.000f)
	},
	{
		"blue_navy_beige_dark_night_sea",
		ImVec4(0.114f, 0.176f, 0.314f, 1.000f),
		ImVec4(0.075f, 0.231f, 0.361f, 1.000f),
		ImVec4(0.118f, 0.373f, 0.455f, 1.000f),
		ImVec4(0.988f, 0.855f, 0.718f, 1.000f)
	},
	{
		"maroon_black_peach_beige_dark_night_vintage",
		ImVec4(0.490f, 0.024f, 0.200f, 1.000f),
		ImVec4(0.192f, 0.067f, 0.173f, 1.000f),
		ImVec4(0.949f, 0.627f, 0.482f, 1.000f),
		ImVec4(0.984f, 0.863f, 0.769f, 1.000f)
	},
	{
		"brown_orange_gold_warm_dark_coffee_skin_night_gradient",
		ImVec4(0.161f, 0.000f, 0.004f, 1.000f),
		ImVec4(0.529f, 0.263f, 0.114f, 1.000f),
		ImVec4(0.784f, 0.475f, 0.255f, 1.000f),
		ImVec4(0.859f, 0.796f, 0.741f, 1.000f)
	},
	{
		"black_navy_red_dark_space_night",
		ImVec4(0.102f, 0.102f, 0.180f, 1.000f),
		ImVec4(0.086f, 0.129f, 0.243f, 1.000f),
		ImVec4(0.059f, 0.204f, 0.376f, 1.000f),
		ImVec4(0.914f, 0.271f, 0.376f, 1.000f)
	},
	{
		"black_green_night_dark_nature_earth",
		ImVec4(0.220f, 0.161f, 0.200f, 1.000f),
		ImVec4(0.231f, 0.322f, 0.286f, 1.000f),
		ImVec4(0.318f, 0.596f, 0.447f, 1.000f),
		ImVec4(0.643f, 0.706f, 0.580f, 1.000f)
	},
	{
		"red_maroon_purple_dark",
		ImVec4(0.929f, 0.400f, 0.388f, 1.000f),
		ImVec4(0.710f, 0.169f, 0.396f, 1.000f),
		ImVec4(0.235f, 0.173f, 0.243f, 1.000f),
		ImVec4(0.349f, 0.251f, 0.361f, 1.000f)
	},
	{
		"teal_purple_dark_night_cold",
		ImVec4(0.294f, 0.365f, 0.404f, 1.000f),
		ImVec4(0.196f, 0.184f, 0.239f, 1.000f),
		ImVec4(0.349f, 0.251f, 0.361f, 1.000f),
		ImVec4(0.529f, 0.333f, 0.435f, 1.000f)
	},
	{
		"brown_orange_red_night_warm_dark",
		ImVec4(0.184f, 0.145f, 0.098f, 1.000f),
		ImVec4(0.290f, 0.247f, 0.208f, 1.000f),
		ImVec4(0.980f, 0.490f, 0.035f, 1.000f),
		ImVec4(1.000f, 0.263f, 0.004f, 1.000f)
	},
	{
		"purple_maroon_beige_space_dark",
		ImVec4(0.271f, 0.016f, 0.416f, 1.000f),
		ImVec4(0.361f, 0.165f, 0.616f, 1.000f),
		ImVec4(0.710f, 0.027f, 0.420f, 1.000f),
		ImVec4(0.945f, 0.922f, 0.733f, 1.000f)
	},
	{
		"navy_red_black_dark_space_night",
		ImVec4(0.086f, 0.141f, 0.278f, 1.000f),
		ImVec4(0.122f, 0.251f, 0.408f, 1.000f),
		ImVec4(0.106f, 0.106f, 0.184f, 1.000f),
		ImVec4(0.894f, 0.247f, 0.353f, 1.000f)
	},
	{
		"maroon_red_orange_warm_dark_sunset",
		ImVec4(0.318f, 0.094f, 0.271f, 1.000f),
		ImVec4(0.565f, 0.047f, 0.247f, 1.000f),
		ImVec4(0.780f, 0.000f, 0.224f, 1.000f),
		ImVec4(1.000f, 0.341f, 0.200f, 1.000f)
	},
	{
		"black_purple_red_orange_dark_space",
		ImVec4(0.125f, 0.125f, 0.251f, 1.000f),
		ImVec4(0.329f, 0.220f, 0.392f, 1.000f),
		ImVec4(1.000f, 0.388f, 0.388f, 1.000f),
		ImVec4(1.000f, 0.741f, 0.412f, 1.000f)
	},
	{
		"navy_dark_night_winter_gradient_sea",
		ImVec4(0.212f, 0.188f, 0.384f, 1.000f),
		ImVec4(0.302f, 0.298f, 0.490f, 1.000f),
		ImVec4(0.510f, 0.451f, 0.592f, 1.000f),
		ImVec4(0.847f, 0.725f, 0.765f, 1.000f)
	},
	{
		"black_grey_red_orange_dark_space",
		ImVec4(0.000f, 0.000f, 0.000f, 1.000f),
		ImVec4(0.196f, 0.196f, 0.196f, 1.000f),
		ImVec4(1.000f, 0.118f, 0.337f, 1.000f),
		ImVec4(1.000f, 0.675f, 0.255f, 1.000f)
	},
	{
		"navy_blue_purple_dark_night",
		ImVec4(0.255f, 0.235f, 0.412f, 1.000f),
		ImVec4(0.290f, 0.278f, 0.639f, 1.000f),
		ImVec4(0.678f, 0.384f, 0.667f, 1.000f),
		ImVec4(0.918f, 0.725f, 0.788f, 1.000f)
	},
	{
		"navy_blue_cold_winter_night_dark_sea",
		ImVec4(0.078f, 0.157f, 0.314f, 1.000f),
		ImVec4(0.153f, 0.286f, 0.427f, 1.000f),
		ImVec4(0.047f, 0.482f, 0.576f, 1.000f),
		ImVec4(0.000f, 0.659f, 0.800f, 1.000f)
	},
	{
		"maroon_red_teal_dark_wedding_space_retro",
		ImVec4(0.329f, 0.071f, 0.231f, 1.000f),
		ImVec4(0.518f, 0.078f, 0.176f, 1.000f),
		ImVec4(0.753f, 0.153f, 0.224f, 1.000f),
		ImVec4(0.161f, 0.780f, 0.675f, 1.000f)
	},
	{
		"black_navy_blue_winter_cold_dark_night_gradient_sea",
		ImVec4(0.106f, 0.149f, 0.173f, 1.000f),
		ImVec4(0.059f, 0.298f, 0.459f, 1.000f),
		ImVec4(0.196f, 0.510f, 0.722f, 1.000f),
		ImVec4(0.733f, 0.882f, 0.980f, 1.000f)
	},
	{
		"navy_blue_dark_cold_winter_night_sky_sea",
		ImVec4(0.082f, 0.098f, 0.396f, 1.000f),
		ImVec4(0.196f, 0.251f, 0.482f, 1.000f),
		ImVec4(0.318f, 0.333f, 0.522f, 1.000f),
		ImVec4(0.275f, 0.710f, 0.820f, 1.000f)
	},
	{
		"black_maroon_red_dark_night_halloween",
		ImVec4(0.176f, 0.075f, 0.173f, 1.000f),
		ImVec4(0.502f, 0.075f, 0.212f, 1.000f),
		ImVec4(0.780f, 0.173f, 0.255f, 1.000f),
		ImVec4(0.933f, 0.271f, 0.251f, 1.000f)
	},
	{
		"purple_peach_maroon_halloween_dark",
		ImVec4(0.000f, 0.247f, 0.361f, 1.000f),
		ImVec4(0.278f, 0.169f, 0.384f, 1.000f),
		ImVec4(0.737f, 0.282f, 0.451f, 1.000f),
		ImVec4(0.984f, 0.357f, 0.353f, 1.000f)
	},
	{
		"navy_blue_orange_night_dark",
		ImVec4(0.004f, 0.000f, 0.220f, 1.000f),
		ImVec4(0.161f, 0.227f, 0.502f, 1.000f),
		ImVec4(0.325f, 0.494f, 0.773f, 1.000f),
		ImVec4(0.953f, 0.580f, 0.133f, 1.000f)
	},
	{
		"navy_purple_peach_night_dark_halloween",
		ImVec4(0.035f, 0.000f, 0.341f, 1.000f),
		ImVec4(0.341f, 0.000f, 0.494f, 1.000f),
		ImVec4(0.769f, 0.000f, 0.776f, 1.000f),
		ImVec4(1.000f, 0.627f, 0.412f, 1.000f)
	},
	{
		"brown_black_red_orange_dark_warm_halloween_neon_space",
		ImVec4(0.267f, 0.216f, 0.216f, 1.000f),
		ImVec4(0.153f, 0.129f, 0.129f, 1.000f),
		ImVec4(1.000f, 0.000f, 0.000f, 1.000f),
		ImVec4(1.000f, 0.302f, 0.000f, 1.000f)
	},
	{
		"black_navy_purple_dark_night_halloween",
		ImVec4(0.125f, 0.125f, 0.251f, 1.000f),
		ImVec4(0.125f, 0.125f, 0.376f, 1.000f),
		ImVec4(0.376f, 0.125f, 0.502f, 1.000f),
		ImVec4(0.690f, 0.188f, 0.690f, 1.000f)
	},
};
