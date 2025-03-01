/*
 * Allwinner SoCs hdmi2.0 driver.
 *
 * Copyright (C) 2016 Allwinner.
 *
 * This file is licensed under the terms of the GNU General Public
 * License version 2.  This program is licensed "as is" without any
 * warranty of any kind, whether express or implied.
 */
#include "log.h"
#include "general_ops.h"

#include "core/main_controller.h"
#include "core/video.h"
#include "core/audio.h"
#include "core/packets.h"
#include "core/irq.h"
#include "core/fc_video.h"
#include "core/fc_audio.h"
#include "core/ctrl_phy.h"

#include "phy_model/aw_phy.h"
#include "phy_model/inno_phy.h"
#include "phy_model/snps_phy.h"

#include "edid.h"
#include "hdcp.h"
#include "access.h"
#include "scdc.h"

#include "hdmitx_dev.h"
#include "identification.h"
#include "core_api.h"

#include "api.h"

static hdmi_tx_dev_t				*hdmi_api;

static int api_phy_write(u8 addr, u32 data)
{
#if defined(CONFIG_AW_PHY)
	aw_phy_write(addr, data);
#elif defined(CONFIG_AW_INNOPHY)
	inno_phy_write(addr, (u8)data);
#elif defined(CONFIG_AW_SNPSPHY)
	snps_phy_write(hdmi_api, addr, (u16)data);
#else
	pr_err("[%s] not config phy model!!!\n", __func__);
	return -1;
#endif
	return 0;
}

static int api_phy_read(u8 addr, u32 *value)
{
#if defined(CONFIG_AW_PHY)
	aw_phy_read(addr, value);
#elif defined(CONFIG_AW_INNOPHY)
	inno_phy_read(addr, (u8 *)value);
#elif defined(CONFIG_AW_SNPSPHY)
	snps_phy_read(hdmi_api, addr, (u16 *)value);
#else
	pr_err("[%s] not config phy model!!!\n", __func__);
	return -1;
#endif
	return 0;
}

static int api_phy_configure(hdmi_tx_dev_t *dev, u16 phy_model, encoding_t EncodingOut)
{
#if defined(CONFIG_AW_PHY)
	return aw_phy_configure(dev, EncodingOut);
#elif defined(CONFIG_AW_INNOPHY)
	return inno_phy_configure(dev);
#elif defined(CONFIG_AW_SNPSPHY)
	return snps_phy_configure(dev, phy_model);
#else
	pr_err("[%s] not config phy model!!!\n", __func__);
	return -1;
#endif
	return 0;
}

static void api_phy_reset(void)
{
#if defined(CONFIG_AW_PHY)
	aw_phy_reset();
#else
	pr_err("[%s] not config phy model!!!\n", __func__);
#endif
}

static int api_phy_config_resume(void)
{
#if defined(CONFIG_AW_PHY)
	return aw_phy_config_resume();
#else
	pr_err("[%s] not config phy model!!!\n", __func__);
#endif
	return 0;
}

static void api_set_phy_base(uintptr_t base)
{
#if defined(CONFIG_AW_PHY)
	HDMI_INFO_MSG("hdmi use aw phy!\n");
	aw_phy_set_reg_base(base);
#elif defined(CONFIG_AW_INNOPHY)
	HDMI_INFO_MSG("hdmi use inno phy!\n");
	inno_phy_set_reg_base(base);
#else
	HDMI_INFO_MSG("[%s] not config phy model!!!\n", __func__);
#endif
}

static u32 api_get_phy_power_state(void)
{
	return ctrl_phy_power_state(hdmi_api);
}

static u32 api_get_phy_pll_lock_state(void)
{
	return ctrl_phy_pll_lock_state(hdmi_api);
}

static u32 api_get_phy_rxsense_state(void)
{
	return  ctrl_phy_rxsense_state(hdmi_api);
}

#ifndef SUPPORT_ONLY_HDMI14
static int api_scdc_read(u8 address, u8 size, u8 *data)
{
	return scdc_read(hdmi_api, address, size, data);
}
#endif

static int api_scdc_write(u8 address, u8 size, u8 *data)
{
	return scdc_write(hdmi_api, address, size, data);
}
#if defined(__LINUX_PLAT__)

