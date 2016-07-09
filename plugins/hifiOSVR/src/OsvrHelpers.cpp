//
//  OsvrHelpers.cpp
//  plugins/hifiOSVR/src
//
//  Created by David Rowe on 8 Jul 2016.
//  Copyright 2016 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#include "OsvrHelpers.h"

#include <mutex>

#include <SharedUtil.h>

static osvr::clientkit::ClientContext* context{ nullptr };
static osvr::clientkit::DisplayConfig* display{ nullptr };

bool isOsvrAvailable() {
    static std::once_flag once;
    static bool result{ false };
    std::call_once(once, [&] {
        static const char* OSVR_APPLICATION_IDENTIFIER = "com.highfidelity.Interface";
        // It is OK for the identifier to be identical on multiple clients connected to the same OSVR server.
        context = new osvr::clientkit::ClientContext(OSVR_APPLICATION_IDENTIFIER);
        result = context->checkStatus();
        if (!result) {
            delete context;
            context = nullptr;
        }

        qDebug() << "OSVR available:" << result;
    });

    return result;
}

bool isOsvrDisplayAvailable() {
    static std::once_flag once;
    static bool result{ false };
    std::call_once(once, [&] {
        if (isOsvrAvailable()) {
            display = new osvr::clientkit::DisplayConfig(*context);
            result = display->valid();

            if (result) {
                static const quint64 OSVR_DISPLAY_STARTUP_TIMEOUT = 1000000;  // 1 sec
                quint64 timeout = usecTimestampNow() + OSVR_DISPLAY_STARTUP_TIMEOUT;
                while (!display->checkStartup() && usecTimestampNow() < timeout) {
                    context->update();
                }
                result = display->checkStartup();
            }

            if (!result) {
                delete display;
                display = nullptr;
            }

            qDebug() << "OSVR display available:" << result;
        }
    });

    return result;
}

osvr::clientkit::ClientContext* getOsvrContext() {
    return context;
}

osvr::clientkit::DisplayConfig* getOsvrDisplay() {
    return display;
}
