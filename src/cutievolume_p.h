#pragma once
#include "cutievolume.h"

#include <QtQml/qqml.h>
#include <QtQml/QQmlExtensionPlugin>
#include <pulse/pulseaudio.h>

class CutieVolumePrivate : public QObject {
	Q_OBJECT
	Q_DECLARE_PUBLIC(CutieVolume)

    public:
	CutieVolumePrivate(CutieVolume *q);
	~CutieVolumePrivate();

    private:
	pa_threaded_mainloop *_mainloop;
	pa_mainloop_api *_mainloop_api;
	pa_context *_context;
	QString _default_sink_name;
	pa_cvolume _cvolume;

	float m_volume;
	bool m_muted;

	static void onContextStateCallback(pa_context *c, void *userdata);
	static void onServerInfoCallback(pa_context *c, const pa_server_info *i,
					 void *userdata);
	static void onSubscribeCallback(pa_context *c,
					pa_subscription_event_type_t type,
					uint32_t idx, void *userdata);
	static void onSinkInfoCallback(pa_context *c, const pa_sink_info *i,
				       int eol, void *userdata);

	CutieVolume *q_ptr;
};