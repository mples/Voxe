#include "Vertex.h"

Vertex::Vertex(float x, float y, float z, float u, float v, float nx, float ny, float nz) : pos_(x, y, z), texCoord_(u, v), normal_(nx, ny, nz) {
}
