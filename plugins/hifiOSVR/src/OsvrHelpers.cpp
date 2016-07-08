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

#ifdef HAVE_OSVR
#include <osvr/ClientKit/ClientKit.h>
#endif

static osvr::clientkit::ClientContext* context{ nullptr };
static bool isOsvrAvailableChecked{ false };
static bool isOsvrAvailableResult{ false };

static const char* OSVR_APPLICATION_IDENTIFIER = "com.highfidelity.Interface";
// It is OK for the identifier to be identical on multiple clients connected to the same OSVR server.

bool isOsvrAvailable() {
#ifdef HAVE_OSVR
    if (!isOsvrAvailableChecked) {
        context = new osvr::clientkit::ClientContext(OSVR_APPLICATION_IDENTIFIER);

        isOsvrAvailableResult = context->checkStatus();

        if (!isOsvrAvailableResult) {
            delete context;
        }
    }

    return isOsvrAvailableResult;
#else
    return false;
#endif
}
