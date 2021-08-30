#pragma once
#include "GuiCode2/pch.hpp"
namespace GuiCode
{
    template<typename T>
    struct Vec4;

    template<typename T>
    struct Vec2
    {
        union { T width, x, r, start; };
        union { T height, y, g, end; };

        Vec2 operator -() { return { -x, -y }; }
        Vec2 operator -(const Vec2& other) { return { x - other.x, y - other.y }; }
        Vec2 operator +(const Vec2& other) { return { x + other.x, y + other.y }; }
        Vec2 operator *(const Vec2& other) { return { x * other.x, y * other.y }; }
        Vec2 operator *(T other) { return { x * other, y * other }; }
        Vec2 operator +(T other) { return { x + other, y + other }; }
        Vec2& operator =(const Vec2& other) { x = other.x, y = other.y; return *this; }
        Vec2& operator+=(const Vec2& other) { x += other.x; y += other.y; return *this; }
        Vec2& operator-=(const Vec2& other) { x -= other.x; y -= other.y; return *this; }

        operator glm::vec2() { return { x, y }; }

        bool Inside(const Vec4<T>& o) const
        {
            return x >= o.x && x <= o.x + o.width && y > o.y && y < o.y + o.height;
        }

        template <size_t I>
        auto& get()&
        {
            if constexpr (I == 0) return x;
            else if constexpr (I == 1) return y;
        }

        template <size_t I>
        auto const& get() const&
        {
            if constexpr (I == 0) return x;
            else if constexpr (I == 1) return y;
        }

        template <size_t I>
        auto&& get()&&
        {
            if constexpr (I == 0) return std::move(x);
            else if constexpr (I == 1) return std::move(y);
        }
    };

    template<typename T>
    struct Vec3
    {
        union { T width, x, r; };
        union { T height, y, g; };
        union { T depth, z, b; };

        Vec3 operator -() { return { -x, -y, -z }; }
        Vec3 operator -(const Vec3& other) { return { x - other.x, y - other.y, z - other.z }; }
        Vec3 operator +(const Vec3& other) { return { x + other.x, y + other.y, z + other.z }; }
        Vec3 operator *(const Vec3& other) { return { x * other.x, y * other.y, z * other.z }; }
        Vec3 operator *(T other) { return { x * other, y * other, z * other }; }
        Vec3& operator =(const Vec3& other) { x = other.x, y = other.y; z = other.z; return *this; }
        Vec3& operator+=(const Vec3& other) { x += other.x; y += other.y; z += other.z; return *this; }
        Vec3& operator-=(const Vec3& other) { x -= other.x; y -= other.y; z -= other.z; return *this; }

        operator glm::vec3() { return { x, y, z }; }

        template <size_t I>
        auto& get()&
        {
            if constexpr (I == 0) return x;
            else if constexpr (I == 1) return y;
            else if constexpr (I == 2) return z;
        }

        template <size_t I>
        auto const& get() const&
        {
            if constexpr (I == 0) return x;
            else if constexpr (I == 1) return y;
            else if constexpr (I == 2) return z;
        }

        template <size_t I>
        auto&& get()&&
        {
            if constexpr (I == 0) return std::move(x);
            else if constexpr (I == 1) return std::move(y);
            else if constexpr (I == 2) return std::move(z);
        }
    };

    template<typename T>
    struct Vec4
    {
        union { T x, r; };
        union { T y, g; };
        union { T z, b, width; };
        union { T w, a, height; };

        Vec4 operator -() const { return { -x, -y, -z }; }
        Vec4 operator -(const Vec4& other) const { return { x - other.x, y - other.y, z - other.z, w - other.w }; }
        Vec4 operator +(const Vec4& other) const { return { x + other.x, y + other.y, z + other.z, w + other.w }; }
        Vec4 operator *(const Vec4& other) const { return { x * other.x, y * other.y, z * other.z, w * other.w }; }
        Vec4 operator *(T other) const { return { x * other, y * other, z * other, w * other }; }
        Vec4 operator /(T other) const { return { x / other, y / other, z / other, w / other }; }
        Vec4& operator =(const Vec4& other) { x = other.x, y = other.y; z = other.z; w = other.w; return *this; }
        Vec4& operator+=(const Vec4& other) { x += other.x; y += other.y; z += other.z; w += other.w; return *this; }
        Vec4& operator-=(const Vec4& other) { x -= other.x; y -= other.y; z -= other.z; w -= other.w; return *this; }
        bool operator==(const Vec4& other) const { return x == other.x && y == other.y && z == other.z && w == other.w; }

        Vec4 Overlap(const Vec4& o) const
        {
            if (width == -1 || height == -1)
                return o;
            T x5 = max(x, o.x);
            T y5 = max(y, o.y);
            T x6 = min(x + width, o.x + o.width);
            T y6 = min(y + height, o.y + o.height);
            if (x5 > x6 || y5 > y6)
                return { 0, 0, 0, 0 };
            return { x5, y5, x6 - x5, y6 - y5 };
        }

        Vec4 Translate(const Vec2<T>& o) const
        {
            return { x - o.x, y - o.y, width, height };
        }

        bool Contains(const Vec2<T>& o) const
        {
            return o.x >= x && o.x <= x + width && o.y > y && o.y < y + height;
        }

        operator glm::vec4() { return { x, y, z, w }; }

        template <size_t I>
        auto& get()&
        {
            if constexpr (I == 0) return x;
            else if constexpr (I == 1) return y;
            else if constexpr (I == 2) return z;
            else if constexpr (I == 3) return w;
        }

        template <size_t I>
        auto const& get() const&
        {
            if constexpr (I == 0) return x;
            else if constexpr (I == 1) return y;
            else if constexpr (I == 2) return z;
            else if constexpr (I == 3) return w;
        }

        template <size_t I>
        auto&& get()&&
        {
            if constexpr (I == 0) return std::move(x);
            else if constexpr (I == 1) return std::move(y);
            else if constexpr (I == 2) return std::move(z);
            else if constexpr (I == 3) return std::move(w);
        }
    };
}

namespace std
{
    template <typename T> struct tuple_size<GuiCode::Vec4<T>> : std::integral_constant<size_t, 4> { };

    template <typename T> struct tuple_element<0, GuiCode::Vec4<T>> { using type = T; };
    template <typename T> struct tuple_element<1, GuiCode::Vec4<T>> { using type = T; };
    template <typename T> struct tuple_element<2, GuiCode::Vec4<T>> { using type = T; };
    template <typename T> struct tuple_element<3, GuiCode::Vec4<T>> { using type = T; };

    template <typename T> struct tuple_size<GuiCode::Vec3<T>> : std::integral_constant<size_t, 3> { };

    template <typename T> struct tuple_element<0, GuiCode::Vec3<T>> { using type = T; };
    template <typename T> struct tuple_element<1, GuiCode::Vec3<T>> { using type = T; };
    template <typename T> struct tuple_element<2, GuiCode::Vec3<T>> { using type = T; };

    template <typename T> struct tuple_size<GuiCode::Vec2<T>> : std::integral_constant<size_t, 2> { };

    template <typename T> struct tuple_element<0, GuiCode::Vec2<T>> { using type = T; };
    template <typename T> struct tuple_element<1, GuiCode::Vec2<T>> { using type = T; };
}
