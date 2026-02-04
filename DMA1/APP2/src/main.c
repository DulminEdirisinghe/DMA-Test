#include <stdio.h>
#include "xil_io.h"
#include "xparameters.h"
#include "xaxidma.h"
#include "xil_cache.h"
#include "sdCard.h"
#include "xscugic.h"
#define inputImageWidth 28
#define inputImageHeight 28
#define scale 1
#define outputImageWidth 28/scale
#define outputImageHeight 28/scale


/*user functions*/
u32 checkHalted(u32 baseAddress,u32 offset);
void print_28x28_u8(const char *img);


char inputImage[inputImageWidth*inputImageHeight];
char outputImage[outputImageWidth*outputImageHeight];


/*global dma instance and configuration*/
XAxiDma g_dma_instance;
XAxiDma_Config *g_dma_config_ptr;


int main()
{

	u32 status;

	g_dma_config_ptr = XAxiDma_LookupConfigBaseAddr(XPAR_AXI_DMA_0_BASEADDR);
	status = XAxiDma_CfgInitialize(&g_dma_instance, g_dma_config_ptr);
	if(status != XST_SUCCESS)
	{
		print("dma_initialization_failed\r\n");
		return -1;
	}else
	{
		print("dma_initialization successful\r\n");
	}


	/*SD Card Initialize*/
	status = SD_Init();
    if (status != XST_SUCCESS)
    {
    	 print("file_system_init_failed\n\r");
    	 return XST_FAILURE;
    }else
    {
    	print("file_system_init_successful\n\r");

    }


	xil_printf("input_image_buffer_address %0x\n\r",inputImage);
	xil_printf("output_image_Buffer_address %0x\n\r",outputImage);

	status = ReadFile("image.bin",(u32)inputImage);
    if (status != XST_SUCCESS)
    {
  		print("file_read_failed\n\r");
    	 return XST_FAILURE;
    }

    Xil_DCacheFlush();
	print_28x28_u8(inputImage);

	status = XAxiDma_SimpleTransfer(&g_dma_instance, (u32)outputImage,(outputImageWidth*outputImageHeight),XAXIDMA_DEVICE_TO_DMA);
	if(status != XST_SUCCESS)
	{
		xil_printf("dma_failed\n\r",status);
	}

	status = XAxiDma_SimpleTransfer(&g_dma_instance, (u32)inputImage,(inputImageWidth*inputImageHeight),XAXIDMA_DMA_TO_DEVICE);
	if(status != XST_SUCCESS)
	{
		xil_printf("dma_failed\n\r",status);
	}
	while(status != 1)
	{
		status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x4);
	}

	status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x34);
	while(status != 1)
	{
	  status = checkHalted(XPAR_AXI_DMA_0_BASEADDR,0x34);
	}

	print("dma_transfer_success..\n\r");
	Xil_DCacheInvalidate();
	print_28x28_u8(outputImage);
	status = WriteFile("lenag.bin",(outputImageWidth*outputImageHeight),(u32)outputImage);
	if (status != XST_SUCCESS)
	{
		print("file_write_failed\n\r");
	    return XST_FAILURE;
	}
    xil_printf("done..");
	return 0;
}



u32 checkHalted(u32 baseAddress,u32 offset)
{
	u32 status;
	status = (XAxiDma_ReadReg(baseAddress,offset))&XAXIDMA_HALTED_MASK;
	return status;
}
void print_28x28_u8(const char *img)
{
    for (int r = 0; r < 28; r++)
	{
        for (int c = 0; c < 28; c++)
		{
            unsigned v = (unsigned)(unsigned char)img[r*28 + c];
            xil_printf("%3d ", v);
        }
        xil_printf("\r\n");
    }
}
