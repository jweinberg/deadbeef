uniform sampler2D texture;
varying highp vec2 texcoord;

const highp float blurSize = 1.0/512.0; // I've chosen this size because this will result in that every step will be one pixel wide if the RTScene texture is of size 512x512
 
void main(void)
{
   /*highp vec4 sum = vec4(0.0);
 
   // blur in y (vertical)
   // take nine samples, with the distance blurSize between them
   sum += texture2D(texture, vec2(texcoord.x - 4.0*blurSize, texcoord.y)) * 0.05;
   sum += texture2D(texture, vec2(texcoord.x - 3.0*blurSize, texcoord.y)) * 0.09;
   sum += texture2D(texture, vec2(texcoord.x - 2.0*blurSize, texcoord.y)) * 0.12;
   sum += texture2D(texture, vec2(texcoord.x - blurSize, texcoord.y)) * 0.15;
   sum += texture2D(texture, vec2(texcoord.x, texcoord.y)) * 0.16;
   sum += texture2D(texture, vec2(texcoord.x + blurSize, texcoord.y)) * 0.15;
   sum += texture2D(texture, vec2(texcoord.x + 2.0*blurSize, texcoord.y)) * 0.12;
   sum += texture2D(texture, vec2(texcoord.x + 3.0*blurSize, texcoord.y)) * 0.09;
   sum += texture2D(texture, vec2(texcoord.x + 4.0*blurSize, texcoord.y)) * 0.05;
 
   gl_FragColor = sum;*/
   gl_FragColor = texture2D(texture, texcoord);
}
