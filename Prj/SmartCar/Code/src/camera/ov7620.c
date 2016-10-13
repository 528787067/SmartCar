#include "ov7620.h"
#include "Landzo.h"

#define		SUCCESS			1
#define		FAIL 			0

#define		MAX_WIDTH		240							//图像最大宽度
#define		MAX_HEIGHT		240							//图像最大高度

#define		REAL_WIDTH		222							//图像有效宽度
#define		REAL_HEIGHT		240							//图像有效高度

#define		VSYNC_PORT		PTA29						// 场中断管脚配置
#define		HERF_PORT		PTA28						// 行中断管脚配置
#define		CLK_PORT		PTA27						// DMA 中断管脚配置

#define		VSYNC_NUM		29							// 场中断管脚号
#define		HERF_NUM		28							// 行中断管脚号
#define		CLK_NUM			27							// DMA 中断管脚号


typedef enum					//用来保存摄像头的工作状态
{
	OV7620_NOTINIT = 0,
	OV7620_FINISH,           	//图像采集完毕
	OV7620_FAIL,             	//图像采集失败(采集行数少了)
	OV7620_GATHER,          	//图像采集中
	OV7620_START,          		//开始采集图像
	OV7620_STOP,             	//禁止图像采集
} OV7620_STATUS_e;

uint8 		*imgAddr;								//图像数组指针
volatile	uint8 	ov7620_img_buff[MAX_WIDTH]={0};	//当前行缓存数组
volatile	uint16	count;							//用来标记当前转移的点数
volatile	uint8	line;							//用来标记当前采集的行数
volatile 	OV7620_STATUS_e		ov7620_img_flag;	//标识摄像头工作状态

uint8 lineDis;										//采集行间隔
uint8 countDis;										//采集点间隔
uint8 lineEnd;										//图像结束行
uint8 errorCount;									//无效填充点

uint8 ov7620_portInit();							//初始化管脚
uint8 ov7620_initCameraReg();						//初始化寄存器
void OV7620_PORTA_IRQHandler();						//管脚中断函数
void OV7620_DMA0_IRQHandler();						//DMA 中断函数
void ov7620_vsync();								//场中断函数
void ov7620_href();									//行中断函数
void ov7620_dma();									//DMA中断函数

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * 初始化摄像头
 */
uint8 ov7620_initCamera(uint8 *imgaddr)
{		
	
	lineDis = MAX_HEIGHT/IMAGE_H;						//采集行间隔
	countDis = MAX_WIDTH/IMAGE_W;						//采集点间隔
	lineEnd = MAX_HEIGHT-(MAX_HEIGHT/IMAGE_H);			//图像结束行
	errorCount = (MAX_WIDTH - REAL_WIDTH)/countDis/2;	//无效填充点
	
	imgAddr = imgaddr;									//对图像地址进行赋值
	
	//while(!ov7620_initCameraReg())
		//DELAY_MS(100);								//初始化寄存器成功
	while(!LandzoIICEEROM_INIT())
	{
		printf("寄存器初始化失败\n");
		DELAY_MS(100);
	}
	while(!ov7620_portInit());							//初始化摄像头管脚
	return SUCCESS;
}

/**
 * 初始化管脚
 */
uint8 ov7620_portInit()
{
	//DMA通道0初始化，PTA27触发源(默认上升沿)，源地址为PTB_B0_IN，目的地址为：ov7620_img_buff，每次传输1Byte
    dma_portx2buff_init(CAMERA_DMA_CH, (void *)&PTB_B0_IN, (void *)ov7620_img_buff, CLK_PORT, DMA_BYTE1, MAX_HEIGHT, DADDR_KEEPON);
	DMA_DIS(CAMERA_DMA_CH);										//禁止通道0的DMA请求
	disable_irq(PORTA_IRQn); 									//关闭PTA的中断
    DMA_IRQ_CLEAN(CAMERA_DMA_CH);                   			//清除通道传输中断标志位	
    DMA_IRQ_EN(CAMERA_DMA_CH);									//允许通道0传输完成触发中断
	
    port_init(CLK_PORT, ALT1 | DMA_FALLING | PULLDOWN );   		//PCLK
    port_init(HERF_PORT, ALT1 | IRQ_RISING  | PULLDOWN | PF);   //行中断，下拉，上降沿触发中断，带滤波
    port_init(VSYNC_PORT, ALT1 | IRQ_FALLING | PULLUP   | PF);  //场中断，上拉，下降沿触发中断，带滤波
	
    //配置中断服务函数
    set_vector_handler(PORTA_VECTORn ,OV7620_PORTA_IRQHandler); //设置LPTMR的中断服务函数为 PORTA_IRQHandler
    set_vector_handler(DMA0_VECTORn ,OV7620_DMA0_IRQHandler);   //设置DMA0的中断服务函数为 DMA0_IRQHandler	
	return SUCCESS;
}

