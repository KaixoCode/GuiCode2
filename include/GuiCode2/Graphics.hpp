#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Shader.hpp"

namespace GuiCode
{
    struct Command
    {
        union
        {
            Color color;
            Vec4<float> a;
            struct { Vec2<float> a; Vec2<float> b; } as;
        };

        union
        {
            Vec2<float> b;
            struct { float a; float b; } bs;
        };

        enum Type : char {
            Fill = 0, Clip, PushClip, PopClip, ClearClip, Translate, PushMatrix,
            PopMatrix, Viewport, Line, Quad, Ellipse, Triangle
        } type;

    };

    struct CommandCollection
    {
        std::queue<Command>& Get() { return m_Commands; }

        void Fill(const Color& a) { m_Commands.push(Command{ .color = a, .type = Command::Fill }); };
        void Clip(const Vec4<float>& a) { m_Commands.push(Command{.a = a, .type = Command::Clip }); };
        void PushClip() { m_Commands.push(Command{ .type = Command::PushClip }); };
        void PopClip() { m_Commands.push(Command{ .type = Command::PopClip }); };
        void ClearClip() { m_Commands.push(Command{ .type = Command::ClearClip }); };
        void Translate(const Vec2<float>& a) { m_Commands.push(Command{ .as = {.a = a }, .type = Command::Translate }); };
        void PushMatrix() { m_Commands.push(Command{ .type = Command::PushMatrix }); };
        void PopMatrix() { m_Commands.push(Command{ .type = Command::PopMatrix }); };
        void Viewport(const Vec4<float>& a) { m_Commands.push(Command{.a = a, .type = Command::Viewport }); };
        void Line(const Vec4<float>& a, float thickness) { m_Commands.push(Command{.a = a, .bs = {.a = thickness }, .type = Command::Line }); };
        void Quad(const Vec4<float>& a, float b = 0) { m_Commands.push(Command{ .a = a, .bs = {.a = b }, .type = Command::Quad }); };
        void Ellipse(const Vec4<float>& a, const Vec2<float>& b) { m_Commands.push(Command{ .a = a, .b = b, .type = Command::Ellipse }); };
        void Triangle(const Vec4<float>& a, float b = 0) { m_Commands.push(Command{ .a = a, .bs{.a = b }, .type = Command::Triangle }); };
        void Clear() {  }

    private:
        std::queue<Command> m_Commands;
    };

    class GraphicsBase
    {
        static inline int m_GraphicsIdCounter = 0;
    public:

        virtual void Render() = 0;
        virtual void SetProjection(const glm::mat4& proj);
    
        CommandCollection collection;

    protected:
        virtual void Translate(const glm::vec2&);
        virtual void Scale(const glm::vec2&);
        virtual void PushMatrix();
        virtual void PopMatrix();
        virtual void Fill(const Color&) = 0;
        virtual void Quad(const glm::vec4&, float) = 0;
        virtual void Line(const glm::vec4&, float) = 0;
        virtual void Ellipse(const glm::vec4&, const glm::vec2&) = 0;
        virtual void Triangle(const glm::vec4&, float) = 0;
        virtual void Clip(const glm::vec4&) = 0;
        virtual void PushClip() = 0;
        virtual void PopClip() = 0;
        virtual void ClearClip() = 0;
        virtual void Viewport(const glm::vec4&) = 0;

        std::stack<Vec4<float>> m_ClipStack;
        Vec4<float> m_Clip;

        glm::mat4 m_Projection{ 0 };
        glm::mat4 m_ViewProj{ 0 };
        std::stack<glm::mat4> m_MatrixStack;
        glm::mat4 m_Matrix{ 1.0f }; 
        Vec2<float> m_Size;
        int m_GraphicsId = m_GraphicsIdCounter++;
        float m_Scaling = 1;
    };

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
        void CreateLineBuffers();
        void Line(const glm::vec4&, float) override;
        void CreateEllipseBuffers();
        void Ellipse(const glm::vec4&, const glm::vec2&) override;
        void CreateTriangleBuffers();
        void Triangle(const glm::vec4&, float) override;
        void Clip(const glm::vec4&) override;
        void PushClip() override;
        void PopClip() override;
        void ClearClip() override;
        void Viewport(const glm::vec4&) override;

        int m_PreviousShader = -1;
        Color m_Fill{ 1, 1, 1, 1 };

        struct { unsigned int vao, vbo; } quad;
        struct { unsigned int vao, vbo; } line;
        struct { unsigned int vao, vbo; } ellipse;
        struct { unsigned int vao, vbo; } triangle;
    };
}