static void set_hdcp_on(u8 on)
{
	hdmi_api->snps_hdmi_ctrl.hdcp_on = on;
}

static u8 get_hdcp_on(void)
{
	return hdmi_api->snps_hdmi_ctrl.hdcp_on;
}

static void set_hdcp_encrypt_on(u8 on)
{
	hdmi_api->snps_hdmi_ctrl.hdcp_encrypt_on = on;
}

static u8 get_hdcp_encrypt_on(void)
{
	return hdmi_api->snps_hdmi_ctrl.hdcp_encrypt_on;
}

static u8 get_hdcp_type(void)
{
	return hdmi_api->snps_hdmi_ctrl.hdcp_type;
}

static u8 api_hdcp_event_handler(int *param, u32 irq_stat)
{
	return hdcp_event_handler(hdmi_api, param, irq_stat);
}

static int api_get_hdcp_status(void)
{
	return get_hdcp_status();
}
#endif
static void resistor_calibration(u32 reg, u32 data)
{
	dev_write(hdmi_api, reg * 4, data);
	dev_write(hdmi_api, (reg + 1) * 4, data >> 8);
	dev_write(hdmi_api, (reg + 2) * 4, data >> 16);
	dev_write(hdmi_api, (reg + 3) * 4, data >> 24);
}

static void api_set_hdmi_ctrl(hdmi_tx_dev_t *dev, videoParams_t *video)
{
	video_mode_t hdmi_on = 0;
	struct hdmi_tx_ctrl *tx_ctrl = &dev->snps_hdmi_ctrl;

	hdmi_on = video->mHdmi;
	tx_ctrl->hdmi_on = (hdmi_on == HDMI) ? 1 : 0;
	/*tx_ctrl->hdmi_on = 1;*/
	tx_ctrl->audio_on = (hdmi_on == HDMI) ? 1 : 0;
	/*tx_ctrl->audio_on = 1;*/
	tx_ctrl->pixel_clock = videoParams_GetPixelClock(dev, video);
	tx_ctrl->color_resolution = video->mColorResolution;
	tx_ctrl->pixel_repetition = video->mDtd.mPixelRepetitionInput;
	tx_ctrl->hspol = video->mDtd.mHSyncPolarity;
	tx_ctrl->vspol = video->mDtd.mVSyncPolarity;
}

static void api_avmute(hdmi_tx_dev_t *dev, int enable)
{
	packets_AvMute(dev, enable);
	hdcp_av_mute(dev, enable);
}

static int api_audio_configure(audioParams_t *audio, videoParams_t *video)
{
	int success = true;
	struct hdmi_tx_ctrl *tx_ctrl = &hdmi_api->snps_hdmi_ctrl;
	u32 tmds_clk = 0;

	api_set_hdmi_ctrl(hdmi_api, video);

	switch (video->mColorResolution) {
	case COLOR_DEPTH_8:
		tmds_clk = tx_ctrl->pixel_clock;
		break;
	case COLOR_DEPTH_10:
		if (video->mEncodingOut != YCC422)
			tmds_clk = tx_ctrl->pixel_clock * 125 / 100;
		else
			tmds_clk = tx_ctrl->pixel_clock;

		break;
	default:
		break;
	}
	tx_ctrl->tmds_clk = tmds_clk;
	/* Audio - Workaround */
	audio_Initialize(hdmi_api);
	success = audio_Configure(hdmi_api, audio);
	if (success == false)
		HDMI_INFO_MSG("ERROR:Audio not configured\n");

	return success;
}

static void api_fc_drm_up(fc_drm_pb_t *pb)
{
	fc_drm_up(hdmi_api, pb);
}

static int api_Standby(void)
{
	ctrl_phy_standby(hdmi_api);
	mc_disable_all_clocks(hdmi_api);

	hdmi_api->snps_hdmi_ctrl.hdmi_on = 1;
	hdmi_api->snps_hdmi_ctrl.pixel_clock = 0;
	hdmi_api->snps_hdmi_ctrl.color_resolution = 0;
	hdmi_api->snps_hdmi_ctrl.pixel_repetition = 0;
	hdmi_api->snps_hdmi_ctrl.audio_on = 1;

	return true;
}

