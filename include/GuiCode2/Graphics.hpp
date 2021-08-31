#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Shader.hpp"

namespace GuiCode
{
    struct Command
    {
        struct Fill { Color color; };
        struct Clip { Vec4<double> area; };
        struct PushClip { };
        struct PopClip { };
        struct ClearClip { };
        struct Translate { Vec2<double> amount; };
        struct PushMatrix { };
        struct PopMatrix { };
        struct Viewport { Vec4<double> area; };
        struct Line { Vec4<double> points; double thickness; };
        struct Quad { Vec4<double> dimensions; double rotation; };
        struct Ellipse { Vec4<double> dimensions; Vec2<double> angles; };
        struct Triangle { Vec4<double> dimensions; double rotation; };
        std::variant<Fill, Clip, PushClip, PopClip, ClearClip, Translate, PushMatrix, 
            PopMatrix, Viewport, Line, Quad, Ellipse, Triangle> data;
    };

    struct CommandCollection
    {
        std::vector<Command>& Get() { return m_Commands; }

        void Fill(const Color& a) { m_Commands.emplace_back(Command::Fill{ a }); };
        void Clip(const Vec4<double>& a) { m_Commands.emplace_back(Command::Clip{ a }); };
        void PushClip() { m_Commands.emplace_back(Command::PushClip{ }); };
        void PopClip() { m_Commands.emplace_back(Command::PopClip{ }); };
        void ClearClip() { m_Commands.emplace_back(Command::ClearClip{ }); };
        void Translate(const Vec2<double>& a) { m_Commands.emplace_back(Command::Translate{ a }); };
        void PushMatrix() { m_Commands.emplace_back(Command::PushMatrix{}); };
        void PopMatrix() { m_Commands.emplace_back(Command::PopMatrix{ }); };
        void Viewport(const Vec4<double>& a) { m_Commands.emplace_back(Command::Viewport{ a }); };
        void Line(const Vec4<double>& a, double thickness) { m_Commands.emplace_back(Command::Line{ a, thickness }); };
        void Quad(const Vec4<double>& a, double b = 0) { m_Commands.emplace_back(Command::Quad{ a, b }); };
        void Ellipse(const Vec4<double>& a, const Vec2<double>& b) { m_Commands.emplace_back(Command::Ellipse{ a, b }); };
        void Triangle(const Vec4<double>& a, double b = 0) { m_Commands.emplace_back(Command::Triangle{ a, b }); };
        void Clear() { m_Commands.clear(); }

    private:
        std::vector<Command> m_Commands;
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
        virtual void Quad(const glm::vec4&, double) = 0;
        virtual void Line(const glm::vec4&, double) = 0;
        virtual void Ellipse(const glm::vec4&, const glm::vec2&) = 0;
        virtual void Triangle(const glm::vec4&, float) = 0;
        virtual void Clip(const glm::vec4&) = 0;
        virtual void PushClip() = 0;
        virtual void PopClip() = 0;
        virtual void ClearClip() = 0;
        virtual void Viewport(const glm::vec4&) = 0;

        std::stack<Vec4<double>> m_ClipStack;
        Vec4<double> m_Clip;

        glm::mat4 m_Projection{ 0 };
        glm::mat4 m_ViewProj{ 0 };
        std::stack<glm::mat4> m_MatrixStack;
        glm::mat4 m_Matrix{ 1.0f }; 
        Vec2<double> m_Size;
        int m_GraphicsId = m_GraphicsIdCounter++;
        float m_Scaling = 1;
    };

    class OpenGL : public GraphicsBase
    {
    public:
        void Render() override;

    private:
        glm::vec4 FlipY(const glm::vec4& v) { return { v.x, m_Size.height - v.y - v.w, v.z, v.w }; };
        glm::vec4 FlipY2(const glm::vec4& v) { return { v.x, m_Size.height - v.y, v.z, m_Size.height - v.w }; };
        glm::vec2 FlipY(const glm::vec2& v) { return { v.x, m_Size.height - v.y }; };

        void Fill(const Color&) override;
        void Quad(const glm::vec4&, double) override;
        void Line(const glm::vec4&, double) override;
        void Ellipse(const glm::vec4&, const glm::vec2&) override;
        void Triangle(const glm::vec4&, float) override;
        void Clip(const glm::vec4&) override;
        void PushClip() override;
        void PopClip() override;
        void ClearClip() override;
        void Viewport(const glm::vec4&) override;

        int m_PreviousShader = -1;
        Color m_Fill{ 1, 1, 1, 1 };
    };
}