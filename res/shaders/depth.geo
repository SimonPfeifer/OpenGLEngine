#version 330 core
// max_vertices needs to change for different number of shadow cascades. It is 
// equal to numVertices * numCascades.
layout(triangle_strip, max_vertices=12) out;

layout(triangles) in;

// The view * projection matrix for each shadow cascade slice.
uniform mat4 lightViewProjection[4];


void main()
{
  // Transform the vertices into each of the shadow cascade slice's perspective
  // and send them off to a fragment shader to be depth tested. This allows the
  // depth testing to happen in parallel on the GPU via the invocations.
  for (int slice=0; slice<4; ++slice)
  {
    gl_Layer = slice;
    for(int i=0; i<3; ++i)
    {
      gl_Position = lightViewProjection[slice] * gl_in[i].gl_Position;

      // Because a texture array is bound to the frame buffer, the layer
      // specifies the depth, i.e. which texture in the array, to render to.
      EmitVertex();
    }    
    EndPrimitive();
  }
}  