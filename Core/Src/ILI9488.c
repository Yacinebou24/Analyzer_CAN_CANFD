/*
 * ili9488.c
 *
 *  Created on: Dec 14, 2021
 *  Author: timagr615
 */

#include <ILI9488.h>
#include "spi.h"
#include "tim.h"
#include "stm32h7xx_ll_gpio.h"

extern SPI_HandleTypeDef hspi4;
volatile uint8_t Displ_SpiAvailable = 1; // 0 if SPI is busy or 1 if it is free (transm cplt)

static __IO uint8_t spiDmaTransferComplete = 0;
Displ_Orientat_e current_orientation;
uint16_t width = DISPL_WIDTH;
uint16_t height = DISPL_HEIGHT;

// if using TouchGFX buffer are not used (size set to 2 bytes for software convenience)
// unless using ILI9488V1.0 (RGB666) which needs dispBuffer1 for color format conversion
// if not using TouchGFX double buffering is needed
#define BUFLEVEL 12
#define SIZEBUF (1<<BUFLEVEL)
static uint8_t dispBuffer1[SIZEBUF];
static uint8_t dispBuffer2[SIZEBUF];
static uint8_t *dispBuffer = dispBuffer1;

#define  LCD_SPI_SETDATASIZE_8BIT(hlcdspi)        MODIFY_REG(hspi4.Instance->CFG1, SPI_CFG1_DSIZE, SPI_DATASIZE_8BIT)
#define  LCD_SPI_SETDATASIZE_16BIT(hlcdspi)       MODIFY_REG(hspi4.Instance->CFG1, SPI_CFG1_DSIZE, SPI_DATASIZE_16BIT)

//1. Write Command to LCD
void ILI9488_SendCommand(uint8_t com) {
	uint8_t tmpCmd = com;
	//Set DC HIGH for COMMAND mode
	DC_COMMAND();
	//Put CS LOW
	CS_A();
	//Write byte using SPI
	HAL_SPI_Transmit(&hspi4, &tmpCmd, 1, 1);
	//Bring CS HIGH
	CS_D();
}

//2. Write data to LCD
void ILI9488_SendData(uint8_t data) {
	uint8_t tmpCmd = data;
	//Set DC LOW for DATA mode
	DC_DATA();
	//Put CS LOW
	CS_A();
	//Write byte using SPI
	HAL_SPI_Transmit(&hspi4, &tmpCmd, 1, 1);
	//Bring CS HIGH
	CS_D();
}

//2.2 Write multiple/DMA
void ILI9488_SendData_Multi(uint8_t *buff, size_t buff_size) {
	DC_DATA();
	CS_A();
	while (buff_size > 0) {
		spiDmaTransferComplete = 0;
		uint16_t chunk_size = buff_size > 65536 ? 65536 : buff_size;
		//uint16_t chunk_size = buff_size > 32768 ? 32768 : buff_size;
		HAL_SPI_Transmit_DMA(&hspi4, buff, chunk_size);	//, HAL_MAX_DELAY
		buff += chunk_size;
		buff_size -= chunk_size;
		while (!spiDmaTransferComplete) {
		}
	}
	CS_D();
}

