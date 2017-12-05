/****************************************************************************
 * Copyright (C) 2017 Telechips Inc.
 *
 * This program is free software; you can redistribute it and/or modify it under the terms
 * of the GNU General Public License as published by the Free Software Foundation;
 * either version 2 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY;
 * without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
 * PURPOSE. See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 59 Temple Place,
 * Suite 330, Boston, MA 02111-1307 USA
 ****************************************************************************/

#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/miscdevice.h>
#include <linux/fs.h>
#include <linux/errno.h>

#include <linux/slab.h>
#include <linux/dma-buf.h>
#include <linux/dma-mapping.h>
#include <linux/interrupt.h>
#include <linux/wait.h>
#include <linux/sched.h>
#include <linux/clk.h>
#include <linux/poll.h>
#include <linux/of.h>
#include <linux/of_address.h>
#include <linux/of_device.h>
#include <linux/of_irq.h>
#include <asm/io.h>

#include <soc/tcc/pmap.h>

#ifdef CONFIG_ARCH_TCC898X
#include <video/tcc/vioc_intr.h>
#include <video/tcc/vioc_api.h>
#include <video/tcc/vioc_config.h>
#include <video/tcc/vioc_scaler.h>
#include <video/tcc/vioc_viqe.h>
#include <video/tcc/vioc_global.h>
#include <video/tcc/vioc_rdma.h>
#include <video/tcc/vioc_wdma.h>
#include <video/tcc/vioc_wmix.h>
#include <video/tcc/tvc_ioctrl.h>
#include <video/tcc/vioc_blk.h>
#else
#include <mach/vioc_intr.h>
#include <mach/vioc_api.h>
#include <mach/vioc_config.h>
#include <mach/vioc_scaler.h>
#include <mach/vioc_viqe.h>
#include <mach/vioc_global.h>
#include <mach/vioc_rdma.h>
#include <mach/vioc_wdma.h>
#include <mach/vioc_wmix.h>
#include <mach/tvc_ioctrl.h>
#endif

static int debug	   		= 0;
#define dprintk(msg...)	if(debug) { 	printk( "TVC_DRV:  " msg); 	}

struct tvc_data {
	// wait for poll
	wait_queue_head_t	poll_wq;
	spinlock_t			poll_lock;
	unsigned int		poll_count;

	// wait for ioctl command
	wait_queue_head_t	cmd_wq;
	spinlock_t			cmd_lock;
	unsigned int		cmd_count;

	struct mutex		io_mutex;
	unsigned char		block_operating;
	unsigned char		block_waiting;
	unsigned char		irq_reged;
	unsigned int		dev_open_cnt;
};

struct tvc_vioc_blocks {
	struct vioc_block_info rdmas[MAX_INPUT_CHANNELS];
	struct vioc_block_info wmix;
	struct vioc_block_info wdma;
	struct vioc_block_info sc;
	struct vioc_block_info di_block;
};

struct tvc_drv_type {
	struct vioc_intr_type	*vioc_intr;

	unsigned int		irq;
	struct miscdevice	*misc;

	struct clk		*clk;
	struct tvc_vioc_blocks *vioc;
	struct tvc_data	*data;
	struct tvc_common_ioctl_info		*info;
};

/*  support for dynamic_plugin */
static int sc_plug_status = 0;
static int di_plug_status = 0;

/*  support for deinterlacing */
static pmap_t pmap_viqe;
static int viqe_frm_cnt = 0;
static VIOC_VIQE_DEINTL_MODE viqe_di_mode = VIOC_VIQE_DEINTL_MODE_2D;
static struct tvc_dma_info buf_backup;

/*
 * External interface
 * */
#ifdef CONFIG_TVC_EXTRA_VIQE_SHARE
extern int tvc_extra_lock_vipe(unsigned int uType, unsigned int uPath);
extern void tvc_extra_unlock_viqe(unsigned int uType);
#endif

#ifdef CONFIG_TVC_EXTRA_DIRECT_DISPLAY
extern void tvc_extra_display_update(struct tvc_drv_type *pdata, struct tvc_dma_info *output, unsigned int enable);
#endif

extern void tccxxx_GetAddress(unsigned char format, unsigned int base_Yaddr, unsigned int src_imgx, unsigned int  src_imgy,
		                                unsigned int start_x, unsigned int start_y, unsigned int* Y, unsigned int* U,unsigned int* V);
extern int range_is_allowed(unsigned long pfn, unsigned long size);
static int tvc_drv_mmap(struct file *filp, struct vm_area_struct *vma)
{
	if (range_is_allowed(vma->vm_pgoff, vma->vm_end - vma->vm_start) < 0) {
		printk(KERN_ERR	 "%s():  This address is not allowed. \n", __func__);
		return -EAGAIN;
	}

	vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
	if (remap_pfn_range(vma,vma->vm_start, vma->vm_pgoff , vma->vm_end - vma->vm_start, vma->vm_page_prot)) {
		printk(KERN_ERR	 "%s():  Virtual address page port error. \n", __func__);
		return -EAGAIN;
	}

	vma->vm_ops	= NULL;
	vma->vm_flags 	|= VM_IO;
	vma->vm_flags 	|= VM_DONTEXPAND | VM_PFNMAP;

	return 0;
}

static unsigned int tvc_drv_poll(struct file *filp, poll_table *wait)
{
	struct miscdevice	*misc = (struct miscdevice *)filp->private_data;
	struct tvc_drv_type	*ptvc_data = dev_get_drvdata(misc->parent);
	int ret = 0;

	if(ptvc_data->data == NULL)
		return -EFAULT;

	poll_wait(filp, &(ptvc_data->data->poll_wq), wait);
	spin_lock_irq(&(ptvc_data->data->poll_lock));
	if (ptvc_data->data->block_operating == 0)
		ret = (POLLIN|POLLRDNORM);
	spin_unlock_irq(&(ptvc_data->data->poll_lock));

	return ret;
}

