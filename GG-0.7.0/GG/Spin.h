// -*- C++ -*-
/* GG is a GUI for SDL and OpenGL.
   Copyright (C) 2003 T. Zachary Laine

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public License
   as published by the Free Software Foundation; either version 2.1
   of the License, or (at your option) any later version.
   
   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.
    
   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free
   Software Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
   02111-1307 USA

   If you do not wish to comply with the terms of the LGPL please
   contact the author as other terms are available for a fee.
    
   Zach Laine
   whatwasthataddress@hotmail.com */

/** \file Spin.h
    Contains the Spin class template, which provides a spin-box control that allows the user to select a value from a 
    range an arbitrary type (int, double, an enum, etc.). */

#ifndef _GG_Spin_h_
#define _GG_Spin_h_

#include <GG/Button.h>
#include <GG/DrawUtil.h>
#include <GG/Edit.h>
#include <GG/GUI.h>
#include <GG/StyleFactory.h>
#include <GG/WndEditor.h>
#include <GG/WndEvent.h>

#include <cmath>
#include <limits>


namespace GG {

// forward declaration of helper functions and classes
namespace spin_details {
    template <class T> T mod(T, T);
    template <class T> T div(T, T);

    template <class T>
    struct SetValueAction : AttributeChangedAction<T>
    {
        SetValueAction(Spin<T>* spin) : m_spin(spin) {}
        void operator()(const T& value) {m_spin->SetValue(m_spin->Value());}
    private:
        Spin<T>* m_spin;
    };

    template <class T>
    struct SetMinValueAction : AttributeChangedAction<T>
    {
        SetMinValueAction(Spin<T>* spin) : m_spin(spin) {}
        void operator()(const T& value) {m_spin->SetValue(value);}
    private:
        Spin<T>* m_spin;
    };

    template <class T>
    struct SetMaxValueAction : AttributeChangedAction<T>
    {
        SetMaxValueAction(Spin<T>* spin) : m_spin(spin) {}
        void operator()(const T& value) {m_spin->SetValue(value);}
    private:
        Spin<T>* m_spin;
    };

    template <class T>
    struct SetButtonWidthAction : AttributeChangedAction<int>
    {
        SetButtonWidthAction(Spin<T>* spin) : m_spin(spin) {}
        void operator()(const int& width) {m_spin->SetButtonWidth(width);}
    private:
        Spin<T>* m_spin;
    };
}


/** a spin box control.  This control class is templated so that arbitrary data types can be used with Spin.  All the
    built-in numeric types are supported by the code here.  If you want to use some other type, such as an enum type,
    you need to define operator+(), operator-(), and template specializations of spin_details::mod() and
    spin_details::div().  Spin controls are optionally directly editable by the user.  When the user inputs a value that
    is not valid for the Spin's parameters (not on a step boundary, or outside the allowed range), the input gets locked
    to the nearest valid value.  The user is responsible for selecting a min, max, and step size that make sense.  For
    instance, min = 0, max = 4, step = 3 may produce odd results if the user increments all the way to the top, then
    back down, to produce the sequence 0, 3, 4, 1, 0.  To avoid this, choose the values so that (max - min) mod step ==
    0.  It is possible to provide custom buttons for a Spin to use; if you choose to add custom buttons, make sure they
    look alright at arbitrary sizes, and note that Spin buttons are always H wide by H/2 tall, where H is the height of
    the Spin, less the thickness of the Spin's border.  Note that if you want Spin controls to be automatically
    serialized in your application, you need to export each instantiation of Spin<> yourself (e.g. Spin<int> or
    Spin<double>).  See the boost serialization documentation for details. */
template <class T>
class Spin : public Control
{
public:
    /** \name Signal Types */ ///@{
    typedef typename boost::signal<void (T)> ValueChangedSignalType;  ///< emitted whenever the value of the Spin has changed
    //@}

    /** \name Slot Types */ ///@{
    typedef typename ValueChangedSignalType::slot_type ValueChangedSlotType;   ///< type of functor(s) invoked on a ValueChangedSignalType
    //@}

