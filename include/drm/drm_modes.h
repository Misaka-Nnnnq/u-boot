/*
 * (C) Copyright 2008-2016 Fuzhou Rockchip Electronics Co., Ltd
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef _DRM_MODES_H
#define _DRM_MODES_H

#include "fdtdec.h"
#include <linux/list.h>

#define PICOS2KHZ(a)	(1000000000UL / (a))
#define KHZ2PICOS(a)	(1000000000UL / (a))

#define DRM_DISPLAY_INFO_LEN	32
#define DRM_CONNECTOR_NAME_LEN	32
#define DRM_DISPLAY_MODE_LEN	32
#define DRM_PROP_NAME_LEN	32

#define DRM_MODE_TYPE_BUILTIN	(1<<0)
#define DRM_MODE_TYPE_CLOCK_C	((1<<1) | DRM_MODE_TYPE_BUILTIN)
#define DRM_MODE_TYPE_CRTC_C	((1<<2) | DRM_MODE_TYPE_BUILTIN)
#define DRM_MODE_TYPE_PREFERRED	(1<<3)
#define DRM_MODE_TYPE_DEFAULT	(1<<4)
#define DRM_MODE_TYPE_USERDEF	(1<<5)
#define DRM_MODE_TYPE_DRIVER	(1<<6)

/* Video mode flags */
/* bit compatible with the xorg definitions. */
#define DRM_MODE_FLAG_PHSYNC			(1 << 0)
#define DRM_MODE_FLAG_NHSYNC			(1 << 1)
#define DRM_MODE_FLAG_PVSYNC			(1 << 2)
#define DRM_MODE_FLAG_NVSYNC			(1 << 3)
#define DRM_MODE_FLAG_INTERLACE			(1 << 4)
#define DRM_MODE_FLAG_DBLSCAN			(1 << 5)
#define DRM_MODE_FLAG_CSYNC			(1 << 6)
#define DRM_MODE_FLAG_PCSYNC			(1 << 7)
#define DRM_MODE_FLAG_NCSYNC			(1 << 8)
#define DRM_MODE_FLAG_HSKEW			(1 << 9) /* hskew provided */
#define DRM_MODE_FLAG_BCAST			(1 << 10)
#define DRM_MODE_FLAG_PIXMUX			(1 << 11)
#define DRM_MODE_FLAG_DBLCLK			(1 << 12)
#define DRM_MODE_FLAG_CLKDIV2			(1 << 13)
/*
 * When adding a new stereo mode don't forget to adjust DRM_MODE_FLAGS_3D_MAX
 * (define not exposed to user space).
 */
#define DRM_MODE_FLAG_3D_MASK			(0x1f << 14)
#define  DRM_MODE_FLAG_3D_NONE			(0 << 14)
#define  DRM_MODE_FLAG_3D_FRAME_PACKING		(1 << 14)
#define  DRM_MODE_FLAG_3D_FIELD_ALTERNATIVE	(2 << 14)
#define  DRM_MODE_FLAG_3D_LINE_ALTERNATIVE	(3 << 14)
#define  DRM_MODE_FLAG_3D_SIDE_BY_SIDE_FULL	(4 << 14)
#define  DRM_MODE_FLAG_3D_L_DEPTH		(5 << 14)
#define  DRM_MODE_FLAG_3D_L_DEPTH_GFX_GFX_DEPTH	(6 << 14)
#define  DRM_MODE_FLAG_3D_TOP_AND_BOTTOM	(7 << 14)
#define  DRM_MODE_FLAG_3D_SIDE_BY_SIDE_HALF	(8 << 14)

/* Panel Mirror control */
#define DRM_MODE_FLAG_XMIRROR			(1<<28)
#define DRM_MODE_FLAG_YMIRROR			(1<<29)
#define DRM_MODE_FLAG_XYMIRROR			(DRM_MODE_FLAG_XMIRROR | DRM_MODE_FLAG_YMIRROR)

