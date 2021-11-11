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

        size_t m_RefCount = 0;
    };

    // Binder without type info about what kind of functor it contains, only return and arg types.
    template<typename Return, typename ...Args>
    struct _FunctionStorageCaller : public _FunctionStorageBase {
        virtual Return Call(Args&&...) = 0;
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