#include "pvc_mesh.hh"

#include <glad/glad.h>

namespace Rendering {

void PvcMesh::draw()
{
	glBindVertexArray(vao_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao_); // why is this needed?
	glDrawElements(GL_TRIANGLES, faces_.size() * 3, GL_UNSIGNED_INT, 0);
}

void PvcMesh::setup()
{
	glGenVertexArrays(1, &vao_);
	glBindVertexArray(vao_);

	glGenBuffers(1, &eao_);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, eao_);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Face) * faces_.size(), faces_.data(), GL_STATIC_DRAW);

	glGenBuffers(1, &vbo_);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices_.size(), vertices_.data(), GL_STATIC_DRAW);

	// first param: xyz
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0); // uses currently bound VAO
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

} // namespace OT
