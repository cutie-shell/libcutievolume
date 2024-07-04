#pragma once
#include <QtQuick>

class CutieVolumePrivate;

class CutieVolume : public QObject {
	Q_OBJECT
	Q_DECLARE_PRIVATE(CutieVolume)
	Q_PROPERTY(float volume READ volume WRITE setVolume NOTIFY volumeChanged)
	Q_PROPERTY(bool muted READ muted WRITE setMuted NOTIFY mutedChanged)

    public:
	CutieVolume(QObject *parent = 0);
	~CutieVolume();

	float volume();
	bool muted();
	void setVolume(float volume);
	void setMuted(bool muted);

	static QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine);

    signals:
	void volumeChanged(float volume);
	void mutedChanged(bool muted);

    private:
	CutieVolumePrivate *d_ptr;
};