#include <string.h>
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
    m_Pipeline = gst_parse_launch("filesrc location=./assets/test.mp4 ! parsebin ! avdec_aac ! audioconvert ! audio/x-raw,format=S16LE ! appsink name=audsink emit-signals=true", NULL);

    Q_ASSERT(m_Pipeline);

    m_AudioSink = gst_bin_get_by_name(GST_BIN(m_Pipeline), "audsink");

    Q_ASSERT(m_AudioSink);

    GstAppSinkCallbacks audioCallbacks = {
        .eos = audioEos,
        .new_preroll = audioNewPreroll,
        .new_sample = audioNewSample,
    };
    gst_app_sink_set_callbacks(GST_APP_SINK(m_AudioSink), &audioCallbacks, this, NULL);
}

Backend::~Backend()
{
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
    int channels = 0;
    Q_ASSERT(gst_structure_get_int(cap, "channels", &channels));
    
    qDebug() << gst_caps_to_string(caps);

    NDL_DIRECTAUDIO_DATA_INFO info = {
        .numChannel = channels,
        .bitPerSample = 16,
        .nodelay = 1,
        .upperThreshold = 48,
        .lowerThreshold = 16,
        .channel = NDL_DIRECTAUDIO_CH_MAIN,
        .srcType = NDL_DIRECTAUDIO_SRC_TYPE_PCMMC,
        .samplingFreq = NDL_DIRECTAUDIO_SAMPLING_FREQ_48KHZ};
    NDL_DirectAudioOpen(&info);

    gst_sample_unref(preroll);
    return GST_FLOW_OK;
}

GstFlowReturn Backend::audioNewSample(GstAppSink *appsink, gpointer user_data)
{
    GstSample *sample = gst_app_sink_pull_sample(appsink);

    GstBuffer *buf = gst_sample_get_buffer(sample);

    gsize bufsize = gst_buffer_get_size(buf);
    gpointer rawbuf = malloc(bufsize);

    gst_buffer_extract(buf, 0, rawbuf, bufsize);

    NDL_DirectAudioPlay(rawbuf, bufsize);

    free(rawbuf);
    gst_sample_unref(sample);
    return GST_FLOW_OK;
}