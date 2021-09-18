#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/BasicEvents.hpp"

namespace GuiCode
{
    class Key
    {
    public:

        enum Combo
        {
            NONE = 0,
            CTRL_A = ('A' << 16) | EventMods::Control, CTRL_SHIFT_A = ('A' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_A = ('A' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_B = ('B' << 16) | EventMods::Control, CTRL_SHIFT_B = ('B' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_B = ('B' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_C = ('C' << 16) | EventMods::Control, CTRL_SHIFT_C = ('C' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_C = ('C' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_D = ('D' << 16) | EventMods::Control, CTRL_SHIFT_D = ('D' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_D = ('D' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_E = ('E' << 16) | EventMods::Control, CTRL_SHIFT_E = ('E' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_E = ('E' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_F = ('F' << 16) | EventMods::Control, CTRL_SHIFT_F = ('F' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_F = ('F' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_G = ('G' << 16) | EventMods::Control, CTRL_SHIFT_G = ('G' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_G = ('G' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_H = ('H' << 16) | EventMods::Control, CTRL_SHIFT_H = ('H' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_H = ('H' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_I = ('I' << 16) | EventMods::Control, CTRL_SHIFT_I = ('I' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_I = ('I' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_J = ('J' << 16) | EventMods::Control, CTRL_SHIFT_J = ('J' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_J = ('J' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_K = ('K' << 16) | EventMods::Control, CTRL_SHIFT_K = ('K' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_K = ('K' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_L = ('L' << 16) | EventMods::Control, CTRL_SHIFT_L = ('L' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_L = ('L' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_M = ('M' << 16) | EventMods::Control, CTRL_SHIFT_M = ('M' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_M = ('M' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_N = ('N' << 16) | EventMods::Control, CTRL_SHIFT_N = ('N' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_N = ('N' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_O = ('O' << 16) | EventMods::Control, CTRL_SHIFT_O = ('O' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_O = ('O' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_P = ('P' << 16) | EventMods::Control, CTRL_SHIFT_P = ('P' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_P = ('P' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_Q = ('Q' << 16) | EventMods::Control, CTRL_SHIFT_Q = ('Q' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_Q = ('Q' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_R = ('R' << 16) | EventMods::Control, CTRL_SHIFT_R = ('R' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_R = ('R' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_S = ('S' << 16) | EventMods::Control, CTRL_SHIFT_S = ('S' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_S = ('S' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_T = ('T' << 16) | EventMods::Control, CTRL_SHIFT_T = ('T' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_T = ('T' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_U = ('U' << 16) | EventMods::Control, CTRL_SHIFT_U = ('U' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_U = ('U' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_V = ('V' << 16) | EventMods::Control, CTRL_SHIFT_V = ('V' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_V = ('V' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_W = ('W' << 16) | EventMods::Control, CTRL_SHIFT_W = ('W' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_W = ('W' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_X = ('X' << 16) | EventMods::Control, CTRL_SHIFT_X = ('X' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_X = ('X' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_Y = ('Y' << 16) | EventMods::Control, CTRL_SHIFT_Y = ('Y' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_Y = ('Y' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_Z = ('Z' << 16) | EventMods::Control, CTRL_SHIFT_Z = ('Z' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_Z = ('Z' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_1 = ('1' << 16) | EventMods::Control, CTRL_SHIFT_1 = ('1' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_1 = ('1' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_2 = ('2' << 16) | EventMods::Control, CTRL_SHIFT_2 = ('2' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_2 = ('2' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_3 = ('3' << 16) | EventMods::Control, CTRL_SHIFT_3 = ('3' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_3 = ('3' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_4 = ('4' << 16) | EventMods::Control, CTRL_SHIFT_4 = ('4' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_4 = ('4' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_5 = ('5' << 16) | EventMods::Control, CTRL_SHIFT_5 = ('5' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_5 = ('5' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_6 = ('6' << 16) | EventMods::Control, CTRL_SHIFT_6 = ('6' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_6 = ('6' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_7 = ('7' << 16) | EventMods::Control, CTRL_SHIFT_7 = ('7' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_7 = ('7' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_8 = ('8' << 16) | EventMods::Control, CTRL_SHIFT_8 = ('8' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_8 = ('8' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_9 = ('9' << 16) | EventMods::Control, CTRL_SHIFT_9 = ('9' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_9 = ('9' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_0 = ('0' << 16) | EventMods::Control, CTRL_SHIFT_0 = ('0' << 16) | EventMods::Control | EventMods::Shift, CTRL_ALT_0 = ('0' << 16) | EventMods::Control | EventMods::Alt,
            CTRL_SPACE = (' ' << 16) | EventMods::Control,
            CTRL_COMMA = (188 << 16) | EventMods::Control,
            ALT_F4     = (0x73 << 16) | EventMods::Alt,
            CTRL_F4    = (0x73 << 16) | EventMods::Control,
            SHIFT_TAB  = (0x9 << 16) | EventMods::Shift,
            F11        = (0x7A << 16)
        };

        enum Value
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
            Alt = 0x12
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

        bool operator==(Key::Combo a) const { return m_Value == a; }
        bool operator!=(Key::Combo a) const { return m_Value != a; }
        bool operator <(Key::Combo a) const { return m_Value < a; }
        bool operator >(Key::Combo a) const { return m_Value > a; }
        bool operator<=(Key::Combo a) const { return m_Value <= a; }
        bool operator>=(Key::Combo a) const { return m_Value >= a; }
        operator int()       const { return static_cast<int>(m_Value); }

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
                int _value = static_cast<int>(m_Value);
                bool _c = _value & EventMods::Control;
                bool _s = _value & EventMods::Shift;
                bool _a = _value & EventMods::Alt;
                int _l = (_value & 0xffffff00) >> 16;
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

    static inline bool operator ==(const KeyPress& a, const Key b) { return (a.keycode == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const KeyPress&& a, const Key b) { return (a.keycode == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const KeyRelease& a, const Key b) { return (a.keycode == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const KeyRelease&& a, const Key b) { return (a.keycode == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const KeyType& a, const Key b) { return (a.key == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const KeyType&& a, const Key b) { return (a.key == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const Key b, const KeyPress& a) { return (a.keycode == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const Key b, const KeyPress&& a) { return (a.keycode == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const Key b, const KeyRelease& a) { return (a.keycode == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const Key b, const KeyRelease&& a) { return (a.keycode == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const Key b, const KeyType& a) { return (a.key == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
    static inline bool operator ==(const Key b, const KeyType&& a) { return (a.key == (b >> 16)) && ((a.mod & 0xFF) == (b & 0xFF)); }
}