/**
 * 开始获取一副图像
 */
uint8 *ov7620_startGetImg()
{
	ov7620_img_flag = OV7620_START;                 //开始采集图像
    PORTA_ISFR = ~0;                        		//写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
    enable_irq(PORTA_IRQn);                 		//允许PTA的中断
    while(ov7620_img_flag != OV7620_FINISH)        	//等待图像采集完毕
    {
        if(ov7620_img_flag == OV7620_FAIL)        	//假如图像采集错误，则重新开始采集
        {
            ov7620_img_flag = OV7620_START;         //开始采集图像
            PORTA_ISFR = ~0;                		//写1清中断标志位(必须的，不然回导致一开中断就马上触发中断)
            enable_irq(PORTA_IRQn);                 //允许PTA的中断
        }
    }
	return (uint8 *)imgAddr;
}

/**
 * 摄像头停止获取图像，即关闭中断，清除 DMA 中断标志
 */
uint8 ov7620_stopGetImg()
{
	disable_irq(PORTA_VECTORn);						//关闭中断
	PORTA_ISFR = ~0;								//清除中断标识
	DMA_DIS(CAMERA_DMA_CH);							//禁止通道0的DMA请求
	DMA_IRQ_CLEAN(CAMERA_DMA_CH);           		//清除通道传输中断标志位
	ov7620_img_flag = OV7620_STOP;					//设置图像采集标志位
	return SUCCESS;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * PORTA中断服务函数
 */
void OV7620_PORTA_IRQHandler()
{
	
    uint32 flag = PORTA_ISFR;
    PORTA_ISFR  = ~0;           	//清中断标志位
    
	//场中断
    if(flag & (1 << VSYNC_NUM))     //PTA29触发中断
    {
        ov7620_vsync();				//触发场中断函数
    }
	
	//行中断
	if(flag & (1 << HERF_NUM))      //PTA29触发中断
	{
		// 判断是否处于图像采集状态
		if(ov7620_img_flag == OV7620_GATHER)
			ov7620_href();			//触发场中断函数
	}
	
	
}

/**
 * DMA0中断服务函数
 */
void OV7620_DMA0_IRQHandler()
{
    ov7620_dma();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * 场中断函数
 */
void ov7620_vsync()
{
	if(ov7620_img_flag == OV7620_START)							//开始采集
	{     
		ov7620_img_flag = OV7620_GATHER;						//标记图像采集中
		count = 0;
		line = 0;
	}
	else//图像采集错误
    {
        disable_irq(PORTA_IRQn);                				//关闭PTA的中断
        ov7620_img_flag = OV7620_FAIL;             				//标记图像采集失败
    }
}

/**
 * 行中断函数
 */
void ov7620_href()
{
	if(line % lineDis == 0)
	{
	#if 1
		//PORTA_ISFR = 1 <<  PT27;            					//清空PCLK标志位
		DMA_DADDR(CAMERA_DMA_CH) = (uint32)ov7620_img_buff;		//恢复地址
		//PORTA_ISFR = 1 <<  PT27;            					//清空PCLK标志位
		DMA_EN(CAMERA_DMA_CH);                  				//使能通道CHn 硬件请求
	#else
		//PORTA_ISFR = 1 <<  PT27;            					//清空PCLK标志位
		dma_repeat(CAMERA_DMA_CH, (void *)&PTB_B0_IN, (void *)ov7620_img_buff,MAX_HEIGHT);
		DMA_EN(CAMERA_DMA_COUNT);                  				//使能通道CHn 硬件请求
	#endif
	}
	line++;
}

/**
 * DMA中断服务函数
 */
int i;
void ov7620_dma()
{
	DMA_DIS(CAMERA_DMA_CH);									//禁止通道0的DMA请求
    DMA_IRQ_CLEAN(CAMERA_DMA_CH);           				//清除通道传输中断标志位
	
	//判断图像采集是否结束，采集结束则将图像状态标识设置为结束状态
	if(line >= lineEnd)
	{
		ov7620_img_flag = OV7620_FINISH ;
		disable_irq(PORTA_IRQn);
	}
	//else
	{
		for(i = 0; i < errorCount; i++)						//填充无效像素点
			imgAddr[count++] = 255;
		for(i = 0; i < REAL_WIDTH; i++)						//每个4个像素点取一个点
		{
			if(i%countDis == 0)
			#if (BIN_LEVEL == -1)							//不使用二值化
				imgAddr[count++] = ov7620_img_buff[i];
			#else											//二值化图像
				imgAddr[count++] = (ov7620_img_buff[i] >= BIN_LEVEL) ? 255 : 0;
			#endif
		}
		for(i = 0; i < errorCount; i++)						//填充无效像素点
			imgAddr[count++] = 255;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////

uint8  CamreaReg[] = {
   0x03,0x00,   0x0c,0x00,  0X11,0X00,  0X12,0X01,  0X13,0X17,  0X15,0X02,  0X16,0X03,  0X03,0X00,
   0X17,0X00,   0X18,0Xa0,  0X19,0X00,  0X1a,0X78,  0X1b,0X2a,  0X1f,0X20,  0X20,0X02,  0X09,0X03,
   0X21,0X00,   0X22,0X20,  0X23,0X33,  0X24,0X8a,  0X25,0X7a,  0X2A,0X00,  0X2B,0X00,  0X2c,0X00,
   0X2d,0X4f,   0X2e,0X96,  0X2f,0X00,  0X3a,0X00,  0X3b,0X60,  0X3e,0X34,  0X3f,0Xb0,  0X40,0X4c,
   0X41,0X3b,   0X42,0X30,  0X43,0X25,  0X44,0X1d,  0X45,0X15,  0X46,0X10,  0X47,0X0f,  0X48,0X0d,
   0X49,0X0b,   0X4b,0X09,  0X4c,0X09,  0X4e,0X08,  0X4f,0X06,  0X50,0X05,  0X51,0X2f,  0X52,0X90,
   0X53,0X01,   0X54,0X87,  0X56,0X70,  0X57,0X32,  0X58,0X8b,  0X59,0X06,  0X5a,0X9b,  0X5b,0X35,
   0X64,0X02,   0X67,0X80,  0X68,0X80,  0X69,0XA0,  0X6a,0X89,  0X70,0Xae,  0X71,0Xa6,  0X72,0X4f,
   0X73,0X5f,   0X74,0X27,  0X80,0X41,  0X81,0X08,  0X82,0X16,  0X83,0X24,  0X84,0X24,  0X85,0X2a,
   0X86,0X4f,   0X89,0Xb2,  0X8a,0Xaa,  0X8b,0X02,  0X8e,0X02,  0X8f,0X70,  0X95,0X14,  0X9d,0X36,
   0X9e,0X04,   0Xa1,0X27,  0Xa2,0X12,  0Xa3,0X2d,  0Xa4,0X08,  0Xa5,0X2e,  0Xa6,0X04,  0Xa7,0X80,
   0Xa8,0X80,   0Xa9,0X21,  0Xaa,0X21,  0Xab,0X21,  0Xac,0X0a,  0Xad,0Xf0,  0Xae,0Xff,  0Xb1,0Xb0,
   0Xb2,0Xb0,   0Xb6,0X80,  0Xb7,0X80,  0Xb8,0X80,  0Xb9,0X00,  0Xe7,0X7c,  0Xf0,0X00,  0Xf1,0X01,
   0Xf1,0X01,   0Xf1,0X01,
};

/**
 * 初始化摄像头寄存器
 */
uint8 ov7620_initCameraReg()
{
    uint8 sccbOk;                                                       //sccb操作成功标志
    uint8 loopNum;                                                      //用于循环的零时变量
    uint8 readData;                                                     //用于读取寄存器中的数据
    uint8 eromCount = sizeof(CamreaReg)/sizeof(CamreaReg[0])/2;        //操作的寄存器个数
    
    SCCB_GPIO_init();                                                   //初始化 SCCB
    
    //往寄存器中写数据
    for(loopNum = 0; loopNum < eromCount; loopNum++)
    {
        sccbOk = SCCB_WriteByte(CamreaReg[loopNum * 2], CamreaReg[loopNum * 2 + 1]);        
        if(sccbOk == FAIL)                                         		//写入寄存器失败
        {
            DEBUG_PRINTF("\n警告:SCCB写数据错误");
            return FAIL;
        }
    }
	
	
	//从寄存器中读取数据
    for(loopNum = 0; loopNum < eromCount; loopNum++)
    {
        sccbOk = SCCB_ReadByte( &readData, 1, CamreaReg[loopNum * 2]);
        if(sccbOk == FAIL)                                         		//读取寄存器失败
        {
            DEBUG_PRINTF("\n警告:SCCB写数据错误");
            return FAIL;
        }
        
		if(readData != CamreaReg[loopNum * 2 + 1])
        {
            DEBUG_PRINTF("\n警告:读取的数据错误");
            return FAIL;
        }
		
    }
	
	DEBUG_PRINTF("\n读写寄存器成功！");
	
    return SUCCESS;    
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////