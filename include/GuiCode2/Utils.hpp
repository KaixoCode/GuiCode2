#pragma once
#include "GuiCode2/pch.hpp"

namespace GuiCode
{
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

    template<typename T>
    class Ref
    {
        T& m_Ref;
    public:
        using Type = T;

        Ref(T& r)
            : m_Ref(r)
        {}

        operator T& () { return m_Ref; }
        operator T const& () const { return m_Ref; }

        template<typename Type>
        T& operator=(Type&& type) { m_Ref = type; return m_Ref; }
    };

    /**
     * Simple pointer wrapper, either contains pointer to
     * heap allocated object, or a reference to an
     * existing object. Automatically deletes object
     * when there are 0 references left.
     */
    template<typename T>
    class Pointer
    {
        constexpr static inline size_t NO = static_cast<size_t>(-1);
        struct Object
        {
            Object(T* ptr, size_t r)
                : data(ptr), refs(r)
            {}

            T* data;
            size_t refs = 1;
        };
    public:
        Pointer()
            : m_Data(new Object{ nullptr, NO })
        {}

        template<std::derived_from<T> Type>
        Pointer(Type&& c)
            : m_Data(new Object{ new Type{ std::move(c) }, 1 })
        {
            std::cout << "MOVE" << std::endl;
        }

        template<std::derived_from<T> Type>
        Pointer(Type& c)
            : m_Data(new Object{ &c, NO })
        {}

        template<std::derived_from<T> Type>
        Pointer(Type* c)
            : m_Data(new Object{ c, 1 })
        {}

        template<std::derived_from<T> Type>
        Pointer(Type* c, int refs)
            : m_Data(new Object{ c, refs })
        {}

        template<typename Type> requires (std::derived_from<Type, T> || std::derived_from<T, Type>)
        Pointer(Pointer<Type>&& c)
            : m_Data(new Object{ dynamic_cast<T*>(c.m_Data->data), c.m_Data->refs })
        {
            c.Invalidate();
        }

        Pointer(const Pointer& c)
            : m_Data(c.m_Data)
        {
            if (c.m_Data->refs != NO)
                c.m_Data->refs++;
        }

        Pointer& operator=(const Pointer& c)
        {
            if (m_Data->refs != NO)
            {
                m_Data->refs--;
                if (m_Data->refs == 0)
                {
                    delete m_Data->data;
                    delete m_Data;
                }
            }

            m_Data = c.m_Data;
            if (m_Data->refs != NO)
                m_Data->refs++;

            return *this;
        }

        template<typename Type> requires (std::derived_from<Type, T> || std::derived_from<T, Type>)
        Pointer& operator=(Pointer<Type>&& c)
        {
            if (m_Data->refs != NO)
            {
                m_Data->refs--;
                if (m_Data->refs == 0)
                {
                    delete m_Data->data;
                    delete m_Data;
                }
            }

            m_Data->data = dynamic_cast<T*>(c.m_Data->data);
            m_Data->refs = c.m_Data->refs;
            c.Invalidate();

            return *this;
        }

        T* operator->() { return m_Data->data; }
        T* operator&() { return m_Data->data; }
        T& operator*() { return *m_Data->data; }
        const T* operator->() const { return m_Data->data; }
        const T* operator&() const { return m_Data->data; }
        const T& operator*() const { return *m_Data->data; }

        template<std::derived_from<T> Type>
        operator Type& () { return *dynamic_cast<Type*>(m_Data->data); }

        template<std::derived_from<T> Type>
        operator const Type& () const { return *dynamic_cast<Type*>(m_Data->data); }

        template<std::derived_from<T> Type>
        operator Type* () { return dynamic_cast<Type*>(m_Data->data); }

        template<std::derived_from<T> Type>
        operator const Type* () const { return dynamic_cast<Type*>(m_Data->data); }

        operator T& () { return *m_Data->data; }
        operator const T& () const { return *m_Data->data; }

        operator T* () { return m_Data->data; }
        operator const T* () const { return m_Data->data; }

        operator bool() { return m_Data && m_Data->data; }
        operator bool() const { return m_Data && m_Data->data; }

        bool operator==(const Pointer& other) const { return other.m_Data->data == m_Data->data; }
        bool operator==(T* other) const { return other == m_Data->data; }

        ~Pointer()
        {
            if (!m_Data)
                return;

            if (m_Data->refs != NO)
                m_Data->refs--;

            if (m_Data->refs == 0)
            {
                delete m_Data->data;
                delete m_Data;
            }
        }

        /**
         * Invalidates this pointer, does not clear memory.
         */
        void Invalidate()
        {
            delete m_Data;
            m_Data = nullptr;
        }

    private:
        Object* m_Data = nullptr;

        template<typename T>
        friend class Pointer;
    };

    template<typename T>
    concept StateType = requires(T t, int a, Color b)
    {
        t.State(a, b);
    };

    template<size_t N, typename Arg>
    struct DropN;

    template<size_t N, typename Arg, typename ...Args> requires (N > 0)
    struct DropN<N, std::tuple<Arg, Args...>> : public DropN<N - 1, std::tuple<Args...>>
    {};

    template<typename ...Args>
    struct DropN<0, std::tuple<Args...>>
    {
        using Type = std::tuple<Args...>;
    };

    struct _FunctionStorageBase {
        _FunctionStorageBase() { m_RefCount++; }
        virtual ~_FunctionStorageBase() { }

        inline _FunctionStorageBase* Clone() { m_RefCount++; return this; }
        virtual bool Lambda() const { return false; }
        virtual void CallWithString(std::any*, int s, std::string_view&) {};

        size_t m_RefCount = 0;
    };

