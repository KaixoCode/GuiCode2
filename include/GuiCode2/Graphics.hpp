#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Shader.hpp"

namespace GuiCode
{
    struct Command
    {
        struct Fill { Color color; };
        struct Clip { Vec4<double> area; };
        struct PopClip { };
        struct ClearClip { };
        struct Translate { Vec2<double> amount; };
        struct PushMatrix { };
        struct PopMatrix { };
        struct Viewport { Vec4<double> area; };
        struct Line { Vec2<double> start; Vec2<double> end; double thickness; };
        struct Quad { Vec4<double> dimensions; double rotation; };
        struct Ellipse { Vec4<double> dimensions; Vec2<double> angles; };
        struct Triangle { Vec4<double> dimensions; double rotation; };
        std::variant<Fill, Clip, PopClip, ClearClip, Translate, PushMatrix, 
            PopMatrix, Viewport, Line, Quad, Ellipse, Triangle> data;
    };

    struct CommandCollection
    {
        std::vector<Command>& Get() { return m_Commands; }

        void Fill(const Color& a) { m_Commands.emplace_back(Command::Fill{ a }); };
        void Clip(const Vec4<double>& a) { m_Commands.emplace_back(Command::Clip{ a }); };
        void PopClip() { m_Commands.emplace_back(Command::PopClip{ }); };
        void ClearClip() { m_Commands.emplace_back(Command::ClearClip{ }); };
        void Translate(const Vec2<double>& a) { m_Commands.emplace_back(Command::Translate{ a }); };
        void PushMatrix() { m_Commands.emplace_back(Command::PushMatrix{}); };
        void PopMatrix() { m_Commands.emplace_back(Command::PopMatrix{ }); };
        void Viewport(const Vec4<double>& a) { m_Commands.emplace_back(Command::Viewport{ a }); };
        void Line(const Vec2<double>& a, const Vec2<double>& b, double thickness) { m_Commands.emplace_back(Command::Line{ a, b, thickness }); };
        void Quad(const Vec4<double>& a, double b = 0) { m_Commands.emplace_back(Command::Quad{ a, b }); };
        void Ellipse(const Vec4<double>& a, const Vec2<double>& b) { m_Commands.emplace_back(Command::Ellipse{ a, b }); };
        void Triangle(const Vec4<double>& a, double b = 0) { m_Commands.emplace_back(Command::Triangle{ a, b }); };

    private:
        std::vector<Command> m_Commands;
    };

    void SetProjection(const glm::mat4& proj);
    void CurrentWindow(int a);

    void RunCommands(CommandCollection&);
}