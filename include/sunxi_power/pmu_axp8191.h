/*
 * Copyright (C) 2019 Allwinner.
 * liufeng <liufeng@allwinnertech.com>
 *
 * SUNXI AXP8191  Driver
 *
 * SPDX-License-Identifier: GPL-2.0+
 */

#ifndef __AXP8191_H__
#define __AXP8191_H__

/* Default to using I2C communication */
#define AXP8191_DEVICE_ADDR       (0x3A3)
#ifndef CONFIG_SYS_SUNXI_R_I2C0_SLAVE
#define AXP8191_RUNTIME_ADDR      (0x2d)
#else
#ifndef CONFIG_AXP8191_SUNXI_I2C_SLAVE
#define AXP8191_RUNTIME_ADDR      CONFIG_SYS_SUNXI_R_I2C0_SLAVE
#else
#define AXP8191_RUNTIME_ADDR      CONFIG_AXP8191_SUNXI_I2C_SLAVE
#endif
#endif

/* define AXP8191 REGISTER */
#define   AXP8191_IC_TYPE                     (0x03)
#define   AXP8191_CHIP_ID                     (0x0E)
#define   AXP8191_CHIP_VER                    (0x0F)
#define   AXP8191_DCDC_POWER_ON_OFF_CTL1      (0x10)
#define   AXP8191_DCDC_POWER_ON_OFF_CTL2      (0x11)

#define   AXP8191_DC1OUT_VOL                  (0x12)
#define   AXP8191_DC2OUT_VOL                  (0x13)
#define   AXP8191_DC3OUT_VOL                  (0x14)
#define   AXP8191_DC4OUT_VOL                  (0x15)
#define   AXP8191_DC5OUT_VOL                  (0x16)
#define   AXP8191_DC6OUT_VOL                  (0x17)
#define   AXP8191_DC7OUT_VOL                  (0x18)
#define   AXP8191_DC8OUT_VOL                  (0x19)
#define   AXP8191_DC9OUT_VOL                  (0x1A)

#define   AXP8191_DCDC_MODE_CTL1                (0x1B)
#define   AXP8191_DCDC_MODE_CTL2                (0x1C)
#define   AXP8191_DCDC_MODE_CTL3                (0x1D)
#define   AXP8191_DCDC_MODE_CTL4                (0x1E)

#define   AXP8191_DC8SET_STATUS                 (0x1F)

#define   AXP8191_LDO_POWER_ON_OFF_CTL1         (0x20)
#define   AXP8191_LDO_POWER_ON_OFF_CTL2         (0x21)
#define   AXP8191_LDO_POWER_ON_OFF_CTL3         (0x22)
#define   AXP8191_LDO_POWER_ON_OFF_CTL4         (0x23)

#define   AXP8191_ALDO1OUT_VOL                  (0x24)
#define   AXP8191_ALDO2OUT_VOL                  (0x25)
#define   AXP8191_ALDO3OUT_VOL                  (0x26)
#define   AXP8191_ALDO4OUT_VOL                  (0x27)
#define   AXP8191_ALDO5OUT_VOL                  (0x28)
#define   AXP8191_ALDO6OUT_VOL                  (0x29)

#define   AXP8191_BLDO1OUT_VOL                  (0x2A)
#define   AXP8191_BLDO2OUT_VOL                  (0x2B)
#define   AXP8191_BLDO3OUT_VOL                  (0x2C)
#define   AXP8191_BLDO4OUT_VOL                  (0x2D)
#define   AXP8191_BLDO5OUT_VOL                  (0x2E)

#define   AXP8191_CLDO1OUT_VOL                  (0x2F)
#define   AXP8191_CLDO2OUT_VOL                  (0x30)
#define   AXP8191_CLDO3OUT_VOL                  (0x31)
#define   AXP8191_CLDO4OUT_VOL                  (0x32)
#define   AXP8191_CLDO5OUT_VOL                  (0x33)

#define   AXP8191_DLDO1OUT_VOL                  (0x34)
#define   AXP8191_DLDO2OUT_VOL                  (0x35)
#define   AXP8191_DLDO3OUT_VOL                  (0x36)
#define   AXP8191_DLDO4OUT_VOL                  (0x37)
#define   AXP8191_DLDO5OUT_VOL                  (0x38)
#define   AXP8191_DLDO6OUT_VOL                  (0x39)