    // Binder without type info about what kind of functor it contains, only return and arg types.
    template<typename Return, typename ...Args>
    struct _FunctionStorageCaller : public _FunctionStorageBase {
        virtual Return Call(Args&&...) = 0;
        virtual void CallWithString(std::any* data, int s, std::string_view& view) override
        {
            CallSeq(data, // Given arguments from code
                s,        // Amount of given arguments from code
                view,     // Remaining arguments in a string_view
                std::make_index_sequence<sizeof...(Args)>{}); 
        }

    private:
        template<size_t ...Is>
        void CallSeq(std::any* data, int s, std::string_view& view, std::index_sequence<Is...>)
        {
             (((Is == s) // If index is amount that needs to be dropped
                && (CallSeq2(data,        // Send our type erased array of void*
                    Parsers<DropN<Is, std::tuple<std::decay_t<Args>...>>::Type>::Parse(view), // Parse the last couple from Args from string
                    std::make_index_sequence<Is>{},                   // Create sequence for the parsed arguments
                    std::make_index_sequence<sizeof...(Args) - Is>{}) // Create sequence for the void* arguments
                    , false)), ...
                );
        }

        template<typename ...Tys, size_t ...Is, size_t ...Ts>
        void CallSeq2(std::any* data, std::tuple<Tys...>&& tuple, std::index_sequence<Is...>, std::index_sequence<Ts...>)
        {
            if constexpr (sizeof...(Ts) == sizeof...(Tys))
            Call(std::forward<NthTypeOf<Is, Args...>>((std::any_cast<NthTypeOf<Is, Args...>>(data[Is])))..., // Pack expand void* and cast them
                std::forward<NthTypeOf<Ts, Tys...>>(std::get<Ts>(tuple))...);                   // Pack expand parsed arguments from tuple
        }
    };

    // Full binder contains all the type info, which allows the Finalize method to cast 
    // all void pointers back to their original types, to then call the lambda/function pointer
    template<typename, typename> struct _TypedFunctionStorage;
    template<typename T, typename Return, typename ...Args>
    struct _TypedFunctionStorage<T, Return(Args...)> : public _FunctionStorageCaller<Return, Args...> {
        _TypedFunctionStorage(const T& fun)
            : function(fun) {}

        virtual Return Call(Args&&...args) override { return function(static_cast<Args&&>(args)...); };
        T function;
    };

    template<typename, typename> struct _MemberFunctionStorage;
    template<typename T, typename Return, typename ...Args>
    struct _MemberFunctionStorage<T, Return(Args...)> : public _FunctionStorageCaller<Return, Args...> {
        _MemberFunctionStorage(Return(T::* function)(Args...), T& obj)
            : obj(obj), function(function) {}

        virtual Return Call(Args&&...args) override { return (obj.*function)(static_cast<Args&&>(args)...); };
        T& obj;
        Return(T::* function)(Args...);
    };

    template<typename T>
    struct Function;

    // Main partial application Function class
    template<typename Return, typename ...Args>
    struct Function<Return(Args...)> {
        using FunType = Return(*)(Args...);

        Function() {}

        template<typename T>
        Function(Return(T::* a)(Args...), T& t)
            : m_Storage(new _MemberFunctionStorage<T, Return(Args...)>{ a, t }) 
        {}

        // Capturing lambda constructor
        template<typename T, typename = typename LambdaSignature<T>::type>
        Function(const T& t)
            : m_Storage(new _TypedFunctionStorage<T, typename LambdaSignature<T>::type>{ t }) 
        {}

        // Function pointer constructor
        Function(FunType t)
            : m_Storage(new _TypedFunctionStorage<FunType, Return(Args...)>{ t })
        {}

        Function(const Function<Return(Args...)>& f)
            : m_Storage(f.m_Storage)
        {
            if (m_Storage)
                m_Storage->m_RefCount++;
        }

        Function(Function<Return(Args...)>&& f) 
            : m_Storage(f.m_Storage)
        {
            f.m_Storage = nullptr;
        }

        Function& operator=(const Function<Return(Args...)>& f) 
        {
            Clean();
            m_Storage = f.m_Storage;
            if (m_Storage)
                m_Storage->m_RefCount++;
            return *this;
        }

        Function& operator=(Function<Return(Args...)>&& f) 
        {
            Clean();
            m_Storage = f.m_Storage;
            f.m_Storage = nullptr;
            return *this;
        }

        ~Function() 
        {
            Clean();
        }

        void Clean()
        {
            if (!m_Storage)
                return;

            m_Storage->m_RefCount--;
            if (m_Storage->m_RefCount == 0)
                delete m_Storage, m_Storage = nullptr;
        }

        inline Return operator()(Args ...args) const 
        {
            return m_Storage->Call(static_cast<Args&&>(args)...);
        }

        inline operator bool() const { return m_Storage; }

        _FunctionStorageCaller<Return, Args...>* m_Storage = nullptr;
    };

    // Function constructor deduction guide for function pointers
    template <class Ret, class ...Args>
    Function(Ret(Args...))->Function<Ret(Args...)>;

    // Function constructor deduction guide for member functions
    template <class Ret, class T, class ...Args>
    Function(Ret(T::* a)(Args...), T&)->Function<Ret(Args...)>;

    // Function constructor deduction guide for lambdas
    template <class _Fx>
    Function(_Fx)->Function<typename std::_Deduce_signature<_Fx>::type>;


}

namespace std
{
    template<> struct tuple_size<GuiCode::Color> : std::integral_constant<size_t, 4> { };
    
    template<> struct tuple_element<0, GuiCode::Color> { using type = float; };
    template<> struct tuple_element<1, GuiCode::Color> { using type = float; };
    template<> struct tuple_element<2, GuiCode::Color> { using type = float; };
    template<> struct tuple_element<3, GuiCode::Color> { using type = float; };
}