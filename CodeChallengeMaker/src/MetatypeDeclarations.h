/**
 * Copyright(c) 2023 Timo Wiesemann. All rights reserved.
 */

#pragma once

#include <QtCore/QMetaType>
#include <data/DownloadData.h>
#include <data/DownloadQueueRecord.h>
#include <data/SpaceInfo.h>

#include <TimeStamp.h>
#include <UniqueId.h>

Q_DECLARE_METATYPE(DownloadQueueRecord)
Q_DECLARE_METATYPE(UniqueId)
Q_DECLARE_METATYPE(DownloadData)
Q_DECLARE_METATYPE(TimeStamp)
Q_DECLARE_METATYPE(SpaceInfo)