void ILI9488_Init() {

	CS_D();
	RST_A();
	HAL_Delay(10);
	RST_D();
	width = ILI9488_TFTWIDTH;
	height = ILI9488_TFTHEIGHT;
	//CS_A();

	ILI9488_SendCommand(ILI9488_GMCTRP1);	//0xE0
	ILI9488_SendData(0x00);
	ILI9488_SendData(0x03);
	ILI9488_SendData(0x09);
	ILI9488_SendData(0x08);
	ILI9488_SendData(0x16);
	ILI9488_SendData(0x0A);
	ILI9488_SendData(0x3F);
	ILI9488_SendData(0x78);
	ILI9488_SendData(0x4C);
	ILI9488_SendData(0x09);
	ILI9488_SendData(0x0A);
	ILI9488_SendData(0x08);
	ILI9488_SendData(0x16);
	ILI9488_SendData(0x1A);
	ILI9488_SendData(0x0F);
	//Negative Gamma Control
	ILI9488_SendCommand(ILI9488_GMCTRN1);
	ILI9488_SendData(0x00);
	ILI9488_SendData(0x16);
	ILI9488_SendData(0x19);
	ILI9488_SendData(0x03);
	ILI9488_SendData(0x0F);
	ILI9488_SendData(0x05);
	ILI9488_SendData(0x32);
	ILI9488_SendData(0x45);
	ILI9488_SendData(0x46);
	ILI9488_SendData(0x04);
	ILI9488_SendData(0x0E);
	ILI9488_SendData(0x0D);
	ILI9488_SendData(0x35);
	ILI9488_SendData(0x37);
	ILI9488_SendData(0x0F);
	//Power Control 1
	ILI9488_SendCommand(ILI9488_PWCTR1);
	ILI9488_SendData(0x17);	//Vreg1out
	ILI9488_SendData(0x15);	//Verg2out
	//Power Control 2
	ILI9488_SendCommand(ILI9488_PWCTR2);
	ILI9488_SendData(0x41);	//VGH,VGL
	//Power Control 5 (For Partial Mode)
	ILI9488_SendCommand(ILI9488_VMCTR1);
	ILI9488_SendData(0x00);
	ILI9488_SendData(0x12);
	ILI9488_SendData(0x80);
	//Memory Access Control This command defines read/write scanning direction of the frame memory.
	ILI9488_SendCommand(ILI9488_MADCTL);
	ILI9488_SendData(0x48);
	// Interface Pixel Format
	ILI9488_SendCommand(ILI9488_PIXFMT);
	ILI9488_SendData(0x66);	// RGB666 = 0x66 (18 bit)//RGB565 = 0x55  (16 bit)
	// Interface Mode Control
	ILI9488_SendCommand(ILI9488_EXTC);
	ILI9488_SendData(0x00);	//0x80  SDO NOT USE  0x00 SDO USE
	//Frame rate
	ILI9488_SendCommand(ILI9488_FRMCTR1);
	ILI9488_SendData(0xA0);	//60Hz
	//Display Inversion Control
	ILI9488_SendCommand(ILI9488_INVCTR);
	ILI9488_SendData(0x02);	//2-dot
	//Display Function Control  RGB/MCU Interface Control
	ILI9488_SendCommand(ILI9488_DFUNCTR);
	ILI9488_SendData(0x02);	//MCU
	ILI9488_SendData(0x02);	//Source,Gate scan dieection
	// Set Image Function
	ILI9488_SendCommand(ILI9488_SETIMAGE);
	ILI9488_SendData(0x00);	// Disable 24 bit data
	// Adjust Control
	ILI9488_SendCommand(ILI9488_ADJCTR);
	ILI9488_SendData(0xA9);
	ILI9488_SendData(0x51);
	ILI9488_SendData(0x2C);
	ILI9488_SendData(0x82);    // D7 stream, loose

	//TURN ON DISPLAY
	ILI9488_SendCommand(ILI9488_DISPON);
	ILI9488_SendData(0x2C);

	ili9488_DisplayOn();
	ili9488_DisplayBrightness(100);   //Display on

//	ILI9488_SendCommand(ILI9488_WRDISBV);////This command is used to adjust the brightness value of the display.
//	ILI9488_SendData(0x2C);//In principle, 00h value means the lowest brightness and FFh value means the highest brightness

	HAL_Delay(120);
}

void setAddrWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1) {
	ILI9488_SendCommand(ILI9488_CASET); // Column addr set
	{
		uint8_t data[4] = { (x0 >> 8) & 0xFF, x0 & 0xFF, (x1 >> 8) & 0xFF, x1
				& 0xFF };
		ILI9488_SendData_Multi(data, 4);
	}
	ILI9488_SendCommand(ILI9488_PASET);
	{
		uint8_t data[4] = { (y0 >> 8) & 0xFF, y0 & 0xFF, (y1 >> 8) & 0xFF, y1
				& 0xFF };
		ILI9488_SendData_Multi(data, 4);
	}
	ILI9488_SendCommand(ILI9488_RAMWR); // write to RAM*/

}

void setScrollArea(uint16_t topFixedArea, uint16_t bottomFixedArea) {
	ILI9488_SendCommand(0x33); // Vertical scroll definition
	ILI9488_SendData(topFixedArea >> 8);
	ILI9488_SendData(topFixedArea);
	ILI9488_SendData((height - topFixedArea - bottomFixedArea) >> 8);
	ILI9488_SendData(height - topFixedArea - bottomFixedArea);
	ILI9488_SendData(bottomFixedArea >> 8);
	ILI9488_SendData(bottomFixedArea);
}

