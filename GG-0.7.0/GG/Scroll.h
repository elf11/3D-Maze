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

/** \file Scroll.h
    Contains the Scroll scrollbar control class. */

#ifndef _GG_Scroll_h_
#define _GG_Scroll_h_

#include <GG/Control.h>


namespace GG {

class Button;

/** This is a basic scrollbar control.  The range of the values the scrollbar represents is [m_range_min, m_range_max].
    However, m_posn can only range over [m_range_min, m_range_max - m_page_sz], because the tab has a logical width of
    m_page_sz.  So the region of the scrollbar's range being viewed at any one time is [m_posn, m_posn + m_page_sz].
    (m_posn + m_page_sz is actually the last + 1 element of the range.)  The parent of the control is notified of a
    scroll via ScrolledSignalType signals; these are emitted from the Scroll*() functions and the UpdatePosn() function.
    This should cover every instance in which m_posn is altered.  The parent can poll the control to get its current
    view area with a call to GetPosnRange().  An increase in a vertical scroll is down, and a decrease is up; since GG
    assumes the y-coordinates are downwardly increasing.  The rather plain default buttons and tab can be replaced by
    any Button-derived controls desired.  However, if you want to serialize a Scroll that has custom buttons and/or tab,
    you must make sure they are properly registered.  See the boost serialization documentation for details.*/
class GG_API Scroll : public Control
{
public:
    /// the clickable regions of a Scroll
    enum ScrollRegion {
        SBR_NONE,
        SBR_PAGE_DN,
        SBR_PAGE_UP
    };

    /** \name Signal Types */ ///@{
    typedef boost::signal<void (int, int, int, int)> ScrolledSignalType; ///< emitted whenever the scrollbar is moved; the upper and lower extents of the tab and the upper and lower bounds of the scroll's range are indicated, respectively
    typedef boost::signal<void (int, int, int, int)> ScrolledAndStoppedSignalType; ///< emitted when the scrollbar's tab is stopped after being dragged, the scrollbar is adjusted using the keyboard, or the scrollbar is moved programmatically; the upper and lower extents of the tab and the upper and lower bounds of the scroll's range are indicated, respectively
    //@}

    /** \name Slot Types */ ///@{
    typedef ScrolledSignalType::slot_type ScrolledSlotType; ///< type of functor(s) invoked on a ScrolledSignalType
    typedef ScrolledAndStoppedSignalType::slot_type ScrolledAndStoppedSlotType; ///< type of functor(s) invoked on a ScrolledAndStoppedSignalType
    //@}

    /** \name Structors */ ///@{
    /** ctor. */
    Scroll(int x, int y, int w, int h, Orientation orientation, Clr color, Clr interior,
           Flags<WndFlag> flags = CLICKABLE | REPEAT_BUTTON_DOWN);
    //@}

    /** \name Accessors */ ///@{
    virtual Pt           MinUsableSize() const;

    std::pair<int, int>  PosnRange() const;         ///< range currently being viewed
    std::pair<int, int>  ScrollRange() const;       ///< defined possible range of control
    int                  LineSize() const;          ///< returns the current line size
    int                  PageSize() const;          ///< returns the current page size

    Clr                  InteriorColor() const;     ///< returns the color used to render the interior of the Scroll
    Orientation          ScrollOrientation() const; ///< returns the orientation of the Scroll

    mutable ScrolledSignalType           ScrolledSignal;           ///< the scrolled signal object for this Scroll
    mutable ScrolledAndStoppedSignalType ScrolledAndStoppedSignal; ///< the scrolled-and-stopped signal object for this Scroll
    //@}

    /** \name Mutators */ ///@{
    virtual void   Render();
    virtual void   LButtonDown(const Pt& pt, Flags<ModKey> mod_keys);
    virtual void   LButtonUp(const Pt& pt, Flags<ModKey> mod_keys);
    virtual void   LClick(const Pt& pt, Flags<ModKey> mod_keys);
    virtual void   MouseHere(const Pt& pt, Flags<ModKey> mod_keys);

    virtual void   SizeMove(const Pt& ul, const Pt& lr);

    virtual void   Disable(bool b = true);
    virtual void   SetColor(Clr c);

    void           SetInteriorColor(Clr c); ///< sets the color painted into the client area of the control
    void           SizeScroll(int min, int max, int line, int page); ///< sets the logical ranges of the control, and the logical increment values
    void           SetMax(int max);         ///< sets the maximum value of the scroll
    void           SetMin(int min);         ///< sets the minimum value of the scroll
    void           SetLineSize(int line);   ///< sets the size of a line in the scroll. This is the number of logical units the tab moves when either of the up or down buttons is pressed.
    void           SetPageSize(int page);   ///< sets the size of a line page in the scroll. This is the number of logical units the tab moves when either of the page-up or page-down areas is clicked.

