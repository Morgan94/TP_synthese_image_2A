#version 150

in vec4 color; 
in vec2 uv;
out vec4 fragColor ; 

uniform sampler2D colormap;
uniform vec3 diffuse;


uniform float time;

void main()
{

    /*!todo exercise 1: implement texture sampling*/
    //fragColor=vec4(1,1,1,1);

	float color = 0.5f;
	color += sin( cos( time / 15.0 ) * 80.0 ) + cos(  cos( time / 15.0 ) * 10.0 );
	color += sin( sin( time / 10.0 ) * 40.0 ) + cos( sin( time / 25.0 ) * 40.0 );
	color += sin( sin( time / 5.0 ) * 10.0 ) + sin( sin( time / 35.0 ) * 80.0 );
	color *= sin( time / 10.0 ) * 0.5;

	vec4 tmp = vec4( vec3( color, color * 0.5, sin( color + time / 3.0 ) * 0.75 ), 1.0 );

        fragColor = tmp * texture(colormap, uv);

}