void scroll(uint16_t pixels) {
	ILI9488_SendCommand(0x37); // Vertical scrolling start address
	ILI9488_SendData(pixels >> 8);
	ILI9488_SendData(pixels);
}
void pushColor(uint16_t color) {
	DC_DATA();
	CS_A();
	write16BitColor(color);
	CS_D();

}

void pushColors(uint16_t *data, uint8_t len, uint8_t first) {
	uint16_t color;
	uint8_t buff[len * 3 + 1];
	uint16_t count = 0;
	uint8_t lencount = len;
	CS_A(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_RESET);
	if (first == 1) {
		DC_DATA(); //HAL_GPIO_WritePin(DISPL_DC_GPIO_Port, DISPL_DC_Pin, GPIO_PIN_SET);
	}
	while (lencount--) {
		color = *data++;
		buff[count] = (((color & 0xF800) >> 11) * 255) / 31;
		count++;
		buff[count] = (((color & 0x07E0) >> 5) * 255) / 63;
		count++;
		buff[count] = ((color & 0x001F) * 255) / 31;
		count++;
	}
	//HAL_SPI_Transmit_DMA(&hspi4, buff, len * 3, 100);//
	HAL_SPI_Transmit_DMA(&hspi4, buff, len * 3); //, 100
	while (!spiDmaTransferComplete) {
	};
	CS_D(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_SET);
}

void drawImage(const uint8_t *img, uint16_t x, uint16_t y, uint16_t w,
		uint16_t h) {
	//extern DMA_HandleTypeDef hdma_spi4_tx;
	if ((x >= width) || (y >= height))
		return;
	if ((x + w - 1) >= width)
		w = width - x;
	if ((y + h - 1) >= height)
		h = height - y;
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	DC_DATA(); //HAL_GPIO_WritePin(DISPL_DC_GPIO_Port, DISPL_DC_Pin, GPIO_PIN_SET);
	CS_A(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_RESET);
	uint8_t linebuff[w * 3 + 1];
	uint32_t count = 0;
	uint16_t pixcount;
	uint16_t countline = 0;
	for (uint16_t i = 0; i < h; i++) {
		pixcount = 0;
		for (uint16_t o = 0; o < w; o++) {
			uint8_t b1 = img[count];
			count++;
			uint8_t b2 = img[count];
			count++;
			uint16_t color = b1 << 8 | b2;
			linebuff[pixcount] = (((color & 0xF800) >> 11) * 255) / 31;
			pixcount++;
			linebuff[pixcount] = (((color & 0x07E0) >> 5) * 255) / 63;
			pixcount++;
			linebuff[pixcount] = ((color & 0x001F) * 255) / 31;
			pixcount++;
		}
		countline++;
		spiDmaTransferComplete = 0;
		//HAL_SPI_Transmit(&hspi4, linebuff, w * 3, 100);
		HAL_SPI_Transmit_DMA(&hspi4, linebuff, w * 3);
		while (!spiDmaTransferComplete) {
		};
		//printf("\tPixcount=%d\tCount =%d Countline =%d\n\r", pixcount, count,countline);
	}

	//printf("\tPixcount value =%d\tCount value =%d\n\r\n\r", pixcount, count);
	CS_D(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_SET);
}

void drawPixel(int16_t x, int16_t y, uint16_t color) {
	if ((x < 0) || (x >= width) || (y < 0) || (y >= height))
		return;
	setAddrWindow(x, y, x + 1, y + 1);
	//DC_DATA(); //HAL_GPIO_WritePin(DISPL_DC_GPIO_Port, DISPL_DC_Pin, GPIO_PIN_SET);
	//CS_A(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_RESET);
	write16BitColor(color);
	//CS_D(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_SET);

}

void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {

	if ((x >= width) || (y >= height))
		return;

	if ((y + h - 1) >= height)
		h = height - y;

	setAddrWindow(x, y, x, y + h - 1);
	DC_DATA(); //HAL_GPIO_WritePin(DISPL_DC_GPIO_Port, DISPL_DC_Pin, GPIO_PIN_SET);
	CS_A(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_RESET);
	while (h--) {
		write16BitColor(color);
	}
	CS_D(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_SET);

}

