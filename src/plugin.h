#pragma once
#include <QDebug>
#include <QtQuick>
#include <QtQml/qqml.h>
#include <QtQml/QQmlExtensionPlugin>

#include "cutievolume.h"

class CutieVolumePlugin : public QQmlExtensionPlugin {
	Q_OBJECT
	Q_PLUGIN_METADATA(IID QQmlEngineExtensionInterface_iid FILE
			  "cutievolume.json")
    public:
	explicit CutieVolumePlugin()
	{
	}

	void registerTypes(const char *uri) override;
};