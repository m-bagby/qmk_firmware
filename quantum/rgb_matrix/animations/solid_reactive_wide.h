#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
#    if defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE) || defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE)

#        ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
RGB_MATRIX_EFFECT(SOLID_REACTIVE_WIDE)
#        endif

#        ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
RGB_MATRIX_EFFECT(SOLID_REACTIVE_MULTIWIDE)
#        endif

#        ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static HSV SOLID_REACTIVE_WIDE_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    uint8_t primaryHue = hsv.h; //Primary color is color setting
    uint8_t secondaryHue = rgb_matrix_config.hsv.s; //Secondary color is saturation setting
    uint8_t minBrightness = rgb_matrix_config.hsv.v; //Brightness used for background
    uint8_t maxBrightness = 255; //Max brightness hardcoded to full 255

    //Reset saturation to full (since we used the setting for secondary color)
    hsv.s = 255;

    //Set splash width. Lower number = wider splash
    uint16_t effect = tick + dist * 7;
    if (effect > 255) effect = 255;

    //Set "Strength" (0 to 255)
    //255 = Center of press (Strongest interaction)
    //0 = Far away or time passed (Back to normal)
    uint8_t strength = 255 - effect;

    //Shift hue
    if (strength > 0) {
        //Calculate the raw distance between the two hues
        //Casting to int8_t automatically finds the shortest path (-128 to +127)
        int8_t hueDiff = (int8_t)(secondaryHue - primaryHue);

        //Calculate how much of that distance we have traversed based on strength
        //(hueDiff * strength) / 255
        int8_t hueShift = (int8_t)scale8((uint8_t)abs(hueDiff), strength);

        //Restore the sign of the shift
        if (hueDiff < 0) hueShift = -hueShift;

//         hsv.h = primaryHue + hueShift;
        hsv.h += hueShift;
    }

    //Shift brightness
    uint8_t headroom = maxBrightness - minBrightness; //how much increase in brightness to hit max
    uint8_t brightnessShift = scale8(headroom, strength);
    //Using qadd8 to prevent wrapping if math goes slightly over 255
//     hsv.v = qadd8(minBrightness, brightnessShift);
    hsv.v = qadd8(hsv.v, brightnessShift);

//ORIGINAL
// #            ifdef RGB_MATRIX_SOLID_REACTIVE_GRADIENT_MODE
//     hsv.h = scale16by8(g_rgb_timer, qadd8(rgb_matrix_config.speed, 8) >> 4);
// #            endif
//     hsv.v = qadd8(hsv.v, 255 - effect);

    return hsv;
}

#            ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE
bool SOLID_REACTIVE_WIDE(effect_params_t* params) {
    return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &SOLID_REACTIVE_WIDE_math);
}
#            endif

#            ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE
bool SOLID_REACTIVE_MULTIWIDE(effect_params_t* params) {
    return effect_runner_reactive_splash(0, params, &SOLID_REACTIVE_WIDE_math);
}
#            endif

#        endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#    endif     // !defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_WIDE) || !defined(ENABLE_RGB_MATRIX_SOLID_REACTIVE_MULTIWIDE)
#endif         // RGB_MATRIX_KEYREACTIVE_ENABLED
