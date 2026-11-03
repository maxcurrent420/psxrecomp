#ifndef PSX_FAST_FORWARD_H
#define PSX_FAST_FORWARD_H
#include <math.h>
#include <stdlib.h>
#include <string.h>
#ifdef __cplusplus
#include <cmath>
#endif

/* User preference: tenths of real time, 1.0..5.0. No guest-clock change when
 * inactive; the host divides its wall-clock wait only while forwarding. */
static inline double psx_fast_forward_preference(double value) {
#ifdef __cplusplus
    if (!std::isfinite(value)) return 4.0;
#else
    if (!isfinite(value)) return 4.0;
#endif
    if (value < 1.0) value = 1.0;
    if (value > 5.0) value = 5.0;
    return floor(value * 10.0 + 0.5) / 10.0;
}
static inline double psx_fast_forward_target(double preference, const char *env) {
    if (!env || !*env) return psx_fast_forward_preference(preference);
    if (!strcmp(env,"max") || !strcmp(env,"MAX") || !strcmp(env,"0")) return -1.0;
    char *end;
    const double value = strtod(env, &end);
    /* Preserve the developer override's wider range without truncating it. */
#ifdef __cplusplus
    return end != env && !*end && std::isfinite(value) && value >= 1.0 && value <= 16.0
#else
    return end != env && !*end && isfinite(value) && value >= 1.0 && value <= 16.0
#endif
        ? floor(value * 10.0 + 0.5) / 10.0 : psx_fast_forward_preference(preference);
}
static inline int psx_fast_forward_present_every(double factor) {
    /* Keep accelerated render submissions at/below the stock cadence, also
     * on a 50 Hz panel with VSync. 1x must not discard alternate frames. */
    return factor < 0.0 ? 4 : (int)ceil(factor);
}
#endif
