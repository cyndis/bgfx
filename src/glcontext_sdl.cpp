/*
 * Copyright 2011-2013 Branimir Karadzic. All rights reserved.
 *                2013 Mikko Perttunen. All rights reserved.
 * License: http://www.opensource.org/licenses/BSD-2-Clause
 */

#include "bgfx_p.h"

#if (BGFX_CONFIG_RENDERER_OPENGLES2|BGFX_CONFIG_RENDERER_OPENGLES3|BGFX_CONFIG_RENDERER_OPENGL)
#	include "renderer_gl.h"

extern void *eglGetProcAddress(char const *procname);

namespace bgfx
{
#	define GL_IMPORT(_optional, _proto, _func) _proto _func
#		include "glimports.h"
#	undef GL_IMPORT

	void GlContext::create(uint32_t _width, uint32_t _height)
	{
		int success = SDL_Init(SDL_INIT_EVERYTHING);
		BGFX_FATAL(success != -1, Fatal::UnableToInitialize, "Failed to initialize SDL");

		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
		//SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_EGL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 0);

		success = SDL_GL_LoadLibrary(NULL);
		BGFX_FATAL(success == 0, Fatal::UnableToInitialize, "Failed to load OpenGL library");

		m_window = SDL_CreateWindow("bgfx", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
									_width, _height, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
		BGFX_FATAL(m_window, Fatal::UnableToInitialize, "Failed to create window");

		m_context = SDL_GL_CreateContext(m_window);
		BGFX_FATAL(m_context, Fatal::UnableToInitialize, "Failed to create context");

		success = SDL_GL_MakeCurrent(m_window, m_context);
		BGFX_FATAL(success == 0, Fatal::UnableToInitialize, "Failed to make context current");

		SDL_GL_SetSwapInterval(0);

		m_gllib = dlopen("/system/lib/libGLESv2.so", RTLD_LAZY | RTLD_GLOBAL);
		BGFX_FATAL(m_gllib, Fatal::UnableToInitialize, "dlopen failed");

		import();
	}

	void GlContext::destroy()
	{
		SDL_GL_DeleteContext(m_context);
		SDL_DestroyWindow(m_window);
		SDL_GL_UnloadLibrary();
	}

	void GlContext::resize(uint32_t _width, uint32_t _height, bool _vsync)
	{
		int32_t interval = _vsync ? 1 : 0;

		SDL_SetWindowSize(m_window, _width, _height);
		SDL_GL_SetSwapInterval(interval);
	}

	void GlContext::swap()
	{
		SDL_GL_MakeCurrent(m_window, m_context);
		SDL_GL_SwapWindow(m_window);
	}

	void GlContext::import()
	{
		BX_TRACE("Importing OpenGL symbols\n");
#	define GL_IMPORT(_optional, _proto, _func) \
	{ \
		_func = (_proto)dlsym(m_gllib, (const char*)#_func); \
		BGFX_FATAL(_optional || NULL != _func, Fatal::UnableToInitialize, "Failed to create OpenGL context. SDL_GL_GetProcAddress %s", #_func); \
		BX_TRACE("%s at %p", #_func, _func); \
	}
#	include "glimports.h"
#	undef GL_IMPORT
	}

} // namespace bgfx

#endif // (BGFX_CONFIG_RENDERER_OPENGLES2|BGFX_CONFIG_RENDERER_OPENGLES3|BGFX_CONFIG_RENDERER_OPENGL)
