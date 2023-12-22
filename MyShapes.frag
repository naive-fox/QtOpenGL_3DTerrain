#version 330 core
out vec4 FragColor;
in vec3 ourColor;
in vec2 TexCord;
uniform sampler2D texture0;
uniform float zoomratio;
vec3 color;
void main(){
    float height = ourColor.y * zoomratio;
    if(height > 1250.0 )
    {
        color = vec3(1.0,0.298,0.0);
    }
    else if(height > 650.0)
    {
        color = vec3(1.0,(650.0 - height)/850.0 + 1.0,0.0);
    }
    else if(height > 300.0 )
    {
        color = vec3((height - 300.0)/350.0,1.0,0.0);
    }
    else if(height > 120.0)
    {
        color = vec3(0.0, 1.0, (120.0 - height) / 180.0 + 1.0);
    }
    else if(height > 0.0)
    {
        color = vec3(0.0,(2.0 * height + 15.0) / 255.0,1.0);
    }
    else
    {
        color = vec3(0.0,0.0,1.0);
    }

    FragColor = vec4(color,1.0);
}