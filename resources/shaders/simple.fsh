precision mediump float;

varying vec2 v_texCoord;
uniform sampler2D mytexture;
uniform int selected;
void main()
{
	if(selected == 0) {
        gl_FragColor = texture2D(mytexture, v_texCoord.st);
	} else {
		gl_FragColor = texture2D(mytexture, v_texCoord.st) * vec4(0.0, 0.8, 0.0, 1.0);
	}
}