/* Picture aspect ratio options */
#define DRM_MODE_PICTURE_ASPECT_NONE		0
#define DRM_MODE_PICTURE_ASPECT_4_3		1
#define DRM_MODE_PICTURE_ASPECT_16_9		2
#define DRM_MODE_PICTURE_ASPECT_64_27		3
#define DRM_MODE_PICTURE_ASPECT_256_135		4

/* Aspect ratio flag bitmask (4 bits 22:19) */
#define DRM_MODE_FLAG_PIC_AR_MASK		(0x0F << 19)
#define  DRM_MODE_FLAG_PIC_AR_NONE \
			(DRM_MODE_PICTURE_ASPECT_NONE << 19)
#define  DRM_MODE_FLAG_PIC_AR_4_3 \
			(DRM_MODE_PICTURE_ASPECT_4_3 << 19)
#define  DRM_MODE_FLAG_PIC_AR_16_9 \
			(DRM_MODE_PICTURE_ASPECT_16_9 << 19)
#define  DRM_MODE_FLAG_PIC_AR_64_27 \
			(DRM_MODE_PICTURE_ASPECT_64_27 << 19)
#define  DRM_MODE_FLAG_PIC_AR_256_135 \
			(DRM_MODE_PICTURE_ASPECT_256_135 << 19)

#define DRM_MODE_CONNECTOR_Unknown	0
#define DRM_MODE_CONNECTOR_VGA		1
#define DRM_MODE_CONNECTOR_DVII		2
#define DRM_MODE_CONNECTOR_DVID		3
#define DRM_MODE_CONNECTOR_DVIA		4
#define DRM_MODE_CONNECTOR_Composite	5
#define DRM_MODE_CONNECTOR_SVIDEO	6
#define DRM_MODE_CONNECTOR_LVDS		7
#define DRM_MODE_CONNECTOR_Component	8
#define DRM_MODE_CONNECTOR_9PinDIN	9
#define DRM_MODE_CONNECTOR_DisplayPort	10
#define DRM_MODE_CONNECTOR_HDMIA	11
#define DRM_MODE_CONNECTOR_HDMIB	12
#define DRM_MODE_CONNECTOR_TV		13
#define DRM_MODE_CONNECTOR_eDP		14
#define DRM_MODE_CONNECTOR_VIRTUAL      15
#define DRM_MODE_CONNECTOR_DSI		16
#define DRM_MODE_CONNECTOR_DPI		17
#define DRM_MODE_CONNECTOR_WRITEBACK	18
#define DRM_MODE_CONNECTOR_SPI		19
#define DRM_MODE_CONNECTOR_USB		20

#define DRM_EDID_PT_HSYNC_POSITIVE (1 << 1)
#define DRM_EDID_PT_VSYNC_POSITIVE (1 << 2)
#define DRM_EDID_PT_SEPARATE_SYNC  (3 << 3)
#define DRM_EDID_PT_STEREO         (1 << 5)
#define DRM_EDID_PT_INTERLACED     (1 << 7)


#define DRM_MODE(nm, t, c, hd, hss, hse, ht, hsk, vd, vss, vse, vt, vs, f) \
	.name = nm, .status = 0, .type = (t), .clock = (c), \
	.hdisplay = (hd), .hsync_start = (hss), .hsync_end = (hse), \
	.htotal = (ht), .hskew = (hsk), .vdisplay = (vd), \
	.vsync_start = (vss), .vsync_end = (vse), .vtotal = (vt), \
	.vscan = (vs), .flags = (f)

/* see also http://vektor.theorem.ca/graphics/ycbcr/ */
enum v4l2_colorspace {
	/*
	 * Default colorspace, i.e. let the driver figure it out.
	 * Can only be used with video capture.
	 */
	V4L2_COLORSPACE_DEFAULT       = 0,

	/* SMPTE 170M: used for broadcast NTSC/PAL SDTV */
	V4L2_COLORSPACE_SMPTE170M     = 1,

	/* Obsolete pre-1998 SMPTE 240M HDTV standard, superseded by Rec 709 */
	V4L2_COLORSPACE_SMPTE240M     = 2,

