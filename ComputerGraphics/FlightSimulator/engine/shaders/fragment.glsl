#version 330

uniform sampler2D textureMap0;
uniform sampler2D textureMap1;
uniform bool isSpecular;

in vec2 iTexCoord;
in vec4 iNormal;
in vec4 ic;
in vec4 l;
in vec4 v;
out vec4 pixelColor;

void main(void) {
	vec4 mn = normalize(iNormal);
	vec4 ml = normalize(l);
	vec4 mv = normalize(v);

	vec4 mr = reflect(-ml, mn);

	float nl = clamp(dot(mn, ml), 0, 1);
	float rv = pow(clamp(dot(mr, mv), 0, 1), 50);

	vec4 ambient = vec4(0.1,0.1,0.1,0);
	vec4 ks = vec4(1, 0.8, 0.3, 1);
	if(isSpecular) ks = texture(textureMap1, iTexCoord);
	vec4 texCol = texture(textureMap0,iTexCoord);

	pixelColor= vec4(texCol.rgb * nl, texCol.a) + vec4(ks.rgb*rv, 0) + ambient;
	//pixelColor=vec4(color, texCol.a);
}