static void api_hpd_enable(u8 enable)
{
	irq_hpd_sense_enable(hdmi_api, enable);
}

static u8 api_dev_hpd_status(void)
{
	return ctrl_phy_hot_plug_state(hdmi_api);
}
#if defined(__LINUX_PLAT__)

static void api_hdcp_configure(hdcpParams_t *hdcp, videoParams_t *video)
{
	hdcp_configure_new(hdmi_api, hdcp, video);
}

static void api_hdcp_disconfigure(void)
{
	hdcp_disconfigure_new(hdmi_api);
}
#endif
static int api_dtd_fill(dtd_t *dtd, u32 code, u32 refreshRate)
{
	return dtd_fill(hdmi_api, dtd, code, refreshRate);

}
#if defined(__LINUX_PLAT__)

static int api_edid_parser_cea_ext_reset(sink_edid_t *edidExt)
{
	return edid_parser_CeaExtReset(hdmi_api, edidExt);
}
static int api_edid_read(struct edid *edid)
{
	return edid_read(hdmi_api, edid);
}

int api_edid_extension_read(int block, u8 *edid_ext)
{
	return edid_extension_read(hdmi_api, block, edid_ext);
}

static int api_edid_parser(u8 *buffer, sink_edid_t *edidExt,
							u16 edid_size)
{
	return edid_parser(hdmi_api, buffer, edidExt, edid_size);
}
#endif

static int api_Configure(videoParams_t *video,
				audioParams_t *audio, productParams_t *product,
				hdcpParams_t *hdcp, u16 phy_model)
{
	int success = true;
	unsigned int tmds_clk = 0;
	hdmi_tx_dev_t				*dev = hdmi_api;

	LOG_TRACE();

	if (!video || !audio || !product || !hdcp) {
		HDMI_INFO_MSG("ERROR:There is NULL value arguments: video=%lx; audio=%lx; product=%lx; hdcp=%lx\n",
					(uintptr_t)video, (uintptr_t)audio,
					(uintptr_t)product, (uintptr_t)hdcp);
		return false;
	}

	api_set_hdmi_ctrl(dev, video);

	/*i2cddc_fast_mode(dev, 0);
	i2cddc_clk_config(dev, I2C_SFR_CLK,
			       I2C_MIN_SS_SCL_LOW_TIME,
			       I2C_MIN_SS_SCL_HIGH_TIME,
			       I2C_MIN_FS_SCL_LOW_TIME,
			       I2C_MIN_FS_SCL_HIGH_TIME);*/
	/*be used to calculate tmds clk*/
	HDMI_INFO_MSG("video pixel clock=%d\n", dev->snps_hdmi_ctrl.pixel_clock);
	/*for auto scrambling if tmds_clk > 3.4Gbps*/
	switch (video->mColorResolution) {
	case COLOR_DEPTH_8:
		tmds_clk = dev->snps_hdmi_ctrl.pixel_clock;
		break;
	case COLOR_DEPTH_10:
		if (video->mEncodingOut != YCC422)
			tmds_clk = dev->snps_hdmi_ctrl.pixel_clock * 125 / 100;
		else
			tmds_clk = dev->snps_hdmi_ctrl.pixel_clock;

		break;
	default:
		break;
	}
	dev->snps_hdmi_ctrl.tmds_clk = tmds_clk;

	if (video->mEncodingIn == YCC420) {
		dev->snps_hdmi_ctrl.pixel_clock = dev->snps_hdmi_ctrl.pixel_clock / 2;
		dev->snps_hdmi_ctrl.tmds_clk /= 2;
	}
	if (video->mEncodingIn == YCC422)
		dev->snps_hdmi_ctrl.color_resolution = 8;
#ifndef CONFIG_SUNXI_HDMI_IN_BOOT0
	api_avmute(dev, true);

	ctrl_phy_standby(dev);

	/* Disable interrupts */
	irq_mute(dev);

	success = video_Configure(dev, video);
	if (success == false)
		HDMI_INFO_MSG("Could not configure video\n");
#endif
	/* Audio - Workaround */
	audio_Initialize(dev);
	success = audio_Configure(dev, audio);
	if (success == false)
		HDMI_INFO_MSG("ERROR:Audio not configured\n");


#ifndef CONFIG_SUNXI_HDMI_IN_BOOT0
	/* Packets */
	success = packets_Configure(dev, video, product);
	if (success == false)
		HDMI_INFO_MSG("ERROR:Could not configure packets\n");

	mc_enable_all_clocks(dev);
	snps_sleep(10000);
#if defined(__LINUX_PLAT__)
	if ((dev->snps_hdmi_ctrl.tmds_clk  > 340000) && (video->scdc_ability)) {
		scrambling(dev, 1);
		HDMI_INFO_MSG("enable scrambling\n");
	} else if (video->scdc_ability) {
		scrambling(dev, 0);
		HDMI_INFO_MSG("disable scrambling\n");
	}
#else
	if (dev->snps_hdmi_ctrl.tmds_clk  > 340000) {
		scrambling(dev, 1);
		HDMI_INFO_MSG("enable scrambling\n");
	} else {
		scrambling(dev, 0);
		HDMI_INFO_MSG("disable scrambling\n");
	}
#endif

	/*add calibrated resistor configuration for all video resolution*/
	dev_write(dev, 0x40018, 0xc0);
	dev_write(dev, 0x4001c, 0x80);

	success = api_phy_configure(dev, phy_model, video->mEncodingOut);
	if (success == false)
		HDMI_INFO_MSG("ERROR:Could not configure PHY\n");

	/* Disable blue screen transmission
		after turning on all necessary blocks (e.g. HDCP) */
	fc_force_output(dev, false);

	/* TODO:This should be called at HPD event */
	/* HDCP is PHY independent */
	if (hdcp_initialize(dev) != true)
		HDMI_INFO_MSG("ERROR:Could not initialize HDCP\n");
#if defined(__LINUX_PLAT__)
	if (dev->snps_hdmi_ctrl.hdcp_on) {
		snps_sleep(20000);
		hdcp_configure_new(dev, hdcp, video);
	}
#endif
	irq_mask_all(dev);
	/* enable interrupts */
	irq_unmute(dev);

	snps_sleep(100000);
	api_avmute(dev, false);
#else
	mc_audio_sampler_clock_enable(dev,
				dev->snps_hdmi_ctrl.audio_on ? 0 : 1);

#endif
	return success;
}

