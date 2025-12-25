#include "VertexArray.h"

// Специализации шаблона для разных типов данных
template<>
void VertexBufferLayout::push<float>(uint32_t count) {
    m_elements.push_back({ GL_FLOAT, count, GL_FALSE });
    m_stride += count * VertexBufferElement::getSizeOfType(GL_FLOAT);
}

VertexArray::VertexArray() {
    glGenVertexArrays(1, &m_id);
}

VertexArray::~VertexArray() {
    glDeleteVertexArrays(1, &m_id);
}

void VertexArray::bind() const {
    glBindVertexArray(m_id);
}

void VertexArray::unbind() const {
    glBindVertexArray(0);
}

void VertexArray::addBuffer(uint32_t vbo, const VertexBufferLayout& layout) {
    bind();
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    const auto& elements = layout.getElements();
    uint64_t offset = 0;

    for (uint32_t i = 0; i < elements.size(); i++) {
        const auto& element = elements[i];

        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            element.count,
            element.type,
            element.normalized,
            layout.getStride(),
            (const void*)offset
            );

        offset += (uint64_t)element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}
