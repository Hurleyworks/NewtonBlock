/*
    src/checkbox.cpp -- Two-state check box widget

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

NAMESPACE_BEGIN (nanogui)

CheckBox::CheckBox (Widget * parent, const std::string & caption,
                    const std::function<void (bool) > & callback)
   : Widget (parent), mCaption (caption), mPushed (false), mChecked (false),
     mCallback (callback) { }

bool CheckBox::mouseButtonEvent (const ivec2 & p, int button, bool down,
                                 int modifiers)
{
   Widget::mouseButtonEvent (p, button, down, modifiers);
   if (!mEnabled)
      return false;
   if (button == MOUSE_BUTTON_LEFT)
   {
      if (down)
         mPushed = true;
      else
         if (mPushed)
         {
            if (contains (p))
            {
               mChecked = !mChecked;
               if (mCallback)
                  mCallback (mChecked);
            }
            mPushed = false;
         }
      return true;
   }
   return false;
}

ivec2 CheckBox::preferredSize (NVGcontext * ctx) const
{
   if (mFixedSize != ivec2(0))
      return mFixedSize;
   nvgFontSize (ctx, fontSize());
   nvgFontFace (ctx, "sans");
   return ivec2 (
             nvgTextBounds (ctx, 0, 0, mCaption.c_str(), nullptr, nullptr) +
             1.7f * fontSize(),
             fontSize() * 1.3f);
}

void CheckBox::draw (NVGcontext * ctx)
{
   Widget::draw (ctx);
   nvgFontSize (ctx, fontSize());
   nvgFontFace (ctx, "sans");
   nvgFillColor (ctx,
                 mEnabled ? mTheme->mTextColor : mTheme->mDisabledTextColor);
   nvgTextAlign (ctx, NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE);
   nvgText (ctx, mPos.x + 1.2f * mSize.y + 5, mPos.y + mSize.y * 0.5f,
            mCaption.c_str(), nullptr);
   NVGpaint bg = nvgBoxGradient (ctx, mPos.x + 1.5f, mPos.y + 1.5f,
                                 mSize.y - 2.0f, mSize.y - 2.0f, 3, 3,
                                 mPushed ? Colour (0, 100) : Colour (0, 32),
                                 Colour (0, 0, 0, 180));
   nvgBeginPath (ctx);
   nvgRoundedRect (ctx, mPos.x + 1.0f, mPos.y + 1.0f, mSize.y - 2.0f,
                   mSize.y - 2.0f, 3);
   nvgFillPaint (ctx, bg);
   nvgFill (ctx);
   if (mChecked)
   {
      nvgFontSize (ctx, 1.8 * mSize.y);
      nvgFontFace (ctx, "icons");
      nvgFillColor (ctx, mEnabled ? mTheme->mIconColor
                    : mTheme->mDisabledTextColor);
      nvgTextAlign (ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
      nvgText (ctx, mPos.x + mSize.y * 0.5f + 1,
               mPos.y + mSize.y * 0.5f, utf8 (ENTYPO_ICON_CHECK).data(),
               nullptr);
   }
}

NAMESPACE_END (nanogui)
