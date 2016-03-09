/*
 * Copyright (C) 2008 Kenneth Rohde Christiansen
 *           (C) 2008 Afonso Rabelo Costa Jr.
 *           (C) 2009-2010 ProFUSION embedded systems
 *           (C) 2009-2010 Samsung Electronics
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "SharedTimer.h"

#include <FL/Fl.H>
#include <wtf/Assertions.h>
#include <wtf/CurrentTime.h>
#include <wtf/MainThread.h>

namespace WebCore {

static void (*_timerFunction)();
static double _interval = 0;
static bool running = false;

static void callback(void *) {
	if (_timerFunction) {
		_timerFunction();
		if (_interval < 0.005)
			_interval = 0.005;
		Fl::repeat_timeout(_interval, callback);
	}
}

void setSharedTimerFiredFunction(void (*func)())
{
	_timerFunction = func;
}

void stopSharedTimer()
{
	if (running)
		Fl::remove_timeout(callback);
	running = false;
}

void setSharedTimerFireInterval(double interval)
{
	_interval = interval;

	if (running)
		stopSharedTimer();
	Fl::add_timeout(interval, callback);
	Fl::awake();
	running = true;
}

void invalidateSharedTimer() {
}

}