void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {

	if ((x >= width) || (y >= height))
		return;
	if ((x + w - 1) >= width)
		w = width - x;
	setAddrWindow(x, y, x + w - 1, y);
	DC_DATA(); //HAL_GPIO_WritePin(DISPL_DC_GPIO_Port, DISPL_DC_Pin, GPIO_PIN_SET);
	CS_A(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_RESET);
	while (w--) {
		write16BitColor(color);
	}
	CS_D(); //HAL_GPIO_WritePin(DISPL_CS_GPIO_Port, DISPL_CS_Pin, GPIO_PIN_SET);
}

void drawLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {
	if (x0 == x1) {
		if (y0 > y1)
			swap(y0, y1);
		drawFastVLine(x0, y0, y1 - y0 + 1, color);
	} else if (y0 == y1) {
		if (x0 > x1)
			swap(x0, x1);
		drawFastHLine(x0, y0, x1 - x0 + 1, color);
	} else {

		writeLine(x0, y0, x1, y1, color);

	}

}

void writeLine(int16_t x0, int16_t y0, int16_t x1, int16_t y1, uint16_t color) {

	int16_t steep = abs(y1 - y0) > abs(x1 - x0);
	if (steep) {
		swap(x0, y0);
		swap(x1, y1);
	}

	if (x0 > x1) {
		swap(x0, x1);
		swap(y0, y1);
	}

	int16_t dx, dy;
	dx = x1 - x0;
	dy = abs(y1 - y0);

	int16_t err = dx / 2;
	int16_t ystep;

	if (y0 < y1) {
		ystep = 1;
	} else {
		ystep = -1;
	}

	for (; x0 <= x1; x0++) {
		if (steep) {
			drawPixel(y0, x0, color);
		} else {
			drawPixel(x0, y0, color);
		}
		err -= dy;
		if (err < 0) {
			y0 += ystep;
			err += dx;
		}
	}
}
//6. Fill the entire screen with a background color

void fillScreen(uint16_t color) {
	fillRect(0, 0, width, height, color);
}

void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
	uint32_t i, n, cnt, buf_size;
	if ((x >= width) || (y >= height))
		return;
	if ((x + w - 1) >= width)
		w = width - x;
	if ((y + h - 1) >= height)
		h = height - y;
	setAddrWindow(x, y, x + w - 1, y + h - 1);
	uint8_t Rbyte = (color & 0xF800) >> 8;
	uint8_t Gbyte = (color & 0x07E0) >> 3;
	uint8_t Bbyte = (color & 0x001F) << 3;
	n = w * h * 3;
	if (n <= 65535) {
		cnt = 1;
		buf_size = n;
	} else {
		cnt = n / 3;
		buf_size = 3;
		uint8_t min_cnt = n / 65535 + 1;
		for (i = min_cnt; i < n / 3; i++) {
			if (n % i == 0) {
				cnt = i;
				buf_size = n / i;
				break;
			}
		}
	}
	uint8_t frm_buf[buf_size];
	for (i = 0; i < buf_size / 3; i++) {
		frm_buf[i * 3] = Rbyte;
		frm_buf[i * 3 + 1] = Gbyte;
		frm_buf[i * 3 + 2] = Bbyte;
	}
	DC_DATA();
	CS_A();
	while (cnt > 0) {
		spiDmaTransferComplete = 0;
		HAL_SPI_Transmit_DMA(&hspi4, frm_buf, buf_size); //, HAL_MAX_DELAY
		cnt -= 1;
		while (!spiDmaTransferComplete) {
		}
	}
	// waiting for a free SPI port. Flag is set to 1 by transmission-complete interrupt callback

	CS_D();
}
/**********************************************
 * @brief		set orientation of the display
 * @param  	r	rottion
 **********************************************/
