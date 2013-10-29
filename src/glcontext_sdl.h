/*
 * Copyright 2011-2013 Branimir Karadzic. All rights reserved.
 *                2013 Mikko Perttunen. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#ifndef __GLCONTEXT_SDL_H__
#define __GLCONTEXT_SDL_H__

#if BGFX_USE_SDL

#   include "SDL2/SDL.h"

namespace bgfx
{
	struct GlContext
	{
		GlContext()
			: m_window(NULL), m_context(0)
		{
		}

		void create(uint32_t _width, uint32_t _height);
		void destroy();
		void resize(uint32_t _width, uint32_t _height, bool _vsync);
		void swap();
		void import();

		bool isValid() const
		{
			return NULL != m_window && 0 != m_context;
		}

		SDL_Window *m_window;
		SDL_GLContext m_context;
		void *m_gllib;
	};
} // namespace bgfx

#endif // BGFX_USE_SDL

#endif // __GLCONTEXT_SDL_H__
