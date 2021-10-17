#pragma once
#include "GuiCode2/pch.hpp"

namespace GuiCode 
{
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

        Color operator -() const { return { -x, -y, -z, -w }; }
        Color operator +(const Color& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
        Color operator -(const Color& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
        Color operator *(const Color& other) const { return { x * other.x, y * other.y, z * other.z, w * other.w }; }
        Color operator /(const Color& other) const { return { x / other.x, y / other.y, z / other.z, w / other.w }; }
        Color operator +(float other) const { return { x + other, y + other, z + other, w + other }; }
        Color operator -(float other) const { return { x - other, y - other, z - other, w - other }; }
        Color operator *(float other) const { return { x * other, y * other, z * other, w * other }; }
        Color operator /(float other) const { return { x / other, y / other, z / other, w / other }; }
        Color& operator+=(const Color& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
        Color& operator-=(const Color& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
        Color& operator*=(const Color& other) { x *= other.x; y *= other.y; z *= other.z; w *= other.w; return *this; }
        Color& operator/=(const Color& other) { x /= other.x; y /= other.y; z /= other.z; w /= other.w; return *this; }
        Color& operator=(const Color& other) { x = other.x, y = other.y, z = other.z, w = other.w; return *this; }
        bool operator==(const Color& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }
        bool operator!=(const Color& other) const { return x != other.x || y != other.y || z != other.z || w != other.w; }

        union { float x = 0; float r; float h; };
        union { float y = 0; float g; float s; };
        union { float z = 0; float b; float v; };
        union { float w = 0; float a; };

        Color Lerp(const Color& other, float percent) const
        {
            Color _new;
            _new.r = r * (1 - percent) + other.r * percent;
            _new.g = g * (1 - percent) + other.g * percent;
            _new.b = b * (1 - percent) + other.b * percent;
            _new.a = a * (1 - percent) + other.a * percent;
            return _new;
        }

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

}

namespace std
{
    template<> struct tuple_size<GuiCode::Color> : std::integral_constant<size_t, 4> { };

    template<> struct tuple_element<0, GuiCode::Color> { using type = float; };
    template<> struct tuple_element<1, GuiCode::Color> { using type = float; };
    template<> struct tuple_element<2, GuiCode::Color> { using type = float; };
    template<> struct tuple_element<3, GuiCode::Color> { using type = float; };
}