	/* Rec.709: used for HDTV */
	V4L2_COLORSPACE_REC709        = 3,

	/*
	 * Deprecated, do not use. No driver will ever return this. This was
	 * based on a misunderstanding of the bt878 datasheet.
	 */
	V4L2_COLORSPACE_BT878         = 4,

	/*
	 * NTSC 1953 colorspace. This only makes sense when dealing with
	 * really, really old NTSC recordings. Superseded by SMPTE 170M.
	 */
	V4L2_COLORSPACE_470_SYSTEM_M  = 5,

	/*
	 * EBU Tech 3213 PAL/SECAM colorspace. This only makes sense when
	 * dealing with really old PAL/SECAM recordings. Superseded by
	 * SMPTE 170M.
	 */
	V4L2_COLORSPACE_470_SYSTEM_BG = 6,

	/*
	 * Effectively shorthand for V4L2_COLORSPACE_SRGB, V4L2_YCBCR_ENC_601
	 * and V4L2_QUANTIZATION_FULL_RANGE. To be used for (Motion-)JPEG.
	 */
	V4L2_COLORSPACE_JPEG          = 7,

	/* For RGB colorspaces such as produces by most webcams. */
	V4L2_COLORSPACE_SRGB          = 8,

	/* AdobeRGB colorspace */
	V4L2_COLORSPACE_ADOBERGB      = 9,

	/* BT.2020 colorspace, used for UHDTV. */
	V4L2_COLORSPACE_BT2020        = 10,

	/* Raw colorspace: for RAW unprocessed images */
	V4L2_COLORSPACE_RAW           = 11,

	/* DCI-P3 colorspace, used by cinema projectors */
	V4L2_COLORSPACE_DCI_P3        = 12,
};

#define CRTC_INTERLACE_HALVE_V	(1 << 0) /* halve V values for interlacing */
#define CRTC_STEREO_DOUBLE	(1 << 1) /* adjust timings for stereo modes */
#define CRTC_NO_DBLSCAN		(1 << 2) /* don't adjust doublescan */
#define CRTC_NO_VSCAN		(1 << 3) /* don't adjust doublescan */
#define CRTC_STEREO_DOUBLE_ONLY	(CRTC_STEREO_DOUBLE | CRTC_NO_DBLSCAN | \
				 CRTC_NO_VSCAN)

#define DRM_MODE_FLAG_3D_MAX	DRM_MODE_FLAG_3D_SIDE_BY_SIDE_HALF

#define DRM_MODE_MATCH_TIMINGS		(1 << 0)
#define DRM_MODE_MATCH_CLOCK		(1 << 1)
#define DRM_MODE_MATCH_FLAGS		(1 << 2)
#define DRM_MODE_MATCH_3D_FLAGS		(1 << 3)
#define DRM_MODE_MATCH_ASPECT_RATIO	(1 << 4)

