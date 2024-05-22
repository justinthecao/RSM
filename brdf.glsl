
  //first. we need to some how get one sphere on the screen,
  //we will have two spheres left and right, one representing BRDF
  //other will have light preservation on the right
  //

  const vec3 lightPosition = vec3(10.0, 40.0, 0.0);
  const vec3 lightColor = vec3(1.0);
  const vec3 ambientColor = vec3(0.0);

  struct Sphere
  {
      vec3 cen;
      float rad;
  } spheres[2];

  const float sphereRad = 2.5;
  const float gap = 0.5;
  float roughness = 0.5;
  vec3 baseColor = vec3(1.0, 0.84, 0.0);
  const float PI = 3.141592653589793238462643;
    
  const float sliderVertPos = -0.8;
  const float sliderHorzCover = 1.0;
  float sliderCursorPos = 0.0;
  float sliderCursorSize = 0.02;
    
  const float colorSliderVertPos = -0.6;
  const float colorSliderHorzCover = 1.0;
  const float colorSliderVertSize = 0.2;
  float colorSliderCursorPos = 0.0;
  float colorSliderCursorSize = 0.02;
  
  
  void makeSpheres();
  float calculateSphereIntersection(in vec3 ro, in vec3 rd, in Sphere sph);
  float calculatePlaneIntersection(in vec3 ro, in vec3 rd);
  vec3 getNormal(in vec3 pos, in Sphere sph);

  float randomFloat(vec3 co);
  vec3 perpendicularVector(in vec3 nrm);
  vec3 randomDirection(in vec3 base, in vec3 u, in vec3 v, in float r2, in vec3 seed);
  vec3 sampleDiffuseEnvironment(in vec3 dir);
  vec3 sampleReflectedEnvironment(in vec3 dir, in float r, in float f0);

  vec3 getColor(float h) {
    h = h * 360.0;
    float x = (1.0 - abs(mod(h / 60.0, 2.0) - 1.0));
    vec3 color;
    
    if (h < 60.0) {
        color = vec3(1.0, x, 0.0);
    } else if (h < 120.0) {
        color = vec3(x, 1.0, 0.0);
    } else if (h < 180.0) {
        color = vec3(0.0, 1.0, x);
    } else if (h < 240.0) {
        color = vec3(0.0, x, 1.0);
    } else if (h < 300.0) {
        color = vec3(x, 0.0, 1.0);
    } else {
        color = vec3(1.0, 0.0, x);
    }
    return color;
  }

  vec3 FresnelSchlick(float VdotH, vec3 F0)
  {
      return F0 + (1.0 - F0) * pow(1.0 - VdotH, 5.0);
  }

 float GGX(vec3 lightDir, vec3 nrm, vec3 halfVec){
    //GGX is alpha^2/ pi((n.h)(n.h) * (alpha^2-1) + 1)^2
    roughness = clamp(roughness, 0.1, 0.9);
    float numerator = pow(roughness,2.0);

    float denominator = PI * pow(pow(dot(nrm, halfVec), 2.0) * (pow(roughness,2.0)-1.0) + 1.0, 2.0);

    return numerator/ denominator;
  }
  
  float SmithGGX(float NdotV)
  {
      float a2 = roughness * roughness;
      float numerator = 2.0 * NdotV;
      float denominator = NdotV + sqrt(a2 + (1.0 - a2) * NdotV * NdotV);
      return 1.0 / (numerator / denominator + 1e-5);
  }

  //we don't need shadows
  vec3 shadeFragment(in vec3 pos, in vec3 nrm, in vec3 view, in bool lightComp){
    //for shading fragments
    //using brdf model, we need to get the vortex size from roughness
    //schlicks approximation + geometry occlusion + normal distribution
    vec3 lightDir = normalize(lightPosition - pos);
    vec3 halfVec = normalize(lightDir + view);

    float NdotL = normalize(max(dot(nrm, lightDir), 0.0));
    float NdotV = normalize(max(dot(nrm, view), 0.0));
    float VdotH = normalize(max(dot(view, halfVec), 0.0));

    float D = GGX(lightDir, nrm, halfVec);
    float G = SmithGGX(NdotL) * SmithGGX(NdotV);
    vec3 F = FresnelSchlick(VdotH, vec3(1.000, 0.766, 0.336));
    
   vec3 specular = D * G * F / (4.0 * NdotL * NdotV + 0.001);
    
    vec3 diffuse = baseColor * NdotL;

    return specular + diffuse;
  }

  void mainImage( out vec4 fragColor, in vec2 fragCoord )
  {
    
    makeSpheres();
    vec2 p = (2.0 * fragCoord.xy - iResolution.xy) / iResolution.y;
    vec3 viewPoint = vec3(0.0, 15, 55);
    //normalize coordinates around the origin
    vec3 viewCenter = vec3(0.0, 0.0, 0.0);

    //Make the Sliders
    float xResNorm = iResolution.x/iResolution.y;
    float mouseRatio = iMouse.x/iResolution.x;
    float sliderX = sliderHorzCover * xResNorm;
    sliderCursorPos = texture(iChannel1, vec2(0,0)).r * sliderX * 2.0 - sliderX;
    if(abs(p.x) < sliderX && p.y < sliderVertPos){
      
      //get the cursor for the slider
      //v is ratio of how much cursor takes up of slider
      //pos of cursor is sliderWidth v * sliderWidth * 2
      //v/2
      sliderCursorPos = texture(iChannel1, vec2(0,0)).r * sliderX * 2.0 - sliderX;
      sliderCursorPos = clamp(sliderCursorPos, -sliderX + sliderCursorSize, sliderX - sliderCursorSize);
      if(p.x > sliderCursorPos - sliderCursorSize && p.x < sliderCursorPos + sliderCursorSize) fragColor = vec4(0.0);
      else fragColor = vec4((sliderCursorPos + sliderX)/(sliderX * 2.0));
      return;
    }
    roughness = (sliderCursorPos + sliderX)/(sliderX * 2.0);
    float colorSliderX = sliderHorzCover * xResNorm;

    float colorSliderCursorPos = texture(iChannel2, vec2(0,0)).r * colorSliderX * 2.0 - colorSliderX;
    if(abs(p.x) < colorSliderX && p.y < colorSliderVertPos && p.y > colorSliderVertPos-colorSliderVertSize){
      
      //get the cursor for the slider
      //v is ratio of how much cursor takes up of slider
      //pos of cursor is sliderWidth v * sliderWidth * 2
      //v/2
      float colorSliderCursorPos = texture(iChannel2, vec2(0,0)).r * colorSliderX * 2.0 - colorSliderX;
      colorSliderCursorPos = clamp(colorSliderCursorPos, -colorSliderX + sliderCursorSize, colorSliderX - sliderCursorSize);
      if(p.x > colorSliderCursorPos - sliderCursorSize && p.x < colorSliderCursorPos + sliderCursorSize) fragColor = vec4(0.0);
      else fragColor = vec4(getColor(fragCoord.x/iResolution.x),1.0);
      return;
    }
    
    baseColor = getColor((colorSliderCursorPos + colorSliderX)/(colorSliderX*2.0));
    //END Sliders
    

    //shaderToy is ray marched based
    vec3 up = vec3(0.0, 1.0, 0.0);
    //going to be my z axis
    vec3 ww = normalize(viewCenter - viewPoint);
    //going to be my x axis
    vec3 uu = normalize(cross(ww, up));
    //going to be my y axis
    vec3 vv = normalize(cross(uu, ww));
    //ray direction
    vec3 rd = normalize(p.x * uu + p.y * vv + 7.0 * ww);    

    //find which object the ray hits
    float tmin = 1.0e+10;
    float t1 =  calculatePlaneIntersection(viewPoint, rd);
    if(t1 > 0.0) tmin = t1;

    //find t parameter for the spheres as well
    int sphereIndex = -1;
    vec3 nrm = vec3(0.0, 1.0, 0.0);

    for(int i = 0 ; i < 2 ;i ++){
      float t2 = calculateSphereIntersection(viewPoint, rd, spheres[i]);
      if((t2 > 0.0 && (t2 < tmin)))
      {
        tmin = t2;
        nrm = getNormal(viewPoint + t2*rd, spheres[i]);
        sphereIndex= i;
      }
    }

    //if ray is in the sky
    if(tmin > 10000.0){
      fragColor = vec4(0.0);
      return;
    }

    vec3 pos = viewPoint + tmin * rd;
    vec3 linearColor = vec3(0.0);

    if (sphereIndex == -1)
    {
      
        float inten = 1.0/(length(pos-viewPoint) * length(pos-viewPoint));
      
        linearColor= vec3(1.0) * exp(-0.03 * length(viewPoint - pos));
    }
    else if(sphereIndex == 0){
      linearColor = shadeFragment(pos, nrm, normalize(viewPoint - pos), false);
    }
    else{
      linearColor = shadeFragment(pos, nrm, normalize(viewPoint - pos), true);
    }

    fragColor = vec4(pow(linearColor, vec3(1.0/2.2)), 1.0);
  }

  void makeSpheres(){
    //two spheres

    spheres[0] = Sphere(vec3(-sphereRad - gap, sphereRad, 0), sphereRad);
    spheres[1] = Sphere(vec3(sphereRad + gap, sphereRad, 0), sphereRad);
  }


  float calculatePlaneIntersection(in vec3 ro, in vec3 rd)
  {
      return (-1.0 - ro.y) / rd.y;
  }   

  float calculateSphereIntersection(in vec3 ro, in vec3 rd, in Sphere sph)
  {
      vec3 dv = sph.cen - ro;
    float b = dot(rd, dv);
    float d = b * b - dot(dv, dv) + sph.rad * sph.rad;
    return (d < 0.0) ? -1.0 : b - sqrt(d);
  }
  vec3 getNormal(in vec3 pos, in Sphere sph)
  {
      return normalize((pos - sph.cen) / sph.rad);
  }
  
  