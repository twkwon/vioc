/****************************************************************************
 * Copyright (C) 2015 Telechips Inc.
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

#include <mach/vioc_intr.h>
#include <mach/of_vioc_rdma.h>
#include <mach/of_vioc_wmix.h>
#include <mach/of_vioc_sc.h>
#include <mach/of_vioc_wdma.h>


#include <mach/tcc_scaler_ioctrl.h>
#include <mach/vioc_api.h>
#include <mach/vioc_config.h>
#include <mach/vioc_scaler.h>
#include <mach/vioc_rdma.h>
#include <mach/vioc_wdma.h>
#include <mach/vioc_wmix.h>
#include <mach/vioc_plugin.h>
#include <mach/iomap.h>

#include <mach/tvc_drv.h>
//#include <mach/bsp.h>
//#include <mach/vioc_config.h>
//#include <mach/vioc_scaler.h>
//#include <mach/vioc_api.h>
//#include <mach/vioc_global.h>
//#include <mach/tcc_wmixer_ioctrl.h>



static int debug	   		= 0;
#define dprintk(msg...)	if(debug) { 	\
	printk("TVC:" msg );}

#define DBUG_RDMA 		0
#define DBUG_SC 			0
#define DBUG_WMIXER 	0
#define DBUG_WDMA 		0
//static int debug	   		= 0;
//#define dprintk(msg...)	if(debug) { 	printk( "TCC_SCALER1:  " msg); 	}



struct tvc_data {
	// wait for poll
	wait_queue_head_t	poll_wq;
	spinlock_t		poll_lock;
	unsigned int		poll_count;

	// wait for ioctl command
	wait_queue_head_t	cmd_wq;
	spinlock_t		cmd_lock;
	unsigned int		cmd_count;

	struct mutex		io_mutex;
	unsigned char		block_operating;
	unsigned char		block_waiting;
	unsigned char		irq_reged;
	unsigned int		dev_open_cnt;
};

struct tvc_drv_type {
	struct vioc_intr_type	*vioc_intr;

	unsigned int		irq;
	struct miscdevice	*misc;

	struct vioc_rdma_device	*rdma[MAX_RDMA_CHANNEL];
 	struct vioc_wmix_device	*wmix;
	struct vioc_sc_device		*sc;
	struct vioc_wdma_device	*wdma;

	struct clk		*clk;
	struct tvc_data	*data;
	struct tvc_common_ioctl_info		*info;
};

extern int range_is_allowed(unsigned long pfn, unsigned long size);

static void dev_rdma_debug(struct vioc_rdma_device *rdma)
{
#if DBUG_RDMA
	struct _vioc_rdma_data *data = &rdma->_data;

	printk("RDMA@%p addr : 0x%x 0x%x 0x%x , size : %d %d , offset:%d %d fmt:%d swap:%d \n", rdma->reg,
		data->base0, data->base1, data->base2, data->width, data->height, data->offsetY, data->offsetUV, data->format, data->swap);
#endif//
}

static void dev_wdma_debug(struct vioc_wdma_device *wdma)
{
#if DBUG_WDMA
	struct _vioc_wdma_data *data = &wdma->_data;

	printk("WDMA@%p addr : 0x%x 0x%x 0x%x , size : %d %d , offset:%d %d fmt:%d swap:%d \n", wdma->reg, 
		data->base0, data->base1, data->base2, data->width, data->height, data->offsetY, data->offsetUV, data->format, data->swap);
#endif//	
}

static void dev_sc_debug(struct vioc_sc_device *sc)
{
#if DBUG_SC
	struct vioc_sc_data *data = &sc->data;
	printk("SCALER@%p bypass :%d dest:%d x %d , output : %d x %d : %d x %d \n", sc->reg,
		data->bypass, data->dst_width, data->dst_height, data->out_x, data->out_y, data->out_width, data->out_height);
#endif//
}

static void dev_wmixer_debug(struct vioc_wmix_device *wmix)
{
#if DBUG_WMIXER
	printk("WMIXER@%p  size : %d  x %d \n", wmix->reg, wmix->size.width, wmix->size.height);
#endif//
}

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

static void tvc_driver_block_reset(struct tvc_drv_type *ptvc_data)
{
	int i;
	// enable reset
	if(ptvc_data->wdma != NULL)
		vioc_wdma_swreset(ptvc_data->wdma, 1);

	if(ptvc_data->sc != NULL)
		vioc_sc_swreset(ptvc_data->sc, 1);
	
	for(i = 0; i < MAX_RDMA_CHANNEL; i++)
	{
		if(ptvc_data->rdma[i] != NULL)
			vioc_rdma_swreset(ptvc_data->rdma[i], 1);
	}
// rdma 
	if(ptvc_data->wmix != NULL)
		vioc_wmix_swreset(ptvc_data->wmix, 1);


	//release reset
	for(i = 0; i < MAX_RDMA_CHANNEL; i++)
	{
		if(ptvc_data->rdma[i] != NULL)
			vioc_rdma_swreset(ptvc_data->rdma[i], 0);
	}

	if(ptvc_data->sc != NULL)
		vioc_sc_swreset(ptvc_data->sc, 0);

	if(ptvc_data->wmix != NULL)
		vioc_wmix_swreset(ptvc_data->wmix, 0);

	if(ptvc_data->wdma != NULL)
		vioc_wdma_swreset(ptvc_data->wdma, 0);
	
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



static irqreturn_t tvc_drv_handler(int irq, void *client_data)
{
	struct tvc_drv_type *ptvc_data = (struct tvc_drv_type *)client_data;

	if (is_vioc_intr_activatied(ptvc_data->vioc_intr->id, ptvc_data->vioc_intr->bits) == false)
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


static char tcc_tvc_full_function(struct tvc_drv_type *ptvc_data, struct tvc_common_ioctl_info *info)
{
	int i = 0, ret = 0;
	int rdma_cnt = 0, rdma_id[MAX_RDMA_CHANNEL];

	dprintk("==> %s():  \n", __func__);

	spin_lock_irq(&(ptvc_data->data->cmd_lock));

	for(i = 0; i < MAX_RDMA_CHANNEL; i++)
	{
		struct tvc_rdma_info *src = &info->src[i];

		//rdma setting.
		if (ptvc_data->rdma) 
		{

			if((src->size.width == 0) || (src->size.height == 0))	{
				_vioc_rdma_set_image(ptvc_data->rdma[i], &ptvc_data->rdma[i]->_data, 0);
			}
			else {
				ptvc_data->rdma[i]->_data.base0 = (unsigned int)src->addr.addr0;
				ptvc_data->rdma[i]->_data.base1 = (unsigned int)src->addr.addr1;
				ptvc_data->rdma[i]->_data.base2 = (unsigned int)src->addr.addr2;

				ptvc_data->rdma[i]->_data.width = src->size.width;
				ptvc_data->rdma[i]->_data.height = src->size.height;
				ptvc_data->rdma[i]->_data.offsetY = src->size.offsetY;
				ptvc_data->rdma[i]->_data.offsetUV = src->size.offsetUV;

				ptvc_data->rdma[i]->_data.format = src->format.format;
				ptvc_data->rdma[i]->_data.swap = src->format.swap;
				ptvc_data->rdma[i]->_data.interlace = src->intl;
				
				ptvc_data->rdma[i]->_data.en_y2r = src->y2r_en;
				ptvc_data->rdma[i]->_data.y2r_mode = src->y2rmode;

				ptvc_data->rdma[i]->_data.en_r2y = src->r2y_en;
				ptvc_data->rdma[i]->_data.r2y_mode = src->r2ymode;

				dev_rdma_debug(ptvc_data->rdma[i]);

				_vioc_rdma_set_image(ptvc_data->rdma[i], &ptvc_data->rdma[i]->_data, 1);
				rdma_id[rdma_cnt] = ptvc_data->rdma[i]->id;
				rdma_cnt++;

			}

		}
	}

	if(rdma_cnt == 0)	{
		pr_info("%s driver cann't find enabed RDMA \n",ptvc_data->misc->name );
		goto err_available_data;
	}
	else 
	{
		int wmixer_bypass = -1;  // if mix_bypass < 0 , mixer is used.
		
		if(rdma_cnt == 1)	{
			wmixer_bypass = vioc_rdma_to_wmix_path(rdma_id[0], 0);// WMIX bypass mode
		}
		else{
			for(i = 0; i < rdma_cnt; i++)
				vioc_rdma_to_wmix_path(rdma_id[i], 1);// WMIX mixing mode
		}

		if ((wmixer_bypass < 0) &&  (ptvc_data->wmix))
		{
			ptvc_data->wmix->size.width= info->wmixer.width;
			ptvc_data->wmix->size.height = info->wmixer.height;

			dev_wmixer_debug(ptvc_data->wmix);
			vioc_wmix_set_size(ptvc_data->wmix, 1);
		}

	}
		
	if (ptvc_data->sc) {
		ptvc_data->sc->data.bypass = 0;
		ptvc_data->sc->data.dst_width = info->scaler.dest_width;
		ptvc_data->sc->data.dst_height = info->scaler.dest_height;
		ptvc_data->sc->data.out_x = info->scaler.pos_x;
		ptvc_data->sc->data.out_y = info->scaler.pos_y;
		ptvc_data->sc->data.out_width = info->scaler.out_width;
		ptvc_data->sc->data.out_height = info->scaler.out_height;


		/* set plug-in */
		ptvc_data->sc->path = sc_plugin_rdma_index(ptvc_data->rdma[0]->id);
		if(ptvc_data->sc->path < 0)
		{
			pr_err("%s  plugin number over range rdma ID: %d \n", ptvc_data->misc->name, ptvc_data->rdma[0]->id);
			goto err_available_data;
		}

		dev_sc_debug(ptvc_data->sc);
		vioc_sc_set_image(ptvc_data->sc, 1);
	}


	

	if (ptvc_data->wdma) {
		ptvc_data->wdma->_data.format = ptvc_data->info->wdma.format.format;
		ptvc_data->wdma->_data.swap = ptvc_data->info->wdma.format.swap;
		ptvc_data->wdma->_data.width = ptvc_data->info->wdma.size.width;
		ptvc_data->wdma->_data.height = ptvc_data->info->wdma.size.height;
		ptvc_data->wdma->_data.offsetY = ptvc_data->info->wdma.size.offsetY;
		ptvc_data->wdma->_data.offsetUV = ptvc_data->info->wdma.size.offsetUV;
		ptvc_data->wdma->_data.base0 = (unsigned int)ptvc_data->info->wdma.addr.addr0;
		ptvc_data->wdma->_data.base1 = (unsigned int)ptvc_data->info->wdma.addr.addr1;
		ptvc_data->wdma->_data.base2 = (unsigned int)ptvc_data->info->wdma.addr.addr2;
		ptvc_data->wdma->_data.cont = 0;

		dev_wdma_debug(ptvc_data->wdma);
		_vioc_wdma_set_image(ptvc_data->wdma, 1);
	}
	spin_unlock_irq(&(ptvc_data->data->cmd_lock));
	if (ptvc_data->info->responsetype  == TVC_RSP_POLLING) {


		ret = wait_event_interruptible_timeout(ptvc_data->data->poll_wq,  ptvc_data->data->block_operating == 0, msecs_to_jiffies(500));
		if (ret <= 0) {
			ptvc_data->data->block_operating = 0;
			printk("%s():  time out(%d), line(%d). \n", __func__, ret, __LINE__);
		}		
	} else if (ptvc_data->info->responsetype  == TVC_RSP_NOWAIT) {
		
	}

	return ret;

