#pragma once
#include "GuiCode2/pch.hpp"

namespace GuiCode
{
    static inline void remove(auto& container, auto& element) {
        //container.erase(std::remove(container.begin(), container.end(), element));
    }

    // trim from start (in place)
    static inline void ltrim(std::string& s) {
        s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
            return !std::isspace(ch);
            }));
    }

    // trim from end (in place)
    static inline void rtrim(std::string& s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
            }).base(), s.end());
    }

    // trim from both ends (in place)
    static inline void trim(std::string& s) {
        ltrim(s);
        rtrim(s);
    }

    // trim from start (copying)
    static inline std::string ltrim_copy(std::string s) {
        ltrim(s);
        return s;
    }

    // trim from end (copying)
    static inline std::string rtrim_copy(std::string s) {
        rtrim(s);
        return s;
    }

    // trim from both ends (copying)
    static inline std::string trim_copy(std::string s) {
        trim(s);
        return s;
    }

    template<typename T>
    constexpr static inline bool contains(std::vector<T>& v, const T& t)
    {
        return std::find(v.begin(), v.end(), t) != v.end();
    };
    
    template<int N, typename... Ts> using NthTypeOf =
        typename std::tuple_element<N, std::tuple<Ts...>>::type;

    template <class, template <class> class>
    struct is_instance : public std::false_type {};

    template <class T, template <class> class U>
    struct is_instance<U<T>, U> : public std::true_type {};

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

    static inline std::vector<std::string_view> Split(const std::string_view str, const char delim = ',')
    {
        std::vector<std::string_view> result;

        int indexCommaToLeftOfColumn = 0;
        int indexCommaToRightOfColumn = -1;

        for (int i = 0;i < static_cast<int>(str.size());i++)
        {
            if (str[i] == delim)
            {
                indexCommaToLeftOfColumn = indexCommaToRightOfColumn;
                indexCommaToRightOfColumn = i;
                int index = indexCommaToLeftOfColumn + 1;
                int length = indexCommaToRightOfColumn - index;

                std::string_view column(str.data() + index, length);
                result.push_back(column);
            }
        }
        const std::string_view finalColumn(str.data() + indexCommaToRightOfColumn + 1, str.size() - indexCommaToRightOfColumn - 1);
        result.push_back(finalColumn);
        return result;
    }


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
}