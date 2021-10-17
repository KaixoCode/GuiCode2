#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Utils/Utils.hpp"
#include "GuiCode2/Utils/Color.hpp"

namespace GuiCode
{
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
        virtual std::any CallWithString(std::any*, int s, std::string_view&) = 0;

        size_t m_RefCount = 0;
    };

    // Binder without type info about what kind of functor it contains, only return and arg types.
    template<typename Return, typename ...Args>
    struct _FunctionStorageCaller : public _FunctionStorageBase {
        virtual Return Call(Args&&...) = 0;
        virtual std::any CallWithString(std::any* data, int s, std::string_view& view) override
        {
            //if constexpr ((std::is_copy_assignable_v<Args> && ...))
            //{
            if constexpr (std::is_same_v<Return, void>)
            {
                CallSeq(data, // Given arguments from code
                    s,        // Amount of given arguments from code
                    view,     // Remaining arguments in a string_view
                    std::make_index_sequence<sizeof...(Args) + 1>{});
                return nullptr;
            }
            else
            {
                return CallSeq(data, // Given arguments from code
                    s,        // Amount of given arguments from code
                    view,     // Remaining arguments in a string_view
                    std::make_index_sequence<sizeof...(Args) + 1>{});
            }
            //}
            return nullptr;
        }

    private:
        template<size_t ...Is>
        Return CallSeq(std::any* data, int s, std::string_view& view, std::index_sequence<Is...>)
        {
            if constexpr (std::is_same_v<void, Return>)
                (((Is == s) // If index is amount that needs to be dropped
                    && (CallSeq2(data,        // Send our type erased array of void*
                        Parsers<DropN<Is, std::tuple<std::decay_t<Args>...>>::Type>::Parse(view), // Parse the last couple from Args from string
                        std::make_index_sequence<Is>{},                   // Create sequence for the parsed arguments
                        std::make_index_sequence<sizeof...(Args) - Is>{}) // Create sequence for the void* arguments
                        , false)), ...
                    );
            else
            {
                Return _value;
                (((Is == s) // If index is amount that needs to be dropped
                    && (_value = CallSeq2(data,        // Send our type erased array of void*
                        Parsers<DropN<Is, std::tuple<std::decay_t<Args>...>>::Type>::Parse(view), // Parse the last couple from Args from string
                        std::make_index_sequence<Is>{},                   // Create sequence for the parsed arguments
                        std::make_index_sequence<sizeof...(Args) - Is>{}) // Create sequence for the void* arguments
                        , false)), ...
                    );
                return _value;
            }
        }

        template<typename ...Tys, size_t ...Is, size_t ...Ts>
        Return CallSeq2(std::any* data, std::tuple<Tys...>&& tuple, std::index_sequence<Is...>, std::index_sequence<Ts...>)
        {
            if constexpr (sizeof...(Ts) == sizeof...(Tys))
                return Call(std::forward<NthTypeOf<Is, Args...>>((std::any_cast<NthTypeOf<Is, Args...>>(data[Is])))..., // Pack expand void* and cast them
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

        template<typename T>
        Function(const T& t)
            : m_Storage(new _TypedFunctionStorage<T, Return(Args...)>{ t })
        {}

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

    template <class Ret, class ...Args>
    Function(Ret(Args...))->Function<Ret(Args...)>;

    template <class Ret, class T, class ...Args>
    Function(Ret(T::* a)(Args...), T&)->Function<Ret(Args...)>;

    template <class _Fx>
    Function(_Fx)->Function<typename std::_Deduce_signature<_Fx>::type>;
}