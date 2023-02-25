// Created by lonelyinnovator on 2023/2/21 23:56.

#include "mesh.h"

Mesh::Mesh(std::vector<Vertex> &vertices, std::vector<unsigned int> &indices, std::vector<Texture> &textures,
           std::vector<int> triangle_seg) {
  this->vertices_ = vertices;
  this->indices_ = indices;
  this->textures_ = textures;

  FillTriangles(triangle_seg);

  ComputeVertexNormals();

  if (triangle_seg.size() * 3 == indices.size()) {
    ComputeVertexSeg();
  }

  // now that we have all the required data, set the vertex buffers and its attribute pointers.
  SetupMesh();
}

Mesh::~Mesh() {

}

void Mesh::Draw(Shader &shader) {
  // bind appropriate textures
  unsigned int diffuseNr = 1;
  unsigned int specularNr = 1;
  unsigned int normalNr = 1;
  unsigned int heightNr = 1;
  for (unsigned int i = 0; i < textures_.size(); i++) {
    glActiveTexture(GL_TEXTURE0 + i); // active proper texture unit before binding
    // retrieve texture number (the N in diffuse_textureN)
    std::string number;
    std::string name = textures_[i].type;
    if (name == "texture_diffuse")
      number = std::to_string(diffuseNr++);
    else if (name == "texture_specular")
      number = std::to_string(specularNr++); // transfer unsigned int to string
    else if (name == "texture_normal")
      number = std::to_string(normalNr++); // transfer unsigned int to string
    else if (name == "texture_height")
      number = std::to_string(heightNr++); // transfer unsigned int to string

    // now set the sampler to the correct texture unit
    glUniform1i(glGetUniformLocation(shader.ID, (name + number).c_str()), i);
    // and finally bind the texture
    glBindTexture(GL_TEXTURE_2D, textures_[i].id);
  }

  // draw mesh
  glBindVertexArray(vao_);
  glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices_.size()), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);

  // always good practice to set everything back to defaults once configured.
  glActiveTexture(GL_TEXTURE0);
}

const std::vector<Vertex> &Mesh::GetVertices() const {
  return vertices_;
}

const std::vector<unsigned int> &Mesh::GetIndices() const {
  return indices_;
}

const std::vector<Triangle> &Mesh::GetTriangles() const {
  return triangles_;
}

const std::vector<Texture> &Mesh::GetTextures() const {
  return textures_;
}

unsigned int Mesh::GetVao() const {
  return vao_;
}

unsigned int Mesh::GetVbo() const {
  return vbo_;
}

unsigned int Mesh::GetEbo() const {
  return ebo_;
}

const std::vector<std::vector<unsigned int>> &Mesh::GetTriPerVer() const {
  return tri_per_ver_;
}

void Mesh::FillTriangles(std::vector<int> &triangle_seg) {
  bool valid = triangle_seg.size() * 3 == indices_.size();
  // resize first!
  tri_per_ver_.resize(vertices_.size());
  for (int i = 0; i < indices_.size() / 3; ++i) {
    Triangle t{};
    t.vertex_idx1 = indices_[3 * i];
    t.vertex_idx2 = indices_[3 * i + 1];
    t.vertex_idx3 = indices_[3 * i + 2];

    // compute tri_per_ver
    tri_per_ver_[t.vertex_idx1].push_back(i);
    tri_per_ver_[t.vertex_idx2].push_back(i);
    tri_per_ver_[t.vertex_idx3].push_back(i);

    // set triangle seg
    if (valid) {
      t.seg = triangle_seg[i];
    }

    // compute triangle normal, only use the outside normal, not the inside
    auto v1 = vertices_[t.vertex_idx1].position - vertices_[t.vertex_idx2].position;
    auto v2 = vertices_[t.vertex_idx1].position - vertices_[t.vertex_idx3].position;
    auto normal = glm::normalize(glm::cross(glm::normalize(v1), glm::normalize(v2)));
    t.normal = normal;

    triangles_.push_back(t);
  }
}

void Mesh::ComputeVertexNormals() {
  for (int i = 0; i < tri_per_ver_.size(); ++i) {
    vertices_[i].normal = glm::vec3(0.0f);
    // triangle indices of every vertex
    auto tri_indices = tri_per_ver_[i];
    // average all the normals of triangles on which the vertex lays
    for (auto &tri_idx: tri_indices) {
      vertices_[i].normal += triangles_[tri_idx].normal;
    }
    vertices_[i].normal /= tri_indices.size();
//        std::cout << vertices[i].Normal.x << " " << vertices[i].Normal.y << " " << vertices[i].Normal.z << std::endl;
  }
}

void Mesh::ComputeVertexSeg() {
  std::vector<int> tri_seg;
  for (int i = 0; i < tri_per_ver_.size(); ++i) {
    // triangle indices of every vertex
    auto tri_indices = tri_per_ver_[i];
    tri_seg.clear();
    for (auto &tri_idx: tri_indices) {
      tri_seg.push_back(triangles_[tri_idx].seg);
    }
    // set the most frequently appeared seg number in all triangles to vertex seg
    vertices_[i].seg = GeneralAlgorithm::MostFreqNum(tri_seg);
//        std::cout << vertices[i].Seg << std::endl;
  }
}

void Mesh::SetupMesh() {
  // create buffers/arrays
  glGenVertexArrays(1, &vao_);
  glGenBuffers(1, &vbo_);
  glGenBuffers(1, &ebo_);

  glBindVertexArray(vao_);
  // load data into vertex buffers
  glBindBuffer(GL_ARRAY_BUFFER, vbo_);
  // A great thing about structs is that their memory layout is sequential for all its items.
  // The effect is that we can simply pass a pointer to the struct and it translates perfectly to a glm::vec3/2 array which
  // again translates to 3/2 floats which translates to a byte array.
  glBufferData(GL_ARRAY_BUFFER, vertices_.size() * sizeof(Vertex), &vertices_[0], GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices_.size() * sizeof(unsigned int), &indices_[0], GL_STATIC_DRAW);

  // set the vertex attribute pointers
  // vertex Positions
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) 0);
  // vertex normals
  glEnableVertexAttribArray(1);
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, normal));
  // vertex seg result
  glEnableVertexAttribArray(2);
  glVertexAttribIPointer(2, 1, GL_INT, sizeof(Vertex), (void *) offsetof(Vertex, seg));
  // vertex texture coords
  glEnableVertexAttribArray(3);
  glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tex_coords));
  // vertex tangent
  glEnableVertexAttribArray(4);
  glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, tangent));
  // vertex bi_tangent
  glEnableVertexAttribArray(5);
  glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, bi_tangent));
  // ids
  glEnableVertexAttribArray(6);
  glVertexAttribIPointer(6, 4, GL_INT, sizeof(Vertex), (void *) offsetof(Vertex, m_bone_ids));
  // weights
  glEnableVertexAttribArray(7);
  glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *) offsetof(Vertex, m_weights));

  glBindVertexArray(0);
}

