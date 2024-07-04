#include "plugin.h"

void CutieVolumePlugin::registerTypes(const char *uri)
{
	qmlRegisterSingletonType<CutieVolume>(uri, 1, 0, "CutieVolume",
					      &CutieVolume::provider);
}