    /** \name Structors */ ///@{
    /** ctor that does not required height. Height is determined from the font and point size used.*/
    Spin(int x, int y, int w, T value, T step, T min, T max, bool edits, const boost::shared_ptr<Font>& font, Clr color, 
         Clr text_color = CLR_BLACK, Clr interior = CLR_ZERO, Flags<WndFlag> flags = CLICKABLE);

    ~Spin(); // dtor
    //@}

    /** \name Accessors */ ///@{
    virtual Pt MinUsableSize() const;

    T     Value() const;              ///< returns the current value of the control's text
    T     StepSize() const;           ///< returns the step size of the control
    T     MinValue() const;           ///< returns the minimum value of the control
    T     MaxValue() const;           ///< returns the maximum value of the control
    bool  Editable() const;           ///< returns true if the spinbox can have its value typed in directly

    int   ButtonWidth() const;        ///< returns the width used for the up and down buttons

    Clr   TextColor() const;          ///< returns the text color
    Clr   InteriorColor() const;      ///< returns the the interior color of the control
    Clr   HiliteColor() const;        ///< returns the color used to render hiliting around selected text
    Clr   SelectedTextColor() const;  ///< returns the color used to render selected text

    mutable ValueChangedSignalType ValueChangedSignal; ///< the value changed signal object for this DynamicGraphic
    //@}

    /** \name Mutators */ ///@{
    virtual void Render();
    virtual void KeyPress(Key key, Flags<ModKey> mod_keys);
    virtual void MouseWheel(const Pt& pt, int move, Flags<ModKey> mod_keys);

    virtual void SizeMove(const Pt& ul, const Pt& lr);

    virtual void Disable(bool b = true);
    virtual void SetColor(Clr c);

    void Incr();  ///< increments the value of the control's text by StepSize(), up to at most MaxValue()
    void Decr();  ///< decrements the value of the control's text by StepSize(), down to at least MinValue()

    /** sets the value of the control's text to \a value, locked to the range [MinValue(), MaxValue()]*/
    void SetValue(T value);

    void SetStepSize(T step);   ///< sets the step size of the control to \a step
    void SetMinValue(T value);  ///< sets the minimum value of the control to \a value
    void SetMaxValue(T value);  ///< sets the maximum value of the control to \a value

    /** turns on or off the mode that allows the user to edit the value in the spinbox directly. */
    void AllowEdits(bool b = true);

    void SetButtonWidth(int width);    ///< sets the width used for the up and down buttons

    void SetTextColor(Clr c);          ///< sets the text color
    void SetInteriorColor(Clr c);      ///< sets the interior color of the control
    void SetHiliteColor(Clr c);        ///< sets the color used to render hiliting around selected text
    void SetSelectedTextColor(Clr c);  ///< sets the color used to render selected text   

    virtual void DefineAttributes(WndEditor* editor);
    //@}

protected:
    typedef T ValueType;

    enum {BORDER_THICK = 2, PIXEL_MARGIN = 5};

    /** \name Structors */ ///@{
    Spin(); ///< default ctor
    //@}

    /** \name Accessors */ ///@{
    Button*     UpButton() const;   ///< returns a pointer to the Button control used as this control's up button
    Button*     DownButton() const; ///< returns a pointer to the Button control used as this control's down button
    Edit*       GetEdit() const;    ///< returns a pointer to the Edit control used to render this control's text and accept keyboard input
    //@}

    /** \name Mutators */ ///@{
    virtual bool EventFilter(Wnd* w, const WndEvent& event);
    //@}

private:
    void ConnectSignals();
    void Init(const boost::shared_ptr<Font>& font, Clr color, Clr text_color, Clr interior, Flags<WndFlag> flags);
    void ValueUpdated(const std::string& val_text);

    T          m_value;
    T          m_step_size;
    T          m_min_value;
    T          m_max_value;

    bool       m_editable;

    Edit*      m_edit;
    Button*    m_up_button;
    Button*    m_down_button;

    int        m_button_width;

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version);
};


// template implementations
template<class T>
Spin<T>::Spin() : 
    Control(),
    m_value(),
    m_step_size(),
    m_min_value(),
    m_max_value(),
    m_editable(false),
    m_edit(0),
    m_up_button(0),
    m_down_button(0),
    m_button_width(15)
{}

