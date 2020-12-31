#ifndef ZSTACK_H
#define ZSTACK_H

#include <zstack/config.h>

#include <zstack/algo.h>
#include <zstack/log.h>
#include <zstack/misc.h>
#include <zstack/yuv.h>
#include <zstack/platform.h>
#include <zstack/window_class.h>
#include <zstack/ringbuf.h>

#include <zstack/vp9.h>
#include <zstack/h264.h>
#include <zstack/h265.h>

#ifdef ZSTACK_ENABLE_VSILICON
#include <zstack/vs/avmips.h>
#include <zstack/vs/dbg.h>
#include <zstack/vs/pman.h>
#include <zstack/vs/sx.h>
#endif

extern int window_class_register(void);

#endif