/**
 * enum drm_mode_status - hardware support status of a mode
 * @MODE_OK: Mode OK
 * @MODE_HSYNC: hsync out of range
 * @MODE_VSYNC: vsync out of range
 * @MODE_H_ILLEGAL: mode has illegal horizontal timings
 * @MODE_V_ILLEGAL: mode has illegal vertical timings
 * @MODE_BAD_WIDTH: requires an unsupported linepitch
 * @MODE_NOMODE: no mode with a matching name
 * @MODE_NO_INTERLACE: interlaced mode not supported
 * @MODE_NO_DBLESCAN: doublescan mode not supported
 * @MODE_NO_VSCAN: multiscan mode not supported
 * @MODE_MEM: insufficient video memory
 * @MODE_VIRTUAL_X: mode width too large for specified virtual size
 * @MODE_VIRTUAL_Y: mode height too large for specified virtual size
 * @MODE_MEM_VIRT: insufficient video memory given virtual size
 * @MODE_NOCLOCK: no fixed clock available
 * @MODE_CLOCK_HIGH: clock required is too high
 * @MODE_CLOCK_LOW: clock required is too low
 * @MODE_CLOCK_RANGE: clock/mode isn't in a ClockRange
 * @MODE_BAD_HVALUE: horizontal timing was out of range
 * @MODE_BAD_VVALUE: vertical timing was out of range
 * @MODE_BAD_VSCAN: VScan value out of range
 * @MODE_HSYNC_NARROW: horizontal sync too narrow
 * @MODE_HSYNC_WIDE: horizontal sync too wide
 * @MODE_HBLANK_NARROW: horizontal blanking too narrow
 * @MODE_HBLANK_WIDE: horizontal blanking too wide
 * @MODE_VSYNC_NARROW: vertical sync too narrow
 * @MODE_VSYNC_WIDE: vertical sync too wide
 * @MODE_VBLANK_NARROW: vertical blanking too narrow
 * @MODE_VBLANK_WIDE: vertical blanking too wide
 * @MODE_PANEL: exceeds panel dimensions
 * @MODE_INTERLACE_WIDTH: width too large for interlaced mode
 * @MODE_ONE_WIDTH: only one width is supported
 * @MODE_ONE_HEIGHT: only one height is supported
 * @MODE_ONE_SIZE: only one resolution is supported
 * @MODE_NO_REDUCED: monitor doesn't accept reduced blanking
 * @MODE_NO_STEREO: stereo modes not supported
 * @MODE_NO_420: ycbcr 420 modes not supported
 * @MODE_STALE: mode has become stale
 * @MODE_BAD: unspecified reason
 * @MODE_ERROR: error condition
 *
 * This enum is used to filter out modes not supported by the driver/hardware
 * combination.
 */
enum drm_mode_status {
	MODE_OK = 0,
	MODE_HSYNC,
	MODE_VSYNC,
	MODE_H_ILLEGAL,
	MODE_V_ILLEGAL,
	MODE_BAD_WIDTH,
	MODE_NOMODE,
	MODE_NO_INTERLACE,
	MODE_NO_DBLESCAN,
	MODE_NO_VSCAN,
	MODE_MEM,
	MODE_VIRTUAL_X,
	MODE_VIRTUAL_Y,
	MODE_MEM_VIRT,
	MODE_NOCLOCK,
	MODE_CLOCK_HIGH,
	MODE_CLOCK_LOW,
	MODE_CLOCK_RANGE,
	MODE_BAD_HVALUE,
	MODE_BAD_VVALUE,
	MODE_BAD_VSCAN,
	MODE_HSYNC_NARROW,
	MODE_HSYNC_WIDE,
	MODE_HBLANK_NARROW,
	MODE_HBLANK_WIDE,
	MODE_VSYNC_NARROW,
	MODE_VSYNC_WIDE,
	MODE_VBLANK_NARROW,
	MODE_VBLANK_WIDE,
	MODE_PANEL,
	MODE_INTERLACE_WIDTH,
	MODE_ONE_WIDTH,
	MODE_ONE_HEIGHT,
	MODE_ONE_SIZE,
	MODE_NO_REDUCED,
	MODE_NO_STEREO,
	MODE_NO_420,
	MODE_STALE = -3,
	MODE_BAD = -2,
	MODE_ERROR = -1
};


struct drm_display_mode {
	/**
	 * @head:
	 *
	 * struct list_head for mode lists.
	 */
	struct list_head head;

	char name[128];

	/* Proposed mode values */
	int clock;		/* in kHz */
	int hdisplay;
	int hsync_start;
	int hsync_end;
	int htotal;
	int vdisplay;
	int vsync_start;
	int vsync_end;
	int vtotal;
	int vrefresh;
	int vscan;
	unsigned int flags;
	int picture_aspect_ratio;
	int hskew;
	unsigned int type;
	/* Actual mode we give to hw */
	int crtc_clock;         /* in KHz */
	int crtc_hdisplay;
	int crtc_hblank_start;
	int crtc_hblank_end;
	int crtc_hsync_start;
	int crtc_hsync_end;
	int crtc_htotal;
	int crtc_hskew;
	int crtc_vdisplay;
	int crtc_vblank_start;
	int crtc_vblank_end;
	int crtc_vsync_start;
	int crtc_vsync_end;
	int crtc_vtotal;
	bool invalid;