template<class T>
Spin<T>::Spin(int x, int y, int w, T value, T step, T min, T max, bool edits, const boost::shared_ptr<Font>& font, Clr color, 
              Clr text_color/* = CLR_BLACK*/, Clr interior/* = CLR_ZERO*/, Flags<WndFlag> flags/* = CLICKABLE*/) : 
    Control(x, y, w, font->Height() + 2 * PIXEL_MARGIN, flags),
    m_value(value),
    m_step_size(step),
    m_min_value(min),
    m_max_value(max),
    m_editable(edits),
    m_edit(0),
    m_up_button(0),
    m_down_button(0),
    m_button_width(15)
{
    Init(font, color, text_color, interior, flags);
}

template<class T>
Spin<T>::~Spin()
{}

template<class T>
Pt Spin<T>::MinUsableSize() const
{
    Pt edit_min = m_edit->MinUsableSize();
    Pt up_min = m_up_button->MinUsableSize();
    Pt down_min = m_down_button->MinUsableSize();
    return Pt(edit_min.x + std::max(up_min.x, down_min.x) + 2 * BORDER_THICK,
              std::max(up_min.y + down_min.y, edit_min.y) + 2 * BORDER_THICK);
}

template<class T>
T Spin<T>::Value() const
{
    return m_value;
}

template<class T>
T Spin<T>::StepSize() const
{
    return m_step_size;
}

template<class T>
T Spin<T>::MinValue() const
{
    return m_min_value;
}

template<class T>
T Spin<T>::MaxValue() const
{
    return m_max_value;
}

template<class T>
bool Spin<T>::Editable() const 
{
    return m_editable;
}

template<class T>
int Spin<T>::ButtonWidth() const
{
    return m_button_width;
}

template<class T>
Clr Spin<T>::TextColor() const
{
    return m_edit->TextColor();
}

template<class T>
Clr Spin<T>::InteriorColor() const
{
    return m_edit->InteriorColor();
}

template<class T>
Clr Spin<T>::HiliteColor() const
{
    return m_edit->HiliteColor();
}

template<class T>
Clr Spin<T>::SelectedTextColor() const
{
    return m_edit->SelectedTextColor();
}

template<class T>
void Spin<T>::Render()
{
    Clr color_to_use = Disabled() ? DisabledColor(Color()) : Color();
    Clr int_color_to_use = Disabled() ? DisabledColor(InteriorColor()) : InteriorColor();
    Pt ul = UpperLeft(), lr = LowerRight();
    BeveledRectangle(ul.x, ul.y, lr.x, lr.y, int_color_to_use, color_to_use, false, BORDER_THICK);
}

template<class T>
void Spin<T>::KeyPress(Key key, Flags<ModKey> mod_keys)
{
    switch (key) {
    case GGK_HOME:
        SetValue(m_min_value);
        break;
    case GGK_END:
        SetValue(m_max_value);
        break;
    case GGK_PAGEUP:
    case GGK_UP:
    case GGK_PLUS:
    case GGK_KP_PLUS:
        Incr();
        break;
    case GGK_PAGEDOWN:
    case GGK_DOWN:
    case GGK_MINUS:
    case GGK_KP_MINUS:
        Decr();
        break;
    default:
        break;
    }
}

template<class T>
void Spin<T>::MouseWheel(const Pt& pt, int move, Flags<ModKey> mod_keys)
{
    for (int i = 0; i < move; ++i) {
        Incr();
    }
    for (int i = 0; i < -move; ++i) {
        Decr();
    }
}

template<class T>
void Spin<T>::SizeMove(const Pt& ul, const Pt& lr)
{
    Wnd::SizeMove(ul, lr);
    const int BUTTON_X_POS = Width() - m_button_width - BORDER_THICK;
    const int BUTTONS_HEIGHT = Height() - 2 * BORDER_THICK; // height of *both* buttons
    m_edit->SizeMove(Pt(0, 0), Pt(Width() - m_button_width, Height()));
    m_up_button->SizeMove(Pt(BUTTON_X_POS, BORDER_THICK),
                          Pt(BUTTON_X_POS + m_button_width, BORDER_THICK + BUTTONS_HEIGHT / 2));
    m_down_button->SizeMove(Pt(BUTTON_X_POS, BORDER_THICK + BUTTONS_HEIGHT / 2),
                            Pt(BUTTON_X_POS + m_button_width, BORDER_THICK + BUTTONS_HEIGHT));
}