void setRotation(Displ_Orientat_e orientation) {
	ILI9488_SendCommand(ILI9488_MADCTL);
	uint8_t rotation = orientation % 4; // can't be higher than 3
	switch (rotation) {
	case 0:
		ILI9488_SendData(MADCTL_MY | MADCTL_BGR);
		width = ILI9488_TFTWIDTH;
		height = ILI9488_TFTHEIGHT;
		break;
	case 1:
		ILI9488_SendData(MADCTL_MX | MADCTL_MY | MADCTL_MV | MADCTL_BGR);
		width = ILI9488_TFTHEIGHT;
		height = ILI9488_TFTWIDTH;
		break;
	case 2:
		ILI9488_SendData(MADCTL_MX | MADCTL_BGR);
		width = ILI9488_TFTWIDTH;
		height = ILI9488_TFTHEIGHT;
		break;
	case 3:
		ILI9488_SendData(MADCTL_MV | MADCTL_BGR);
		width = ILI9488_TFTHEIGHT;
		height = ILI9488_TFTWIDTH;
		break;
	}
	current_orientation = orientation; //stores active orientation into a global variable for touch routines
}

void invertDisplay(uint8_t i) {
	ILI9488_SendCommand(i ? ILI9488_INVON : ILI9488_INVOFF);

}
uint16_t color565(uint8_t r, uint8_t g, uint8_t b) {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

/***********************
 * @brief	display one character on the display
 * @param 	x,y: top left corner of the character to be printed
 * 			ch, font, color, bgcolor: as per parameter name
 * 			size: (1 or 2) single or double wided printing
 **********************/
void Displ_WChar(uint16_t x, uint16_t y, char ch, sFONT font, uint8_t size,
		uint16_t color, uint16_t bgcolor) {

	uint32_t i, b, bytes, j, bufSize, mask;
	const uint8_t *pos;
	uint8_t wsize = font.Width; //printing char width
	if (size == 2)
		wsize <<= 1;
	bufSize = 0;
	bytes = font.Height * font.Size;
	pos = font.table + (ch - 32) * bytes; //that's char position in table
	switch (font.Size) {
	case 3:
		mask = 0x800000;
		break;
	case 2:
		mask = 0x8000;
		break;
	default:
		mask = 0x80;
	}
//  setting up char image in RGB666 format

	uint8_t Rcol = (color & 0xF800) >> 8;
	uint8_t Gcol = (color & 0x07E0) >> 3;
	uint8_t Bcol = (color & 0x001F) << 3;
	uint8_t Rbak = (bgcolor & 0xF800) >> 8;
	uint8_t Gbak = (bgcolor & 0x07E0) >> 3;
	uint8_t Bbak = (bgcolor & 0x001F) << 3;
	for (i = 0; i < (bytes); i += font.Size) {
		b = 0;
		switch (font.Size) {
		case 3:
			b = pos[i] << 16 | pos[i + 1] << 8 | pos[i + 2];
			break;
		case 2:
			b = pos[i] << 8 | pos[i + 1];
			break;
		default:
			b = pos[i];
		}
		for (j = 0; j < font.Width; j++) {
			if ((b << j) & mask) {
				dispBuffer[bufSize++] = Rcol;
				dispBuffer[bufSize++] = Gcol;
				dispBuffer[bufSize++] = Bcol;

				if (size == 2) {
					dispBuffer[bufSize++] = Rcol;
					dispBuffer[bufSize++] = Gcol;
					dispBuffer[bufSize++] = Bcol;
				}
			} else {
				dispBuffer[bufSize++] = Rbak;
				dispBuffer[bufSize++] = Gbak;
				dispBuffer[bufSize++] = Bbak;
				if (size == 2) {
					dispBuffer[bufSize++] = Rbak;
					dispBuffer[bufSize++] = Gbak;
					dispBuffer[bufSize++] = Bbak;
				}
			}
		}
	}
	setAddrWindow(x, y, x + wsize - 1, y + font.Height - 1);
	ILI9488_SendData_Multi(dispBuffer, bufSize);
	dispBuffer = (dispBuffer == dispBuffer1 ? dispBuffer2 : dispBuffer1); // swapping buffer

}
/************************
 * @brief	print a string on display starting from a defined position
 * @params	x, y	top left area-to-print corner
 * 			str		string to print
 * 			font	to bu used
 * 			size	1 (normal), 2 (double width)
 * 			color	font color
 * 			bgcolor	background color
 ************************/
void Displ_WString(uint16_t x, uint16_t y, const char *str, sFONT font,
		uint8_t size, uint16_t color, uint16_t bgcolor) {
	uint16_t delta = font.Width;
	if (size > 1)
		delta <<= 1;

	while (*str) {

		//  these rows split oversize string in more screen lines
		/*        if(x + font.Width >= width) {
		 x = 0;
		 y += font.Height;
		 if(y + font.Height >= height) {
		 break;
		 }
		 if(*str == ' ') {
		 // skip spaces in the beginning of the new line
		 str++;
		 continue;
		 }
		 }*/

		Displ_WChar(x, y, *str, font, size, color, bgcolor);
		x += delta;
		str++;
	}
}
/************************
 * @brief	print a string on display centering into a defined area
 * @params	x0, y0	top left area corner
 * 			x1, y1	bottom right corner
 * 			str		string to print
 * 			font	to bu used
 * 			size	1 (normal), 2 (double width)
 * 			color	font color
 * 			bgcolor	background color
 ************************/
void Displ_CString(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,
		const char *str, sFONT font, uint8_t size, uint16_t color,
		uint16_t bgcolor) {
	uint16_t x, y;
	uint16_t wsize = font.Width;
	static uint8_t cambia = 0;
	if (size > 1)
		wsize <<= 1;
	if ((strlen(str) * wsize) > (x1 - x0 + 1))
		x = x0;
	else
		x = (x1 + x0 + 1 - strlen(str) * wsize) >> 1;
	if (font.Height > (y1 - y0 + 1))
		y = y0;
	else
		y = (y1 + y0 + 1 - font.Height) >> 1;

	if (x > x0) {
		fillRect(x0, y0, x - x0, y1 - y0 + 1, bgcolor);
	} else
		x = x0; // fixing here mistake could be due to roundings: x lower than x0.
	if (x1 > (strlen(str) * wsize + x0))
		fillRect(x1 - x + x0 - 1, y0, x - x0 + 1, y1 - y0 + 1, bgcolor);

	if (y > y0) {
		fillRect(x0, y0, x1 - x0 + 1, y - y0, bgcolor);
	} else
		y = y0; //same comment as above
	if (y1 >= (font.Height + y0))
		fillRect(x0, y1 - y + y0, x1 - x0 + 1, y - y0 + 1, bgcolor);

	cambia = !cambia;

	Displ_WString(x, y, str, font, size, color, bgcolor);

}

void testLines(uint8_t color) {
	//unsigned long start;
	int x1, y1, x2, y2, w = width, h = height;
	fillScreen(ILI9488_BLACK);
	x1 = y1 = 0;
	y2 = h - 1;
	for (x2 = 0; x2 < w; x2 += 6)
		drawLine(x1, y1, x2, y2, color);
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6)
		drawLine(x1, y1, x2, y2, color);
	fillScreen(ILI9488_BLACK);
	x1 = w - 1;
	y1 = 0;
	y2 = h - 1;
	for (x2 = 0; x2 < w; x2 += 6)
		drawLine(x1, y1, x2, y2, color);
	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
		drawLine(x1, y1, x2, y2, color);
	fillScreen(ILI9488_BLACK);
	x1 = 0;
	y1 = h - 1;
	y2 = 0;
	for (x2 = 0; x2 < w; x2 += 6)
		drawLine(x1, y1, x2, y2, color);
	x2 = w - 1;
	for (y2 = 0; y2 < h; y2 += 6)
		drawLine(x1, y1, x2, y2, color);
	fillScreen(ILI9488_BLACK);
	x1 = w - 1;
	y1 = h - 1;
	y2 = 0;
	for (x2 = 0; x2 < w; x2 += 6)
		drawLine(x1, y1, x2, y2, color);
	x2 = 0;
	for (y2 = 0; y2 < h; y2 += 6)
		drawLine(x1, y1, x2, y2, color);
}
void DrawCircle(int16_t x, int16_t y, int16_t Radius,uint16_t color)
{
  int32_t  D;       /* Decision Variable */
  int32_t  CurX;   /* Current X Value */
  int32_t  CurY;   /* Current Y Value */

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;

  while (CurX <= CurY)
  {
	  drawPixel((x + CurX), (y - CurY), color);
	  drawPixel((x - CurX), (y - CurY), color);
	  drawPixel((x + CurY), (y - CurX), color);
	  drawPixel((x - CurY), (y - CurX), color);
      drawPixel((x + CurX), (y + CurY), color);
      drawPixel((x - CurX), (y + CurY), color);
      drawPixel((x + CurY), (y + CurX), color);
      drawPixel((x - CurY), (y + CurX), color);

    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }
}
void FillCircle(uint16_t x, uint16_t y, uint16_t Radius,uint16_t color)
{
  int32_t  D;        /* Decision Variable */
  uint32_t  CurX;    /* Current X Value */
  uint32_t  CurY;    /* Current Y Value */

  D = 3 - (Radius << 1);
  CurX = 0;
  CurY = Radius;
  while (CurX <= CurY)
  {
    if(CurY > 0)
    {
      drawFastHLine(x - CurY, y + CurX, 2*CurY,color);
      drawFastHLine(x - CurY, y - CurX, 2*CurY,color);
    }

    if(CurX > 0)
    {
    	drawFastHLine(x - CurX, y - CurY, 2*CurX,color);
    	drawFastHLine(x - CurX, y + CurY, 2*CurX,color);
    }
    if (D < 0)
    {
      D += (CurX << 2) + 6;
    }
    else
    {
      D += ((CurX - CurY) << 2) + 10;
      CurY--;
    }
    CurX++;
  }

  DrawCircle(x, y, Radius,color);
}
/*
 *
 */
