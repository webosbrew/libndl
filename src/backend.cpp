#include <QDebug>

#include "backend.h"

extern "C"
{
#include <NDL_directmedia.h>
}

Backend::Backend(QObject *parent)
    : QObject(parent),
      m_Pipeline(0)
{
    
    char appid[] = "org.webosbrew.sample.ndl-directmedia";

    if (NDL_DirectMediaInit(appid, NULL)) {
        qDebug() << NDL_DirectMediaGetError();
        return;
    }

    NDL_DirectMediaSetAppState(NDL_DIRECTMEDIA_APP_STATE_FOREGROUND);

    m_Pipeline = gst_parse_launch("filesrc location=./assets/test.mp4 ! qtdemux name=demux \
    demux.audio_0 ! queue ! aacparse ! audio/mpeg,mpegversion=4,stream-format=adts ! appsink name=audsink \
    demux.video_0 ! queue ! h264parse config-interval=-1 ! video/x-h264,stream-format=byte-stream,alignment=nal ! appsink name=vidsink",
                                  NULL);

    Q_ASSERT(m_Pipeline);

    m_AudioSink = gst_bin_get_by_name(GST_BIN(m_Pipeline), "audsink");
    Q_ASSERT(m_AudioSink);
    m_VideoSink = gst_bin_get_by_name(GST_BIN(m_Pipeline), "vidsink");
    Q_ASSERT(m_VideoSink);


    GstAppSinkCallbacks audioCallbacks = {
        .eos = audioEos,
        .new_preroll = audioNewPreroll,
        .new_sample = audioNewSample,
    };
    gst_app_sink_set_callbacks(GST_APP_SINK(m_AudioSink), &audioCallbacks, this, NULL);

    GstAppSinkCallbacks videoCallbacks = {
        .eos = videoEos,
        .new_preroll = videoNewPreroll,
        .new_sample = videoNewSample,
    };
    gst_app_sink_set_callbacks(GST_APP_SINK(m_VideoSink), &videoCallbacks, this, NULL);
}

Backend::~Backend()
{
    NDL_DirectMediaQuit();
}

void Backend::test()
{
    Q_ASSERT(gst_element_set_state(m_Pipeline, GST_STATE_PLAYING));
}

void Backend::playCb(void *arg)
{
    qDebug() << "playCb called";
}

void Backend::audioEos(GstAppSink *appsink, gpointer user_data)
{
    NDL_DirectAudioClose();
}

GstFlowReturn Backend::audioNewPreroll(GstAppSink *appsink, gpointer user_data)
{
    GstSample *preroll = gst_app_sink_pull_preroll(appsink);
    GstCaps *caps = gst_sample_get_caps(preroll);
    GstStructure *cap = gst_caps_get_structure(caps, 0);
    int channels = 0, rate = 0;
    Q_ASSERT(gst_structure_get_int(cap, "channels", &channels));
    Q_ASSERT(gst_structure_get_int(cap, "rate", &rate));

    qDebug() << gst_caps_to_string(caps);

    NDL_DIRECTAUDIO_DATA_INFO info = {
        .numChannel = channels,
        .bitPerSample = 16,
        .nodelay = 1,
        .upperThreshold = 48,
        .lowerThreshold = 16,
        .channel = NDL_DIRECTAUDIO_CH_MAIN,
        .srcType = NDL_DIRECTAUDIO_SRC_TYPE_AAC,
        .samplingFreq = NDL_DIRECTAUDIO_SAMPLING_FREQ_OF(rate)};
    NDL_DirectAudioOpen(&info);

    gst_sample_unref(preroll);
    return GST_FLOW_OK;
}

GstFlowReturn Backend::audioNewSample(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = gst_app_sink_pull_sample(appsink);

    GstBuffer *buf = gst_sample_get_buffer(sample);

    GstMapInfo info;
    gst_buffer_map(buf, &info, GST_MAP_READ);

    NDL_DirectAudioPlay(info.data, info.size);

    gst_buffer_unmap(buf, &info);
    gst_sample_unref(sample);
    return GST_FLOW_OK;
}

void Backend::videoEos(GstAppSink *appsink, gpointer user_data)
{
    NDL_DirectVideoClose();
}

GstFlowReturn Backend::videoNewPreroll(GstAppSink *appsink, gpointer user_data)
{
    GstSample *preroll = gst_app_sink_pull_preroll(appsink);
    GstCaps *caps = gst_sample_get_caps(preroll);
    GstStructure *cap = gst_caps_get_structure(caps, 0);
    int channels = 0;
    qDebug() << gst_caps_to_string(caps);

    NDL_DIRECTVIDEO_DATA_INFO info;
    Q_ASSERT(gst_structure_get_int(cap, "width", &info.width));
    Q_ASSERT(gst_structure_get_int(cap, "height", &info.height));
    
    NDL_DirectVideoOpen(&info);
    NDL_DirectVideoSetArea(0, 0, 1920, 1080);
    NDL_DirectVideoSetCallback(playCb);

    gst_sample_unref(preroll);
    return GST_FLOW_OK;
}

GstFlowReturn Backend::videoNewSample(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = gst_app_sink_pull_sample(appsink);

    GstBuffer *buf = gst_sample_get_buffer(sample);

    GstMapInfo info;
    gst_buffer_map(buf, &info, GST_MAP_READ);

    NDL_DirectVideoPlay(info.data, info.size);

    gst_buffer_unmap(buf, &info);
    gst_sample_unref(sample);
    return GST_FLOW_OK;
}