template<class T>
void Spin<T>::Disable(bool b/* = true*/)
{
    Control::Disable(b);
    m_edit->Disable(b);
    m_up_button->Disable(b);
    m_down_button->Disable(b);
}

template<class T>
void Spin<T>::SetColor(Clr c)
{
    Control::SetColor(c);
    m_up_button->SetColor(c);
    m_down_button->SetColor(c);
}

template<class T>
void Spin<T>::Incr()
{
    SetValue(m_value + m_step_size);
}

template<class T>
void Spin<T>::Decr()
{
    SetValue(m_value - m_step_size);
}

template<class T>
void Spin<T>::SetValue(T value)
{
    T old_value = m_value;
    if (value < m_min_value) {
        m_value = m_min_value;
    } else if (m_max_value < value) {
        m_value = m_max_value;
    } else {
        // if the value supplied does not equal a valid value
        if (std::abs(spin_details::mod((value - m_min_value), m_step_size)) > std::numeric_limits<T>::epsilon()) {
            // find nearest valid value to the one supplied
            T closest_below = spin_details::div((value - m_min_value), m_step_size) * m_step_size + m_min_value;
            T closest_above = closest_below + m_step_size;
            m_value = ((value - closest_below) < (closest_above - value) ? closest_below : closest_above);
        } else {
            m_value = value;
        }
    }
    *m_edit << m_value;
    if (m_value != old_value)
        ValueChangedSignal(m_value);
}

template<class T>
void Spin<T>::SetStepSize(T step)
{
    m_step_size = step;
    SetValue(m_value);
}

template<class T>
void Spin<T>::SetMinValue(T value)
{
    m_min_value = value;
    if (m_value < m_min_value)
        SetValue(m_min_value);
}

template<class T>
void Spin<T>::SetMaxValue(T value)
{
    m_max_value = value;
    if (m_max_value < m_value)
        SetValue(m_max_value);
}

template<class T>
void Spin<T>::SetTextColor(Clr c)
{
    m_edit->SetTextColor(c);
}

template<class T>
void Spin<T>::SetButtonWidth(int width)
{
    if (1 <= width) {
        if (Width() - 2 * BORDER_THICK - 1 < width)
            width = Width() - 2 * BORDER_THICK - 1;
        m_button_width = width;
        SizeMove(RelativeUpperLeft(), RelativeLowerRight());
    }
}

template<class T>
void Spin<T>::SetInteriorColor(Clr c)
{
    m_edit->SetInteriorColor(c);
}

template<class T>
void Spin<T>::SetHiliteColor(Clr c)
{
    m_edit->SetHiliteColor(c);
}

template<class T>
void Spin<T>::SetSelectedTextColor(Clr c)
{
    m_edit->SetSelectedTextColor(c);
}

template<class T>
void Spin<T>::DefineAttributes(WndEditor* editor)
{
    if (!editor)
        return;
    Control::DefineAttributes(editor);
    if (boost::is_same<T, int>::value)
        editor->Label("Spin<int>");
    else if (boost::is_same<T, double>::value)
        editor->Label("Spin<double>");
    else
        editor->Label("Spin<T>");
    boost::shared_ptr<spin_details::SetValueAction<T> > set_value_action(new spin_details::SetValueAction<T>(this));
    editor->Attribute<T>("Value", m_value, set_value_action);
    editor->Attribute<T>("Step Size", m_step_size, set_value_action);
    boost::shared_ptr<spin_details::SetMinValueAction<T> > set_min_value_action(new spin_details::SetMinValueAction<T>(this));
    editor->Attribute<T>("Min Value", m_min_value, set_min_value_action);
    boost::shared_ptr<spin_details::SetMaxValueAction<T> > set_max_value_action(new spin_details::SetMaxValueAction<T>(this));
    editor->Attribute<T>("Max Value", m_max_value, set_max_value_action);
    editor->Attribute("Editable", m_editable);
    boost::shared_ptr<spin_details::SetButtonWidthAction<T> > set_button_width_action(new spin_details::SetButtonWidthAction<T>(this));
    editor->Attribute<int>("Button Width", m_button_width, set_button_width_action);
}

