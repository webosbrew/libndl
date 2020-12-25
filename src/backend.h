// Copyright (c) 2020 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#pragma once

#include <QObject>

extern "C"
{
#include <gst/gst.h>
#include <gst/app/app.h>
}

class Backend : public QObject
{
    Q_OBJECT

public:
    Backend(QObject *parent = nullptr);
    ~Backend();

    Q_INVOKABLE void test();

private:
    static void playCb(void *arg);

    static void audioEos(GstAppSink *appsink, void *userData);
    static GstFlowReturn audioNewPreroll(GstAppSink *appsink, void *userData);
    static GstFlowReturn audioNewSample(GstAppSink *appsink, void *userData);

    static void videoEos(GstAppSink *appsink, void *userData);
    static GstFlowReturn videoNewPreroll(GstAppSink *appsink, void *userData);
    static GstFlowReturn videoNewSample(GstAppSink *appsink, void *userData);

    GstElement *m_Pipeline;
    GstElement *m_AudioSink;
    GstElement *m_VideoSink;
};