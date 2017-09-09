#pragma once

namespace Rendering
{

class PvcMesh;
class Mesh;

// Cube mesh contains a cube with side of 1.0, centered around the origin (-0.5	- +0.5)
PvcMesh buildCubePvcMesh();
Mesh buildCubeMesh();


Mesh buildSkyboxMesh();

// Spehere mesh. Radius: 1.0. Segments per quarter-circle
Mesh buildSphereMesh(int segments = 10);

}
