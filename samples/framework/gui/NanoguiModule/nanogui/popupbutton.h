/*
    nanogui/popupbutton.h -- Button which launches a popup widget

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

#pragma once

NAMESPACE_BEGIN (nanogui)

class  PopupButton : public Button
{
   public:
      PopupButton (Widget * parent, const std::string & caption = "Untitled",
                   int buttonIcon = 0,
                   int chevronIcon = ENTYPO_ICON_CHEVRON_SMALL_RIGHT);

      void setChevronIcon (int icon)
      {
         mChevronIcon = icon;
      }
      int chevronIcon() const
      {
         return mChevronIcon;
      }

      Popup * popup()
      {
         return mPopup;
      }
      const Popup * popup() const
      {
         return mPopup;
      }

      virtual void draw (NVGcontext * ctx);
      virtual ivec2 preferredSize (NVGcontext * ctx) const;
      virtual void performLayout (NVGcontext * ctx);

   protected:
      Popup * mPopup;
      int mChevronIcon;
};

NAMESPACE_END (nanogui)
