uniform sampler2D texture;
uniform vec2 texSize;
uniform vec2 camPos;
uniform vec2 mousePos;
uniform float zoom;
uniform float time;
uniform int N;
#define CSIZE 10
uniform sampler2D colorsTex;
uniform bool useGrad;
uniform int functionID;

vec2 cx_sqr(vec2 a) {
  float x2 = a.x*a.x;
  float y2 = a.y*a.y;
  float xy = a.x*a.y;
  return vec2(x2 - y2, xy + xy);
}

vec2 mandelbrot(vec2 z, vec2 c) {
  return cx_sqr(z) + c;
}
vec2 burning_ship(vec2 z, vec2 c) {
  return vec2(z.x*z.x - z.y*z.y, 2.0*abs(z.x * z.y)) + c;
}

vec2 demo(vec2 z, vec2 c)
{
	return cx_sqr(cx_sqr(z) + cx_sqr(c)) + z;
}

float lenght(vec2 vec)
{
	return sqrt(vec.x*vec.x + vec.y*vec.y);
}

void main()
{
    vec4 pixel = vec4(1.f);
    vec2 uv = gl_TexCoord[0].xy;
    vec2 uvMouse = mousePos/texSize;
    uv.y = 1.f - uv.y;
    uvMouse.y = 1.f - uvMouse.y;
    uv.x = uv.x * 3.f - 2.f;
    uvMouse.x = uvMouse.x * 3.f - 2.f;
	  uv.y = uv.y * 2.f - 1.f;
    uvMouse.y = uvMouse.y * 2.f - 1.f;

    vec2 c = uv/zoom + camPos;
    vec2 z = vec2(0.f);
    if (functionID == 2)
    {
      c = uvMouse;
      z = uv/zoom + camPos;
    }
    float color = 0.f;

    for (int i = 0; i < N; ++i)
    {
    	color += 1.f;
      switch(functionID)
      {
        case 2:
        {
          z = mandelbrot(z, c);
          break;
        }
        case 1:
        {
          z = burning_ship(z, c);
          break;
        }
        case 0:
        {
          z = mandelbrot(z, c);
          break;
        }
      }
    	if (lenght(z) > 4.f)
    	{
    		break;
    	}
    }

    float grad = (color/float(N));
    if (grad > 0.75f)
    {
        pixel = texture2D( colorsTex, vec2(1.f, 0.f) );
    }
    else
    {
      grad *= 1.3333f;
      int colorIdLow = int(floor( grad*float(CSIZE-1) ));
      int colorIdHight = int(ceil( grad*float(CSIZE-1) ));
      vec4 targetColorLow = texture2D( colorsTex, vec2(float(colorIdLow)/float(CSIZE), 0.f) );
      vec4 targetColorHight = texture2D( colorsTex, vec2(float(colorIdHight)/float(CSIZE), 0.f) );
      float colorGap = colorIdHight - grad*float(CSIZE-1);
      pixel = targetColorLow * colorGap + targetColorHight * (1.f - colorGap);
    }

    if (useGrad)
    {
      pixel *= grad;
    }

    gl_FragColor = gl_Color * pixel;
}