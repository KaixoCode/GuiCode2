#pragma once
#include "GuiCode2/pch.hpp"

namespace GuiCode
{
    // Get the signature of a member function
    template<typename T>
    struct MemberSignature {};
    template<typename Ret, typename Type, typename... Args>
    struct MemberSignature<Ret(Type::*)(Args...) const>
    {
        using type = Ret(Args...);
    };
    template<typename Ret, typename Type, typename... Args>
    struct MemberSignature<Ret(Type::*)(Args...)>
    {
        using type = Ret(Args...);
    };

    // Get the signature of a lambda's operator()
    template<typename _Fx, typename = void>
    struct LambdaSignature {};
    template<typename _Fx>
    struct LambdaSignature<_Fx, std::void_t<decltype(&_Fx::operator())>>
    {
        using type = typename MemberSignature<decltype(&_Fx::operator())>::type;
    };

    template<typename _Fx>
    using LambdaSignatureT = typename LambdaSignature<_Fx>::type;

    struct Color
    {
        constexpr Color()
        {}

        constexpr Color(float r, float g, float b, float a)
            : r(r), g(g), b(b), a(a)
        {}

        constexpr Color(float r, float g, float b)
            : r(r), g(g), b(b), a(255)
        {}

        constexpr Color(unsigned int hex)
            : r((hex & 0x00FF0000) >> 16), g((hex & 0x0000FF00) >> 8), b(hex & 0x000000FF), a((hex & 0xFF000000) >> 24)
        {}        
        
        constexpr Color(int hex)
            : r((hex & 0x00FF0000) >> 16), g((hex & 0x0000FF00) >> 8), b(hex & 0x000000FF), a(255)
        {}

        Color operator -() const { return { -r, -g, -b, -a }; }
        Color operator -(const Color& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
        Color operator +(const Color& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
        Color operator *(const Color& other) const { return { x * other.x, y * other.y, z * other.z, w * other.w }; }
        Color operator *(float other) const { return { x * other, y * other, z * other, w * other }; }
        Color operator /(float other) const { return { x / other, y / other, z / other, w / other }; }
        Color operator +(float other) const { return { x + other, y + other, z + other, w + other }; }
        Color operator -(float other) const { return { x + other, y + other, z - 2 * other, w - 2 * other }; }
        Color& operator =(const Color& other) { x = other.x, y = other.y; z = other.z; w = other.w; return *this; }
        Color& operator+=(const Color& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
        Color& operator-=(const Color& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
        bool operator==(const Color& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }

        union{ float x = 0; float r; float h; };
        union{ float y = 0; float g; float s; };
        union{ float z = 0; float b; float v; };
        union{ float w = 0; float a; };

        template <size_t I>
        auto& get()&
        {
            if constexpr (I == 0) return r;
            else if constexpr (I == 1) return g;
            else if constexpr (I == 2) return b;
            else if constexpr (I == 3) return a;
        }

        template <size_t I>
        auto const& get() const&
        {
            if constexpr (I == 0) return r;
            else if constexpr (I == 1) return g;
            else if constexpr (I == 2) return b;
            else if constexpr (I == 3) return a;
        }

        template <size_t I>
        auto&& get()&&
        {
            if constexpr (I == 0) return r;
            else if constexpr (I == 1) return g;
            else if constexpr (I == 2) return b;
            else if constexpr (I == 3) return a;
        }
    };

    struct Align
    {
        enum {
            Left = 0x0000,    // aligned to left on x-axis
            Right = 0x0001,   // aligned to right on x-axis
            CenterX = 0x0002, // aligned to center on x-axis
            Top = 0x0000,     // aligned to top on y-axis
            Bottom = 0x0010,  // aligned to bottom on y-axis
            CenterY = 0x0020, // aligned to center on y-axis
            Center = 0x0022   // aligned to center on both axis
        };
    };
}

namespace std
{
    template<> struct tuple_size<GuiCode::Color> : std::integral_constant<size_t, 4> { };
    
    template<> struct tuple_element<0, GuiCode::Color> { using type = float; };
    template<> struct tuple_element<1, GuiCode::Color> { using type = float; };
    template<> struct tuple_element<2, GuiCode::Color> { using type = float; };
    template<> struct tuple_element<3, GuiCode::Color> { using type = float; };
}