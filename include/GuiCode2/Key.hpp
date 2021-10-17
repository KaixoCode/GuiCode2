#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/BasicEvents.hpp"

namespace GuiCode
{
    struct Mods
    {
        enum {
            Control = 0x00010000,
            RightControl = 0x01000000,
            LeftControl = 0x02000000,
            Shift = 0x00020000,
            RightShift = 0x04000000,
            LeftShift = 0x08000000,
            Alt = 0x00040000,
            LeftAlt = 0x10000000,
            RightAlt = 0x20000000,
            NumLock = 0x00080000,
            CapsLock = 0x00100000
        };
    };

    class Key
    {
    public:
        enum
        {
            Backspace = 0x8,
            Escape = 0x1B,
            Tab = 0x9,
            Delete = 0x2e,
            Enter = 0xd,
            Left = 0x25,
            Right = 0x27,
            Up = 0x26,
            Down = 0x28,
            Shift = 0x10,
            Control = 0x11,
            Alt = 0x12,
            F4 = 0x73,
            F11 = 0x7A
        };

        Key()
        {};

        Key(int v)
            : m_Value(v)
        {}

        Key(const KeyType& v)
            : m_Value((v.key << 16) | v.mod)
        {}

        Key(const KeyPress& v)
            : m_Value((v.keycode << 16) | v.mod)
        {}

        Key(const KeyRelease& v)
            : m_Value((v.keycode << 16) | v.mod)
        {}

        bool operator==(int a) const { return m_Value == a; }
        bool operator!=(int a) const { return m_Value != a; }
        bool operator <(int a) const { return m_Value < a; }
        bool operator >(int a) const { return m_Value > a; }
        bool operator<=(int a) const { return m_Value <= a; }
        bool operator>=(int a) const { return m_Value >= a; }
        operator int() const { return m_Value; }

        inline const std::string& ToString() const
        {
            static std::string _ctrl = "Ctrl+";
            static std::string _shift = "Shift+";
            static std::string _alt = "Alt+";
            static std::unordered_map<int, std::string> _keys;
            _keys.reserve(33);

            std::unordered_map<int, std::string>::iterator _it;
            if ((_it = _keys.find(m_Value)) == _keys.end())
            {
                int _value = m_Value;
                bool _c = _value & Mods::Control;
                bool _s = _value & Mods::Shift;
                bool _a = _value & Mods::Alt;
                int _l = (_value & 0x0000ffff);
                bool _f = _l >= 0x70 && _l <= 0x7B;

                std::string _str;
                _str.reserve(_c ? _ctrl.size() : 0 + _s ? _shift.size() : 0 + _a ? _alt.size() : 0 + _f ? 2 : 1);

                if (_c) _str += _ctrl;
                if (_s) _str += _shift;
                if (_a) _str += _alt;
                if (_f)
                    _str = _str + "F" + std::to_string(_l - 0x6F);
                if (_l == 188) _str += ',';
                else if (_l == ' ') _str += "SPACE";
                else if (!_f)
                    _str += static_cast<char>(_l);

                return _keys.emplace(m_Value, std::move(_str)).first->second;
            }
            else
            {
                return _it->second;
            }
        }

    private:
        int m_Value = 0;
    };

    static inline bool operator ==(const KeyPress& a, const Key b) { return (a.keycode | a.mod) == b; }
    static inline bool operator ==(const KeyPress&& a, const Key b) { return (a.keycode | a.mod) == b; }
    static inline bool operator ==(const KeyRelease& a, const Key b) { return (a.keycode | a.mod) == b; }
    static inline bool operator ==(const KeyRelease&& a, const Key b) { (a.keycode | a.mod) == b; }
    static inline bool operator ==(const KeyType& a, const Key b) { return (a.key | a.mod) == b; }
    static inline bool operator ==(const KeyType&& a, const Key b) { return (a.key | a.mod) == b; }
    static inline bool operator ==(const Key b, const KeyPress& a) { return (a.keycode | a.mod) == b; }
    static inline bool operator ==(const Key b, const KeyPress&& a) { return (a.keycode | a.mod) == b; }
    static inline bool operator ==(const Key b, const KeyRelease& a) { return (a.keycode | a.mod) == b; }
    static inline bool operator ==(const Key b, const KeyRelease&& a) { return (a.keycode | a.mod) == b; }
    static inline bool operator ==(const Key b, const KeyType& a) { return (a.key | a.mod) == b; }
    static inline bool operator ==(const Key b, const KeyType&& a) { return (a.key | a.mod) == b; }
}