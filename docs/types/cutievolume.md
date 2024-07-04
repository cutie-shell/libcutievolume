---
title: CutieVolume QML Type
---

CutieVolume is used to read and change volume settings on Cutie through PulseAudio.

Inherits: QObject

### Properties

- [volume](#volume): float
- [muted](#muted): bool

## Detailed Description

This singleton type is used to read and change volume settings on Cutie through PulseAudio.

## Property Documentation

#### volume

float

Holds the master volume level as a floating point number between 0.0 and 1.0. This value can be set to change master volume level.

#### muted

bool

Holds true is master audio is muted, otherwise holds false. This value is independent from [volume](#volume) and it can be set to mute or unmute the device.
