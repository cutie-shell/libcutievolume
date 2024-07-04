#include "cutievolume_p.h"

CutieVolume::CutieVolume(QObject *parent)
	: QObject(parent)
	, d_ptr(new CutieVolumePrivate(this))
{
}

CutieVolume::~CutieVolume()
{
}

QObject *CutieVolume::provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
	Q_UNUSED(scriptEngine);
	return new CutieVolume(engine);
}

float CutieVolume::volume()
{
	Q_D(CutieVolume);
	return d->m_volume;
}

bool CutieVolume::muted()
{
	Q_D(CutieVolume);
	return d->m_muted;
}

void CutieVolume::setVolume(float volume)
{
	Q_D(CutieVolume);
	d->m_volume = volume;
	pa_cvolume_set(&(d->_cvolume), 2, PA_VOLUME_NORM * volume);
	pa_context_set_sink_volume_by_name(
		d->_context, d->_default_sink_name.toUtf8().constData(),
		&(d->_cvolume), nullptr, this);
	emit volumeChanged(volume);
}

void CutieVolume::setMuted(bool muted)
{
	Q_D(CutieVolume);
	d->m_muted = muted;
	pa_context_set_sink_mute_by_name(
		d->_context, d->_default_sink_name.toUtf8().constData(), muted,
		nullptr, this);
	emit mutedChanged(muted);
}

CutieVolumePrivate::CutieVolumePrivate(CutieVolume *q)
	: q_ptr(q)
{
	_mainloop = pa_threaded_mainloop_new();
	if (!_mainloop)
		return;
	_mainloop_api = pa_threaded_mainloop_get_api(_mainloop);
	if (!_mainloop_api)
		return;
	_context = pa_context_new(_mainloop_api, "Cutie");
	if (!_context)
		return;
	if (pa_context_connect(_context, NULL, PA_CONTEXT_NOAUTOSPAWN, NULL) <
	    0)
		return;
	pa_context_set_state_callback(_context, this->onContextStateCallback,
				      this);
	pa_threaded_mainloop_start(_mainloop);
}

CutieVolumePrivate::~CutieVolumePrivate()
{
}

void CutieVolumePrivate::onContextStateCallback(pa_context *c, void *userdata)
{
	assert(c && userdata);

	switch (pa_context_get_state(c)) {
	case PA_CONTEXT_CONNECTING:
	case PA_CONTEXT_AUTHORIZING:
	case PA_CONTEXT_SETTING_NAME:
	case PA_CONTEXT_TERMINATED:
		break;

	case PA_CONTEXT_READY:
		pa_context_get_server_info(c, onServerInfoCallback, userdata);
		pa_context_set_subscribe_callback(c, onSubscribeCallback,
						  userdata);
		pa_context_subscribe(c, PA_SUBSCRIPTION_MASK_SINK, NULL, NULL);
		break;

	case PA_CONTEXT_FAILED:
	default:
		qWarning() << "Connection failure: "
			   << pa_strerror(pa_context_errno(c));
		break;
	}
}

void CutieVolumePrivate::onServerInfoCallback(pa_context *c,
					      const pa_server_info *i,
					      void *userdata)
{
	CutieVolumePrivate *vol = (CutieVolumePrivate *)userdata;
	vol->_default_sink_name = QString(i->default_sink_name);
	pa_context_get_sink_info_by_name(c, i->default_sink_name,
					 onSinkInfoCallback, userdata);
}

void CutieVolumePrivate::onSubscribeCallback(pa_context *c,
					     pa_subscription_event_type_t type,
					     uint32_t idx, void *userdata)
{
	unsigned facility = type & PA_SUBSCRIPTION_EVENT_FACILITY_MASK;

	pa_operation *op = NULL;

	switch (facility) {
	case PA_SUBSCRIPTION_EVENT_SINK:
		op = pa_context_get_sink_info_by_index(
			c, idx, onSinkInfoCallback, userdata);
		break;

	default:
		assert(0);
		break;
	}

	if (op)
		pa_operation_unref(op);
}

void CutieVolumePrivate::onSinkInfoCallback(pa_context *c,
					    const pa_sink_info *i, int eol,
					    void *userdata)
{
	Q_UNUSED(c);
	Q_UNUSED(eol);
	CutieVolumePrivate *vol = (CutieVolumePrivate *)userdata;
	if (i) {
		vol->_cvolume = i->volume;
		float volume = (float)pa_cvolume_avg(&(vol->_cvolume)) /
			       (float)PA_VOLUME_NORM;
		if (volume != vol->m_volume) {
			vol->m_volume = volume;
			emit vol->q_ptr->volumeChanged(vol->m_volume);
		}
		if (i->mute != vol->m_muted) {
			vol->m_muted = i->mute;
			emit vol->q_ptr->mutedChanged(vol->m_muted);
		}
	}
}
