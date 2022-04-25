#version 430

layout(location = 0) in vec2 inUV;
layout(location = 0) out vec4 outColor;

uniform layout(binding = 0) sampler2D s_Image;

uniform vec3 u_nvVec = vec3(0.8, 0.3, 0.1);

uniform float loadTime = 0.0f;

uniform float isToggleOn = 0.0f;

uniform float deltaTime = 0.0f;

float goggleTime = 3.0f;
// we aim to dominant green color.
void main() { 
    vec4 nvColor;
    vec4 inColor = texture(s_Image, inUV);
    nvColor = inColor;

    float u_lum = dot(inColor.rgb, u_nvVec);
    nvColor.rgb = vec3(0.0, u_lum, 0.0);

    if(isToggleOn == 0.0f)
    {
        if(loadTime > 0.1f)
        {
            float IncPercent = (goggleTime - loadTime) / goggleTime;

            vec2 pixelUV = floor(inUV * (IncPercent * 300)) / (IncPercent * 300);
            vec4 pixelColor = texture(s_Image, pixelUV);
            
            nvColor.g = u_lum * IncPercent;

            outColor = mix(nvColor, pixelColor, IncPercent);
        }
        else
        {
            outColor = inColor;
        }
    }
    
    if(isToggleOn == 1.0f)
    {
        if(loadTime > 0.1f)
        {
            float IncPercent = (goggleTime - loadTime) / goggleTime;

            vec2 pixelUV = floor(inUV * (IncPercent * 300)) / (IncPercent * 300);
            vec4 pixelColor = texture(s_Image, pixelUV);
            float lum_pix = dot(pixelColor.rgb, u_nvVec);
            pixelColor.rgb = vec3(0.0f, IncPercent * lum_pix, 0.0f);

            nvColor.g = u_lum * IncPercent;
            
            outColor = mix(pixelColor, nvColor, IncPercent);
        }
        else
        {
            outColor = nvColor;
        }
    }
}