static void tvc_drv_path_reset(struct tvc_vioc_blocks *vioc)
{
	volatile PVIOC_IREQ_CONFIG pIREQConfig = VIOC_IREQConfig_GetAddress();
	int index = 0;

	if(vioc == NULL)
		return;

	/* swreset wdma */
	if(vioc->wdma.reg != NULL) {
		VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_WDMA, vioc->wdma.id, VIOC_CONFIG_RESET);
		VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_WDMA, vioc->wdma.id, VIOC_CONFIG_CLEAR);
	}

	/* swreset wmix */
	if(vioc->wmix.reg != NULL) {
		VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_WMIXER, vioc->wmix.id, VIOC_CONFIG_RESET);
		VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_WMIXER, vioc->wmix.id, VIOC_CONFIG_CLEAR);
	}

	/* swreset scaler */
	if(vioc->sc.reg != NULL) {
		VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_SCALER, vioc->sc.id, VIOC_CONFIG_RESET);
		VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_SCALER, vioc->sc.id, VIOC_CONFIG_CLEAR);
	}

	/* swreset deinterlacer */
	#ifdef CONFIG_ARCH_TCC898X
	if(vioc->di_block.type == VIOC_VIQE || vioc->di_block.type == VIOC_VIQE1)
	#else
	if(vioc->di_block.type == VIOC_VIQE)
	#endif
	{
		if(vioc->di_block.reg != NULL) {
			VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_VIQE, vioc->di_block.id, VIOC_CONFIG_RESET);
			VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_VIQE, vioc->di_block.id, VIOC_CONFIG_CLEAR);
		}
	} else if(vioc->di_block.type == VIOC_DEINTLS) {
		if(vioc->di_block.reg != NULL) {
			VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_DEINTS, vioc->di_block.id, VIOC_CONFIG_RESET);
			VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_DEINTS, vioc->di_block.id, VIOC_CONFIG_CLEAR);
		}
	}

	/* swreset rdma */
	for(index = 0; index < MAX_INPUT_CHANNELS; index++) {
		if(vioc->rdmas[index].reg != NULL) {
			VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_RDMA, vioc->rdmas[index].id, VIOC_CONFIG_RESET);
			VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_RDMA, vioc->rdmas[index].id, VIOC_CONFIG_CLEAR);
		}
	}
}

static int tvc_prepare_dt_parse(struct tvc_drv_type *ptvc_data, struct device_node *np)
{
	int ret = -1;

	if((ptvc_data->vioc != NULL) && (np != NULL))
	{
		struct of_phandle_args args;
		struct tvc_vioc_blocks *vioc = ptvc_data->vioc;
		unsigned int index = 0, channels = 0;

		/* dt parsing - rdma */
		of_property_read_u32(np, "input_channels", &channels);
		for(index = 0; index < channels; index++) {
			if(of_parse_phandle_with_args(np, "rdmas", "#vioc_rdma-cells", index, &args)) {
				printk(KERN_ERR"%s(): can't parse \"rdmas\" property \n", __func__);
				ret = -ENODEV;
				goto err_dt_parse;
			} else {
				vioc->rdmas[index].reg = of_iomap(args.np, 0);
				vioc->rdmas[index].id = of_alias_get_id(args.np, "vioc_rdma");
				if(args.args_count) {
					if(args.args[0] == 1)
						vioc->rdmas[index].type = 1;
					else
						vioc->rdmas[index].type = 0;
				}
				dprintk("%s: RDMA%d type: %d, reg: 0x%p\n", __func__, vioc->rdmas[index].id, vioc->rdmas[index].type, vioc->rdmas[index].reg);
			}
		}

		/* dt parsing - wmix */
		if(of_parse_phandle_with_args(np, "wmixs", "#vioc_wmix-cells", 0, &args)) {
			printk(KERN_ERR"%s(): can't parse \"wmixs\" property \n", __func__);
			ret = -ENODEV;
			goto err_dt_parse;
		} else {
			vioc->wmix.reg = of_iomap(args.np, 0);
			vioc->wmix.id = of_alias_get_id(args.np, "vioc_wmix");
			if(vioc->wmix.id < 0) {
				ret = -ENODEV;
				goto err_dt_parse;
			}

			if(args.args_count)
				vioc->wmix.path = args.args[0];
			else
				vioc->wmix.path = WMIX_MAX;

			dprintk("%s: WMIX%d path: %d, reg: 0x%p\n", __func__, vioc->wmix.id, vioc->wmix.path, vioc->wmix.reg);
		}

		/* dt parsing - scaler */
		if(of_parse_phandle_with_args(np, "scalers", "#vioc_sc-cells", 0, &args)) {
			printk(KERN_ERR"%s(): can't parse \"scalers\" property \n", __func__);
			ret = -ENODEV;
			goto err_dt_parse;
		} else {
			vioc->sc.reg = of_iomap(args.np, 0);
			vioc->sc.id = of_alias_get_id(args.np, "vioc_sc");
			if(vioc->sc.id < 0) {
				ret = -ENODEV;
				goto err_dt_parse;
			}

			if (args.args_count)
				vioc->sc.path = args.args[0];
			else
				vioc->sc.path = 0;

			dprintk("%s: Scaler%d path: %d, reg: 0x%p\n", __func__, vioc->sc.id, vioc->sc.path, vioc->sc.reg);
		}

		/* dt parsing - wdma */
		if(of_parse_phandle_with_args(np, "wdmas", "#vioc_wdma-cells", 0, &args)) {
			printk(KERN_ERR"%s(): can't parse \"wdmas\" property \n", __func__);
			ret = -ENODEV;
			goto err_dt_parse;
		} else {
			vioc->wdma.reg = of_iomap(args.np, 0);
			vioc->wdma.id = of_alias_get_id(args.np, "vioc_wdma");
			if(vioc->wmix.id < 0) {
				ret = -ENODEV;
				goto err_dt_parse;
			}
			ptvc_data->irq = of_irq_to_resource(args.np, 0, NULL);
			if(ptvc_data->vioc_intr) {
				ptvc_data->vioc_intr->id = VIOC_INTR_WD0 + vioc->wdma.id;
				ptvc_data->vioc_intr->bits = (0x1 << VIOC_WDMA_INTR_EOFR);
			}
			dprintk("%s: WDMA%d irq:0x%08x reg: 0x%p\n", __func__, vioc->wdma.id, ptvc_data->irq, vioc->wdma.reg);
		}

		/* dt parsing - deinterlacer */
		of_property_read_u32(np, "deinterlacer", &vioc->di_block.type);
		if(vioc->di_block.type) {
			struct device_node *dev_np = of_parse_phandle(np, "di_block", 0);
			if(dev_np) {
				of_property_read_u32_index(np, "di_block",1, &vioc->di_block.id);
				of_property_read_u32_index(np, "di_block",2, &vioc->di_block.path);
				vioc->di_block.reg = of_iomap(dev_np, vioc->di_block.id);
			}
			dprintk("%s: Deinterlacer type:%d path:%d reg: 0x%p\n", __func__, vioc->di_block.type, vioc->di_block.path, vioc->di_block.reg);
		} else {
			dprintk("%s: This driver does not support deinterlacing \n",__func__);
		}

		of_node_put(args.np);

		ret = 0;
	}

err_dt_parse:
	return ret;
}