static u32 api_get_audio_n(void)
{
	return _audio_clock_n_get(hdmi_api);
}

static u32 api_get_audio_layout(void)
{
	return fc_packet_layout_get(hdmi_api);
}

static u32 api_get_sample_freq(void)
{
	return audio_iec_sampling_freq_get(hdmi_api);
}

static u32 api_get_audio_sample_size(void)
{
	return audio_iec_word_length_get(hdmi_api);
}

static u32 api_get_audio_channel_count(void)
{
	return fc_channel_count_get(hdmi_api);
}

static u32 api_get_tmds_mode(void)
{
	return fc_video_tmdsMode_get(hdmi_api);
}

static u32 api_get_scramble_state(void)
{
	return scrambling_state(hdmi_api);
}

static u32 api_get_avmute_state(void)
{
	return hdcp_av_mute_state(hdmi_api);

}

static u32 api_get_pixelrepetion(void)
{
	return vp_PixelRepetitionFactor_get(hdmi_api);
}

static u32 api_get_colorimetry(void)
{
	return fc_Colorimetry_get(hdmi_api);
}

static u32 api_get_pixel_format(void)
{
	return fc_RgbYcc_get(hdmi_api);
}

static u32 api_get_video_code(void)
{
	return fc_VideoCode_get(hdmi_api);
}

void api_set_video_code(u8 data)
{
	fc_VideoCode_set(hdmi_api, data);
}

static void api_fc_vsif_get(u8 *data)
{
	fc_vsif_get(hdmi_api, data);
}

static void api_fc_vsif_set(u8 *data)
{
	fc_vsif_set(hdmi_api, data);
}

static u32 api_get_color_depth(void)
{
	return vp_ColorDepth_get(hdmi_api);
}

static void api_avmute_enable(u8 enable)
{
	return api_avmute(hdmi_api, enable);
}

static void api_phy_power_enable(u8 enable)
{
	return ctrl_phy_power_enable(hdmi_api, enable);
}

