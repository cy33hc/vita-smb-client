// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan graphics context creation, etc.)

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the sdl_opengl3_example/ folder**
// See imgui_impl_sdl.cpp for details.

#include <imgui_vita.h>
#include <stdio.h>
#include <vita2d.h>

#include "textures.h"
#include "config.h"
#include "gui.h"
#include "fs.h"
#include "net.h"
#include "lang.h"
#include "util.h"
#include "debugScreen.h"

int console_language;
namespace Services
{
	int InitImGui(void)
	{

		// Setup ImGui binding
		ImGui::CreateContext();

		ImGuiIO &io = ImGui::GetIO();
		io.MouseDrawCursor = false;
		io.KeyRepeatRate = 0.005f;
		ImGui::StyleColorsDark();
		auto &style = ImGui::GetStyle();
		ImGui::GetIO().Fonts->Clear();

		static const ImWchar ranges[] = { // All languages with chinese included
			0x0020, 0x00FF, // Basic Latin + Latin Supplement
			0x0100, 0x024F, // Latin Extended
			0x0370, 0x03FF, // Greek
			0x0400, 0x052F, // Cyrillic + Cyrillic Supplement
			0x0590, 0x05FF, // Hebrew
			0x1E00, 0x1EFF, // Latin Extended Additional
			0x1F00, 0x1FFF, // Greek Extended
			0x2000, 0x206F, // General Punctuation
			0x2DE0, 0x2DFF, // Cyrillic Extended-A
			0x2E80, 0x2EFF, // CJK Radicals Supplement
			0x3000, 0x30FF, // CJK Symbols and Punctuations, Hiragana, Katakana
			0x31F0, 0x31FF, // Katakana Phonetic Extensions
			0x3400, 0x4DBF, // CJK Rare
			0x4E00, 0x9FFF, // CJK Ideograms
			0xA640, 0xA69F, // Cyrillic Extended-B
			0xF900, 0xFAFF, // CJK Compatibility Ideographs
			0xFF00, 0xFFEF, // Half-width characters
			0,
		};

		std::string lang = std::string(language);
		lang = Util::Trim(lang, " ");
		if (lang.size() > 0)
		{
			if (strcmp(lang.c_str(), "Ryukyuan") == 0)
			{
				io.Fonts->AddFontFromFileTTF(
					"sa0:/data/font/pvf/jpn0.pvf",
					16.0f,
					NULL,
					io.Fonts->GetGlyphRangesJapanese());
			}
			else if (strcmp(lang.c_str(), "Thai") == 0)
			{
				io.Fonts->AddFontFromFileTTF(
					"ux0:app/SMBCLI001/lang/Thai.ttf",
					20.0f,
					NULL,
					io.Fonts->GetGlyphRangesThai());
			}
			else if (strcmp(lang.c_str(), "Vietnamese") == 0)
			{
				io.Fonts->AddFontFromFileTTF(
					"ux0:app/SMBCLI001/lang/Vietnamese.ttf",
					16.0f,
					NULL,
					io.Fonts->GetGlyphRangesVietnamese());
			}
			else
			{
				io.Fonts->AddFontFromFileTTF(
					"sa0:/data/font/pvf/ltn0.pvf",
					16.0f,
					NULL,
					ranges);
			}
		}
		else
		{
			switch (console_language)
			{
			case SCE_SYSTEM_PARAM_LANG_CHINESE_S:
				io.Fonts->AddFontFromFileTTF(
					"sa0:/data/font/pvf/cn0.pvf",
					17.0f,
					NULL,
					io.Fonts->GetGlyphRangesChineseSimplifiedCommon());
				break;
			case SCE_SYSTEM_PARAM_LANG_CHINESE_T:
				io.Fonts->AddFontFromFileTTF(
					"sa0:/data/font/pvf/cn0.pvf",
					16.0f,
					NULL,
					io.Fonts->GetGlyphRangesChineseFull());
				break;
			case SCE_SYSTEM_PARAM_LANG_KOREAN:
				{
					ImFontConfig config;
					config.MergeMode = true;
					io.Fonts->AddFontFromFileTTF(
						"sa0:/data/font/pvf/ltn0.pvf",
						16.0f,
						NULL,
						io.Fonts->GetGlyphRangesDefault());
					io.Fonts->AddFontFromFileTTF(
						"sa0:/data/font/pvf/kr0.pvf",
						16.0f,
						&config,
						io.Fonts->GetGlyphRangesKorean());
					io.Fonts->Build();
				}
				break;
			case SCE_SYSTEM_PARAM_LANG_JAPANESE:
				io.Fonts->AddFontFromFileTTF(
					"sa0:/data/font/pvf/jpn0.pvf",
					16.0f,
					NULL,
					io.Fonts->GetGlyphRangesJapanese());
				break;
			default:
				io.Fonts->AddFontFromFileTTF(
					"sa0:/data/font/pvf/ltn0.pvf",
					16.0f,
					NULL,
					ranges);
				break;
			}
		}

		style.AntiAliasedLinesUseTex = false;
		style.AntiAliasedLines = true;
		style.AntiAliasedFill = true;
		style.WindowRounding = 0.0f;
		style.FrameRounding = 2.0f;
		style.GrabRounding = 2.0f;

		//Style::LoadStyle(style_path);
        ImVec4 *colors = ImGui::GetStyle().Colors;
		style.Colors[ImGuiCol_Text] = ImVec4( 236.f / 255.f, 240.f / 255.f, 241.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_TextDisabled] = ImVec4( 236.f / 255.f, 240.f / 255.f, 241.f / 255.f, 0.58f );
		style.Colors[ImGuiCol_WindowBg] = ImVec4( 44.f / 255.f, 62.f / 255.f, 80.f / 255.f, 0.95f );
		style.Colors[ImGuiCol_ChildBg] = ImVec4( 57.f / 255.f, 79.f / 255.f, 105.f / 255.f, 0.58f );
		style.Colors[ImGuiCol_Border] = ImVec4( 44.f / 255.f, 62.f / 255.f, 80.f / 255.f, 0.00f );
		style.Colors[ImGuiCol_BorderShadow] = ImVec4( 44.f / 255.f, 62.f / 255.f, 80.f / 255.f, 0.00f );
		style.Colors[ImGuiCol_FrameBg] = ImVec4( 57.f / 255.f, 79.f / 255.f, 105.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_FrameBgHovered] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.78f );
		style.Colors[ImGuiCol_FrameBgActive] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_TitleBg] = ImVec4( 57.f / 255.f, 79.f / 255.f, 105.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4( 57.f / 255.f, 79.f / 255.f, 105.f / 255.f, 0.75f );
		style.Colors[ImGuiCol_TitleBgActive] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_MenuBarBg] = ImVec4( 57.f / 255.f, 79.f / 255.f, 105.f / 255.f, 0.47f );
		style.Colors[ImGuiCol_ScrollbarBg] = ImVec4( 57.f / 255.f, 79.f / 255.f, 105.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_ScrollbarGrab] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.21f );
		style.Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.78f );
		style.Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_CheckMark] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.80f );
		style.Colors[ImGuiCol_SliderGrab] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.50f );
		style.Colors[ImGuiCol_SliderGrabActive] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_Button] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.50f );
		style.Colors[ImGuiCol_ButtonHovered] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.86f );
		style.Colors[ImGuiCol_ButtonActive] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_Header] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.76f );
		style.Colors[ImGuiCol_HeaderHovered] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.86f );
		style.Colors[ImGuiCol_HeaderActive] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_ResizeGrip] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.15f );
		style.Colors[ImGuiCol_ResizeGripHovered] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.78f );
		style.Colors[ImGuiCol_ResizeGripActive] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_PlotLines] = ImVec4( 236.f / 255.f, 240.f / 255.f, 241.f / 255.f, 0.63f );
		style.Colors[ImGuiCol_PlotLinesHovered] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_PlotHistogram] = ImVec4( 236.f / 255.f, 240.f / 255.f, 241.f / 255.f, 0.63f );
		style.Colors[ImGuiCol_PlotHistogramHovered] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 1.00f );
		style.Colors[ImGuiCol_TextSelectedBg] = ImVec4( 41.f / 255.f, 128.f / 255.f, 185.f / 255.f, 0.43f );
		style.Colors[ImGuiCol_PopupBg] = ImVec4( 33.f / 255.f, 46.f / 255.f, 60.f / 255.f, 0.92f );

		vglInitExtended(0, 960, 544, 0x1800000, SCE_GXM_MULTISAMPLE_4X);
		ImGui::CreateContext();
		ImGui_ImplVitaGL_Init();

		ImGui_ImplVitaGL_TouchUsage(true);
		ImGui_ImplVitaGL_UseIndirectFrontTouch(false);
		ImGui_ImplVitaGL_UseRearTouch(false);
		ImGui_ImplVitaGL_GamepadUsage(true);
		ImGui_ImplVitaGL_MouseStickUsage(false);
		ImGui_ImplVitaGL_DisableButtons(SCE_CTRL_SQUARE);
		ImGui_ImplVitaGL_SetAnalogRepeatDelay(1000);

		Textures::Init();

		return 0;
	}

	void ExitImGui(void)
	{
		Textures::Exit();

		// Cleanup
		ImGui_ImplVitaGL_Shutdown();
		ImGui::DestroyContext();
	}

	void initSceAppUtil()
	{
		// Init SceAppUtil
		SceAppUtilInitParam init_param;
		SceAppUtilBootParam boot_param;
		memset(&init_param, 0, sizeof(SceAppUtilInitParam));
		memset(&boot_param, 0, sizeof(SceAppUtilBootParam));
		sceAppUtilInit(&init_param, &boot_param);
		sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, &console_language);

		// Set common dialog config
		SceCommonDialogConfigParam config;
		sceCommonDialogConfigParamInit(&config);
		sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_LANG, (int *)&config.language);
		sceAppUtilSystemParamGetInt(SCE_SYSTEM_PARAM_ID_ENTER_BUTTON, (int *)&config.enterButtonAssign);
		sceCommonDialogSetConfigParam(&config);

		uint32_t scepaf_argp[] = { 0x400000, 0xEA60, 0x40000, 0, 0 };

		SceSysmoduleOpt option;
        option.flags = 0;
        option.result = (int *)&option.flags;
		sceSysmoduleLoadModuleInternalWithArg(SCE_SYSMODULE_INTERNAL_PAF, sizeof(scepaf_argp), scepaf_argp, &option);
		sceSysmoduleLoadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);
		scePromoterUtilityInit();
	}

	int Init(void)
	{
		scePowerSetArmClockFrequency(444);
		scePowerSetBusClockFrequency(222);
		scePowerSetGpuXbarClockFrequency(166);
		scePowerSetGpuClockFrequency(222);

		// Allow writing to ux0:app/VITASHELL
		sceAppMgrUmount("app0:");
		sceAppMgrUmount("savedata0:");

		vita2d_init();
		vita2d_set_clear_color(RGBA8(0x00, 0x00, 0x00, 0xFF));

		initSceAppUtil();

		CONFIG::LoadConfig();
		Lang::SetTranslation(console_language);

		return 0;
	}

	void Exit(void)
	{
		// Shutdown AppUtil
		sceAppUtilShutdown();
		scePromoterUtilityExit();
		sceSysmoduleUnloadModuleInternal(SCE_SYSMODULE_INTERNAL_PROMOTER_UTIL);
		vita2d_fini();
	}
} // namespace Services

#define ip_server "192.168.100.14"
#define port_server 18194

unsigned int _newlib_heap_size_user = 164 * 1024 * 1024;

int main(int, char **)
{
	//debugNetInit(ip_server,port_server, DEBUG);
	if (!FS::FileExists("ur0:/data/libshacccg.suprx") && !FS::FileExists("ur0:/data/external/libshacccg.suprx"))
	{
		psvDebugScreenInit();
		psvDebugScreenSetFont(psvDebugScreenScaleFont2x(psvDebugScreenGetFont()));
		psvDebugScreenPrintf("\n\nlibshacccg.suprx is missing.\n\n");
		psvDebugScreenPrintf("Please extract it before proceeding");
		sceKernelDelayThread(5000000);
		sceKernelExitProcess(0);
	}

	Net::Init();
	Services::Init();
	Services::InitImGui();

	GUI::RenderLoop();

	Services::ExitImGui();
	Services::Exit();
	Net::Exit();

	return 0;
}
