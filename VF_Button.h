// Arduino Button Library
// https://github.com/FedoretsV/VF_Button
// Copyright (C) 2022 by Vitaliy Fedorets and licensed under
// GNU GPL v3.0, https://www.gnu.org/licenses/gpl.html

#ifndef VF_BUTTON_H_INCLUDED
#define VF_BUTTON_H_INCLUDED

#include <Arduino.h>

class Button
{
    public:
        // Button(pin, dbTime) создание экземпляра объекта Button
        //
        // Обязательный параметр:
        // pin      Arduino pin, к которому подключена кнопка
        //
        // Необязательный параметр:
        // dbTime   Время задержки для подавления "дребезга", в миллисекундах (default 50ms)
        
        Button(uint8_t pin, uint32_t dbTime=50)
            : m_pin(pin), m_dbTime(dbTime)) {}

        // Инициализация объекта Button и контакт, к которому он подключен.
        void begin();

        // Возвращает текущее состояние кнопки с отключенным дребезгом,  
        // true при нажатии, false при отжатии.  
        // Функция должна вызываться постоянно.
        bool read();

        // Возвращает true, если кнопка была нажата при последнем вызове read().
        // Не вызывает чтение кнопки.
        bool isPressed();

        // Возвращает true, если кнопка была отжата при последнем вызове read().
        // Не вызывает чтение кнопки.
        bool isReleased();

        // Returns true if the button state at the last call to read() was pressed,
        // and this was a change since the previous read.
        bool wasPressed();

        // Returns true if the button state at the last call to read() was released,
        // and this was a change since the previous read.
        bool wasReleased();

        // Returns true if the button state at the last call to read() was pressed,
        // and has been in that state for at least the given number of milliseconds.
        bool pressedFor(uint32_t ms);

        // Returns true if the button state at the last call to read() was released,
        // and has been in that state for at least the given number of milliseconds.
        bool releasedFor(uint32_t ms);

        // Returns the time in milliseconds (from millis) that the button last
        // changed state.
        uint32_t lastChange();

    private:
        uint8_t m_pin;          // arduino pin number connected to button
        uint32_t m_dbTime;      // debounce time (ms)
        bool m_puEnable;        // internal pullup resistor enabled
        bool m_invert;          // if true, interpret logic low as pressed, else interpret logic high as pressed
        bool m_state;           // current button state, true=pressed
        bool m_lastState;       // previous button state
        bool m_changed;         // state changed since last read
        uint32_t m_time;        // time of current state (ms from millis)
        uint32_t m_lastChange;  // time of last state change (ms)
};

// a derived class for a "push-on, push-off" (toggle) type button.
// initial state can be given, default is off (false).
class ToggleButton : public Button
{
    public:
    
        // constructor is similar to Button, but includes the initial state for the toggle.
        ToggleButton(uint8_t pin, bool initialState=false, uint32_t dbTime=25, uint8_t puEnable=true, uint8_t invert=true)
            : Button(pin, dbTime, puEnable, invert), m_toggleState(initialState) {}

        // read the button and return its state.
        // should be called frequently.
        bool read()
        {
            Button::read();
            if (wasPressed())
            {
                m_toggleState = !m_toggleState;
                m_changed = true;
            }
            else
            {
                m_changed = false;
            }
            return m_toggleState;
        }

        // has the state changed?
        bool changed() {return m_changed;}

        // return the current state
        bool toggleState() {return m_toggleState;}

    private:
        bool m_toggleState;
        bool m_changed;
};
#endif