static void api_dvimode_enable(u8 enable)
{
	return fc_video_DviOrHdmi(hdmi_api, !enable);
}

void hdmitx_api_init(char *name)
{
	struct hdmi_dev_func func;
	struct hdmi_tx_ctrl *tx_ctrl;
	hdmi_api = kmalloc(sizeof(hdmi_tx_dev_t), GFP_KERNEL);
	if (!hdmi_api) {
		HDMI_INFO_MSG("ERROR:Malloc memory fail!\n");
		return ;
	}
	memset(&func, 0, sizeof(struct hdmi_dev_func));
	memset(hdmi_api, 0, sizeof(hdmi_tx_dev_t));
	memcpy(hdmi_api->device_name, name, sizeof(hdmi_api->device_name));

	tx_ctrl = &hdmi_api->snps_hdmi_ctrl;
	tx_ctrl->csc_on = 1;
	tx_ctrl->phy_access = PHY_I2C;
	tx_ctrl->data_enable_polarity = 1;
	tx_ctrl->phy_access = 1;

	func.main_config = api_Configure;
	func.audio_config = api_audio_configure;
	/*func.set_audio_on = api_set_audio_on;*/
#if defined(__LINUX_PLAT__)
	func.hdcp_configure = api_hdcp_configure;
	func.hdcp_disconfigure = api_hdcp_disconfigure;
	func.set_hdcp_on = set_hdcp_on;
	func.get_hdcp_on = get_hdcp_on;
	func.set_hdcp_encrypt_on = set_hdcp_encrypt_on;
	func.get_hdcp_encrypt_on = get_hdcp_encrypt_on;
	func.get_hdcp_type = get_hdcp_type;
	func.hdcp_event_handler = api_hdcp_event_handler;
	func.get_hdcp_status = api_get_hdcp_status;
#endif

	func.hpd_enable     = api_hpd_enable;
	func.dev_hpd_status = api_dev_hpd_status;
	func.dtd_fill       = api_dtd_fill;
#if defined(__LINUX_PLAT__)
	func.edid_parser_cea_ext_reset = api_edid_parser_cea_ext_reset;
	func.edid_read                 = api_edid_read;
	func.edid_parser               = api_edid_parser;
	func.edid_extension_read       = api_edid_extension_read;
#endif
	func.fc_drm_up        = api_fc_drm_up;
	func.device_standby       = api_Standby;
	func.resistor_calibration = resistor_calibration;

	func.phy_write = api_phy_write;
	func.phy_read  = api_phy_read;
	func.scdc_write = api_scdc_write;
	func.scdc_read = api_scdc_read;


	func.get_audio_n              = api_get_audio_n;
	func.get_audio_layout         = api_get_audio_layout;
	func.get_audio_sample_freq    = api_get_sample_freq;
	func.get_audio_sample_size    = api_get_audio_sample_size;
	func.get_audio_channel_count  = api_get_audio_channel_count;

	func.get_phy_rxsense_state    = api_get_phy_rxsense_state;
	func.get_phy_pll_lock_state   = api_get_phy_pll_lock_state;
	func.get_phy_power_state      = api_get_phy_power_state;
	func.get_tmds_mode            = api_get_tmds_mode;
	func.get_scramble_state       = api_get_scramble_state;
	func.get_avmute_state         = api_get_avmute_state;
	func.get_pixelrepetion        = api_get_pixelrepetion;
	func.get_colorimetry		  = api_get_colorimetry;
	func.get_pixel_format		  = api_get_pixel_format;
	func.get_video_code			  = api_get_video_code;
	func.set_video_code           = api_set_video_code;
	func.get_color_depth          = api_get_color_depth;
	func.get_vsif                 = api_fc_vsif_get;
	func.set_vsif                 = api_fc_vsif_set;

	func.avmute_enable		      = api_avmute_enable;
	func.phy_power_enable		  = api_phy_power_enable;
	func.dvimode_enable			  = api_dvimode_enable;
	func.set_phy_base_addr        = api_set_phy_base;
	func.phy_reset                = api_phy_reset;
	func.phy_config_resume        = api_phy_config_resume;

	register_func_to_hdmi_core(func);
}

void hdmitx_api_exit(void)
{
	kfree(hdmi_api);
	hdmi_api = NULL;
}

