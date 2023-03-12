#ifdef RGB_MATRIX_KEYREACTIVE_ENABLED
#    ifdef ENABLE_RGB_MATRIX_SOLID_REACTIVE
RGB_MATRIX_EFFECT(SOLID_REACTIVE)
#        ifdef RGB_MATRIX_CUSTOM_EFFECT_IMPLS

static HSV SOLID_REACTIVE_math(HSV hsv, uint16_t offset) {
//     hsv.h += qsub8(130, offset);

    //using saturation setting to control secondary color and then setting all color's saturation back to full
    int primaryHue = (int)hsv.h; //Primary color is color setting
    if (hsv.s == 0) {
        hsv.s = 1;
    }
    int secondaryHue = (int)hsv.s; //Secondary color is saturation setting

    int hueDifference = primaryHue - secondaryHue;
    int hueShift;

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


    //keys that have been pressed get offset towards the secondary color
    //subtracting the offset channel value blueshifts the hue of secondary color
    //I want the hueshift direction (+=, -=) to go towards the closest. (teal -> purple) (purple <- teal)
    //To do this, I need to assign color hue and find which direction is closer..

    if (hueShift > 0) {
    hsv.h += qsub8((uint8_t)hueShift, offset);
    }
    else {
    hueShift *= -1;
    hsv.h -= qsub8((uint8_t)hueShift, offset);
    }


    //I want a lower background brightness with pressed keys starting at secondary color bright
    //then dimming with their hue shift back to the background brightness
    uint8_t brightness = (uint8_t)(hsv.v / 3); //background 1/3rd of highlight

    brightness += qsub8((brightness * 2), offset);
    hsv.v = brightness;

//OLD NOTES: This was implemented
    //It'd probably be better to have the brightness setting be the max brightness (the brightness a keypress goes to)
    //Which would mean setting any keys that haven't been pressed recently (have no offset?)
    //to a fraction of the brightness setting. I think 1/2 would be good

    return hsv;
}

bool SOLID_REACTIVE(effect_params_t* params) {
    return effect_runner_reactive(params, &SOLID_REACTIVE_math);
}

#        endif // RGB_MATRIX_CUSTOM_EFFECT_IMPLS
#    endif     // ENABLE_RGB_MATRIX_SOLID_REACTIVE
#endif         // RGB_MATRIX_KEYREACTIVE_ENABLED