    void           ScrollTo(int p);  ///< scrolls the control to a certain spot
    void           ScrollLineIncr(); ///< scrolls the control down (or right) by a line
    void           ScrollLineDecr(); ///< scrolls the control up (or left) by a line
    void           ScrollPageIncr(); ///< scrolls the control down (or right) by a page
    void           ScrollPageDecr(); ///< scrolls the control up (or left) by a page

    virtual void   DefineAttributes(WndEditor* editor);
    //@}

protected:
    /** \name Structors */ ///@{
    Scroll(); ///< defalt ctor
    //@}

    /** \name Accessors */ ///@{
    int           TabSpace() const;          ///< returns the space the tab has to move about in (the control's width less the width of the incr & decr buttons)
    int           TabWidth() const;          ///< returns the calculated width of the tab, based on PageSize() and the logical size of the control, in pixels
    ScrollRegion  RegionUnder(const Pt& pt); ///< determines whether a pt is in the incr or decr or tab buttons, or in PgUp/PgDn regions in between

    Button*       TabButton() const;     ///< returns the button representing the tab
    Button*       IncrButton() const;    ///< returns the increase button (line down/line right)
    Button*       DecrButton() const;    ///< returns the decrease button (line up/line left)
    //@}

    /** \name Mutators */ ///@{
    virtual bool  EventFilter(Wnd* w, const WndEvent& event);
    //@}

private:
    void              UpdatePosn();        ///< adjusts m_posn due to a tab-drag
    void              MoveTabToPosn();     ///< adjusts tab due to a button click, PgUp, etc.

    Clr               m_int_color;   ///< color inside border of slide area
    const Orientation m_orientation; ///< vertical or horizontal scroll? (use enum for these declared above)
    int               m_posn;        ///< current position of tab in logical coords (will be in [m_range_min, m_range_max - m_page_sz])
    int               m_range_min;   ///< lowest value in range of scrollbar
    int               m_range_max;   ///< highest value "
    int               m_line_sz;     ///< logical units traversed in a line movement (such as a click on either end button)
    int               m_page_sz;     ///< logical units traversed for a page movement (such as a click in non-tab middle area, or PgUp/PgDn)
    Button*           m_tab;         ///< the button representing the tab
    Button*           m_incr;        ///< the increase button (line down/line right)
    Button*           m_decr;        ///< the decrease button (line up/line left)
    ScrollRegion      m_initial_depressed_region; ///< the part of the scrollbar originally under cursor in LButtonDown msg
    ScrollRegion      m_depressed_region;         ///< the part of the scrollbar currently being "depressed" by held-down mouse button

    friend class boost::serialization::access;
    template <class Archive>
    void serialize(Archive& ar, const unsigned int version);
};

// define EnumMap and stream operators for Scroll::ScrollRegion
GG_ENUM_MAP_BEGIN(Scroll::ScrollRegion)
    GG_ENUM_MAP_INSERT(Scroll::SBR_NONE)
    GG_ENUM_MAP_INSERT(Scroll::SBR_PAGE_DN)
    GG_ENUM_MAP_INSERT(Scroll::SBR_PAGE_UP)
GG_ENUM_MAP_END

GG_ENUM_STREAM_IN(Scroll::ScrollRegion)
GG_ENUM_STREAM_OUT(Scroll::ScrollRegion)

} // namespace GG

// template implementations
template <class Archive>
void GG::Scroll::serialize(Archive& ar, const unsigned int version)
{
    ar  & BOOST_SERIALIZATION_BASE_OBJECT_NVP(Control)
        & BOOST_SERIALIZATION_NVP(m_int_color)
        & boost::serialization::make_nvp("m_orientation", const_cast<Orientation&>(m_orientation))
        & BOOST_SERIALIZATION_NVP(m_posn)
        & BOOST_SERIALIZATION_NVP(m_range_min)
        & BOOST_SERIALIZATION_NVP(m_range_max)
        & BOOST_SERIALIZATION_NVP(m_line_sz)
        & BOOST_SERIALIZATION_NVP(m_page_sz)
        & BOOST_SERIALIZATION_NVP(m_tab)
        & BOOST_SERIALIZATION_NVP(m_incr)
        & BOOST_SERIALIZATION_NVP(m_decr);
}

#endif // _GG_Scroll_h_
