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
#include <linux/kernel.h>
#include <linux/errno.h>
#include <linux/err.h>

#include <asm/io.h>
#include <asm/atomic.h>

#ifdef CONFIG_ARCH_TCC898X
#include <video/tcc/vioc_config.h>
#include <video/tcc/vioc_global.h>
#else
#include <mach/vioc_config.h>
#include <mach/vioc_global.h>
#include <mach/vioc_rdma.h>
#include <mach/tvc_ioctrl.h>
#endif

static int debug	   		= 0;
#define dprintk(msg...)	if(debug) { 	printk( "TVC_EXTRA:  " msg); 	}

#ifdef CONFIG_TVC_EXTRA_VIQE_SHARE
static atomic_t viqe_lock_status = ATOMIC_INIT(0);
int tvc_extra_lock_vipe(unsigned int uType, unsigned int uPath)
{
	int ret = -1;	
	volatile PVIOC_IREQ_CONFIG pIREQConfig = VIOC_IREQConfig_GetAddress();

	if(uType != VIOC_VIQE)
	{
		printk(KERN_ERR"%s(): Invalid parameters(%d %d)\n", __func__, uType, uPath);
		goto END_PROCESS;
	}

	if(atomic_read(&viqe_lock_status))
		goto END_PROCESS;

	if(!atomic_read(&viqe_lock_status))
	{
		VIOC_PlugInOutCheck plug_state;
		VIOC_CONFIG_Device_PlugState(uType, &plug_state);
		if((plug_state.connect_statue == VIOC_PATH_CONNECTED) && (plug_state.connect_device != uPath)) {
			VIOC_CONFIG_PlugOut(uType);
			VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_VIQE, uType == VIOC_VIQE ? 0:1, VIOC_CONFIG_RESET);
			VIOC_CONFIG_SWReset(pIREQConfig, VIOC_CONFIG_VIQE, uType == VIOC_VIQE ? 0:1, VIOC_CONFIG_CLEAR);

			dprintk("%s(): VIQE%d is locked(with PlugOut & SWReset) !!!\n", __func__, uType == VIOC_VIQE ? 0:1);
			ret = 1;
		} 
		else 
		{
			dprintk("%s(): VIQE%d is locked !!! \n", __func__, uType == VIOC_VIQE ? 0:1);
			ret = 0;
		}
		atomic_set(&viqe_lock_status, 1);
	}

END_PROCESS:
	return ret;
}
EXPORT_SYMBOL(tvc_extra_lock_vipe);

void tvc_extra_unlock_viqe(unsigned int uType)
{
	if(uType != VIOC_VIQE) {
		printk(KERN_ERR"%s(): Invalid parameters(%d)\n", __func__, uType);
		return;
	}

	if(atomic_read(&viqe_lock_status)) {
		dprintk("%s(): VIQE%d is unlocked !!!\n", __func__, uType == VIOC_VIQE?0:1);
		atomic_set(&viqe_lock_status, 0);
	}
}
EXPORT_SYMBOL(tvc_extra_unlock_viqe);
#endif

#ifdef CONFIG_TVC_EXTRA_DIRECT_DISPLAY
void tvc_extra_display_update(struct tvc_drv_type *pdata, struct tvc_dma_info *output, unsigned int enable)
{
	volatile PVIOC_RDMA *pRDMA = (volatile PVIOC_RDMA)tcc_p2v(0x12000700);

	if(enable) {
		dprintk("RDMA0x%08x - size: %dx%d fmt:%d base:0x%08x 0x%08x 0x%08x\n",
			(unsigned int )pRDMA, output->size.width, output->size.height, output->fmt.format, 
			output->addr0, output->addr1, output->addr2 );

		VIOC_RDMA_SetImageSize(pRDMA, output->size.width, output->size.height);
		VIOC_RDMA_SetImageFormat(pRDMA, output->fmt.format);
		VIOC_RDMA_SetImageOffset(pRDMA, output->fmt.format, output->size.width);
		VIOC_RDMA_SetImageBase(pRDMA, output->addr0, output->addr1, output->addr2);
		VIOC_RDMA_SetImageEnable(pRDMA);
	} else {
		dprintk("RDMA0x%08x off\n", (unsigned int)pRDMA);
		VIOC_RDMA_SetImageDisable(pRDMA);
	}
}
EXPORT_SYMBOL(tvc_extra_display_update);
#endif
