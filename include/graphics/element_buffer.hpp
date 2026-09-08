#pragma once

class ElementBuffer
{
  private:
    unsigned int m_ebo;
    unsigned int m_count;

  public:
    ElementBuffer(const unsigned int *data, unsigned int count);
    ~ElementBuffer();

    void bind() const;
    void unbind() const;

    inline unsigned int get_count() const
    {
        return m_count;
    };
};