err_available_data:
	spin_unlock_irq(&(ptvc_data->data->cmd_lock));
	return 0;

}



static long tvc_drv_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	struct miscdevice	*misc = (struct miscdevice *)filp->private_data;
	struct tvc_drv_type	*ptvc_data = dev_get_drvdata(misc->parent);


	int ret = 0;

	dprintk("%s():  cmd(%d), block_operating(%d), block_waiting(%d), cmd_count(%d), poll_count(%d). \n", __func__, 	\
			cmd, ptvc_data->data->block_operating, ptvc_data->data->block_waiting, ptvc_data->data->cmd_count, ptvc_data->data->poll_count);

	switch(cmd) {
		case TVC_FULL_FUNC_IOCTL:
		case TVC_FULL_FUNC_KERNEL_IOCTL:
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

			if(cmd == TVC_FULL_FUNC_KERNEL_IOCTL) {
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
				ret = tcc_tvc_full_function(ptvc_data, ptvc_data->info);
				if(ret < 0) 
					ptvc_data->data->block_operating = 0;
			}
			mutex_unlock(&ptvc_data->data->io_mutex);
			return ret;

		default:
			printk(KERN_ALERT "%s():  Not Supported SCALER0_IOCTL(%d). \n", __func__, cmd);
			break;			
	}

	return 0;
}

