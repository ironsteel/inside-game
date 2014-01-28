precision mediump float;

varying vec2 v_texCoord;
varying vec3 v_normal;
varying vec3 v_lightPosition;

uniform sampler2D mytexture;

uniform int selected;

void main()
{
	vec4 textureColor = texture2D(mytexture, v_texCoord.st);

	float coeff = max(0.0, dot(v_normal, v_lightPosition));
	
	vec3 diffuseReflection = vec3(1.0, 1.0, 1.0) * vec3(textureColor.rgb)
	* max(0.0, dot(v_normal, v_lightPosition));
	
	if(selected == 1) {
		diffuseReflection *= vec3(0, 1, 0);
	}
	
	gl_FragColor = vec4(diffuseReflection, 1.0) * 2.0;
}