	u16 width_mm;
	u16 height_mm;

	enum drm_mode_status status;
};

/*
 * Subsystem independent description of a videomode.
 * Can be generated from struct display_timing.
 */
struct videomode {
	unsigned long pixelclock;	/* pixelclock in Hz */

	u32 hactive;
	u32 hfront_porch;
	u32 hback_porch;
	u32 hsync_len;

	u32 vactive;
	u32 vfront_porch;
	u32 vback_porch;
	u32 vsync_len;

	enum display_flags flags; /* display flags */
};


/* Planes blend with or override other bits on the CRTC */
struct drm_mode_set_plane {
	__u32 plane_id;
	__u32 crtc_id;
	__u32 fb_id; /* fb object contains surface format type */
	__u32 flags; /* see above flags */

	/* Signed dest location allows it to be partially off screen */
	__s32 crtc_x;
	__s32 crtc_y;
	__u32 crtc_w;
	__u32 crtc_h;

	/* Source values are 16.16 fixed point */
	__u32 src_x;
	__u32 src_y;
	__u32 src_h;
	__u32 src_w;
};


struct drm_color_lut {
	/*
	 * Values are mapped linearly to 0.0 - 1.0 range, with 0x0 == 0.0 and
	 * 0xffff == 1.0.
	 */
	__u16 red;
	__u16 green;
	__u16 blue;
	__u16 reserved;
};


/**
 * struct drm_mode_fb_cmd2 - Frame-buffer metadata.
 *
 * This struct holds frame-buffer metadata. There are two ways to use it:
 *
 * - User-space can fill this struct and perform a &DRM_IOCTL_MODE_ADDFB2
 *   ioctl to register a new frame-buffer. The new frame-buffer object ID will
 *   be set by the kernel in @fb_id.
 * - User-space can set @fb_id and perform a &DRM_IOCTL_MODE_GETFB2 ioctl to
 *   fetch metadata about an existing frame-buffer.
 *
 * In case of planar formats, this struct allows up to 4 buffer objects with
 * offsets and pitches per plane. The pitch and offset order are dictated by
 * the format FourCC as defined by drm_fourcc.h, e.g. NV12 is described as:
 *
 *     YUV 4:2:0 image with a plane of 8-bit Y samples followed by an
 *     interleaved U/V plane containing 8-bit 2x2 subsampled colour difference
 *     samples.
 *
 * So it would consist of a Y plane at offsets[0] and a UV plane at
 * offsets[1].
 *
 * To accommodate tiled, compressed, etc formats, a modifier can be specified.
 * For more information see the "Format Modifiers" section. Note that even
 * though it looks like we have a modifier per-plane, we in fact do not. The
 * modifier for each plane must be identical. Thus all combinations of
 * different data layouts for multi-plane formats must be enumerated as
 * separate modifiers.
 *
 * All of the entries in @handles, @pitches, @offsets and @modifier must be
 * zero when unused. Warning, for @offsets and @modifier zero can't be used to
 * figure out whether the entry is used or not since it's a valid value (a zero
 * offset is common, and a zero modifier is &DRM_FORMAT_MOD_LINEAR).
 */
struct drm_mode_fb_cmd2 {
	/** @fb_id: Object ID of the frame-buffer. */
	__u32 fb_id;
	/** @width: Width of the frame-buffer. */
	__u32 width;
	/** @height: Height of the frame-buffer. */
	__u32 height;
	/**
	 * @pixel_format: FourCC format code, see DRM_FORMAT_* constants in
	 * drm_fourcc.h.
	 */
	__u32 pixel_format;
	/**
	 * @flags: Frame-buffer flags (see &DRM_MODE_FB_INTERLACED and
	 * &DRM_MODE_FB_MODIFIERS).
	 */
	__u32 flags;

