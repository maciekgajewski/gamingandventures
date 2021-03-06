#pragma once

namespace OT
{

class PvcMesh;
class Mesh;

// Cube mesh contains a cube with side of 1.0, centered around the origin (-0.5	- +0.5)
PvcMesh buildCubePvcMesh();
Mesh buildCubeMesh();

// Spehere mesh. Radius: 1.0. Segments per quarter-circle
Mesh buildSphereMesh(int segments = 10);

}