static int tvc_drv_is_support_format(struct tvc_vioc_blocks *vioc, unsigned int format)
{
	int ret = -1;

	if (vioc != NULL)
	{
		int index;
		for(index = 0; index < MAX_INPUT_CHANNELS; index++)
		{
			if(vioc->rdmas[index].reg) {
				if(format >= VIOC_IMG_FMT_444SEP) {
					if(vioc->rdmas[index].type) {
						ret = index;
						break;
					}
				} else {
					ret = index;
					break;
				}
			} else {
				printk(KERN_ERR"%s: empty rdma info\n", __func__);
				break;
			}
		}
	}

	return ret;
}

static int tvc_drv_calculate_address(struct tvc_dma_info *dma)
{
	int ret = -1;
	if(dma != NULL)
	{
		tccxxx_GetAddress(dma->fmt.format, dma->addr0, dma->size.width, dma->size.height, dma->crop.left, dma->crop.top, &dma->addr0, &dma->addr1, &dma->addr2);
		ret = 0;
	}

	return ret;
}

static void tvc_drv_convert_color(struct vioc_block_info *rdma, struct vioc_block_info *viqe, struct tvc_common_ioctl_info *info)
{
	if(info != NULL)
	{
		unsigned int src_fmt = info->src.fmt.format;
		unsigned int dst_fmt = info->dst.fmt.format;

		if(rdma) {
			if((src_fmt >= VIOC_IMG_FMT_444SEP) && (dst_fmt >= VIOC_IMG_FMT_444SEP)) {
				/* from YUV to YUV */
				VIOC_RDMA_SetImageY2REnable(rdma->reg, false);
				VIOC_RDMA_SetImageR2YEnable(rdma->reg, false);
			} else if ((src_fmt >= VIOC_IMG_FMT_444SEP) && (dst_fmt < VIOC_IMG_FMT_444SEP)) {
				/* from YUV to RGB */
				VIOC_RDMA_SetImageY2REnable(rdma->reg, true);
				VIOC_RDMA_SetImageR2YEnable(rdma->reg, false);
			} else if ((src_fmt < VIOC_IMG_FMT_444SEP) && (dst_fmt < VIOC_IMG_FMT_444SEP)) {
				/* from RGB to RGB */
				VIOC_RDMA_SetImageY2REnable(rdma->reg, false);
				VIOC_RDMA_SetImageR2YEnable(rdma->reg, false);
			} else {
				/* from RGB to YUV */
				VIOC_RDMA_SetImageY2REnable(rdma->reg, false);
				VIOC_RDMA_SetImageR2YEnable(rdma->reg, true);
			}
		}

		if(viqe) {
			if((src_fmt >= VIOC_IMG_FMT_444SEP) && (dst_fmt >= VIOC_IMG_FMT_444SEP)) {
				/* from YUV to YUV */
				VIOC_RDMA_SetImageY2REnable(rdma->reg, false);
				VIOC_VIQE_SetImageY2REnable(viqe->reg, false);
			} else if ((src_fmt >= VIOC_IMG_FMT_444SEP) && (dst_fmt < VIOC_IMG_FMT_444SEP)) {
				/* from YUV to RGB */
				VIOC_RDMA_SetImageY2REnable(rdma->reg, false);
				VIOC_VIQE_SetImageY2REnable(viqe->reg, true);
			} else if ((src_fmt < VIOC_IMG_FMT_444SEP) && (dst_fmt < VIOC_IMG_FMT_444SEP)) {
				/* from RGB to RGB */
				VIOC_RDMA_SetImageY2REnable(rdma->reg, false);
				VIOC_VIQE_SetImageY2REnable(viqe->reg, false);
			} else {
				/* from RGB to YUV */
				VIOC_RDMA_SetImageY2REnable(rdma->reg, false);
				VIOC_VIQE_SetImageY2REnable(viqe->reg, false);
			}
		}
	}
}

static int tvc_drv_check_bufsize(struct tvc_dma_info *dma)
{
	int ret = 0;

	if(dma != NULL)
	{
		if((buf_backup.size.width != dma->size.width) || (buf_backup.size.height != dma->size.height)
				|| (buf_backup.crop.left != dma->crop.left) || (buf_backup.crop.top != dma->crop.top)
				|| (buf_backup.crop.right != dma->crop.right) || (buf_backup.crop.bottom != dma->crop.bottom)
		  )
		{
			dprintk("%s: Before(%dx%d)(%d,%d/%dx%d) ==> After(%dx%d)(%d,%d/%dx%d)\n", __func__,
					buf_backup.size.width, buf_backup.size.height, buf_backup.crop.left, buf_backup.crop.top, buf_backup.crop.right, buf_backup.crop.bottom,
					dma->size.width, dma->size.height, dma->crop.left, dma->crop.top, dma->crop.right, dma->crop.bottom);
			ret = 1;
		}
	}

	return ret;
}

