//OPTION A WAVE SIMULATION

uniform int initialized;
uniform sampler2D texUnit;
uniform float time;
//uniform float amp[4];
//uniform float speed[4];
uniform float amp;
uniform float speed;
uniform float amp1;
uniform float speed1;
uniform float amp2;
uniform float speed2;
uniform float amp3;
uniform float speed3;
uniform float dirx;
uniform float diry;
uniform float dirx1;
uniform float diry1;
uniform float dirx2;
uniform float diry2;
uniform float dirx3;
uniform float diry3;
uniform float length;
uniform float length1;
uniform float length2;
uniform float length3;
uniform float lightx;
uniform float lighty;
uniform int island;

const float sqrt2 = 1.41421356237;
const float PI = 3.1415926535897932384626;
vec2 lightorigin = vec2(0, 0);
vec2 island_cent = vec2(2, 2);
float island_radius = 1.1;


/**
 * Initialize the image.
 * 
 * Use red component as U and blue component as V of Gray-Scott model.
 */
void init()
{
	vec2 texCoord = gl_TexCoord[0].xy;

	if (texCoord.x > 0.48 && texCoord.x < 0.52 && texCoord.y > 0.48 && texCoord.y < 0.52) {
		gl_FragColor = vec4(0.5, 0.0, 0.25, 1.0);
	} else {
		gl_FragColor = vec4(1.0, 0.0, 0.0, 1.0);
	}
}

/**
 * Update the color of each pixel.
 *
 * Use Gray-Scott reaction-diffusion model to update U and V,
 * and U and V are stored in red and blue components of the pixel color, respectively.
 */
