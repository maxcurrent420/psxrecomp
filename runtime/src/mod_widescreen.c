/*
 * Twisted Metal 2 widescreen mod — forces 16:9 squash-mode widescreen.
 *
 * Per docs/WIDESCREEN.md: squash mode compresses a wider FOV into the 320
 * framebuffer and presents it stretched. The runtime's
 * refresh_widescreen_projection() runs after VBlank and handles
 * gte_set_display_aspect + gpu_ws_configure once g_ws_engaged is true
 * (game started) and native_wide=false (mode=1 squash). This mod also
 * calls gl_renderer_set_display_aspect to update the OpenGL present
 * letterbox, which is set once at init and never updated afterward.
 *
 * Toggle in the launcher or pause-menu Mods -> TM2 Widescreen.
 */
#include "mod_plugins.h"
#include "gpu.h"
#include "cpu_state.h"
#include "gpu_gl_renderer.h"

#include <stdint.h>

/* ---- VBlank callback ---- */

static void vblank_widescreen(void) {
    psx_mod_set_fixed_display_aspect(16, 9);
    gl_renderer_set_display_aspect(16, 9);
    gte_set_display_aspect(16, 9);
    gpu_ws_configure(16, 9, 0, 0, 1);
}

/* ---- Plugin registration constructor ---- */
PSX_MOD_CONSTRUCTOR(psx_register_tm2_widescreen_plugins) {
    psx_mod_register_vblank_plugin("tm2.widescreen", vblank_widescreen);
}