static int tvc_drv_setup_rdma(struct vioc_block_info *rdma, struct tvc_common_ioctl_info *info)
{
	int ret = -1;

	#ifdef CONFIG_ARCH_TCC898X
	if (tcc_support_dma_path_config()) {
		int tvc_num = tcc_get_dma_path_select(rdma->id);
		tcc_set_dma_path_unset(tvc_num);
		tcc_set_dma_path_set(TVC_RDMA(rdma->id), TVC_RDMA(rdma->id));
	}
	#endif

	if((rdma != NULL) && (info != NULL))
	{
		/* calculate source base address */
		tvc_drv_calculate_address(&info->src);
		tvc_drv_convert_color(rdma, NULL, info);

		/* set-up rdma */
#ifdef CONFIG_VIOC_10BIT
		if(info->src.fmt.bitdepth == 10) {
			VIOC_RDMA_SetDataFormat(rdma->reg, 0x1, 0x1);
			VIOC_RDMA_SetImageOffset(rdma->reg, info->src.fmt.format, info->src.size.width*2);
		} else if(info->src.fmt.bitdepth == 11) {
			VIOC_RDMA_SetDataFormat(rdma->reg, 0x3, 0x0);	/* YUV real 10bit support */
			VIOC_RDMA_SetImageOffset(rdma->reg, info->src.fmt.format, (info->src.size.width*125)/100);
		} else {
			VIOC_RDMA_SetDataFormat(rdma->reg, 0x0, 0x0);
			VIOC_RDMA_SetImageOffset(rdma->reg, info->src.fmt.format, info->src.size.width);
		}
#else
		VIOC_RDMA_SetImageOffset(rdma->reg, info->src.fmt.format, info->src.size.width);
#endif
		VIOC_RDMA_SetImageBase(rdma->reg, info->src.addr0, info->src.addr1, info->src.addr2);
		VIOC_RDMA_SetImageSize(rdma->reg, (info->src.crop.right - info->src.crop.left), (info->src.crop.bottom - info->src.crop.top));
		VIOC_RDMA_SetImageFormat(rdma->reg, info->src.fmt.format);
		VIOC_RDMA_SetImageIntl(rdma->reg, info->src.scan.interlace);
		VIOC_RDMA_SetImageBfield(rdma->reg, info->src.scan.field);

		dprintk("RDMA%d(%d) size: %dx%d crop: (%d,%d)(%d,%d) fmt: %d Base: 0x%08x 0x%08x 0x%08x\n", rdma->id, info->src.scan.interlace,
			info->src.size.width, info->src.size.height, info->src.crop.left, info->src.crop.top, info->src.crop.right, info->src.crop.bottom,
			info->src.fmt.format, info->src.addr0, info->src.addr1, info->src.addr2);
		ret = 0;
	}

	return ret;
}

static int tvc_drv_setup_wdma(struct vioc_block_info *wdma, struct tvc_common_ioctl_info *info)
{
	int ret = -1;

	if((wdma != NULL) && (info != NULL))
	{
		/* calculate destination base address */
		tvc_drv_calculate_address(&info->dst);

		VIOC_WDMA_SetImageBase(wdma->reg, info->dst.addr0, info->dst.addr1, info->dst.addr2);
		VIOC_WDMA_SetImageSize(wdma->reg, info->dst.size.width, info->dst.size.height);
		VIOC_WDMA_SetImageFormat(wdma->reg, info->dst.fmt.format);
		VIOC_WDMA_SetImageOffset(wdma->reg, info->dst.fmt.format, info->dst.size.width);
		VIOC_WDMA_SetImageInterlaced(wdma->reg, info->dst.scan.interlace);
		VIOC_WDMA_SetImageR2YEnable(wdma->reg, 0);
		VIOC_WDMA_SetImageY2REnable(wdma->reg, 0);
		VIOC_WDMA_SetImageEnable(wdma->reg, 0);

		dprintk("WDMA%d(%d) size: %dx%d fmt: %d Base: 0x%08x 0x%08x 0x%08x\n", wdma->id, info->dst.scan.interlace,
			info->dst.size.width, info->dst.size.height, info->dst.fmt.format,
			info->dst.addr0, info->dst.addr1, info->dst.addr2);

		ret = 0;
	}

	return ret;
}

static int tvc_drv_setup_wmix(struct vioc_block_info *wmix, struct tvc_common_ioctl_info *info)
{
	int ret = -1;

	if((wmix != NULL) && (info != NULL))
	{
		if(wmix->path == WMIX13)
			VIOC_CONFIG_WMIXPath(wmix->path, 0); // wmixer op is bypass mode.
		else
			VIOC_CONFIG_WMIXPath(wmix->path, 1); // wmixer op is mixing mode.

		VIOC_WMIX_SetOverlayPriority(wmix->reg, 24);	/* ovp 24: 0-1-2-3 */
		VIOC_WMIX_SetSize(wmix->reg, info->dst.size.width, info->dst.size.height);
		VIOC_WMIX_SetUpdate(wmix->reg);

		dprintk("WMIX%d size: %dx%d\n", wmix->id, info->dst.size.width, info->dst.size.height);
		ret = 0;
	}

	return ret;
}

static int tvc_drv_setup_scaler(struct vioc_block_info *scaler, struct tvc_common_ioctl_info *info)
{
	int ret = -1;

	if((scaler != NULL) && (info != NULL))
	{
		unsigned int bypass = 0;

		/* set-up scaler */
		if(((info->src.crop.right - info->src.crop.left) != info->dst.size.width) || ((info->src.crop.bottom - info->src.crop.top) != info->dst.size.height))
			bypass = 0;
		else
			bypass = 1;
		VIOC_SC_SetBypass(scaler->reg, bypass);
		VIOC_SC_SetOutPosition(scaler->reg, 0, 0);
		VIOC_SC_SetDstSize(scaler->reg, info->dst.size.width, info->dst.size.height);
		VIOC_SC_SetOutSize(scaler->reg, info->dst.size.width, info->dst.size.height);

		if(!sc_plug_status) {
			VIOC_CONFIG_PlugIn(scaler->id, scaler->path);
			sc_plug_status = 1;
		}

		VIOC_SC_SetUpdate(scaler->reg);

		dprintk("SC%d(%d) DST/OUT size: %dx%d\n", scaler->id, bypass, info->dst.size.width, info->dst.size.height);
		ret = 0;
	}

	return ret;
}