static int tvc_drv_release(struct inode *inode, struct file *filp)
{
	struct miscdevice	*misc = (struct miscdevice *)filp->private_data;
	struct tvc_drv_type	*ptvc_data = dev_get_drvdata(misc->parent);

	int ret = 0;
	dprintk("%s():  In -open(%d), block(%d), wait(%d), cmd(%d), irq(%d) \n", __func__, ptvc_data->data->dev_open_cnt, ptvc_data->data->block_operating, \
			ptvc_data->data->block_waiting, ptvc_data->data->cmd_count, ptvc_data->data->irq_reged);

	if (ptvc_data->data->dev_open_cnt > 0) {
		ptvc_data->data->dev_open_cnt--;
	}

	if (ptvc_data->data->dev_open_cnt == 0) {
		if (ptvc_data->data->block_operating) {
			ret = wait_event_interruptible_timeout(ptvc_data->data->cmd_wq, ptvc_data->data->block_operating == 0, msecs_to_jiffies(50));
			if (ret <= 0) {
	 			printk(" %s %s(ret:%d):  timed_out block_operation:%d, cmd_count:%d. \n",misc->name, __func__, ret, ptvc_data->data->block_waiting, ptvc_data->data->cmd_count);
			}
		}

		if (ptvc_data->data->irq_reged) {
			free_irq(ptvc_data->irq, ptvc_data);
			ptvc_data->data->irq_reged = 0;
		}


		VIOC_CONFIG_PlugOut(ptvc_data->sc->type);

		tvc_driver_block_reset(ptvc_data);

		ptvc_data->data->block_operating = ptvc_data->data->block_waiting = 0;
		ptvc_data->data->poll_count = ptvc_data->data->cmd_count = 0;
	}

	if (ptvc_data->clk)
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
//		tvc_driver_block_reset(ptvc_data);


		#if defined(CONFIG_ARCH_TCC896X) || (CONFIG_ARCH_TCC897X)
			// Don't STOP_REQ clear because of dual output problem(DISP FIFO under-run), when video is out.
//			vioc_config_stop_req(0);
		#else
			vioc_config_stop_req(1);
		#endif

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
	.unlocked_ioctl		= tvc_drv_ioctl,
	.mmap			= tvc_drv_mmap,
	.open			= tvc_drv_open,
	.release		= tvc_drv_release,
	.poll			= tvc_drv_poll,
};