	/** @pitches: Pitch (aka. stride) in bytes, one per plane. */
	__u32 pitches[4];
	/** @offsets: Offset into the buffer in bytes, one per plane. */
	__u32 offsets[4];
	/**
	 * @modifier: Format modifier, one per plane. See DRM_FORMAT_MOD_*
	 * constants in drm_fourcc.h. All planes must use the same
	 * modifier. Ignored unless &DRM_MODE_FB_MODIFIERS is set in @flags.
	 */
	__u64 modifier[4];
};


#define DRM_MODE_FB_INTERLACED	(1<<0) /* for interlaced framebuffers */
#define DRM_MODE_FB_MODIFIERS	(1<<1) /* enables ->modifer[] */

#define DRM_MODE_BLEND_PREMULTI		0
#define DRM_MODE_BLEND_COVERAGE		1
#define DRM_MODE_BLEND_PIXEL_NONE	2
#define DRM_BLEND_ALPHA_OPAQUE		0xffff

/*
 * DRM_MODE_ROTATE_<degrees>
 *
 * Signals that a drm plane is been rotated <degrees> degrees in counter
 * clockwise direction.
 *
 * This define is provided as a convenience, looking up the property id
 * using the name->prop id lookup is the preferred method.
 */
#define DRM_MODE_ROTATE_0       (1<<0)
#define DRM_MODE_ROTATE_90      (1<<1)
#define DRM_MODE_ROTATE_180     (1<<2)
#define DRM_MODE_ROTATE_270     (1<<3)

/*
 * DRM_MODE_ROTATE_MASK
 *
 * Bitmask used to look for drm plane rotations.
 */
#define DRM_MODE_ROTATE_MASK (\
		DRM_MODE_ROTATE_0  | \
		DRM_MODE_ROTATE_90  | \
		DRM_MODE_ROTATE_180 | \
		DRM_MODE_ROTATE_270)

/*
 * DRM_MODE_REFLECT_<axis>
 *
 * Signals that the contents of a drm plane is reflected along the <axis> axis,
 * in the same way as mirroring.
 * See kerneldoc chapter "Plane Composition Properties" for more details.
 *
 * This define is provided as a convenience, looking up the property id
 * using the name->prop id lookup is the preferred method.
 */
#define DRM_MODE_REFLECT_X      (1<<4)
#define DRM_MODE_REFLECT_Y      (1<<5)

/*
 * DRM_MODE_REFLECT_MASK
 *
 * Bitmask used to look for drm plane reflections.
 */
#define DRM_MODE_REFLECT_MASK (\
		DRM_MODE_REFLECT_X | \
		DRM_MODE_REFLECT_Y)


struct drm_display_mode *drm_mode_create(void);
void drm_mode_copy(struct drm_display_mode *dst,
		   const struct drm_display_mode *src);
struct drm_display_mode *drm_mode_duplicate(const struct drm_display_mode *mode);
void drm_mode_destroy(struct drm_display_mode *mode);
bool drm_mode_match(const struct drm_display_mode *mode1,
		    const struct drm_display_mode *mode2,
		    unsigned int match_flags);
bool drm_mode_equal(const struct drm_display_mode *mode1,
		    const struct drm_display_mode *mode2);
void drm_display_mode_from_videomode(const struct videomode *vm,
				     struct drm_display_mode *dmode);
void drm_display_mode_to_videomode(const struct drm_display_mode *dmode,
				   struct videomode *vm);
int drm_mode_vrefresh(const struct drm_display_mode *mode);
void drm_mode_set_name(struct drm_display_mode *mode);

struct drm_display_mode *drm_cvt_mode(int hdisplay,
				      int vdisplay, int vrefresh,
				      bool reduced, bool interlaced, bool margins);

struct drm_display_mode *
drm_gtf_mode(int hdisplay, int vdisplay, int vrefresh,
	     bool interlaced, int margins);

struct drm_display_mode *
drm_gtf_mode_complex(int hdisplay, int vdisplay,
		     int vrefresh, bool interlaced, int margins,
		     int GTF_M, int GTF_2C, int GTF_K, int GTF_2J);

#endif