static int tvc_drv_setup_viqe(struct vioc_block_info *viqe, struct vioc_block_info *rdma, struct tvc_common_ioctl_info *info, unsigned int reset_frm_cnt)
{
	int ret = -1;

	if((info != NULL) && (viqe != NULL) && (rdma != NULL))
	{
		unsigned int framebufWidth, framebufHeight, framebufSize;
		unsigned int viqe_dma_base0, viqe_dma_base1, viqe_dma_base2, viqe_dma_base3;
		int top_size_dont_use = OFF;		//If this value is OFF, The size information is get from VIOC modules.
		VIOC_VIQE_FMT_TYPE viqe_img_fmt = VIOC_VIQE_FMT_YUV420;

		if(info->src.fmt.format < VIOC_IMG_FMT_444SEP) {
			printk(KERN_ERR"%s: Not supported format(%d) \n", __func__, info->src.fmt.format);
			goto err_viqe_process;
		}

		if(reset_frm_cnt) {
			viqe_frm_cnt = 0;
			viqe_di_mode = VIOC_VIQE_DEINTL_MODE_2D;
		}

		if(viqe_frm_cnt == 0) {
			pmap_get_info("viqe", &pmap_viqe);
			framebufWidth = ROUND_DOWN_8((info->src.crop.right - info->src.crop.left));
			framebufHeight = ROUND_DOWN_4((info->src.crop.bottom - info->src.crop.top));
			framebufSize = (framebufWidth * framebufHeight * 2);

			if(pmap_viqe.size < (framebufSize * 4)) {
				printk(KERN_ERR"%s: Pmap size is not enough for VIQE%d\n", __func__, viqe->id);
				goto err_viqe_process;
			}

			viqe_dma_base0 = pmap_viqe.base;
			viqe_dma_base1 = viqe_dma_base0 + framebufSize;
			viqe_dma_base2 = viqe_dma_base1 + framebufSize;
			viqe_dma_base3 = viqe_dma_base2 + framebufSize;

			VIOC_VIQE_SetControlRegister(viqe->reg, framebufWidth, framebufHeight, viqe_img_fmt);
			VIOC_VIQE_SetDeintlRegister(viqe->reg, viqe_img_fmt, top_size_dont_use, framebufWidth, framebufHeight, viqe_di_mode, viqe_dma_base0, viqe_dma_base1, viqe_dma_base2, viqe_dma_base3);
			VIOC_VIQE_SetControlEnable(viqe->reg, 0, 0, 0, 0, 1);
			VIOC_VIQE_SetImageY2RMode(viqe->reg, info->src.fmt.y2rmode);

			tvc_drv_convert_color(rdma, viqe, info);

			if(!di_plug_status) {
				VIOC_CONFIG_PlugIn(viqe->type, viqe->path);
				di_plug_status = 1;
			}
		} else {
			if(viqe_frm_cnt >= 3) {
				unsigned int JudderCnt = (info->src.size.width + 64)/64 -1;
				viqe_di_mode = VIOC_VIQE_DEINTL_MODE_3D;
				VIOC_VIQE_SetDeintlMode(viqe->reg, viqe_di_mode);
				VIOC_VIQE_SetDeintlJudderCnt(viqe->reg, JudderCnt);
			} else {
				viqe_di_mode = VIOC_VIQE_DEINTL_MODE_2D;
				VIOC_VIQE_SetDeintlMode(viqe->reg, viqe_di_mode);
			}
		}

		viqe_frm_cnt++;
		ret = 0;
	}

err_viqe_process:
	return ret;
}

static int tvc_drv_setup_deinterlace(struct vioc_block_info *di_block, struct vioc_block_info *rdma, struct tvc_common_ioctl_info *info)
{
	int ret = -1;

	if((di_block != NULL) && (rdma != NULL) && (info != NULL))
	{
		#ifdef CONFIG_ARCH_TCC898X
		if(di_block->type == VIOC_VIQE || di_block->type == VIOC_VIQE1)
		#else
		if(di_block->type == VIOC_VIQE)
		#endif
		{
			#ifdef CONFIG_TVC_EXTRA_VIQE_SHARE
			if(tvc_extra_lock_vipe(di_block->type, di_block->path) >= 0) {
				if(info->src.scan.interlace) {
					if(tvc_drv_setup_viqe(di_block, rdma, info, tvc_drv_check_bufsize(&info->src)) < 0) {
						ret = -1;
						goto err_di_setup;
					}
				} else {
					if(di_plug_status) {
						VIOC_VIQE_SetDeintlMode(di_block->reg, VIOC_VIQE_DEINTL_MODE_BYPASS);
						VIOC_VIQE_SetControlMode(di_block->reg, OFF, OFF, OFF, OFF, OFF);
						tvc_drv_convert_color(rdma, NULL, info);
						viqe_di_mode = VIOC_VIQE_DEINTL_MODE_BYPASS;
					}
				}
			} else {
				viqe_frm_cnt = 0;
				viqe_di_mode = VIOC_VIQE_DEINTL_MODE_2D;
			}
			#else
			if(info->src.scan.interlace) {
				if(tvc_drv_setup_viqe(di_block, rdma, info, tvc_drv_check_bufsize(&info->src)) < 0) {
					ret = -1;
					goto err_di_setup;
				}
			} else {
				if(di_plug_status) {
					VIOC_VIQE_SetDeintlMode(di_block->reg, VIOC_VIQE_DEINTL_MODE_BYPASS);
					VIOC_VIQE_SetControlMode(di_block->reg, OFF, OFF, OFF, OFF, OFF);
					tvc_drv_convert_color(rdma, NULL, info);
					viqe_di_mode = VIOC_VIQE_DEINTL_MODE_BYPASS;
				}
			}
			#endif
		} else if (di_block->type == VIOC_DEINTLS) {
			if(info->src.scan.interlace)
				VIOC_CONFIG_PlugIn(di_block->type, di_block->path);
			else
				VIOC_CONFIG_PlugOut(di_block->type);
		} else {
			dprintk("%s: this driver does not support deinterlacing \n", __func__);
		}

		VIOC_RDMA_SetImageEnable(rdma->reg);
		ret = 0;
	}

err_di_setup:
	return ret;
}

