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

    template<typename T, typename ...Ts>
    concept Contains = (std::is_same_v<Ts, T> || ...);

    template <typename> struct tag { };

    template <typename T, typename V>
    struct GetIndex 
    {
        constexpr static bool failed = true;
    };

    template <typename T, typename ...Ts> requires Contains<T, Ts...>
    struct GetIndex<T, std::variant<Ts...>> 
    {
        constexpr static bool failed = false;
        constexpr static size_t value = std::variant<tag<Ts>...>(tag<T>()).index();
    };

    static inline std::vector<std::string> split(const std::string& strv, std::string_view delims = " ")
    {
        std::vector<std::string> output;
        size_t first = 0;

        while (first < strv.size())
        {
            const auto second = strv.find_first_of(delims, first);

            if (first != second)
                output.emplace_back(strv.substr(first, second - first));

            if (second == std::string_view::npos)
                break;

            first = second + 1;
        }

        return output;
    }

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

        union{ float x = 0; float r; float h; };
        union{ float y = 0; float g; float s; };
        union{ float z = 0; float b; float v; };
        union{ float w = 0; float a; };

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

    struct Align
    {
        enum {
            Left = 0x0000,    // aligned to left on x-axis
            Right = 0x0001,   // aligned to right on x-axis
            CenterX = 0x0002, // aligned to center on x-axis
            Top = 0x0000,     // aligned to top on y-axis
            Bottom = 0x0010,  // aligned to bottom on y-axis
            CenterY = 0x0020, // aligned to center on y-axis
            Center = 0x0022,  // aligned to center on both axis

            // For text
            TextTop = Top,
            TextBottom = Bottom,
            Baseline = 0x0040,
            Middle = CenterY,
        };
    };


    /**
     * Simple Component wrapper, either contains pointer to
     * heap allocated Component object, or a reference to an
     * existing Component object.
     */
    template<typename T>
    class Wrapper
    {
    public:
        Wrapper()
        {}

        template<std::derived_from<T> Type>
        Wrapper(Type&& c)
            : m_Data(new Type(std::move(c))), m_Delete(true)
        {}

        template<std::derived_from<T> Type>
        Wrapper(Type& c)
            : m_Data(&c), m_Delete(false)
        {}

        Wrapper(Wrapper&& c)
            : m_Data(c.m_Data), m_Delete(c.m_Delete)
        {
            c.m_Delete = false;
        }

        Wrapper(const Wrapper& c)
            : m_Data(c.m_Data), m_Delete(c.m_Delete)
        {
            c.m_Delete = false;
        }

        Wrapper& operator=(const Wrapper& c)
        {
            m_Data = c.m_Data;
            m_Delete = c.m_Delete;
            c.m_Delete = false;
            return *this;
        }

        Wrapper& operator=(Wrapper&& c)
        {
            m_Data = c.m_Data;
            m_Delete = c.m_Delete;
            c.m_Delete = false;
            return *this;
        }

        T* operator->() { return m_Data; }
        T* operator&() { return m_Data; }
        T& operator*() { return *m_Data; }
        const T* operator->() const { return m_Data; }
        const T* operator&() const { return m_Data; }
        const T& operator*() const { return *m_Data; }

        template<std::derived_from<T> Type>
        operator Type& () { return *dynamic_cast<Type*>(m_Data); }

        template<std::derived_from<T> Type>
        operator const Type& () const { return *dynamic_cast<Type*>(m_Data); }

        template<std::derived_from<T> Type>
        operator Type* () { return dynamic_cast<Type*>(m_Data); }

        template<std::derived_from<T> Type>
        operator const Type* () const { return dynamic_cast<Type*>(m_Data); }

        operator T& () { return *m_Data; }
        operator const T& () const { return *m_Data; }

        operator T* () { return m_Data; }
        operator const T* () const { return m_Data; }

        operator bool() const { return m_Data; }

        bool operator==(Wrapper other) const { return other.m_Data == m_Data; }

        ~Wrapper()
        {
            if (m_Delete)
                delete m_Data;
        }

    private:
        T* m_Data = nullptr;
        mutable bool m_Delete = false;
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