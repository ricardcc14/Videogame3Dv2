#version 330 core
out vec4 FragColor;

in vec3 color;

in vec3 normals;
in vec2 uvs;
in mat3 TBN;

uniform sampler2D tex0;
uniform sampler2D texNormals;

uniform vec3 tint = vec3(1.0, 1.0, 1.0);

uniform vec3 lightDirection = vec3(5.0, -5.0, 0.0);


uniform bool redOnly = false;
void main()
{
    vec4 texColor = texture(tex0, uvs);
    //FragColor = vec4(texColor.r, texColor.g, texColor.b, texColor.a);
    if (redOnly){
        FragColor = vec4(texColor.r, 0.0f, 0.0f, texColor.a);
    } else { 
        vec3 normalMap = (texture(texNormals, uvs).rgb * 2.0f) -1.0f;
        normalMap = normalize(TBN * normalMap); 


        float dotProduct = 1- (dot(normalize(lightDirection), normalize(normalMap)) + 1)/2.0f;

        dotProduct = clamp(dotProduct, 0, 1);

        vec4 directionalLight = vec4(dotProduct, dotProduct, dotProduct, 1);

        FragColor = texColor*directionalLight;

        //FragColor = vec4((normals.z+1.)/2., (normals.z+1.)/2., (normals.z+1.)/2., 1.0f);
       //FragColor = vec4(uvs, 0.0f, 1.0f);
        //FragColor = vec4(0.0f, 1.0f, 0.0f, 1.0f);
   }
}