static irqreturn_t tvc_drv_handler(int irq, void *client_data)
{
	struct tvc_drv_type *ptvc_data = (struct tvc_drv_type *)client_data;

	if (!is_vioc_intr_activatied(ptvc_data->vioc_intr->id, ptvc_data->vioc_intr->bits))
		return IRQ_NONE;

	vioc_intr_clear(ptvc_data->vioc_intr->id, ptvc_data->vioc_intr->bits);

	dprintk("%s():  block_operating(%d), block_waiting(%d), cmd_count(%d), poll_count(%d). \n", __func__, 	\
			ptvc_data->data->block_operating, ptvc_data->data->block_waiting, ptvc_data->data->cmd_count, ptvc_data->data->poll_count);

	if(ptvc_data->data->block_operating >= 1)
		ptvc_data->data->block_operating = 0;

	wake_up_interruptible(&(ptvc_data->data->poll_wq));
	if(ptvc_data->data->block_waiting)
		wake_up_interruptible(&ptvc_data->data->cmd_wq);

	return IRQ_HANDLED;
}

static int tvc_drv_mixing_function(struct tvc_drv_type *ptvc_data, struct tvc_common_ioctl_info *info)
{
	struct tvc_vioc_blocks *vioc = NULL;
	int ret = 0, rdma_idx;
	dprintk("==> %s():  \n", __func__);

	if((info == NULL) || (ptvc_data->vioc == NULL)) {
		printk(KERN_ERR"%s: Invaild parameter\n", __func__);
		return -EINVAL;
	}

	vioc = ptvc_data->vioc;

	ret = tvc_drv_is_support_format(vioc, info->src.fmt.format);
	if(ret < 0)
		return -EINVAL;
	else
		rdma_idx = ret;

	spin_lock_irq(&(ptvc_data->data->cmd_lock));

	ret = tvc_drv_setup_rdma(&vioc->rdmas[rdma_idx], info);
	if(ret < 0)
		goto err_status;

	ret = tvc_drv_setup_deinterlace(&vioc->di_block, &vioc->rdmas[rdma_idx], info);
	if(ret < 0)
		goto err_status;

	ret = tvc_drv_setup_scaler(&vioc->sc, info);
	if(ret < 0)
		goto err_status;

	ret = tvc_drv_setup_wmix(&vioc->wmix, info);
	if(ret < 0)
		goto err_status;

	ret = tvc_drv_setup_wdma(&vioc->wdma, info);
	if(ret < 0)
		goto err_status;
	vioc_intr_clear(ptvc_data->vioc_intr->id, ptvc_data->vioc_intr->bits);

	spin_unlock_irq(&(ptvc_data->data->cmd_lock));

	if (ptvc_data->info->responsetype  == TVC_POLLING) {
		ret = wait_event_interruptible_timeout(ptvc_data->data->poll_wq,  ptvc_data->data->block_operating == 0, msecs_to_jiffies(500));
		if (ret <= 0) {
			ptvc_data->data->block_operating = 0;
			dprintk("%s():  time out(%d), line(%d). \n", __func__, ret, __LINE__);
		}
	}

	#ifdef CONFIG_TVC_EXTRA_VIQE_SHARE
	tvc_extra_unlock_viqe(vioc->di_block.type);
	#endif

	#ifdef CONFIG_TVC_EXTRA_DIRECT_DISPLAY
	tvc_extra_display_update(ptvc_data, &info->dst, 1);
	#endif

	return ret;

err_status:
	spin_unlock_irq(&(ptvc_data->data->cmd_lock));

	printk(KERN_ERR"%s: Invaild parameters \n", __func__);
	return -EINVAL;
}

static long tvc_drv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	int ret = 0;
	struct miscdevice	*misc = (struct miscdevice *)filp->private_data;
	struct tvc_drv_type	*ptvc_data = dev_get_drvdata(misc->parent);
	/* dma_buf specific structure */
	struct dma_buf *dma_buf_user;
	struct dma_buf_attachment *attach;
	struct sg_table *sgt;
	dma_addr_t dma_addr;

	dprintk("%s():  cmd(%d), block_operating(%d), block_waiting(%d), cmd_count(%d), poll_count(%d). \n", __func__, 	\
			cmd, ptvc_data->data->block_operating, ptvc_data->data->block_waiting, ptvc_data->data->cmd_count, ptvc_data->data->poll_count);

	switch(cmd) {
		case TVC_DRV_MIXING_IOCTL:
		case TVC_DRV_MIXING_IOCTL_KERNEL:
			mutex_lock(&ptvc_data->data->io_mutex);
			if(ptvc_data->data->block_operating) {
				ptvc_data->data->block_waiting = 1;
				ret = wait_event_interruptible_timeout(ptvc_data->data->cmd_wq, ptvc_data->data->block_operating == 0, msecs_to_jiffies(200));
				if(ret <= 0) {
					ptvc_data->data->block_operating = 0;
					printk("%s(%d):  timed_out block_operation(%d), cmd_count(%d). \n", __func__, ret, ptvc_data->data->block_waiting, ptvc_data->data->cmd_count);
				}
				ret = 0;
			}

			if(cmd == TVC_DRV_MIXING_IOCTL_KERNEL) {
				memcpy(ptvc_data->info, (struct tvc_common_ioctl_info*)arg, sizeof(struct tvc_common_ioctl_info));
			} else {
				if(copy_from_user(ptvc_data->info, (struct tvc_common_ioctl_info*)arg, sizeof(struct tvc_common_ioctl_info))) {
					printk(KERN_ALERT "%s():  Not Supported copy_from_user(%d). \n", __func__, cmd);
					ret = -EFAULT;
				}
			}

			if(ret >= 0) {
				if(ptvc_data->data->block_operating >= 1) {
					printk("%s():  block_operating(%d), block_waiting(%d), cmd_count(%d), poll_count(%d). \n", __func__, 	\
							ptvc_data->data->block_operating, ptvc_data->data->block_waiting, ptvc_data->data->cmd_count, ptvc_data->data->poll_count);
				}

				ptvc_data->data->block_waiting = 0;
				ptvc_data->data->block_operating = 1;

				/* attach dma-buf and get the physical address. */
				dma_buf_user = dma_buf_get(ptvc_data->info->dmabuf_fd);
				attach = dma_buf_attach(dma_buf_user, misc->this_device);
				sgt = dma_buf_map_attachment(attach, DMA_BIDIRECTIONAL);
				dma_addr = sg_dma_address(sgt->sgl);

				ptvc_data->info->dst.addr0 = dma_addr;
				dprintk("%s:%d dma_addr = %p dmabuf_fd = %d\n", __func__, __LINE__, dma_addr, ptvc_data->info->dmabuf_fd);

				ret = tvc_drv_mixing_function(ptvc_data, ptvc_data->info);
				if(ret < 0)
					ptvc_data->data->block_operating = 0;

				/* detach dma-buf. */
				dma_buf_unmap_attachment(attach, sgt, DMA_BIDIRECTIONAL);
				dma_buf_detach(dma_buf_user, attach);
				dma_buf_put(dma_buf_user);

				/* backup previous buffer information */
				memcpy(&buf_backup, &ptvc_data->info->src, sizeof(struct tvc_dma_info));
			}
			mutex_unlock(&ptvc_data->data->io_mutex);
			return ret;
		default:
			printk(KERN_ALERT "%s():  Not Supported ioctl(%d). \n", __func__, cmd);
			break;
	}

	return 0;
}