void write16BitColor(uint16_t color) {
	uint8_t r = (color & 0xF800) >> 11;
	uint8_t g = (color & 0x07E0) >> 5;
	uint8_t b = color & 0x001F;
	r = (r * 255) / 31;
	g = (g * 255) / 63;
	b = (b * 255) / 31;
	uint8_t data[3] = { r, g, b };
	ILI9488_SendData_Multi(data, 3);
}

/**
 * \brief Draws a rectangle specified by a coordinate pair, a width, and a height.
 *
 * \param x			The x-coordinate of the upper-left corner of the rectangle to draw
 * \param y			The y-coordinate of the upper-left corner of the rectangle to draw
 * \param w			Width of the rectangle to draw
 * \param h			Height of the rectangle to draw
 * \param color		Color
 *
 * \return void
 */
void lcdDrawRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color)
{
	drawFastHLine(x, y, w-1, color);
	drawFastVLine(x, y, h-1 , color);
	drawFastHLine(x, y+h-1, w-1, color);
	drawFastVLine(x + w-1, y, h-1, color);
}



/*
 *
 */
void ili9488_DisplayOn(void) {
	ILI9488_SendCommand(ILI9488_SLPOUT);
	ILI9488_SendCommand(ILI9488_DISPON);    //Display on
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}
/*
 *
 */
