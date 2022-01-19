uniform sampler2D texUnit;
uniform float time;
uniform float thickness;
uniform float speed;
uniform int mousecom;
uniform float mousex;
uniform float mousey;
uniform int numarcs;
uniform int colorOp;

const float pi = 3.1415926535897932384626;

//Modified variant of a known gpu randomizer method
float rand(vec2 n) {
    return fract(sin(dot(n, vec2(11, 3))) * 50000); //fract takes fractional part of a value. I.E. x - floor(x)
}

//Modified variant of a known gpu randomizer method
//Grabs a square of surrounding pixels and randomly offsets them
//By proportional amounts, results int smooth jaggedness
float noise( vec2 p )
{
    vec2 x = floor(p);
    vec2 f = fract(p);
    f = f*f*(3.0-2.0*f);
    
    float randomizer = mix(
                   mix(rand(x), rand(x+vec2(1.0,0.0)),f.x),
                   mix(rand(x + vec2(0.0, 1.0)), rand(x + vec2(1.0, 1.0)), f.x), f.y);
    return randomizer * randomizer;
}

void main( void ) 
{
    vec2 texcoord = 5 * gl_TexCoord[0].xy;
    vec3 mainLineColor = vec3(1.0, 1.0, 1.0);
    vec3 finalColor = vec3(0.0, 0.0, 0.0);

    for (int i = 1; i <= numarcs; i++) {
       float basewidth = 20.0/(float(i) * float(i)); //Thickness decreases by a squared factor as number of arcs increases
       float warblex = noise(texcoord + float(i) * time * speed * pi);
       float warbley = noise(texcoord + float(i) * time * speed * pi);
       float warblez = noise(texcoord + float(i) * time * speed * pi);
       vec3 warble = (warblex, warbley, warblez);
       finalColor += vec3(abs(1 / (sin(texcoord.y + warble) * 140) * basewidth));
    }
    
    finalColor = vec3( smoothstep(0.1, 1.0,(finalColor.x+finalColor.y+finalColor.z) / 4.0) )*finalColor;

    if (colorOp == 0) {
       finalColor *= vec3(0.7, 0.2, 0.2);
    }
    else if (colorOp == 1) {
       finalColor *= vec3(0.6, 0.6, 0.2);
    }
    else if (colorOp == 2) {
        finalColor *= vec3(0.2, 0.7, 0.2);
    }
    else if (colorOp == 3) {
        finalColor *= vec3(0.2, 0.2, 0.7);
    }
    else if (colorOp == 4) {
        finalColor *= vec3(0.6, 0.2, 0.6);
    }
    else {
        finalColor *= vec3(sin(time) + 0.2, cos(time) + 0.2, -sin(time));
        //pulsing is already acheived by above effect, pointless to reach that line
        if (texcoord.y < 0.5) {
            gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
        }
        else {
            gl_FragColor = vec4(finalColor, 1.0);
        }
        return;
	}

    if (texcoord.y < 0.5) { //This is here to handle over lapping of waves so only middle waves appear
        gl_FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
    else {
        gl_FragColor = vec4(finalColor, 1.0);
    }

    //Makes the lightning pulse, thereby looking more realistic
    gl_FragColor *= abs(sin(time));

}