static int  tvc_drv_probe(struct platform_device *pdev)
{
	struct tvc_drv_type *ptvc_data;
	int i, index, ret = -ENODEV;

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


	i = get_count_vioc_rdma(&pdev->dev);
	
	dprintk("%s name : %s  alias  id:%d : rdma num:%d \n", __func__, pdev->name, of_alias_get_id(pdev->dev.of_node, "tvc_drv"), i);

	for(index = 0; index < i; index++)
	{
		ptvc_data->rdma[index] = devm_vioc_rdma_get(&pdev->dev, index);
		if (IS_ERR(ptvc_data->rdma[index])) {
			printk("could not find rdma0 node of %s driver. \n", ptvc_data->misc->name);
			ptvc_data->rdma[index] = NULL;
		}
		else {
			dprintk("RDMA resource index:%d  id:%d reg:%p  \n", index, ptvc_data->rdma[index]->id, ptvc_data->rdma[index]->reg);
		}
	}

	ptvc_data->wmix = devm_vioc_wmix_get(&pdev->dev, 0);
	if (IS_ERR(ptvc_data->wmix)) {
		printk("could not find wmix node of %s driver. \n", ptvc_data->misc->name);
		ptvc_data->wmix = NULL;
	}
	else {
		dprintk("WMIX resource  id:%d  reg:%p\n", ptvc_data->wmix->id, ptvc_data->wmix->reg);
	}
	
	ptvc_data->sc = devm_vioc_sc_get(&pdev->dev, 0);
	if (IS_ERR(ptvc_data->sc)) {
		printk("could not find sc node of %s driver. \n", ptvc_data->misc->name);
		ptvc_data->sc = NULL;
	}
	else {
		switch(ptvc_data->sc->id) {
			case 0 :
				ptvc_data->sc->type = VIOC_SC0;
				break;
			case 1 :
				ptvc_data->sc->type = VIOC_SC1;
				break;
			case 2 :
				ptvc_data->sc->type = VIOC_SC2;
				break;
			case 3 :
				ptvc_data->sc->type = VIOC_SC3;
				break;
			case 4 :
				ptvc_data->sc->type = VIOC_SC4;
				break;
		}
		dprintk("Scaler resource   id:%d   reg:%p \n",  ptvc_data->sc->id, ptvc_data->sc->reg);
	}

	ptvc_data->wdma = devm_vioc_wdma_get(&pdev->dev, 0);
	if (IS_ERR(ptvc_data->wdma)) {
		printk("could not find wdma node of %s driver. \n", ptvc_data->misc->name);
		ptvc_data->wdma = NULL;
	}
	else {
		ptvc_data->irq		= ptvc_data->wdma->irq;
		ptvc_data->vioc_intr->id	= ptvc_data->wdma->intr->id;
		ptvc_data->vioc_intr->bits	= ptvc_data->wdma->intr->bits;
		dprintk("wdma resource id:%d reg:%p \n",  ptvc_data->wdma->id, ptvc_data->wdma->reg);
		
	}

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
err_data_alloc:
	kfree(ptvc_data->info);
err_info_alloc:
	kfree(ptvc_data->misc);
err_misc_alloc:
	kfree(ptvc_data);

	printk("%s: %s: err ret:%d \n", __func__, pdev->name, ret);
	return ret;
}

static int  tvc_drv_remove(struct platform_device *pdev)
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
	struct tvc_drv_type *ptvc_data = (struct tvc_drv_type *)platform_get_drvdata(pdev);

	if(ptvc_data->data->dev_open_cnt > 0) {
		tvc_driver_block_reset(ptvc_data);
	}

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