void ili9488_DisplayOff(void) {
	ILI9488_SendCommand(ILI9488_DISPOFF);
	ILI9488_SendCommand(ILI9488_SLPIN);
	HAL_TIM_PWM_Stop(&htim1, TIM_CHANNEL_2);
}
/*
 *
 */
void ili9488_DisplayBrightness(uint8_t level) {
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, level);
// Start PWM signals generation
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
}

void ili9488_read_chipid(void) {
	int8_t buffer[4];
	uint8_t tmpCmd = ILI9488_RDDID;
	DC_COMMAND();
	CS_A();
	//Write byte using SPI
	HAL_SPI_Transmit(&hspi4, &tmpCmd, 1, 1);
	//CS_D();
	DC_DATA();
	CS_A();
	HAL_SPI_Receive(&hspi4, &buffer[0], 4, HAL_MAX_DELAY);
	CS_D();
	printf("\tDum: %02X\tMIinfo: %02X\tPanel Type: %02X\tDrie Info: %02X\n\r",
			buffer[0], buffer[1], buffer[2], buffer[3]);

	/*LCD_IO_ReadCmd8MultipleData8(0x04, (uint8_t *)&id, 3, 0);
	 id <<= 1;
	 #elif ILI9488_INTERFACE == 1
	 LCD_IO_ReadCmd8MultipleData8(0x04, (uint8_t *)&id, 3, 1);
	 #endif
	 // printf("ID:%08X\r\n", (unsigned int)id);*/

}

void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi) {
	spiDmaTransferComplete = 1;
//		Touch_PenDown=0;    //reset touch interrupt flag: writing onto display will trigger the display interrupt pin
}

