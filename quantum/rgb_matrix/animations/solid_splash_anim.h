#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
#    if defined(ENABLE_RGB_MATRIX_SOLID_SPLASH) || defined(ENABLE_RGB_MATRIX_SOLID_MULTISPLASH)

#        ifdef ENABLE_RGB_MATRIX_SOLID_SPLASH
RGB_MATRIX_EFFECT(SOLID_SPLASH)
#        endif

#        ifdef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
RGB_MATRIX_EFFECT(SOLID_MULTISPLASH)
#        endif

#        ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS





HSV SOLID_SPLASH_math(HSV hsv, int16_t dx, int16_t dy, uint8_t dist, uint16_t tick) {
    //using saturation setting to control secondary color and then setting all color's saturation back to full
    int primaryHue = (int)hsv.h; //Primary color is color setting
    if (hsv.s == 0) {
        hsv.s = 1;
    }
    int secondaryHue = (int)hsv.s; //Secondary color is saturation setting

    int hueDifference = primaryHue - secondaryHue;
    int hueShift;
    uint8_t minBrightness = hsv.v / 3;

    if (hueDifference < 128 && hueDifference > -128) {
      hueShift = - hueDifference;
    }
    else if (hueDifference > 0) {
      hueShift = 255 - hueDifference;
    }
    else {
      hueShift = -(255 + hueDifference);
    }
    hsv.s = 255;


    //Set effect (default code);
    uint16_t effect = tick - dist;
    if (effect > 255) effect = 255;


    //Get percent (0 - 1) of effect strength
    //effect 0 - 255 -> strength 0 - 1
    double strength = 0;
    normalStrength = qadd8(0, 255 - effect);
    if (normalStrength != 0) {
        strength = normalStrength / 255;
    }
    double strength = effect/255;


    //Set color
    //keys that have been pressed get offset towards the secondary color
    //subtracting the offset channel value blueshifts the hue of secondary color
    //I want the hueshift direction (+=, -=) to go towards the closest. (teal -> purple) (purple <- teal)
    //To do this, I need to assign color hue and find which direction is closer..

    //We want maximum change to be hueShift and minimum change to be zero
    //Positive hue shift to secondary color
    if (hueShift > 0) {
         hsv.h += (strength * hueShift);
    }
    //Negative hue shift to secondary color
    else {
    hueShift *= -1;
         hsv.h -= (strength * hueShift);
    }

    //Set brightness
    //I want a lower background brightness with pressed keys starting at secondary color bright
    //then dimming with their hue shift back to the background brightness
    uint8_t brightness = (uint8_t)(minBrightness); //background 1/3rd of highlight

    brightness += (strength * (brightness * 2));
    hsv.v = brightness;


    return hsv;

    //hsv.v = qadd8(hsv.v, 255 - effect); ORIGINAL
}





#            ifdef ENABLE_RGB_MATRIX_SOLID_SPLASH
bool SOLID_SPLASH(effect_params_t* params) {
    return effect_runner_reactive_splash(qsub8(g_last_hit_tracker.count, 1), params, &SOLID_SPLASH_math);
}
#            endif

#            ifdef ENABLE_RGB_MATRIX_SOLID_MULTISPLASH
bool SOLID_MULTISPLASH(effect_params_t* params) {
    return effect_runner_reactive_splash(0, params, &SOLID_SPLASH_math);
}
#            endif

#        endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#    endif     // !defined(ENABLE_RGB_MATRIX_SPLASH) && !defined(ENABLE_RGB_MATRIX_MULTISPLASH)
#endif         // RGB_MATRIX_KEYREACTIVE_ENABLED
