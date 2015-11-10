/*
    src/window.cpp -- Top-level window widget

    NanoGUI was developed by Wenzel Jakob <wenzel@inf.ethz.ch>.
    The widget drawing code is based on the NanoVG demo application
    by Mikko Mononen.

    All rights reserved. Use of this source code is governed by a
    BSD-style license that can be found in the LICENSE.txt file.
*/


NAMESPACE_BEGIN (nanogui)

Window::Window (Widget * parent, const std::string & title)
   : Widget (parent), mTitle (title), mModal (false), mDrag (false) { }

ivec2 Window::preferredSize (NVGcontext * ctx) const
{
	ivec2 result = Widget::preferredSize(ctx);
	nvgFontSize(ctx, 18.0f);
	nvgFontFace(ctx, "sans-bold");
	float bounds[4];
	nvgTextBounds(ctx, 0, 0, mTitle.c_str(), nullptr, bounds);
	return cwiseMax(result, ivec2(bounds[2] - bounds[0] + 20, bounds[3] - bounds[1]));
}

void Window::draw (NVGcontext * ctx)
{
   int ds = mTheme->mWindowDropShadowSize, cr = mTheme->mWindowCornerRadius;
   int hh = mTheme->mWindowHeaderHeight;
   /* Draw window */
   nvgSave (ctx);
   nvgBeginPath (ctx);
   nvgRoundedRect (ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr);
   nvgFillColor (ctx, mMouseFocus ? mTheme->mWindowFillFocused
                 : mTheme->mWindowFillUnfocused);
   nvgFill (ctx);
   /* Draw a drop shadow */
   NVGpaint shadowPaint = nvgBoxGradient (
                             ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr * 2, ds * 2,
                             mTheme->mDropShadow, mTheme->mTransparent);
   nvgBeginPath (ctx);
   nvgRect (ctx, mPos.x - ds, mPos.y - ds, mSize.x + 2 * ds, mSize.y + 2 * ds);
   nvgRoundedRect (ctx, mPos.x, mPos.y, mSize.x, mSize.y, cr);
   nvgPathWinding (ctx, NVG_HOLE);
   nvgFillPaint (ctx, shadowPaint);
   nvgFill (ctx);
   if (!mTitle.empty())
   {
      /* Draw header */
      NVGpaint headerPaint = nvgLinearGradient (
                                ctx, mPos.x, mPos.y, mPos.x,
                                mPos.y + hh,
                                mTheme->mWindowHeaderGradientTop,
                                mTheme->mWindowHeaderGradientBot);
      nvgBeginPath (ctx);
      nvgRoundedRect (ctx, mPos.x, mPos.y, mSize.x, hh, cr);
      nvgFillPaint (ctx, headerPaint);
      nvgFill (ctx);
      nvgBeginPath (ctx);
      nvgRoundedRect (ctx, mPos.x, mPos.y, mSize.x, hh, cr);
      nvgStrokeColor (ctx, mTheme->mWindowHeaderSepTop);
      nvgScissor (ctx, mPos.x, mPos.y, mSize.x, 0.5f);
      nvgStroke (ctx);
      nvgResetScissor (ctx);
      nvgBeginPath (ctx);
      nvgMoveTo (ctx, mPos.x + 0.5f, mPos.y + hh - 1.5f);
      nvgLineTo (ctx, mPos.x + mSize.x - 0.5f, mPos.y + hh - 1.5);
      nvgStrokeColor (ctx, mTheme->mWindowHeaderSepBot);
      nvgStroke (ctx);
      nvgFontSize (ctx, 18.0f);
      nvgFontFace (ctx, "sans-bold");
      nvgTextAlign (ctx, NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE);
      nvgFontBlur (ctx, 2);
      nvgFillColor (ctx, mTheme->mDropShadow);
      nvgText (ctx, mPos.x + mSize.x / 2,
               mPos.y + hh / 2, mTitle.c_str(), nullptr);
      nvgFontBlur (ctx, 0);
      nvgFillColor (ctx, mFocused ? mTheme->mWindowTitleFocused
                    : mTheme->mWindowTitleUnfocused);
      nvgText (ctx, mPos.x + mSize.x / 2, mPos.y + hh / 2 - 1,
               mTitle.c_str(), nullptr);
   }
   nvgRestore (ctx);
   Widget::draw (ctx);
}

void Window::dispose()
{
   Widget * widget = this;
   while (widget->parent())
      widget = widget->parent();
   ((Screen *) widget)->disposeWindow (this);
}

void Window::center()
{
   Widget * widget = this;
   while (widget->parent())
      widget = widget->parent();
   ((Screen *) widget)->centerWindow (this);
}

bool Window::mouseDragEvent (const ivec2 &, const ivec2 & rel,
                             int button, int /* modifiers */)
{
	if (mDrag && (button & (1 << MOUSE_BUTTON_LEFT)) != 0)
	{
		mPos += rel;
		mPos = cwiseMax(mPos, ivec2(0));
		mPos = cwiseMin(mPos, parent()->size() - mSize);
		return true;
	}
	return false;
}

bool Window::mouseButtonEvent (const ivec2 & p, int button, bool down, int modifiers)
{
   if (Widget::mouseButtonEvent (p, button, down, modifiers))
      return true;
   if (button == MOUSE_BUTTON_LEFT)
   {
      mDrag = down && (p.y - mPos.y) < mTheme->mWindowHeaderHeight;
      return true;
   }
   return false;
}

bool Window::scrollEvent (const ivec2 & p, const vec2 & rel)
{
   Widget::scrollEvent (p, rel);
   return true;
}

void Window::refreshRelativePlacement()
{
   /* Overridden in \ref Popup */
}

NAMESPACE_END (nanogui)