static int tvc_drv_release(struct inode *inode, struct file *filp)
{
	int ret = 0;
	struct miscdevice	*misc = (struct miscdevice *)filp->private_data;
	struct tvc_drv_type	*ptvc_data = dev_get_drvdata(misc->parent);

	dprintk("%s():  In -open(%d), block(%d), wait(%d), cmd(%d), irq(%d) \n", __func__, ptvc_data->data->dev_open_cnt, ptvc_data->data->block_operating, \
			ptvc_data->data->block_waiting, ptvc_data->data->cmd_count, ptvc_data->data->irq_reged);

	if(ptvc_data->data->dev_open_cnt > 0)
		ptvc_data->data->dev_open_cnt--;

	if(ptvc_data->data->dev_open_cnt == 0) {
		if (ptvc_data->data->block_operating) {
			ret = wait_event_interruptible_timeout(ptvc_data->data->cmd_wq, ptvc_data->data->block_operating == 0, msecs_to_jiffies(50));
			if (ret <= 0)
				printk(" %s %s(ret:%d):  timed_out block_operation:%d, cmd_count:%d. \n",misc->name, __func__, ret, ptvc_data->data->block_waiting, ptvc_data->data->cmd_count);
		}

		/* reinit backup buffer */
		memset(&buf_backup, 0x0, sizeof(struct tvc_dma_info));

		if (ptvc_data->data->irq_reged) {
			vioc_intr_disable(ptvc_data->vioc_intr->id, ptvc_data->vioc_intr->bits);
			free_irq(ptvc_data->irq, ptvc_data);
			ptvc_data->data->irq_reged = 0;
		}

		if(ptvc_data->vioc->sc.reg != NULL) {
			VIOC_CONFIG_PlugOut(ptvc_data->vioc->sc.id);
			sc_plug_status = 0;
		}

		if(ptvc_data->vioc->di_block.reg != NULL) {
			VIOC_CONFIG_PlugOut(ptvc_data->vioc->di_block.type);
			di_plug_status = 0;
		}

		tvc_drv_path_reset(ptvc_data->vioc);

		viqe_frm_cnt = 0;
		viqe_di_mode = VIOC_VIQE_DEINTL_MODE_2D;

		ptvc_data->data->block_operating = ptvc_data->data->block_waiting = 0;
		ptvc_data->data->poll_count = ptvc_data->data->cmd_count = 0;

		#ifdef CONFIG_TVC_EXTRA_DIRECT_DISPLAY
		tvc_extra_display_update(ptvc_data, NULL, 0);
		#endif
	}

	if(ptvc_data->clk)
		clk_disable_unprepare(ptvc_data->clk);

	dprintk("%s : %s():  Out - open(%d). \n",misc->name, __func__, ptvc_data->data->dev_open_cnt);
	return 0;
}

static int tvc_drv_open(struct inode *inode, struct file *filp)
{
	struct miscdevice	*misc = (struct miscdevice *)filp->private_data;
	struct tvc_drv_type	*ptvc_data = dev_get_drvdata(misc->parent);
	int ret = 0;

	dprintk("%s():  In -open(%d), block(%d), wait(%d), cmd(%d), irq(%d) \n", __func__,
			ptvc_data->data->dev_open_cnt, ptvc_data->data->block_operating, \
			ptvc_data->data->block_waiting, ptvc_data->data->cmd_count, ptvc_data->data->irq_reged);

	if (ptvc_data->clk)
		clk_prepare_enable(ptvc_data->clk);

	if(ptvc_data->data->dev_open_cnt == 0)
	{
		/*reinit backup buffer*/
		memset(&buf_backup, 0x0, sizeof(struct tvc_dma_info));
		di_plug_status = 0;
		viqe_frm_cnt = 0;
		viqe_di_mode = VIOC_VIQE_DEINTL_MODE_2D;

		/* swreset vioc blocks */
		tvc_drv_path_reset(ptvc_data->vioc);

#if defined(CONFIG_ARCH_TCC897X) || defined(CONFIG_ARCH_TCC570X) || defined(CONFIG_ARCH_TCC802X)
		vioc_config_stop_req(0);
#else
		vioc_config_stop_req(1);
#endif

		/* interrupt handler register */
		synchronize_irq(ptvc_data->irq);
		vioc_intr_clear(ptvc_data->vioc_intr->id, ptvc_data->vioc_intr->bits);
		ret = request_irq(ptvc_data->irq, tvc_drv_handler, IRQF_SHARED, ptvc_data->misc->name, ptvc_data);
		if (ret) {
			if (ptvc_data->clk)
				clk_disable_unprepare(ptvc_data->clk);

			printk("failed to aquire %s request_irq. \n", ptvc_data->misc->name);
			return -EFAULT;
		}

		vioc_intr_enable(ptvc_data->vioc_intr->id, ptvc_data->vioc_intr->bits);
		ptvc_data->data->irq_reged = 1;
	}

	ptvc_data->data->dev_open_cnt++;
	dprintk("%s():  Out - open(%d). \n", __func__, ptvc_data->data->dev_open_cnt);

	return ret;
}