#define   AXP8191_ELDO1OUT_VOL                  (0x3A)
#define   AXP8191_ELDO2OUT_VOL                  (0x3B)
#define   AXP8191_ELDO3OUT_VOL                  (0x3C)
#define   AXP8191_ELDO4OUT_VOL                  (0x3D)
#define   AXP8191_ELDO5OUT_VOL                  (0x3E)
#define   AXP8191_ELDO6OUT_VOL                  (0x3F)

#define    AXP8191_IRQ_ENABLE1                  (0x40)
#define    AXP8191_IRQ_ENABLE2                  (0x41)
#define    AXP8191_IRQ_ENABLE3                  (0x42)
#define    AXP8191_IRQ_ENABLE4                  (0x43)

#define    AXP8191_IRQ_STATUS1                  (0x48)
#define    AXP8191_IRQ_STATUS2                  (0x49)
#define    AXP8191_IRQ_STATUS3                  (0x4A)
#define    AXP8191_IRQ_STATUS4                  (0x4B)

#define    AXP8191_POWER_ON_OFF_SOURCE_INDIVATION        (0x50)
#define    AXP8191_POWER_OFF_SOURCE_INDIVATION           (0x51)
#define    AXP8191_POWER_ON_OFF_SOURCE_EN1               (0x52)
#define    AXP8191_POWER_ON_OFF_SOURCE_EN2               (0x53)
#define    AXP8191_OVP_DISCHARGE_TEMPERATURE_CFG         (0x54)
#define    AXP8191_POWER_DISABLE_POWER_DOWN_SEQUENCE     (0x55)
#define    AXP8191_WAKEUP_CTRL_VOFF_SET                  (0x56)
#define    AXP8191_POWERON_LEVEL_POWEROF_SET             (0x57)
#define    AXP8191_AUTO_SLEEP_CFG1                       (0x58)
#define    AXP8191_AUTO_SLEEP_CFG2                       (0x59)
#define    AXP8191_AUTO_SLEEP_CFG3                       (0x5A)
#define    AXP8191_AUTO_SLEEP_CFG4                       (0x5B)
#define    AXP8191_AUTO_SLEEP_CFG5                       (0x5C)
#define    AXP8191_AUTO_SLEEP_CFG6                       (0x5D)
#define    AXP8191_TS_CTRL                               (0x60)
#define    AXP8191_TS_HYSL2H                             (0x61)
#define    AXP8191_TS_HYSH2L                             (0x62)
#define    AXP8191_VLTF_WORK                             (0x63)
#define    AXP8191_VHTF_WORK                             (0x64)
#define    AXP8191_TS_ADC_EN_DATA_H                      (0x65)
#define    AXP8191_TS_ADC_DATA_L                         (0x66)
#define    AXP8191_TDIE_ADC_EN_DATA_H                    (0x67)
#define    AXP8191_TDIE_ADC_DATA_L                       (0x68)
#define    AXP8191_GPADC_EN_DATA_H                       (0x69)
#define    AXP8191_GPADC_DATA_L                          (0x6A)
#define    AXP8191_PS_ADC_EN_DATA_H                      (0x6B)
#define    AXP8191_PS_ADC_DATA_L                         (0x6C)

#define    AXP8191_CHANNEL_DEBUG_ADC_SEL                 (0x6D)

#define    AXP8191_ADC_CTL                               (0x6E)
#define    AXP8191_ADC_CTL                               (0x6E)
#define    AXP8191_GPIO_FUNC_CTL                         (0x70)
#define    AXP8191_GPIO_INPUT_CTL                        (0x71)
#define    AXP8191_GPIO_OUPUT_CTL                        (0x72)
#define    AXP8191_PWM_CTL1                              (0x73)
#define    AXP8191_PWM_CTL2                              (0x74)
#define    AXP8191_PWM_CTL3                              (0x75)
#define    AXP8191_BACKUP_BAT_CHARGE_CTL                 (0x76)
#define    AXP8191_WATCHDOG_CFG                          (0x77)
#define    AXP8191_WRITE_LOCK_F1                         (0xF0)
#define    AXP8191_EFUSE_CTL                             (0xF1)
#define    AXP8191_VREF                                  (0xF2)
#define    AXP8191_SCL_SDA_CFG                           (0xF3)
#define    AXP8191_REG_ADD_EXT                           (0xFF)

#define    AXP8191_COMMOM_CFG1                           (0x100)
#define    AXP8191_COMMOM_CFG2                           (0x101)
#define    AXP8191_COMMOM_CFG3                           (0x102)
#define    AXP8191_COMMOM_CFG4                           (0x103)

