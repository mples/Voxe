#pragma once
#include "../ECS/Component.h"
#include "../Graphics/VertexBuffer.h"
#include "../Graphics/IndexBuffer.h"
#include "../Graphics/Vertex.h"

class OcclusionQuadsComponent : public Component<OcclusionQuadsComponent> {
	struct Quad {
		VertexBuffer<Vertex> vertexBuffer_;
		IndexBuffer indexBuffer_;
		UINT offset_;
	};

public:
	OcclusionQuadsComponent();
	~OcclusionQuadsComponent();

	Quad positiveX_;
	Quad negativeX_;

	Quad positiveY_;
	Quad negativeY_;

	Quad positiveZ_;
	Quad negativeZ_;
private:
};