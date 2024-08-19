#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;

// NOTE: Add here your custom variables

// NOTE: Render size values must be passed from code
const float renderWidth = 640;
const float renderHeight = 480;

uniform float pixelWidth = 1.0;
uniform float pixelHeight = 1.0;

void main()
{
    vec2 centerPoint = vec2(renderWidth/2, renderHeight/2);
    centerPoint = centerPoint + 8;
    float centerPointMagnitude = length(centerPoint);

    float intensity = distance(gl_FragCoord.xy, centerPoint);

    vec3 tc = texture(texture0, fragTexCoord).rgb;

    finalColor = vec4(tc.rgb, centerPointMagnitude/intensity);
    finalColor = vec4(tc, 1.0);
}
