#ifndef VAO_HPP
#define VAO_HPP
#include "constants.hpp"

namespace vao {
struct Vao {
private:
    GLuint vaoID;
    GLuint vboID;
    GLuint eboID;
public:
    Vao(GLuint vaoID=0u, GLuint vboID=0u, GLuint eboID=0u) : vaoID(vaoID), vboID(vboID), eboID(eboID) {}

    inline void bind() const {
        glBindVertexArray(vaoID);
    }

    inline void unbind() const {
        glBindVertexArray(0u);
    }

    Vao& operator=(Vao&& other) noexcept {
        if (this != &other) { // Protect against self-assignment
            // First, clean up resources currently owned by 'this' object
            if (vaoID != 0) {
                glDeleteVertexArrays(1, &vaoID);
            }
            if (vboID != 0) {
                glDeleteBuffers(1, &vboID);
            }
            if (eboID != 0) {
                glDeleteBuffers(1, &eboID);
            }

            // Then, transfer ownership from 'other'
            vaoID = other.vaoID;
            vboID = other.vboID;
            eboID = other.eboID;

            // Invalidate 'other's IDs
            other.vaoID = 0;
            other.vboID = 0;
            other.eboID = 0;
        }
        return *this;
    }

    ~Vao() {
        glDeleteVertexArrays(1, &vaoID);
        glDeleteVertexArrays(1, &vboID);
        if (eboID)
            glDeleteVertexArrays(1, &eboID);
    }
};

Vao createRectangleVao();
Vao createCircleVao();
inline Vao rectangle;
inline Vao circle;
}
#endif // VAO_HPP
