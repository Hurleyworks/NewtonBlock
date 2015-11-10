/*
    src/progressbar.cpp -- Standard widget for visualizing progress

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/

NAMESPACE_BEGIN (nanogui)

ProgressBar::ProgressBar (Widget * parent)
   : Widget (parent), mValue (0.0f) {}

ivec2 ProgressBar::preferredSize (NVGcontext *) const
{
   return ivec2 (70, 12);
}

void ProgressBar::draw (NVGcontext * ctx)
{
   Widget::draw (ctx);
   NVGpaint paint = nvgBoxGradient (
                       ctx, mPos.x + 1, mPos.y + 1,
                       mSize.x - 2, mSize.y, 3, 4, Colour (0, 32), Colour (0, 92));
   nvgBeginPath (ctx);
   nvgRoundedRect (ctx, mPos.x, mPos.y, mSize.x, mSize.y, 3);
   nvgFillPaint (ctx, paint);
   nvgFill (ctx);
   float value = std::min (std::max (0.0f, mValue), 1.0f);
   int barPos = (int) std::round ((mSize.x - 2) * value);
   paint = nvgBoxGradient (
              ctx, mPos.x, mPos.y,
              barPos + 1.5f, mSize.y - 1, 3, 4,
              Colour (220, 100), Colour (128, 100));
   nvgBeginPath (ctx);
   nvgRoundedRect (
      ctx, mPos.x + 1, mPos.y + 1,
      barPos, mSize.y - 2, 3);
   nvgFillPaint (ctx, paint);
   nvgFill (ctx);
}

NAMESPACE_END (nanogui)
