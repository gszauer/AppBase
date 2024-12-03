#define SOKOL_IMPL
#define IMPL_ROBOTO
#define IMGUI_DEFINE_MATH_OPERATORS

#if defined(_WIN32)
#define SOKOL_GLCORE
#elif defined(__EMSCRIPTEN__)
#define SOKOL_GLES3
#endif

#include "sokol_app.h"
#include "sokol_audio.h"
#include "sokol_fetch.h"
#include "sokol_gfx.h"
#include "sokol_glue.h"
#include "imgui.h"
#include "sokol_imgui.h"
#include "sokol_log.h"
#include "sokol_shape.h"
#include "sokol_time.h"
#include "Roboto.h"