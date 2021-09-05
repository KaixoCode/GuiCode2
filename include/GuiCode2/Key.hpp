#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/BasicEvents.hpp"

namespace GuiCode
{
    class Key
    {
    public:
        /**
         * Type of key combo
         */
        enum Combo
        {
            NONE = 0,
            CTRL_A = ('A' << 8) | EventMods::Control,
            CTRL_B = ('B' << 8) | EventMods::Control,
            CTRL_C = ('C' << 8) | EventMods::Control,
            CTRL_D = ('D' << 8) | EventMods::Control,
            CTRL_E = ('E' << 8) | EventMods::Control,
            CTRL_F = ('F' << 8) | EventMods::Control,
            CTRL_G = ('G' << 8) | EventMods::Control,
            CTRL_H = ('H' << 8) | EventMods::Control,
            CTRL_I = ('I' << 8) | EventMods::Control,
            CTRL_J = ('J' << 8) | EventMods::Control,
            CTRL_K = ('K' << 8) | EventMods::Control,
            CTRL_L = ('L' << 8) | EventMods::Control,
            CTRL_M = ('M' << 8) | EventMods::Control,
            CTRL_N = ('N' << 8) | EventMods::Control,
            CTRL_O = ('O' << 8) | EventMods::Control,
            CTRL_P = ('P' << 8) | EventMods::Control,
            CTRL_Q = ('Q' << 8) | EventMods::Control,
            CTRL_R = ('R' << 8) | EventMods::Control,
            CTRL_S = ('S' << 8) | EventMods::Control,
            CTRL_T = ('T' << 8) | EventMods::Control,
            CTRL_U = ('U' << 8) | EventMods::Control,
            CTRL_V = ('V' << 8) | EventMods::Control,
            CTRL_W = ('W' << 8) | EventMods::Control,
            CTRL_X = ('X' << 8) | EventMods::Control,
            CTRL_Y = ('Y' << 8) | EventMods::Control,
            CTRL_Z = ('Z' << 8) | EventMods::Control,
            CTRL_SPACE = (' ' << 8) | EventMods::Control,
            CTRL_COMMA = (188 << 8) | EventMods::Control, // TODO: This one doesnt work
            CTRL_SHIFT_S = ('S' << 8) | EventMods::Control | EventMods::Shift,
            CTRL_SHIFT_E = ('E' << 8) | EventMods::Control | EventMods::Shift,
            CTRL_SHIFT_W = ('W' << 8) | EventMods::Control | EventMods::Shift,
            ALT_F4 = (0x73 << 8) | EventMods::Alt,
            CTRL_F4 = (0x73 << 8) | EventMods::Control,
            SHIFT_TAB = (0x9 << 8) | EventMods::Shift,
            F11 = (0x7A << 8)
        };

        enum Value
        {
            BACKSPACE = 0x8,
            ESC = 0x1B,
            TAB = 0x9,
            DEL = 0x2e,
            ENTER = 0xd,
            LEFT = 0x25,
            RIGHT = 0x27,
            UP = 0x26,
            DOWN = 0x28,
            SHIFT = 0x10,
            CONTROL = 0x11,
            ALT = 0x12
        };

        /**
         * Constructor
         */
        Key()
        {};

        /**
         * Constructor
         * @param v value
         */
        Key(Key::Combo v)
            : m_Value(v)
        {}

        Key(int v)
            : m_Value(v)
        {}

        /**
         * Constructor
         * v keytyped event, automatically converted to a key combo
         */
        Key(KeyType v)
            : m_Value((v.key << 8) | v.mod)
        {}

        /**
         * Constructor
         * v keypressed event, automatically converted to a key combo
         */
        Key(KeyPress v)
            : m_Value((v.keycode << 8) | v.mod)
        {}

        /**
         * Constructor
         * v keyreleased event, automatically converted to a key combo
         */
        Key(KeyRelease v)
            : m_Value((v.keycode << 8) | v.mod)
        {}

        bool operator==(Key::Combo a) const { return m_Value == a; }
        bool operator!=(Key::Combo a) const { return m_Value != a; }
        bool operator<(Key::Combo a)  const { return m_Value < a; }
        bool operator>(Key::Combo a)  const { return m_Value > a; }
        bool operator<=(Key::Combo a) const { return m_Value <= a; }
        bool operator>=(Key::Combo a) const { return m_Value >= a; }
        operator int()       const { return static_cast<int>(m_Value); }
        //operator Combo()              const { return m_Value; }

        /**
         * Convert the key combo to a string.
         */
        inline const std::string& ToString()
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
                int _l = (_value & 0xffffff00) >> 8;
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
        int m_Value;
    };
}