/*
 * gAndroidWindow.cpp
 *
 *  Created on: June 24, 2023
 *      Author: Metehan Gezer
 */

#include "gAndroidWindow.h"
#include "gAppManager.h"



gAndroidWindow::gAndroidWindow() {
}

gAndroidWindow::~gAndroidWindow() {
}


void gAndroidWindow::initialize(int uwidth, int uheight, int windowMode, bool isResizable) {
	gBaseWindow::initialize(uwidth, uheight, windowMode, isResizable);

	while(!window){ }

	const EGLint attribs[] = {
			EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT, // request OpenGL ES 3.0
            EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
            EGL_BLUE_SIZE, 8,
            EGL_GREEN_SIZE, 8,
            EGL_RED_SIZE, 8,
            EGL_DEPTH_SIZE, 16,
            EGL_NONE
	};
	EGLConfig config;
	EGLint numConfigs;
	EGLint format;


	if ((display = eglGetDisplay(EGL_DEFAULT_DISPLAY)) == EGL_NO_DISPLAY) {
		gLogi("gAndroidWindow") << "eglGetDisplay() returned error " << eglGetError();
        exit(-1);
		return;
	}
	if (!eglInitialize(display, 0, 0)) {
		gLogi("gAndroidWindow") << "eglInitialize() returned error " << eglGetError();
        exit(-1);
		return;
	}

	if (!eglChooseConfig(display, attribs, &config, 1, &numConfigs)) {
		gLogi("gAndroidWindow") << "eglChooseConfig() returned error " << eglGetError();
		//destroy();
        exit(-1);
		return;
	}

	if (!eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format)) {
		gLogi("gAndroidWindow") << "eglGetConfigAttrib() returned error " << eglGetError();
		//destroy();
        exit(-1);
		return;
	}

	//ANativeWindow_setBuffersGeometry(this->window, 0, 0, format);

	if (!(surface = eglCreateWindowSurface(display, config, window, 0))) {
		gLogi("gAndroidWindow") << "eglCreateWindowSurface() returned error " << eglGetError();
		//destroy();
//        exit(-1);
		return;
	}
    const EGLint attribList[] = {EGL_CONTEXT_CLIENT_VERSION, 3, EGL_NONE};

	if (!(context = eglCreateContext(display, config, EGL_NO_CONTEXT, attribList))) {
		gLogi("gAndroidWindow") << "eglCreateContext() returned error " << eglGetError();
		//destroy();
		exit(-1);
		return;
	}

	if (!eglMakeCurrent(display, surface, surface, context)) {
		gLogi("gAndroidWindow") << "eglMakeCurrent() returned error " << eglGetError();
		//destroy();
		exit(-1);
		return;
	}

	if (!eglQuerySurface(display, surface, EGL_WIDTH, &width) ||
		!eglQuerySurface(display, surface, EGL_HEIGHT, &height)) {
		gLogi("gAndroidWindow") << "eglQuerySurface() returned error " << eglGetError();
		//destroy();
		exit(-1);
		return;
	}

//	glDisable(GL_DITHER);
	glViewport(0, 0, width, height);
}


bool gAndroidWindow::getShouldClose() {
	return false;
}

void gAndroidWindow::update() {
	if (!eglSwapBuffers(display, surface)) {
		gLogi("gAndroidWindow") << "eglSwapBuffers() returned error " << eglGetError();
	}
	// End window drawing
//    std::cout << "width:" << width << ", height:" << height << std::endl;
}

void gAndroidWindow::close() {
}

void gAndroidWindow::enableVsync(bool vsync) {
	gBaseWindow::enableVsync(vsync);
}

void gAndroidWindow::setCursor(int cursorNo) {
}

void gAndroidWindow::setCursorMode(int cursorMode) {
}

void gAndroidWindow::setClipboardString(std::string text) {
}

std::string gAndroidWindow::getClipboardString() {
	return "ASD";
}

void gAndroidWindow::setWindowSize(int width, int height) {
}

void gAndroidWindow::setWindowResizable(bool isResizable) {
}

void gAndroidWindow::setWindowSizeLimits(int minWidth, int minHeight, int maxWidth, int maxHeight) {
}

ANativeWindow* gAndroidWindow::window;
extern "C" {
	JNIEXPORT void JNICALL Java_dev_glist_glistapp_GlistNative_setSurface(JNIEnv *env, jclass clazz, jobject surface) {
        if (surface != 0) {
            gAndroidWindow::window = ANativeWindow_fromSurface(env, surface);
        } else {
            ANativeWindow_release(gAndroidWindow::window);
        }
	}
}
