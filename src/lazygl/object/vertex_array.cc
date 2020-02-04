#include "lazygl/object/vertex_array.h"

namespace lazygl
{
VertexArray::VertexArray()
{
}

VertexArray::~VertexArray()
{
  Delete();
}

void VertexArray::Move(VertexArray&& rhs) noexcept
{
  generated_ = rhs.generated_;
  id_ = rhs.id_;

  has_element_buffer_ = rhs.has_element_buffer_;
  element_buffer_bound_ = rhs.element_buffer_bound_;
  num_elements_ = rhs.num_elements_;
  element_buffer_ = rhs.element_buffer_;

  attribs_ = std::move(rhs.attribs_);
  attribs_commanded_ = rhs.attribs_commanded_;

  mode_ = rhs.mode_;



  rhs.generated_ = false;
  rhs.id_ = 0;

  rhs.has_element_buffer_ = false;
  rhs.element_buffer_bound_ = false;
  rhs.num_elements_ = 0;
  rhs.element_buffer_ = nullptr;

  rhs.attribs_commanded_ = false;

  rhs.mode_ = DrawMode::POINTS;
}

VertexArray::VertexArray(VertexArray&& rhs) noexcept
{
  Move(std::move(rhs));
}

VertexArray& VertexArray::operator = (VertexArray&& rhs) noexcept
{
  Move(std::move(rhs));
  return *this;
}

void VertexArray::Generate()
{
  if (!generated_)
  {
    glGenVertexArrays(1, &id_);
    generated_ = true;
  }
}

void VertexArray::Delete()
{
  if (generated_)
  {
    glDeleteVertexArrays(1, &id_);
    generated_ = false;
  }
}

void VertexArray::Indices(DrawMode mode, int count)
{
  mode_ = mode;
  num_elements_ = count;
  has_element_buffer_ = false;
  element_buffer_ = nullptr;
  element_buffer_bound_ = false;
}

void VertexArray::Indices(DrawMode mode, ElementBuffer& buffer, int count)
{
  mode_ = mode;
  element_buffer_ = &buffer;

  if (count < 0)
    num_elements_ = buffer.Size();
  else
    num_elements_ = count;

  has_element_buffer_ = true;
}

void VertexArray::Bind()
{
  if (!generated_)
    Generate();

  glBindVertexArray(id_);
}

void VertexArray::Unbind()
{
  glBindVertexArray(0);
}

void VertexArray::AttribPointerLazy()
{
  if (!attribs_commanded_)
  {
    Bind();

    for (const auto& attrib : attribs_)
    {
      auto& buffer = attrib.buffer.get();

      buffer.Bind(BufferType::ARRAY_BUFFER);
      glVertexAttribPointer(attrib.index, attrib.size, buffer.GlType(), GL_FALSE, buffer.InByte(attrib.stride), (void*)(buffer.InByte(attrib.offset)));
      glEnableVertexAttribArray(attrib.index);
      buffer.Unbind(BufferType::ARRAY_BUFFER);
    }

    Unbind();

    attribs_commanded_ = true;
  }
}

void VertexArray::BindElementBufferLazy()
{
  if (has_element_buffer_ && !element_buffer_bound_)
  {
    Bind();
    element_buffer_->Bind(BufferType::ELEMENT_ARRAY_BUFFER);
    Unbind();
    element_buffer_->Unbind(BufferType::ELEMENT_ARRAY_BUFFER);

    element_buffer_bound_ = true;
  }
}

void VertexArray::UpdateBuffers()
{
  for (const auto& attrib : attribs_)
  {
    auto& buffer = attrib.buffer.get();
    buffer.Update();
  }

  if (element_buffer_ != nullptr)
    element_buffer_->Update();
}

void VertexArray::Draw()
{
  if (!generated_)
    Generate();

  AttribPointerLazy();
  BindElementBufferLazy();

  UpdateBuffers();

  Bind();

  if (has_element_buffer_)
  {
    glDrawElements(static_cast<GLenum>(mode_), num_elements_, GL_UNSIGNED_INT, 0);
  }

  else
  {
    glDrawArrays(static_cast<GLenum>(mode_), 0, num_elements_);
  }

  Unbind();
}
}
