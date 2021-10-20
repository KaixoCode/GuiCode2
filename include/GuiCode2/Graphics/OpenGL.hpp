#pragma once
#include "GuiCode2/Graphics/Graphics.hpp"

namespace GuiCode 
{
    class OpenGL : public GraphicsBase
    {
    public:
        OpenGL();
        void Render() override;

    private:
        glm::vec4 FlipY(const glm::vec4& v) { return { v.x, m_Size.height - v.y - v.w, v.z, v.w }; };
        glm::vec4 FlipY2(const glm::vec4& v) { return { v.x, m_Size.height - v.y, v.z, m_Size.height - v.w }; };

        void Fill(const Color&) override;
        void CreateQuadBuffers();
        void Quad(const glm::vec4&, float) override;
        void CreateTexturedQuadBuffers();
        void TexturedQuad(Texture, const glm::vec4&, float) override;
        void CreateLineBuffers();
        void Line(const glm::vec4&, float) override;
        void CreateEllipseBuffers();
        void Ellipse(const glm::vec4&, const glm::vec2&) override;
        void CreateTriangleBuffers();
        void Triangle(const glm::vec4&, float) override;
        void CreateTextBuffers();
        void Text(std::string_view, const glm::vec2&) override;
        void Font(std::string_view) override;
        void TextAlign(int) override;
        void FontSize(float) override;
        void LineHeight(float) override;
        void Clip(const glm::vec4&) override;
        void PushClip() override;
        void PopClip() override;
        void ClearClip() override;
        void Viewport(const glm::vec4&) override;

        Texture LoadTexture(const TextureData&) override;
        void FreeTexture(Texture) override;

        int m_PreviousShader = -1;
        Color m_Fill{ 1, 1, 1, 1 };
        float m_FontSize = 16;
        float m_LineHeight = 1.2;
        int m_TextAlign = Align::Left | Align::Bottom;

        GuiCode::Font* m_CurrentFont = nullptr;

        struct { unsigned int vao, vbo; } quad;
        struct { unsigned int vao, vbo; } textured;
        struct { unsigned int vao, vbo; } line;
        struct { unsigned int vao, vbo; } ellipse;
        struct { unsigned int vao, vbo; } triangle;
        struct { unsigned int vao, vbo; } text;
    };
}