/*
 * include/clk/clk_plat.h
 *
 * Copyright (c) 2007-2019 Allwinnertech Co., Ltd.
 * Author: zhengxiaobin <zhengxiaobin@allwinnertech.com>
 *
 * This software is licensed under the terms of the GNU General Public
 * License version 2, as published by the Free Software Foundation, and
 * may be copied, distributed, and modified under those terms.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 */
#ifndef __MACH_SUNXI_CLK__H
#define __MACH_SUNXI_CLK__H
#include "asm/io.h"
#include <common.h>
#include <malloc.h>
/*#include <linux/compat.h>*/
#include <linux/list.h>


#define BIT0	0x0001
#define BIT1	0x0002
#define BIT2	0x0004
#define BIT3	0x0008
#define BIT4	0x0010
#define BIT5	0x0020
#define BIT6	0x0040
#define BIT7	0x0080
#define BIT8	0x0100
#define BIT9	0x0200
#define BIT10	0x0400
#define BIT11	0x0800
#define BIT12	0x1000
#define BIT13	0x2000
#define BIT14	0x4000
#define BIT15	0x8000
#define BIT16	0x00010000
#define BIT17	0x00020000
#define BIT18	0x00040000
#define BIT19	0x00080000
#define BIT20	0x00100000
#define BIT21	0x00200000
#define BIT22	0x00400000
#define BIT23	0x00800000
#define BIT24	0x01000000
#define BIT25	0x02000000
#define BIT26	0x04000000
#define BIT27	0x08000000
#define BIT28	0x10000000
#define BIT29	0x20000000
#define BIT30	0x40000000
#define BIT31	0x80000000



#define CLK_SET_RATE_GATE	BIT0 /* must be gated across rate change */
#define CLK_SET_PARENT_GATE	BIT1 /* must be gated across re-parent */
#define CLK_SET_RATE_PARENT	BIT2 /* propagate rate change up one level */
#define CLK_IGNORE_UNUSED	BIT3 /* do not gate even if unused */
#define CLK_IS_ROOT		    BIT4 /* root clk, has no parent */
#define CLK_IS_BASIC		BIT5 /* Basic clk, can't do a to_clk_foo() */
#define CLK_GET_RATE_NOCACHE	BIT6 /* do not use the cached clk rate */
#define CLK_IGNORE_AUTORESET	BIT7 /* for sunxi use */
#define CLK_REVERT_ENABLE       BIT8 /* for sunxi use */
#define CLK_IGNORE_SYNCBOOT     BIT9 /* for sunxi use */
#define CLK_READONLY            BIT10 /* for sunxi use */
#define CLK_IGNORE_DISABLE      BIT11 /* for sunxi use */
#define CLK_RATE_FLAT_FACTORS   BIT12 /* for sunxi use */
#define CLK_RATE_FLAT_DELAY     BIT13 /* for sunxi use */
#define CLK_NO_DISABLE          BIT20 /* for sunxi use */
#define CLK_IGNORE_ENABLE_DISABLE BIT21 /* for sunxi use */


/*#define mdelay(ms) __msdelay(ms)*/
/*#define udelay(us) __udelay(us)*/
#define to_clk_factor(_hw) container_of(_hw, struct sunxi_clk_factors, hw)
#define SETMASK(width, shift)   ((width?((-1U) >> (32-width)):0)  << (shift))
#define CLRMASK(width, shift)   (~(SETMASK(width, shift)))
#define GET_BITS(shift, width, reg)     \
            (((reg) & SETMASK(width, shift)) >> (shift))
#define SET_BITS(shift, width, reg, val) \
            (((reg) & CLRMASK(width, shift)) | (val << (shift)))
#define DEFINE_SPINLOCK(lock) int lock


typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

struct clk {
	const char		        *name;
	const struct clk_ops	*ops;
	struct clk_hw		    *hw;
	struct clk		        *parent;
	const char		        **parent_names;
	struct clk		        **parents;
	u8			            num_parents;
	unsigned long		    rate;
	unsigned long		    new_rate;
	unsigned long		    flags;
	unsigned int		    enable_count;
	unsigned int		    prepare_count;
	struct hlist_head	    children;
	struct hlist_node	    child_node;
	unsigned int		    notifier_count;
#ifdef CONFIG_COMMON_CLK_DEBUG
	struct dentry		   *dentry;
#endif
};

struct clk_hw {
	struct clk *clk;
	const struct clk_init_data *init;
};

/**
 * struct clk_rate_request - Structure encoding the clk constraints that
 * a clock user might require.
 *
 * @rate:		Requested clock rate. This field will be adjusted by
 *			clock drivers according to hardware capabilities.
 * @min_rate:		Minimum rate imposed by clk users.
 * @max_rate:		Maximum rate imposed by clk users.
 * @best_parent_rate:	The best parent rate a parent can provide to fulfill the
 *			requested constraints.
 * @best_parent_hw:	The most appropriate parent clock that fulfills the
 *			requested constraints.
 *
 */
struct clk_rate_request {
	unsigned long rate;
	unsigned long min_rate;
	unsigned long max_rate;
	unsigned long best_parent_rate;
	struct clk_hw *best_parent_hw;
};

struct clk_ops {
	int		    (*prepare)(struct clk_hw *hw);
	void	    (*unprepare)(struct clk_hw *hw);
	int		    (*enable)(struct clk_hw *hw);
	void		(*disable)(struct clk_hw *hw);
	int		    (*is_enabled)(struct clk_hw *hw);
	unsigned long	(*recalc_rate)(struct clk_hw *hw,
						unsigned long parent_rate);
	long		(*round_rate)(struct clk_hw *hw, unsigned long,
					    unsigned long *);
	int		(*determine_rate)(struct clk_hw *hw,
					  struct clk_rate_request *req);
	int		    (*set_parent)(struct clk_hw *hw, u8 index);
	u8		    (*get_parent)(struct clk_hw *hw);
	int		    (*set_rate)(struct clk_hw *hw, unsigned long,
				        unsigned long);
	void		(*init)(struct clk_hw *hw);
};

struct clk_init_data {
	const char		        *name;
	const struct clk_ops	*ops;
	const char		        **parent_names;
	u8			            num_parents;
	unsigned long		    flags;
};

struct clk_fixed_rate {
	struct		    clk_hw hw;
	unsigned long	fixed_rate;
	u8		        flags;
};

struct clk_fixed_factor {
	struct clk_hw	hw;
	unsigned int	mult;
	unsigned int	div;
};

struct clk *clk_register_fixed_rate(void *dev, const char *name,
		const char *parent_name, unsigned long flags,
		unsigned long fixed_rate);

struct clk_hw *clk_hw_register_fixed_factor(void *dev,
		const char *name, const char *parent_name, unsigned long flags,
		unsigned int mult, unsigned int div);

struct clk *clk_register_fixed_factor(void *dev,
		const char *name, const char *parent_name, unsigned long flags,
		unsigned int mult, unsigned int div);

struct sunxi_reg_ops {
        u32 (*reg_readl)(void __iomem * reg);
        void (*reg_writel)(u32 val,void __iomem * reg);
};

int clk_divider_determine_rate(struct clk_hw *hw,
				      struct clk_rate_request *req);
void  init_clocks(void);


#endif
