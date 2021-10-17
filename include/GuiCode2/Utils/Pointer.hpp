#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Utils/Utils.hpp"

namespace GuiCode
{
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
        struct Object
        {
            Object(T* ptr, size_t r, bool del)
                : data(ptr), refs(r), del(del)
            {}

            Object(const Object& other)
                : data(other.data), refs(other.refs)
            {}

            Object(Object&& other)
                : data(other.data), refs(other.refs)
            {}

            ~Object()
            {
                if (del && data)
                    delete data;
            }

            T* data;
            size_t refs = 1;
            bool del = false;
        };
    public:
        Pointer()
            : m_Data(new Object{ nullptr, 1, false })
        {}

        template<typename Type> requires (std::derived_from<Type, T> || std::derived_from<T, Type>)
            Pointer(Type& c)
            : m_Data(new Object{ dynamic_cast<T*>(&c), 1, false })
        {}

        template<typename Type> requires (std::derived_from<Type, T> || std::derived_from<T, Type>)
            Pointer(Type* c)
            : m_Data(new Object{ dynamic_cast<T*>(c), 1, true })
        {}

        template<typename Type> requires (std::derived_from<Type, T> || std::derived_from<T, Type>)
            Pointer(Pointer<Type>&& c)
            : m_Data(reinterpret_cast<Object*>(c.m_Data))
        {
            c.Invalidate();
        }

        template<typename Type> requires (std::derived_from<Type, T> || std::derived_from<T, Type>)
            Pointer(const Pointer<Type>& c)
            : m_Data(reinterpret_cast<Object*>(c.m_Data))
        {
            c.m_Data->refs++;
        }

        Pointer(Pointer&& c)
            : m_Data(c.m_Data)
        {
            c.Invalidate();
        }

        Pointer(const Pointer& c)
            : m_Data(c.m_Data)
        {
            c.m_Data->refs++;
        }

        Pointer& operator=(const Pointer& c)
        {
            if (m_Data == c.m_Data)
                return *this;

            if (m_Data)
            {
                m_Data->refs--;
                if (m_Data->refs == 0)
                    delete m_Data;
            }

            m_Data = c.m_Data;
            m_Data->refs++;

            return *this;
        }

        template<typename Type> requires (std::derived_from<Type, T> || std::derived_from<T, Type>)
            Pointer& operator=(Pointer<Type>&& c)
        {
            if (m_Data)
            {
                m_Data->refs--;
                if (m_Data->refs == 0)
                    delete m_Data;
            }

            m_Data = reinterpret_cast<Object*>(c.m_Data);
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

        template<std::derived_from<T> Type>
        Type& Get() { return *dynamic_cast<Type*>(m_Data->data); }

        template<std::derived_from<T> Type>
        const Type& Get() const { return *dynamic_cast<Type*>(m_Data->data); }

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

            m_Data->refs--;
            if (m_Data->refs == 0)
                delete m_Data;
        }

        void Invalidate()
        {
            m_Data = nullptr;
        }

    private:
        Object* m_Data = nullptr;

        template<typename T>
        friend class Pointer;
    };
}