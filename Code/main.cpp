//------------------------------------------------------------------------------
//  imgui-dock-sapp.c
//
//  Using the Dear ImGui docking branch.
//------------------------------------------------------------------------------

#define IMGUI_DEFINE_MATH_OPERATORS
#include "../Framework/sokol_app.h"
#include "../Framework/sokol_gfx.h"
#include "../Framework/sokol_log.h"
#include "../Framework/sokol_glue.h"
#include "../Framework/imgui.h"
#include "../Framework/sokol_imgui.h"
#include "../Framework/Roboto.h"

static bool show_test_window = true;
static bool show_another_window = false;

static sg_pass_action pass_action;

void init(void) {
    // setup sokol-gfx, sokol-time and sokol-imgui
    sg_desc desc = { };
    desc.environment = sglue_environment();
    desc.logger.func = slog_func;
    sg_setup(&desc);

    // use sokol-imgui with all default-options (we're not doing
    // multi-sampled rendering or using non-default pixel formats)
    simgui_desc_t simgui_desc = { };
    simgui_desc.logger.func = slog_func;
    simgui_desc.no_default_font = true;
    simgui_setup(&simgui_desc);
    ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
#if 1
    // configure Dear ImGui with our own embedded font
    auto& io = ImGui::GetIO();
    ImFontConfig fontCfg;
    fontCfg.FontDataOwnedByAtlas = false;
    fontCfg.OversampleH = 2;
    fontCfg.OversampleV = 2;
    fontCfg.RasterizerMultiply = 1.5f;
    io.Fonts->AddFontFromMemoryTTF((void*)roboto, roboto_size, 12.0f, &fontCfg);

    // create font texture and linear-filtering sampler for the custom font
    // NOTE: linear filtering looks better on low-dpi displays, while
    // nearest-filtering looks better on high-dpi displays
    simgui_font_tex_desc_t font_texture_desc = { };
    font_texture_desc.min_filter = SG_FILTER_LINEAR;
    font_texture_desc.mag_filter = SG_FILTER_LINEAR;
    simgui_create_fonts_texture(&font_texture_desc);
#endif
    // initial clear color
    pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass_action.colors[0].clear_value = { 0.3f, 0.7f, 0.5f, 1.0f };
}

void frame(void) {
    const int width = sapp_width();
    const int height = sapp_height();
    simgui_new_frame({ width, height, sapp_frame_duration(), sapp_dpi_scale() });

    // 1. Show a simple window
    // Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
    static float f = 0.0f;
    ImGui::Text("Drag windows over one another!");
    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);
    ImGui::ColorEdit3("clear color", &pass_action.colors[0].clear_value.r);
    if (ImGui::Button("Test Window")) show_test_window ^= 1;
    if (ImGui::Button("Another Window")) show_another_window ^= 1;
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

    // 2. Show another simple window, this time using an explicit Begin/End pair
    if (show_another_window) {
        ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiCond_FirstUseEver);
        ImGui::Begin("Another Window", &show_another_window);
        ImGui::Text("Hello");
        ImGui::End();
    }

    // 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowDemoWindow()
    if (show_test_window) {
        ImGui::SetNextWindowPos(ImVec2(460, 20), ImGuiCond_FirstUseEver);
        ImGui::ShowDemoWindow();
    }

    // the sokol_gfx draw pass
    sg_pass pass = {};
    pass.action = pass_action;
    pass.swapchain = sglue_swapchain();
    sg_begin_pass(&pass);
    simgui_render();
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    simgui_shutdown();
    sg_shutdown();
}

void input(const sapp_event* event) {
    simgui_handle_event(event);
}

sapp_desc sokol_main(int argc, char* argv[]) {
    (void)argc; (void)argv;
    sapp_desc desc = { };
    desc.init_cb = init;
    desc.frame_cb = frame;
    desc.cleanup_cb = cleanup;
    desc.event_cb = input;
    desc.width = 1024;
    desc.height = 768;
    desc.high_dpi = true;
    desc.window_title = "Dear ImGui Docking (sokol-app)";
    desc.ios_keyboard_resizes_canvas = false;
    desc.icon.sokol_default = true;
    desc.enable_clipboard = false;
    desc.logger.func = slog_func;
    return desc;
}


#if 0
//------------------------------------------------------------------------------
//  clear-sapp.c
//------------------------------------------------------------------------------
#include "../Framework/sokol_gfx.h"
#include "../Framework/sokol_app.h"
#include "../Framework/sokol_glue.h"
#include <cstring>

sg_pass_action pass_action;

void init(void) {
    sg_desc desc = { 0 };
    desc.environment = sglue_environment();
    sg_setup(&desc);

    memset(&pass_action, 0, sizeof(sg_pass_action));
    pass_action.colors[0].load_action = SG_LOADACTION_CLEAR;
    pass_action.colors[0].clear_value = { 1.0f, 0.0f, 0.0f, 1.0f };
}

void frame(void) {
    float g = pass_action.colors[0].clear_value.g + 0.01f;
    pass_action.colors[0].clear_value.g = (g > 1.0f) ? 0.0f : g;
    
    sg_pass pass = { 0 };
    pass.action = pass_action;
    pass.swapchain = sglue_swapchain();

    sg_begin_pass(&pass);
    sg_end_pass();
    sg_commit();
}

void cleanup(void) {
    sg_shutdown();
}

sapp_desc sokol_main(int argc, char* argv[]) {
    sapp_desc app = { 0 };
    app.init_cb = init;
    app.frame_cb = frame;
    app.cleanup_cb = cleanup;
    app.width = 400;
    app.height = 300;
    app.window_title = "Clear (sokol app)";
    app.icon.sokol_default = true;

    return app;
}
#endif