#define    AXP8191_DCDC1VOL_DEFAULT_SET                  (0x104)
#define    AXP8191_DCDC2VOL_DEFAULT_SET                  (0x105)
#define    AXP8191_DCDC3VOL_DEFAULT_SET                  (0x106)
#define    AXP8191_DCDC4VOL_DEFAULT_SET                  (0x107)
#define    AXP8191_DCDC5VOL_DEFAULT_SET                  (0x108)
#define    AXP8191_DCDC6VOL_DEFAULT_SET                  (0x109)
#define    AXP8191_DCDC7VOL_DEFAULT_SET                  (0x10A)
#define    AXP8191_DCDC8VOL_DEFAULT_SET                  (0x10B)
#define    AXP8191_DCDC9VOL_DEFAULT_SET                  (0x10C)

#define    AXP8191_ALDO1_ALDO2_VOL_DEFAULT_SET           (0x10D)
#define    AXP8191_ALDO3_ALDO4_VOL_DEFAULT_SET           (0x10E)
#define    AXP8191_ALDO5_ALDO6_VOL_DEFAULT_SET           (0x10F)

#define    AXP8191_BLDO1_BLDO2_VOL_DEFAULT_SET           (0x110)
#define    AXP8191_BLDO3_BLDO4_VOL_DEFAULT_SET           (0x111)
#define    AXP8191_BLDO5_CLDO1_VOL_DEFAULT_SET           (0x112)
#define    AXP8191_CLDO2_CLDO3_VOL_DEFAULT_SET           (0x113)
#define    AXP8191_CLDO4_CLDO5_VOL_DEFAULT_SET           (0x114)

#define    AXP8191_DLDO1_CLDO2_VOL_DEFAULT_SET           (0x115)
#define    AXP8191_DLDO3_CLDO4_VOL_DEFAULT_SET           (0x116)
#define    AXP8191_DLDO5_CLDO6_VOL_DEFAULT_SET           (0x117)

#define    AXP8191_ELDO1_ELDO2_VOL_DEFAULT_SET           (0x118)
#define    AXP8191_ELDO3_ELDO4_VOL_DEFAULT_SET           (0x119)
#define    AXP8191_ELDO5_ELDO6_VOL_DEFAULT_SET           (0x11A)


#define    AXP8191_DC8_OUTPUT_SET                        (0x12A)
#define    AXP8191_RTC_DCXO_CTL_VOFF                     (0x12C)
#define    AXP8191_DCDC_OC                               (0x12D)
#define    AXP8191_FANOUT_SEQ_TRIM                       (0x12E)
#define    AXP8191_VREF_VRPN_TUNING                      (0x12F)
#define    AXP8191_VREF_VOLTAGE_TUNING                   (0x130)
#define    AXP8191_BIAS_TUNING                           (0x131)
#define    AXP8191_FREQUENCY_TUNING                      (0x132)
#define    AXP8191_ADC_TUNING                            (0x133)
#define    AXP8191_DCDC_TRIM1                            (0x135)
#define    AXP8191_DCDC_TRIM2                            (0x136)

#define    AXP8191_PAGE_SELECT                           (0x137)
#define    AXP8191_TEST_MODE_CTL1                        (0x180)
#define    AXP8191_TEST_MODE_CTL2                        (0x181)
#define    AXP8191_TEST_MODE_CTL3                        (0x188)
#define    AXP8191_DCDC_DEBUG1                           (0x18A)
#define    AXP8191_DCDC_DEBUG2                           (0x18B)
#define    AXP8191_DCDC_DEBUG3                           (0x18C)
#define    AXP8191_DCDC_DEBUG4                           (0x18D)

#define   AXP8191_DCDC1_PWM_BIT                            (7)
#define   AXP8191_DCDC9_PWM_BIT                            (4)

int axp8191_probe_power_key(void);
int axp8191_set_ddr_voltage(int set_vol);
int axp8191_set_ddr4_2v5_voltage(int set_vol);
int axp8191_set_pll_voltage(int set_vol);
int axp8191_set_efuse_voltage(int set_vol);
int axp8191_set_sys_voltage(int set_vol, int onoff);
int axp8191_set_sys_voltage_ext(char *name, int set_vol, int onoff);
int axp8191_axp_init(u8 power_mode);
int axp8191_get_power_source(void);

#endif /* __AXP8191_REGS_H__ */

