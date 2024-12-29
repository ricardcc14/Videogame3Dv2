#version 330 core
out vec4 FragColor;

in vec3 color;

in vec3 normals;
in vec2 uvs;
in mat3 TBN;

uniform sampler2D tex0;
uniform sampler2D texNormals;

uniform vec3 tint;
uniform float bright;
uniform vec3 lightDirection = vec3(5.0, -5.0, 0.0);


void main()
{
    vec4 texColor = texture(tex0, uvs);
    FragColor = vec4(texColor.r, texColor.g, texColor.b, texColor.a);
    //FragColor = FragColor * vec4(1.0, 0.0, 0.0, 1.0f);

    vec3 normalMap = (texture(texNormals, uvs).rgb * 2.0f) -1.0f;
    normalMap = normalize(TBN * normalMap); 
    

    float dotProduct = 1- (dot(normalize(lightDirection), normalize(normalMap)) + 1)/2.0f;

    dotProduct = clamp(dotProduct, 0, 1);

    vec4 directionalLight = vec4(dotProduct, dotProduct, dotProduct, 1);

    FragColor = texColor*vec4(tint.x*bright, tint.y*bright, tint.z*bright, 1.0)*directionalLight;
    //FragColor = texColor*vec4(0.01, 0.01, 0.01, 1.0)*directionalLight;
    
    
    
    //FragColor = vec4((normals.z+1.)/2., (normals.z+1.)/2., (normals.z+1.)/2., 1.0f);
   //FragColor = vec4(uvs, 0.0f, 1.0f);
   //FragColor = vec4(color, 1.0f);
   //}
}