static struct file_operations tvc_drv_fops = {
	.owner			= THIS_MODULE,
	.unlocked_ioctl	= tvc_drv_ioctl,
	.mmap			= tvc_drv_mmap,
	.open			= tvc_drv_open,
	.release		= tvc_drv_release,
	.poll			= tvc_drv_poll,
};

static int tvc_drv_probe(struct platform_device *pdev)
{
	struct tvc_drv_type *ptvc_data;
	int ret = -ENODEV;

	ptvc_data = kzalloc(sizeof(struct tvc_drv_type), GFP_KERNEL);
	if (!ptvc_data)
		return -ENOMEM;

	ptvc_data->clk = of_clk_get(pdev->dev.of_node, 0);
	if (IS_ERR(ptvc_data->clk))
		ptvc_data->clk = NULL;

	ptvc_data->misc = kzalloc(sizeof(struct miscdevice), GFP_KERNEL);
	if (ptvc_data->misc == 0)
		goto err_misc_alloc;

	ptvc_data->info = kzalloc(sizeof(struct tvc_common_ioctl_info), GFP_KERNEL);
	if (ptvc_data->info == 0)
		goto err_info_alloc;

	ptvc_data->data = kzalloc(sizeof(struct tvc_data), GFP_KERNEL);
	if (ptvc_data->data == 0)
		goto err_data_alloc;

	ptvc_data->vioc = kzalloc(sizeof(struct tvc_vioc_blocks), GFP_KERNEL);
	if(ptvc_data->vioc == 0)
		goto err_vioc_alloc;

	ptvc_data->vioc_intr = kzalloc(sizeof(struct vioc_intr_type), GFP_KERNEL);
	if (ptvc_data->vioc_intr == 0)
		goto err_vioc_intr_alloc;

 	/* register tvc misc device */
	ptvc_data->misc->minor = MISC_DYNAMIC_MINOR;
	ptvc_data->misc->fops = &tvc_drv_fops;
	ptvc_data->misc->name = pdev->name;
	ptvc_data->misc->parent = &pdev->dev;
	ret = misc_register(ptvc_data->misc);
	if (ret)
		goto err_misc_register;

	dprintk("%s name : %s  alias  id:%d \n", __func__, pdev->name, of_alias_get_id(pdev->dev.of_node, "tvc_drv"));

	/* device tree parse */
	tvc_prepare_dt_parse(ptvc_data, pdev->dev.of_node);

	spin_lock_init(&(ptvc_data->data->poll_lock));
	spin_lock_init(&(ptvc_data->data->cmd_lock));

	mutex_init(&(ptvc_data->data->io_mutex));

	init_waitqueue_head(&(ptvc_data->data->poll_wq));
	init_waitqueue_head(&(ptvc_data->data->cmd_wq));

	platform_set_drvdata(pdev, ptvc_data);

	dprintk("%s ====  \n", pdev->name);
	return 0;

	misc_deregister(ptvc_data->misc);
err_misc_register:
	kfree(ptvc_data->vioc_intr);
err_vioc_intr_alloc:
	kfree(ptvc_data->data);
err_vioc_alloc:
	kfree(ptvc_data->vioc);
err_data_alloc:
	kfree(ptvc_data->info);
err_info_alloc:
	kfree(ptvc_data->misc);
err_misc_alloc:
	kfree(ptvc_data);

	printk("%s: %s: err ret:%d \n", __func__, pdev->name, ret);
	return ret;
}

static int tvc_drv_remove(struct platform_device *pdev)
{
	struct tvc_drv_type *ptvc_data = (struct tvc_drv_type *)platform_get_drvdata(pdev);

	misc_deregister(ptvc_data->misc);
	kfree(ptvc_data->vioc_intr);
	kfree(ptvc_data->data);
	kfree(ptvc_data->info);
	kfree(ptvc_data->misc);
	kfree(ptvc_data);
	return 0;
}

static int tvc_drv_suspend(struct platform_device *pdev, pm_message_t state)
{
	// TODO:
	return 0;
}

static int tvc_drv_resume(struct platform_device *pdev)
{
	// TODO:
	return 0;
}

static struct of_device_id tvc_of_match[] = {
	{ .compatible = "telechips,tvc_drv" },
	{}
};
MODULE_DEVICE_TABLE(of, tvc_of_match);

static struct platform_driver tvc_driver = {
	.probe		= tvc_drv_probe,
	.remove		= tvc_drv_remove,
	.suspend	= tvc_drv_suspend,
	.resume		= tvc_drv_resume,
	.driver 	= {
		.name	= "tvc_pdev",
		.owner	= THIS_MODULE,
#ifdef CONFIG_OF
		.of_match_table	= of_match_ptr(tvc_of_match),
#endif
	},
};

static int __init tvc_drv_init(void)
{
	dprintk("%s \n", __func__);
	return platform_driver_register(&tvc_driver);
}

static void __exit tvc_drv_exit(void)
{
	platform_driver_unregister(&tvc_driver);
}

module_init(tvc_drv_init);
module_exit(tvc_drv_exit);


MODULE_AUTHOR("Telechips.");
MODULE_DESCRIPTION("Telechips TVC Driver");
MODULE_LICENSE("GPL");