void update()
{
	const float offset = 1.0 / 512.0;
	
	vec2 texCoord = 5 * gl_TexCoord[0].xy;

	lightorigin = vec2(lightx, lighty);
	vec2 lightDir = lightorigin - texCoord;
	lightDir = normalize(lightDir);

	float dist_to_cent = distance(texCoord, island_cent);

	vec2 islandDir = island_cent - texCoord;
	islandDir = normalize(islandDir);


	// parameters for Gray-Scott model
	float F = 0.037;
	float K = 0.055;  //0.06;
	float Du = 0.209;
	float Dv = 0.102;

	float wavelength[4];
	vec2 dir[4];

	wavelength[0] = length;
	wavelength[1] = length1;
	wavelength[2] = length2;
	wavelength[3] = length3;

	dir[0] = vec2(dirx, diry);
	dir[1] = vec2(dirx1, diry1);
	dir[2] = vec2(dirx2, diry2);
	dir[3] = vec2(dirx3, diry3);

	vec2 dirtot = vec2(0.0, 0.0);

	for (int i = 0; i < 4; i++) {
		dirtot += dir[0];
	}

	dirtot = normalize(dirtot);

	if (island == 1) {
		dir[0] =  islandDir/(abs(dist_to_cent)) + dir[0];
		dir[0] = normalize(dir[0]);
		dir[1] =  islandDir/(abs(dist_to_cent)) + dir[1];
		dir[1] = normalize(dir[1]);
		dir[2] =  islandDir/(abs(dist_to_cent)) + dir[2];
		dir[2] = normalize(dir[2]);
		dir[3] =  islandDir/(abs(dist_to_cent)) + dir[3];
		dir[3] = normalize(dir[3]);
	}

	float intensity = 0;

	float freq = 2 * PI / wavelength[0];
	float wx = dot(dir[0], texCoord);
	float rateOfChange = speed * freq;
	intensity += amp * sin(wx * freq + time * rateOfChange);

	freq = 2 * PI / wavelength[1];
	wx = dot(dir[1], texCoord);
	rateOfChange = speed1 * freq;
	intensity += amp1 * sin(wx * freq + time * rateOfChange);

	freq = 2 * PI / wavelength[2];
	wx = dot(dir[2], texCoord);
	rateOfChange = speed2 * freq;
	intensity += amp2 * sin(wx * freq + time * rateOfChange);

	freq = 2 * PI / wavelength[3];
	wx = dot(dir[3], texCoord);
	rateOfChange = speed3 * freq;
	intensity += amp3 * sin(wx * freq + time * rateOfChange);


	//Calculate wave normals
	vec2 normal[4];
	freq = 2 * PI / wavelength[0];
	wx = dot(dir[0], texCoord);
	rateOfChange = speed * freq;
	//Derivative with respect to x is cos(D * spatialfrequency(x,y) + t * timefrequency) * A * D.x * spatialfrequency
	float newamp = amp * dir[0].x * freq;
	float dx = newamp * cos(wx * freq + time * rateOfChange);
	
	//Derivative with repsect to y is cos(D * spatialfrequency(x,y) + t * timefrequnecy) * A * D.y * spatialfrequency)
	newamp = amp * dir[0].y * freq;
	float dy = newamp * cos(wx * freq + time * rateOfChange);
	normal[0] = vec2(dx, dy);
	//normal[0] = normalize(normal[0]);

	freq = 2 * PI / wavelength[1];
	wx = dot(dir[1], texCoord);
	rateOfChange = speed1 * freq;
	newamp = amp1 * dir[1].x * freq;
	dx = newamp * cos(wx * freq + time * rateOfChange);
	newamp = amp1 * dir[1].y * freq;
	dy = newamp * cos(wx * freq + time * rateOfChange);
	normal[1] = vec2(dx, dy);
	//normal[1] = normalize(normal[1]);

	freq = 2 * PI / wavelength[2];
	wx = dot(dir[2], texCoord);
	rateOfChange = speed2 * freq;
	newamp = amp2 * dir[2].x * freq;
	dx = newamp * cos(wx * freq + time * rateOfChange);
	newamp = amp2 * dir[2].y * freq;
	dy = newamp * cos(wx * freq + time * rateOfChange);
	normal[2] = vec2(dx, dy);
	//normal[2] = normalize(normal[2]);

	freq = 2 * PI / wavelength[3];
	wx = dot(dir[3], texCoord);
	rateOfChange = speed3 * freq;
	newamp = amp3 * dir[3].x * freq;
	dx = -1 * newamp * cos(wx * freq + time * rateOfChange);
	newamp = amp3 * dir[3].y * freq;
	dy = newamp * cos(wx * freq + time * rateOfChange);
	normal[3] = vec2(dx, dy);
	//normal[3] = normalize(normal[3]);

	dx = 0;
	dy = 0;
	for (int i = 0; i < 4; i++) {
			dx += normal[i].x;
			dy += normal[i].y;
	}
	vec3 normtot = vec3(-dx, -dy, 1);
	normtot = normalize(normtot);


	vec2 c = texture2D(texUnit, texCoord).rb;
	float U = c.x;
	float V = c.y;

	// get the colors of the pixels
	
	float light_mult = dot(lightDir, normtot);
	float spec_mult = max(0, dot((vec2(1.0, 1.0) * light_mult), dirtot));
	//light_mult = 1.0;

	if (island == 1 && abs(dist_to_cent) < island_radius) {
		gl_FragColor = vec4(0.25, 1.0, 0.25, 1.0);	
	}

	else {
		vec3 finalcolor = (0.0, 0.0, 0.0);
	
		finalcolor += vec3(0.0, 0.2, 0.2) * max(0, light_mult);
		finalcolor += vec3(0.0, 0.2, 0.2) * pow(spec_mult, 4);
		finalcolor += vec3(0.0, max(0, 0.3 * intensity), max(0, 0.4 + 0.2 * intensity));
		gl_FragColor = vec4(finalcolor, 1.0);
	}

	//gl_FragColor = vec4(0.0, 0.7 * intensity * light_mult, 0.9 + 0.1 * light_mult, 1.0);
	vec2 l = texture2D(texUnit, texCoord + vec2(-offset, 0.0)).rg;
	vec2 t = texture2D(texUnit, texCoord + vec2(0.0, offset)).rg;
	vec2 r = texture2D(texUnit, texCoord + vec2(offset, 0.0)).rg;
	vec2 b = texture2D(texUnit, texCoord + vec2(0.0, -offset)).rg;
	//float U = c.x;
	//float V = c.y;
	vec2 lap = l + t + r + b  - c * 4.0;

	float dU = Du * lap.x - U * V * V + F * (1.0 - U);
	float dV = Dv * lap.y + U * V * V - (F + K) * V;

	// use the heat equation to updte the color of this pixel
	//gl_FragColor = vec4(intensity * light_mult * (U + dU), intensity * light_mult * (V + dV), 1.0, 1.0);
}

void main()
{
	if (initialized == 0) {
		init();
	} else {
		update();
	}
}