/*
    src/colorpicker.cpp -- push button with a popup to tweak a color value

    This widget was contributed by Christian Schueller.

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

NAMESPACE_BEGIN (nanogui)

ColorPicker::ColorPicker (Widget * parent, const Colour & color) : PopupButton (parent, "")
{
   setBackgroundColor (color);
   Popup * popup = this->popup();
   popup->setLayout (new GroupLayout());
   mColorWheel = new ColorWheel (popup);
   mPickButton = new Button (popup, "Pick");
   mPickButton->setFixedSize (ivec2 (100, 25));
   PopupButton::setChangeCallback ([&] (bool)
   {
      setColor (backgroundColor());
   });
   mColorWheel->setCallback ([&] (const Colour & value)
   {
      mPickButton->setBackgroundColor (value);
      mPickButton->setTextColor (value.contrastingColor());
   });
   mPickButton->setCallback ([&]()
   {
      Colour value = mColorWheel->color();
      setColor (value);
      mCallback (value);
      setPushed (false);
   });
}

Colour ColorPicker::color() const
{
   return backgroundColor();
}

void ColorPicker::setColor (const Colour & color)
{
   Colour fg = color.contrastingColor();
   setBackgroundColor (color);
   setTextColor (fg);
   mColorWheel->setColor (color);
   mPickButton->setBackgroundColor (color);
   mPickButton->setTextColor (fg);
}

NAMESPACE_END (nanogui)
