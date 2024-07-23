
float32_t3 RGBToHSV(float32_t3 rgb)
{
    float32_t max = rgb.x > rgb.y ? rgb.x : rgb.y;
    max = max > rgb.z ? max : rgb.z;
    float32_t min = rgb.x < rgb.y ? rgb.x : rgb.y;
    min = min < rgb.z ? min : rgb.z;
    
   float32_t h = max - min;
    if (h > 0.0f)
    {
        if (max == rgb.x)
        {
            h = (rgb.y - rgb.z) / h;
            if (h < 0.0f)
            {
                h += 6.0f;
            }
        }
        else if (max == rgb.y)
        {
            h = 2.0f + (rgb.z - rgb.x) / h;
        }
        else
        {
            h = 4.0f + (rgb.x - rgb.y) / h;
        }
    }
    
    h /= 6.0f;
    float32_t s = (max - min);
    if (max != 0.0f)
    {
        s /= max;
    }
    
    float32_t v = max;
    float32_t3 result = { h, s, v };
    
    return result;
    
}

float32_t3 HSVToRGB(float32_t3 hsv)
{
    float32_t r = hsv.z;
    float32_t g = hsv.z;
    float32_t b = hsv.z;
    
    if (hsv.y > 0.0f)
    {
        hsv.x *= 6.0f;
        int32_t i = (int32_t) hsv.x;
        float32_t f = hsv.x - (float32_t) i;
        switch (i)
        {
            case 0:
                g *= 1 - hsv.y * (1 - f);
                b *= 1 - hsv.y;
                break;
            case 1:
                r *= 1 - hsv.y * f;
                b *= 1 - hsv.y;
                break;
            case 2:
                r *= 1 - hsv.y;
                b *= 1 - hsv.y * (1 - f);
                break;
            case 3:
                r *= 1 - hsv.y;
                g *= 1 - hsv.y * f;
                break;
            case 4:
                r *= 1 - hsv.y * (1 - f);
                g *= 1 - hsv.y;
                break;
            case 5:
                g *= 1 - hsv.y;
                b *= 1 - hsv.y * f;
                break;
        }
    }
    
    float32_t3 result = { r, g, b };
    return result;

}