template<class T>
Button* Spin<T>::UpButton() const
{
    return m_up_button;
}

template<class T>
Button* Spin<T>::DownButton() const
{
    return m_down_button;
}

template<class T>
Edit* Spin<T>::GetEdit() const
{
    return m_edit;
}

template<class T>
bool Spin<T>::EventFilter(Wnd* w, const WndEvent& event)
{
    if (w == m_edit) {
        if (!m_editable && event.Type() == WndEvent::GainingFocus) {
            GUI::GetGUI()->SetFocusWnd(this);
            return true;
        } else {
            return !m_editable;
        }
    }
    return false;
}

template<class T>
void Spin<T>::ConnectSignals()
{
    Connect(m_edit->FocusUpdateSignal, &Spin::ValueUpdated, this);
    Connect(m_up_button->ClickedSignal, &Spin::Incr, this);
    Connect(m_down_button->ClickedSignal, &Spin::Decr, this);
}

template<class T>
void Spin<T>::Init(const boost::shared_ptr<Font>& font, Clr color, Clr text_color, Clr interior, Flags<WndFlag> flags)
{
    boost::shared_ptr<StyleFactory> style = GetStyleFactory();
    Control::SetColor(color);
    m_edit = style->NewSpinEdit(0, 0, 1, boost::lexical_cast<std::string>(m_value), font, CLR_ZERO, text_color, interior);
    boost::shared_ptr<Font> small_font = GUI::GetGUI()->GetFont(font->FontName(), static_cast<int>(font->PointSize() * 0.75));
    m_up_button = style->NewSpinIncrButton(0, 0, 1, 1, "+", small_font, color);
    m_down_button = style->NewSpinDecrButton(0, 0, 1, 1, "-", small_font, color);
    m_edit->InstallEventFilter(this);
    m_up_button->InstallEventFilter(this);
    m_down_button->InstallEventFilter(this);
    AttachChild(m_edit);
    AttachChild(m_up_button);
    AttachChild(m_down_button);
    ConnectSignals();
    SizeMove(UpperLeft(), LowerRight());
}

template<class T>
void Spin<T>::ValueUpdated(const std::string& val_text)
{
    T value;
    try {
        value = boost::lexical_cast<T>(val_text);
    } catch (boost::bad_lexical_cast) {
        SetValue(m_min_value);
        return;
    }
    SetValue(value);
}

template <class T>
template <class Archive>
void Spin<T>::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Control)
        & BOOST_SERIALIZATION_NVP(m_value)
        & BOOST_SERIALIZATION_NVP(m_step_size)
        & BOOST_SERIALIZATION_NVP(m_min_value)
        & BOOST_SERIALIZATION_NVP(m_max_value)
        & BOOST_SERIALIZATION_NVP(m_editable)
        & BOOST_SERIALIZATION_NVP(m_edit)
        & BOOST_SERIALIZATION_NVP(m_up_button)
        & BOOST_SERIALIZATION_NVP(m_down_button);

    if (Archive::is_loading::value)
        ConnectSignals();
}

namespace spin_details {
    // provides a typesafe mod function
    template <class T> inline 
    T mod (T dividend, T divisor) {return dividend % divisor;}

    // template specializations
    template <> inline 
    float mod<float> (float dividend, float divisor) {return std::fmod(dividend, divisor);}
    template <> inline 
    double mod<double> (double dividend, double divisor) {return std::fmod(dividend, divisor);}
    template <> inline 
    long double mod<long double> (long double dividend, long double divisor) {return std::fmod(dividend, divisor);}

    // provides a typesafe div function
    template <class T> inline 
    T div (T dividend, T divisor) {return dividend / divisor;}

    // template specializations
    template <> inline 
    float div<float> (float dividend, float divisor) {return std::floor(dividend / divisor);}
    template <> inline 
    double div<double> (double dividend, double divisor) {return std::floor(dividend / divisor);}
    template <> inline 
    long double div<long double> (long double dividend, long double divisor) {return std::floor(dividend / divisor);}
} // namespace spin_details

} // namespace GG

#endif // _GG_Spin_h_

