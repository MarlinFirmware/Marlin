/*!
    \file    drv_usb_regs.h
    \brief   USB cell registers definition and handle macros

    \version 2020-08-01, V3.0.0, firmware for GD32F4xx
*/

/*
    Copyright (c) 2020, GigaDevice Semiconductor Inc.

    Redistribution and use in source and binary forms, with or without modification, 
are permitted provided that the following conditions are met:

    1. Redistributions of source code must retain the above copyright notice, this 
       list of conditions and the following disclaimer.
    2. Redistributions in binary form must reproduce the above copyright notice, 
       this list of conditions and the following disclaimer in the documentation 
       and/or other materials provided with the distribution.
    3. Neither the name of the copyright holder nor the names of its contributors 
       may be used to endorse or promote products derived from this software without 
       specific prior written permission.

    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED 
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. 
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, 
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT 
NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR 
PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY 
OF SUCH DAMAGE.
*/

#ifndef __DRV_USB_REGS_H
#define __DRV_USB_REGS_H

#include "gdusb_conf.h"


#define USBHS_REG_BASE                0x40040000L   /*!< base address of USBHS registers */
#define USBFS_REG_BASE                0x50000000L   /*!< base address of USBFS registers */

#define USBFS_MAX_TX_FIFOS            15U           /*!< FIFO number */

#define USBFS_MAX_PACKET_SIZE         64U           /*!< USBFS max packet size */
#define USBFS_MAX_CHANNEL_COUNT       8U            /*!< USBFS host channel count */
#define USBFS_MAX_EP_COUNT            4U            /*!< USBFS device endpoint count */
#define USBFS_MAX_FIFO_WORDLEN        320U          /*!< USBFS max fifo size in words */

#define USBHS_MAX_PACKET_SIZE         512U          /*!< USBHS max packet size */
#define USBHS_MAX_CHANNEL_COUNT       12U           /*!< USBHS host channel count */
#define USBHS_MAX_EP_COUNT            6U            /*!< USBHS device endpoint count */
#define USBHS_MAX_FIFO_WORDLEN        1280U         /*!< USBHS max fifo size in words */

#define USB_DATA_FIFO_OFFSET          0x1000U       /*!< USB data fifo offset */
#define USB_DATA_FIFO_SIZE            0x1000U       /*!< USB data fifo size */

typedef enum
{
    USB_CORE_ENUM_HS = 0,                           /*!< USB core type is HS */
    USB_CORE_ENUM_FS = 1                            /*!< USB core type is FS */
} usb_core_enum;

enum USB_SPEED {
    USB_SPEED_UNKNOWN = 0,                          /*!< USB speed unknown */
    USB_SPEED_LOW,                                  /*!< USB speed low */
    USB_SPEED_FULL,                                 /*!< USB speed full */
    USB_SPEED_HIGH,                                 /*!< USB speed high */
};

enum usb_reg_offset {
    USB_REG_OFFSET_CORE      = 0x0000U,             /*!< global OTG control and status register */
    USB_REG_OFFSET_DEV       = 0x0800U,             /*!< device mode control and status registers */
    USB_REG_OFFSET_EP        = 0x0020U,
    USB_REG_OFFSET_EP_IN     = 0x0900U,             /*!< device IN endpoint 0 control register */
    USB_REG_OFFSET_EP_OUT    = 0x0B00U,             /*!< device OUT endpoint 0 control register */
    USB_REG_OFFSET_HOST      = 0x0400U,             /*!< host control register */
    USB_REG_OFFSET_CH        = 0x0020U,
    USB_REG_OFFSET_PORT      = 0x0440U,             /*!< host port control and status register */
    USB_REG_OFFSET_CH_INOUT  = 0x0500U,             /*!< Host channel-x control registers */
    USB_REG_OFFSET_PWRCLKCTL = 0x0E00U,             /*!< power and clock register */
};

typedef struct
{
    __IO uint32_t GOTGCS;                           /*!< USB global OTG control and status register       000h */
    __IO uint32_t GOTGINTF;                         /*!< USB global OTG interrupt flag register           004h */
    __IO uint32_t GAHBCS;                           /*!< USB global AHB control and status register       008h */
    __IO uint32_t GUSBCS;                           /*!< USB global USB control and status register       00Ch */
    __IO uint32_t GRSTCTL;                          /*!< USB global reset control register                010h */
    __IO uint32_t GINTF;                            /*!< USB global interrupt flag register               014h */
    __IO uint32_t GINTEN;                           /*!< USB global interrupt enable register             018h */
    __IO uint32_t GRSTATR;                          /*!< USB receive status debug read register           01Ch */
    __IO uint32_t GRSTATP;                          /*!< USB receive status and pop register              020h */
    __IO uint32_t GRFLEN;                           /*!< USB global receive FIFO length register          024h */
    __IO uint32_t DIEP0TFLEN_HNPTFLEN;              /*!< USB device IN endpoint 0/host non-periodic transmit FIFO length register 028h */
    __IO uint32_t HNPTFQSTAT;                       /*!< USB host non-periodic FIFO/queue status register 02Ch */
    uint32_t Reserved30[2];                         /*!< Reserved                                         030h */
    __IO uint32_t GCCFG;                            /*!< USB global core configuration register           038h */
    __IO uint32_t CID;                              /*!< USB core ID register                             03Ch */
    uint32_t Reserved40[48];                        /*!< Reserved                                         040h-0FFh */
    __IO uint32_t HPTFLEN;                          /*!< USB host periodic transmit FIFO length register  100h */
    __IO uint32_t DIEPTFLEN[15];                    /*!< USB device IN endpoint transmit FIFO length register 104h */
} usb_gr;


typedef struct
{
    __IO uint32_t HCTL;                             /*!< USB host control register                             400h */
    __IO uint32_t HFT;                              /*!< USB host frame interval register                      404h */
    __IO uint32_t HFINFR;                           /*!< USB host frame information remaining register         408h */
    uint32_t Reserved40C;                           /*!< Reserved                                              40Ch */
    __IO uint32_t HPTFQSTAT;                        /*!< USB host periodic transmit FIFO/queue status register 410h */
    __IO uint32_t HACHINT;                          /*!< USB host all channels interrupt register              414h */
    __IO uint32_t HACHINTEN;                        /*!< USB host all channels interrupt enable register       418h */
} usb_hr;

typedef struct
{
    __IO uint32_t HCHCTL;                           /*!< USB host channel control register          500h */
    __IO uint32_t HCHSTCTL;                         /*!< Reserved                                   504h */
    __IO uint32_t HCHINTF;                          /*!< USB host channel interrupt flag register   508h */
    __IO uint32_t HCHINTEN;                         /*!< USB host channel interrupt enable register 50Ch */
    __IO uint32_t HCHLEN;                           /*!< USB host channel transfer length register  510h */
    __IO uint32_t HCHDMAADDR;                       /*!< USB host channel-x DMA address register    514h*/
    uint32_t Reserved[2];
} usb_pr;

typedef struct
{
    __IO uint32_t DCFG;                             /*!< USB device configuration register                           800h */
    __IO uint32_t DCTL;                             /*!< USB device control register                                 804h */
    __IO uint32_t DSTAT;                            /*!< USB device status register                                  808h */
    uint32_t Reserved0C;                            /*!< Reserved                                                    80Ch */
    __IO uint32_t DIEPINTEN;                        /*!< USB device IN endpoint common interrupt enable register     810h */
    __IO uint32_t DOEPINTEN;                        /*!< USB device OUT endpoint common interrupt enable register    814h */
    __IO uint32_t DAEPINT;                          /*!< USB device all endpoints interrupt register                 818h */
    __IO uint32_t DAEPINTEN;                        /*!< USB device all endpoints interrupt enable register          81Ch */
    uint32_t Reserved20;                            /*!< Reserved                                                    820h */
    uint32_t Reserved24;                            /*!< Reserved                                                    824h */
    __IO uint32_t DVBUSDT;                          /*!< USB device VBUS discharge time register                     828h */
    __IO uint32_t DVBUSPT;                          /*!< USB device VBUS pulsing time register                       82Ch */
    __IO uint32_t DTHRCTL;                          /*!< device threshold control                                    830h */
    __IO uint32_t DIEPFEINTEN;                      /*!< USB Device IN endpoint FIFO empty interrupt enable register 834h */
    __IO uint32_t DEP1INT;                          /*!< USB device endpoint 1 interrupt register                    838h */
    __IO uint32_t DEP1INTEN;                        /*!< USB device endpoint 1 interrupt enable register             83Ch */
    uint32_t Reserved40;                            /*!< Reserved                                                    840h */
    __IO uint32_t DIEP1INTEN;                       /*!< USB device IN endpoint-1 interrupt enable register          844h */
    uint32_t Reserved48[15];                        /*!< Reserved                                                848-880h */
    __IO uint32_t DOEP1INTEN;                       /*!< USB device OUT endpoint-1 interrupt enable register         884h */
} usb_dr;

typedef struct
{
    __IO uint32_t DIEPCTL;                          /*!< USB device IN endpoint control register         900h + (EpNum * 20h) + 00h */
    uint32_t Reserved04;                            /*!< Reserved                                        900h + (EpNum * 20h) + 04h */
    __IO uint32_t DIEPINTF;                         /*!< USB device IN endpoint interrupt flag register  900h + (EpNum * 20h) + 08h */
    uint32_t Reserved0C;                            /*!< Reserved                                        900h + (EpNum * 20h) + 0Ch */
    __IO uint32_t DIEPLEN;                          /*!< USB device IN endpoint transfer length register 900h + (EpNum * 20h) + 10h */
    __IO uint32_t DIEPDMAADDR;                      /*!< Device IN endpoint-x DMA address register       900h + (EpNum * 20h) + 14h */
    __IO uint32_t DIEPTFSTAT;                       /*!< USB device IN endpoint transmit FIFO status register 900h + (EpNum * 20h) + 18h */
} usb_erin;

typedef struct
{
    __IO uint32_t DOEPCTL;                          /*!< USB device IN endpoint control register         B00h + (EpNum * 20h) + 00h */
    uint32_t Reserved04;                            /*!< Reserved                                        B00h + (EpNum * 20h) + 04h */
    __IO uint32_t DOEPINTF;                         /*!< USB device IN endpoint interrupt flag register  B00h + (EpNum * 20h) + 08h */
    uint32_t Reserved0C;                            /*!< Reserved                                        B00h + (EpNum * 20h) + 0Ch */
    __IO uint32_t DOEPLEN;                          /*!< USB device IN endpoint transfer length register B00h + (EpNum * 20h) + 10h */
    __IO uint32_t DOEPDMAADDR;                      /*!< Device OUT endpoint-x DMA address register      B00h + (EpNum * 20h) + 0Ch */
} usb_erout;

typedef struct _usb_regs
{
    usb_gr       *gr;                               /*!< USBFS global registers */
    usb_dr       *dr;                               /*!< Device control and status registers */
    usb_hr       *hr;                               /*!< Host control and status registers */
    usb_erin     *er_in[6];                         /*!< USB device IN endpoint register */
    usb_erout    *er_out[6];                        /*!< USB device OUT endpoint register */
    usb_pr       *pr[15];                           /*!< USB Host channel-x control register */

    __IO uint32_t     *HPCS;                        /*!< USB host port control and status register */
    __IO uint32_t     *DFIFO[USBFS_MAX_TX_FIFOS];       
    __IO uint32_t     *PWRCLKCTL;                   /*!< USB power and clock control register */
} usb_core_regs;

/* global OTG control and status register bits definitions */
#define GOTGCS_BSV                BIT(19)             /*!< B-Session Valid */
#define GOTGCS_ASV                BIT(18)             /*!< A-session valid */
#define GOTGCS_DI                 BIT(17)             /*!< debounce interval */
#define GOTGCS_CIDPS              BIT(16)             /*!< id pin status */
#define GOTGCS_DHNPEN             BIT(11)             /*!< device HNP enable */
#define GOTGCS_HHNPEN             BIT(10)             /*!< host HNP enable */
#define GOTGCS_HNPREQ             BIT(9)              /*!< HNP request */
#define GOTGCS_HNPS               BIT(8)              /*!< HNP successes */
#define GOTGCS_SRPREQ             BIT(1)              /*!< SRP request */
#define GOTGCS_SRPS               BIT(0)              /*!< SRP successes */

/* global OTG interrupt flag register bits definitions */
#define GOTGINTF_DF               BIT(19)             /*!< debounce finish */
#define GOTGINTF_ADTO             BIT(18)             /*!< A-device timeout */
#define GOTGINTF_HNPDET           BIT(17)             /*!< host negotiation request detected */
#define GOTGINTF_HNPEND           BIT(9)              /*!< HNP end */
#define GOTGINTF_SRPEND           BIT(8)              /*!< SRP end */
#define GOTGINTF_SESEND           BIT(2)              /*!< session end */

/* global AHB control and status register bits definitions */
#define GAHBCS_PTXFTH             BIT(8)              /*!< periodic Tx FIFO threshold */
#define GAHBCS_TXFTH              BIT(7)              /*!< tx FIFO threshold */
#define GAHBCS_DMAEN              BIT(5)              /*!< DMA function Enable */
#define GAHBCS_BURST              BITS(1, 4)          /*!< the AHB burst type used by DMA */
#define GAHBCS_GINTEN             BIT(0)              /*!< global interrupt enable */

/* global USB control and status register bits definitions */
#define GUSBCS_FDM                BIT(30)             /*!< force device mode */
#define GUSBCS_FHM                BIT(29)             /*!< force host mode */
#define GUSBCS_ULPIEOI            BIT(21)             /*!< ULPI external over-current indicator */
#define GUSBCS_ULPIEVD            BIT(20)             /*!< ULPI external VBUS driver */
#define GUSBCS_UTT                BITS(10, 13)        /*!< USB turnaround time */
#define GUSBCS_HNPCEN             BIT(9)              /*!< HNP capability enable */
#define GUSBCS_SRPCEN             BIT(8)              /*!< SRP capability enable */
#define GUSBCS_EMBPHY             BIT(6)              /*!< embedded PHY selected */
#define GUSBCS_TOC                BITS(0, 2)          /*!< timeout calibration */

/* global reset control register bits definitions */
#define GRSTCTL_DMAIDL            BIT(31)             /*!< DMA idle state */
#define GRSTCTL_DMABSY            BIT(30)             /*!< DMA busy */
#define GRSTCTL_TXFNUM            BITS(6, 10)         /*!< tx FIFO number */
#define GRSTCTL_TXFF              BIT(5)              /*!< tx FIFO flush */
#define GRSTCTL_RXFF              BIT(4)              /*!< rx FIFO flush */
#define GRSTCTL_HFCRST            BIT(2)              /*!< host frame counter reset */
#define GRSTCTL_HCSRST            BIT(1)              /*!< HCLK soft reset */
#define GRSTCTL_CSRST             BIT(0)              /*!< core soft reset */

/* global interrupt flag register bits definitions */
#define GINTF_WKUPIF              BIT(31)             /*!< wakeup interrupt flag */
#define GINTF_SESIF               BIT(30)             /*!< session interrupt flag */
#define GINTF_DISCIF              BIT(29)             /*!< disconnect interrupt flag */
#define GINTF_IDPSC               BIT(28)             /*!< id pin status change */
#define GINTF_PTXFEIF             BIT(26)             /*!< periodic tx FIFO empty interrupt flag */
#define GINTF_HCIF                BIT(25)             /*!< host channels interrupt flag */
#define GINTF_HPIF                BIT(24)             /*!< host port interrupt flag */
#define GINTF_PXNCIF              BIT(21)             /*!< periodic transfer not complete interrupt flag */
#define GINTF_ISOONCIF            BIT(21)             /*!< isochronous OUT transfer not complete interrupt flag */
#define GINTF_ISOINCIF            BIT(20)             /*!< isochronous IN transfer not complete interrupt flag */
#define GINTF_OEPIF               BIT(19)             /*!< OUT endpoint interrupt flag */
#define GINTF_IEPIF               BIT(18)             /*!< IN endpoint interrupt flag */
#define GINTF_EOPFIF              BIT(15)             /*!< end of periodic frame interrupt flag */
#define GINTF_ISOOPDIF            BIT(14)             /*!< isochronous OUT packet dropped interrupt flag */
#define GINTF_ENUMFIF             BIT(13)             /*!< enumeration finished */
#define GINTF_RST                 BIT(12)             /*!< USB reset */
#define GINTF_SP                  BIT(11)             /*!< USB suspend */
#define GINTF_ESP                 BIT(10)             /*!< early suspend */
#define GINTF_GONAK               BIT(7)              /*!< global OUT NAK effective */
#define GINTF_GNPINAK             BIT(6)              /*!< global IN non-periodic NAK effective */
#define GINTF_NPTXFEIF            BIT(5)              /*!< non-periodic tx FIFO empty interrupt flag */
#define GINTF_RXFNEIF             BIT(4)              /*!< rx FIFO non-empty interrupt flag */
#define GINTF_SOF                 BIT(3)              /*!< start of frame */
#define GINTF_OTGIF               BIT(2)              /*!< OTG interrupt flag */
#define GINTF_MFIF                BIT(1)              /*!< mode fault interrupt flag */
#define GINTF_COPM                BIT(0)              /*!< current operation mode */

/* global interrupt enable register bits definitions */
#define GINTEN_WKUPIE             BIT(31)             /*!< wakeup interrupt enable */
#define GINTEN_SESIE              BIT(30)             /*!< session interrupt enable */
#define GINTEN_DISCIE             BIT(29)             /*!< disconnect interrupt enable */
#define GINTEN_IDPSCIE            BIT(28)             /*!< id pin status change interrupt enable */
#define GINTEN_PTXFEIE            BIT(26)             /*!< periodic tx FIFO empty interrupt enable */
#define GINTEN_HCIE               BIT(25)             /*!< host channels interrupt enable */
#define GINTEN_HPIE               BIT(24)             /*!< host port interrupt enable */
#define GINTEN_IPXIE              BIT(21)             /*!< periodic transfer not complete interrupt enable */
#define GINTEN_ISOONCIE           BIT(21)             /*!< isochronous OUT transfer not complete interrupt enable */
#define GINTEN_ISOINCIE           BIT(20)             /*!< isochronous IN transfer not complete interrupt enable */
#define GINTEN_OEPIE              BIT(19)             /*!< OUT endpoints interrupt enable */
#define GINTEN_IEPIE              BIT(18)             /*!< IN endpoints interrupt enable */
#define GINTEN_EOPFIE             BIT(15)             /*!< end of periodic frame interrupt enable */
#define GINTEN_ISOOPDIE           BIT(14)             /*!< isochronous OUT packet dropped interrupt enable */
#define GINTEN_ENUMFIE            BIT(13)             /*!< enumeration finish enable */
#define GINTEN_RSTIE              BIT(12)             /*!< USB reset interrupt enable */
#define GINTEN_SPIE               BIT(11)             /*!< USB suspend interrupt enable */
#define GINTEN_ESPIE              BIT(10)             /*!< early suspend interrupt enable */
#define GINTEN_GONAKIE            BIT(7)              /*!< global OUT NAK effective interrupt enable */
#define GINTEN_GNPINAKIE          BIT(6)              /*!< global non-periodic IN NAK effective interrupt enable */
#define GINTEN_NPTXFEIE           BIT(5)              /*!< non-periodic Tx FIFO empty interrupt enable */
#define GINTEN_RXFNEIE            BIT(4)              /*!< receive FIFO non-empty interrupt enable */
#define GINTEN_SOFIE              BIT(3)              /*!< start of frame interrupt enable */
#define GINTEN_OTGIE              BIT(2)              /*!< OTG interrupt enable */
#define GINTEN_MFIE               BIT(1)              /*!< mode fault interrupt enable */

/* global receive status read and pop register bits definitions */
#define GRSTATRP_RPCKST           BITS(17, 20)        /*!< received packet status */
#define GRSTATRP_DPID             BITS(15, 16)        /*!< data PID */
#define GRSTATRP_BCOUNT           BITS(4, 14)         /*!< byte count */
#define GRSTATRP_CNUM             BITS(0, 3)          /*!< channel number */
#define GRSTATRP_EPNUM            BITS(0, 3)          /*!< endpoint number */

/* global receive FIFO length register bits definitions */
#define GRFLEN_RXFD               BITS(0, 15)         /*!< rx FIFO depth */

/* host non-periodic transmit FIFO length register bits definitions */
#define HNPTFLEN_HNPTXFD          BITS(16, 31)        /*!< non-periodic Tx FIFO depth */
#define HNPTFLEN_HNPTXRSAR        BITS(0, 15)         /*!< non-periodic Tx RAM start address */

/**
  * @brief USB IN endpoint 0 transmit FIFO length register bits definitions
  */
#define DIEP0TFLEN_IEP0TXFD       BITS(16, 31)        /*!< IN Endpoint 0 Tx FIFO depth */
#define DIEP0TFLEN_IEP0TXRSAR     BITS(0, 15)         /*!< IN Endpoint 0 TX RAM start address */

/* host non-periodic transmit FIFO/queue status register bits definitions */
#define HNPTFQSTAT_NPTXRQTOP      BITS(24, 30)        /*!< top entry of the non-periodic Tx request queue */
#define HNPTFQSTAT_NPTXRQS        BITS(16, 23)        /*!< non-periodic Tx request queue space */
#define HNPTFQSTAT_NPTXFS         BITS(0, 15)         /*!< non-periodic Tx FIFO space */
#define HNPTFQSTAT_CNUM           BITS(27, 30)        /*!< channel number*/
#define HNPTFQSTAT_EPNUM          BITS(27, 30)        /*!< endpoint number */
#define HNPTFQSTAT_TYPE           BITS(25, 26)        /*!< token type */
#define HNPTFQSTAT_TMF            BIT(24)             /*!< terminate flag */

/* global core configuration register bits definitions */
#define GCCFG_VBUSIG              BIT(21)             /*!< vbus ignored */
#define GCCFG_SOFOEN              BIT(20)             /*!< SOF output enable */
#define GCCFG_VBUSBCEN            BIT(19)             /*!< the VBUS B-device comparer enable */
#define GCCFG_VBUSACEN            BIT(18)             /*!< the VBUS A-device comparer enable */
#define GCCFG_PWRON               BIT(16)             /*!< power on */

/* core ID register bits definitions */
#define CID_CID                   BITS(0, 31)         /*!< core ID */

/* host periodic transmit FIFO length register bits definitions */
#define HPTFLEN_HPTXFD            BITS(16, 31)        /*!< host periodic Tx FIFO depth */
#define HPTFLEN_HPTXFSAR          BITS(0, 15)         /*!< host periodic Tx RAM start address */

/* device IN endpoint transmit FIFO length register bits definitions */
#define DIEPTFLEN_IEPTXFD         BITS(16, 31)        /*!< IN endpoint Tx FIFO x depth */
#define DIEPTFLEN_IEPTXRSAR       BITS(0, 15)         /*!< IN endpoint FIFOx Tx x RAM start address */

/* host control register bits definitions */
#define HCTL_SPDFSLS              BIT(2)              /*!< speed limited to FS and LS */
#define HCTL_CLKSEL               BITS(0, 1)          /*!< clock select for USB clock */

/* host frame interval register bits definitions */
#define HFT_FRI                   BITS(0, 15)         /*!< frame interval */

/* host frame information remaining register bits definitions */
#define HFINFR_FRT                BITS(16, 31)        /*!< frame remaining time */
#define HFINFR_FRNUM              BITS(0, 15)         /*!< frame number */

/* host periodic transmit FIFO/queue status register bits definitions */
#define HPTFQSTAT_PTXREQT         BITS(24, 31)        /*!< top entry of the periodic Tx request queue */
#define HPTFQSTAT_PTXREQS         BITS(16, 23)        /*!< periodic Tx request queue space */
#define HPTFQSTAT_PTXFS           BITS(0, 15)         /*!< periodic Tx FIFO space */
#define HPTFQSTAT_OEFRM           BIT(31)             /*!< odd/eveb frame */
#define HPTFQSTAT_CNUM            BITS(27, 30)        /*!< channel number */
#define HPTFQSTAT_EPNUM           BITS(27, 30)        /*!< endpoint number */
#define HPTFQSTAT_TYPE            BITS(25, 26)        /*!< token type */
#define HPTFQSTAT_TMF             BIT(24)             /*!< terminate flag */


#define TFQSTAT_TXFS              BITS(0, 15)
#define TFQSTAT_CNUM              BITS(27, 30)

/* host all channels interrupt register bits definitions */
#define HACHINT_HACHINT           BITS(0, 11)         /*!< host all channel interrupts */

/* host all channels interrupt enable register bits definitions */
#define HACHINTEN_CINTEN          BITS(0, 11)         /*!< channel interrupt enable */

/* host port control and status register bits definitions */
#define HPCS_PS                   BITS(17, 18)        /*!< port speed */
#define HPCS_PP                   BIT(12)             /*!< port power */
#define HPCS_PLST                 BITS(10, 11)        /*!< port line status */
#define HPCS_PRST                 BIT(8)              /*!< port reset */
#define HPCS_PSP                  BIT(7)              /*!< port suspend */
#define HPCS_PREM                 BIT(6)              /*!< port resume */
#define HPCS_PEDC                 BIT(3)              /*!< port enable/disable change */
#define HPCS_PE                   BIT(2)              /*!< port enable */
#define HPCS_PCD                  BIT(1)              /*!< port connect detected */
#define HPCS_PCST                 BIT(0)              /*!< port connect status */

/* host channel-x control register bits definitions */
#define HCHCTL_CEN                BIT(31)             /*!< channel enable */
#define HCHCTL_CDIS               BIT(30)             /*!< channel disable */
#define HCHCTL_ODDFRM             BIT(29)             /*!< odd frame */
#define HCHCTL_DAR                BITS(22, 28)        /*!< device address */
#define HCHCTL_MPC                BITS(20, 21)        /*!< multiple packet count */
#define HCHCTL_EPTYPE             BITS(18, 19)        /*!< endpoint type */
#define HCHCTL_LSD                BIT(17)             /*!< low-speed device */
#define HCHCTL_EPDIR              BIT(15)             /*!< endpoint direction */
#define HCHCTL_EPNUM              BITS(11, 14)        /*!< endpoint number */
#define HCHCTL_MPL                BITS(0, 10)         /*!< maximum packet length */

/* host channel-x split transaction register bits definitions */
#define HCHSTCTL_SPLEN            BIT(31)             /*!< enable high-speed split transaction */
#define HCHSTCTL_CSPLT            BIT(16)             /*!< complete-split enable */
#define HCHSTCTL_ISOPCE           BITS(14, 15)        /*!< isochronous OUT payload continuation encoding */
#define HCHSTCTL_HADDR            BITS(7, 13)         /*!< HUB address */
#define HCHSTCTL_PADDR            BITS(0, 6)          /*!< port address */

/* host channel-x interrupt flag register bits definitions */
#define HCHINTF_DTER              BIT(10)             /*!< data toggle error */
#define HCHINTF_REQOVR            BIT(9)              /*!< request queue overrun */
#define HCHINTF_BBER              BIT(8)              /*!< babble error */
#define HCHINTF_USBER             BIT(7)              /*!< USB bus Error */
#define HCHINTF_NYET              BIT(6)              /*!< NYET */
#define HCHINTF_ACK               BIT(5)              /*!< ACK */
#define HCHINTF_NAK               BIT(4)              /*!< NAK */
#define HCHINTF_STALL             BIT(3)              /*!< STALL */
#define HCHINTF_DMAER             BIT(2)              /*!< DMA error */
#define HCHINTF_CH                BIT(1)              /*!< channel halted */
#define HCHINTF_TF                BIT(0)              /*!< transfer finished */

/* host channel-x interrupt enable register bits definitions */
#define HCHINTEN_DTERIE           BIT(10)             /*!< data toggle error interrupt enable */
#define HCHINTEN_REQOVRIE         BIT(9)              /*!< request queue overrun interrupt enable */
#define HCHINTEN_BBERIE           BIT(8)              /*!< babble error interrupt enable */
#define HCHINTEN_USBERIE          BIT(7)              /*!< USB bus error interrupt enable */
#define HCHINTEN_NYETIE           BIT(6)              /*!< NYET interrupt enable */
#define HCHINTEN_ACKIE            BIT(5)              /*!< ACK interrupt enable */
#define HCHINTEN_NAKIE            BIT(4)              /*!< NAK interrupt enable */
#define HCHINTEN_STALLIE          BIT(3)              /*!< STALL interrupt enable */
#define HCHINTEN_DMAERIE          BIT(2)              /*!< DMA error interrupt enable */
#define HCHINTEN_CHIE             BIT(1)              /*!< channel halted interrupt enable */
#define HCHINTEN_TFIE             BIT(0)              /*!< transfer finished interrupt enable */

/* host channel-x transfer length register bits definitions */
#define HCHLEN_PING               BIT(31)             /*!< PING token request */
#define HCHLEN_DPID               BITS(29, 30)        /*!< data PID */
#define HCHLEN_PCNT               BITS(19, 28)        /*!< packet count */
#define HCHLEN_TLEN               BITS(0, 18)         /*!< transfer length */

/* host channel-x DMA address register bits definitions */
#define HCHDMAADDR_DMAADDR        BITS(0, 31)         /*!< DMA address */


#define PORT_SPEED(x)             (((uint32_t)(x) << 17) & HPCS_PS) /*!< Port speed */

#define PORT_SPEED_HIGH           PORT_SPEED(0U)                             /*!< high speed */
#define PORT_SPEED_FULL           PORT_SPEED(1U)                             /*!< full speed */
#define PORT_SPEED_LOW            PORT_SPEED(2U)                             /*!< low speed */

#define PIPE_CTL_DAR(x)           (((uint32_t)(x) << 22) & HCHCTL_DAR)      /*!< device address */
#define PIPE_CTL_EPTYPE(x)        (((uint32_t)(x) << 18) & HCHCTL_EPTYPE)   /*!< endpoint type */
#define PIPE_CTL_EPNUM(x)         (((uint32_t)(x) << 11) & HCHCTL_EPNUM)    /*!< endpoint number */
#define PIPE_CTL_EPDIR(x)         (((uint32_t)(x) << 15) & HCHCTL_EPDIR)    /*!< endpoint direction */
#define PIPE_CTL_EPMPL(x)         (((uint32_t)(x) <<  0) & HCHCTL_MPL)      /*!< maximum packet length */
#define PIPE_CTL_LSD(x)           (((uint32_t)(x) << 17) & HCHCTL_LSD)      /*!< low-Speed device */

#define PIPE_XFER_PCNT(x)         (((uint32_t)(x) << 19) & HCHLEN_PCNT)     /*!< packet count */
#define PIPE_XFER_DPID(x)         (((uint32_t)(x) << 29) & HCHLEN_DPID)     /*!< data PID */

#define PIPE_DPID_DATA0           PIPE_XFER_DPID(0)                         /*!< DATA0 */
#define PIPE_DPID_DATA1           PIPE_XFER_DPID(2)                         /*!< DATA1 */
#define PIPE_DPID_DATA2           PIPE_XFER_DPID(1)                         /*!< DATA2 */
#define PIPE_DPID_SETUP           PIPE_XFER_DPID(3)                         /*!< MDATA (non-control)/SETUP (control) */

extern const uint32_t PIPE_DPID[2];

/* device configuration registers bits definitions */
#define DCFG_EOPFT                BITS(11, 12)        /*!< end of periodic frame time */
#define DCFG_DAR                  BITS(4, 10)         /*!< device address */
#define DCFG_NZLSOH               BIT(2)              /*!< non-zero-length status OUT handshake */
#define DCFG_DS                   BITS(0, 1)          /*!< device speed */

/* device control registers bits definitions */
#define DCTL_POIF                 BIT(11)             /*!< power-on initialization finished */
#define DCTL_CGONAK               BIT(10)             /*!< clear global OUT NAK */
#define DCTL_SGONAK               BIT(9)              /*!< set global OUT NAK */
#define DCTL_CGINAK               BIT(8)              /*!< clear global IN NAK */
#define DCTL_SGINAK               BIT(7)              /*!< set global IN NAK */
#define DCTL_GONS                 BIT(3)              /*!< global OUT NAK status */
#define DCTL_GINS                 BIT(2)              /*!< global IN NAK status */
#define DCTL_SD                   BIT(1)              /*!< soft disconnect */
#define DCTL_RWKUP                BIT(0)              /*!< remote wakeup */

/* device status registers bits definitions */
#define DSTAT_FNRSOF              BITS(8, 21)         /*!< the frame number of the received SOF. */
#define DSTAT_ES                  BITS(1, 2)          /*!< enumerated speed */
#define DSTAT_SPST                BIT(0)              /*!< suspend status */

/* device IN endpoint common interrupt enable registers bits definitions */
#define DIEPINTEN_NAKEN           BIT(13)             /*!< NAK handshake sent by USBHS interrupt enable bit */
#define DIEPINTEN_TXFEEN          BIT(7)              /*!< transmit FIFO empty interrupt enable bit */
#define DIEPINTEN_IEPNEEN         BIT(6)              /*!< IN endpoint NAK effective interrupt enable bit */
#define DIEPINTEN_EPTXFUDEN       BIT(4)              /*!< endpoint Tx FIFO underrun interrupt enable bit */
#define DIEPINTEN_CITOEN          BIT(3)              /*!< control In Timeout interrupt enable bit */
#define DIEPINTEN_EPDISEN         BIT(1)              /*!< endpoint disabled interrupt enable bit */
#define DIEPINTEN_TFEN            BIT(0)              /*!< transfer finished interrupt enable bit */ 

/* device OUT endpoint common interrupt enable registers bits definitions */
#define DOEPINTEN_NYETEN          BIT(14)             /*!< NYET handshake is sent interrupt enable bit */
#define DOEPINTEN_BTBSTPEN        BIT(6)              /*!< back-to-back SETUP packets interrupt enable bit */
#define DOEPINTEN_EPRXFOVREN      BIT(4)              /*!< endpoint Rx FIFO overrun interrupt enable bit */
#define DOEPINTEN_STPFEN          BIT(3)              /*!< SETUP phase finished interrupt enable bit */
#define DOEPINTEN_EPDISEN         BIT(1)              /*!< endpoint disabled interrupt enable bit */
#define DOEPINTEN_TFEN            BIT(0)              /*!< transfer finished interrupt enable bit */

/* device all endpoints interrupt registers bits definitions */
#define DAEPINT_OEPITB            BITS(16, 21)        /*!< device all OUT endpoint interrupt bits */
#define DAEPINT_IEPITB            BITS(0, 5)          /*!< device all IN endpoint interrupt bits */

/* device all endpoints interrupt enable registers bits definitions */
#define DAEPINTEN_OEPIE           BITS(16, 21)        /*!< OUT endpoint interrupt enable */
#define DAEPINTEN_IEPIE           BITS(0, 3)          /*!< IN endpoint interrupt enable */

/* device Vbus discharge time registers bits definitions */
#define DVBUSDT_DVBUSDT           BITS(0, 15)         /*!< device VBUS discharge time */

/* device Vbus pulsing time registers bits definitions */
#define DVBUSPT_DVBUSPT           BITS(0, 11)         /*!< device VBUS pulsing time */

/* device IN endpoint FIFO empty interrupt enable register bits definitions */
#define DIEPFEINTEN_IEPTXFEIE     BITS(0, 5)          /*!< IN endpoint Tx FIFO empty interrupt enable bits */

/* device endpoint 0 control register bits definitions */
#define DEP0CTL_EPEN              BIT(31)             /*!< endpoint enable */
#define DEP0CTL_EPD               BIT(30)             /*!< endpoint disable */
#define DEP0CTL_SNAK              BIT(27)             /*!< set NAK */
#define DEP0CTL_CNAK              BIT(26)             /*!< clear NAK */
#define DIEP0CTL_TXFNUM           BITS(22, 25)        /*!< tx FIFO number */
#define DEP0CTL_STALL             BIT(21)             /*!< STALL handshake */
#define DOEP0CTL_SNOOP            BIT(20)             /*!< snoop mode */
#define DEP0CTL_EPTYPE            BITS(18, 19)        /*!< endpoint type */
#define DEP0CTL_NAKS              BIT(17)             /*!< NAK status */
#define DEP0CTL_EPACT             BIT(15)             /*!< endpoint active */
#define DEP0CTL_MPL               BITS(0, 1)          /*!< maximum packet length */

/* device endpoint x control register bits definitions */
#define DEPCTL_EPEN               BIT(31)             /*!< endpoint enable */
#define DEPCTL_EPD                BIT(30)             /*!< endpoint disable */
#define DEPCTL_SODDFRM            BIT(29)             /*!< set odd frame */
#define DEPCTL_SD1PID             BIT(29)             /*!< set DATA1 PID */
#define DEPCTL_SEVNFRM            BIT(28)             /*!< set even frame */
#define DEPCTL_SD0PID             BIT(28)             /*!< set DATA0 PID */
#define DEPCTL_SNAK               BIT(27)             /*!< set NAK */
#define DEPCTL_CNAK               BIT(26)             /*!< clear NAK */
#define DIEPCTL_TXFNUM            BITS(22, 25)        /*!< tx FIFO number */
#define DEPCTL_STALL              BIT(21)             /*!< STALL handshake */
#define DOEPCTL_SNOOP             BIT(20)             /*!< snoop mode */
#define DEPCTL_EPTYPE             BITS(18, 19)        /*!< endpoint type */
#define DEPCTL_NAKS               BIT(17)             /*!< NAK status */
#define DEPCTL_EOFRM              BIT(16)             /*!< even/odd frame */
#define DEPCTL_DPID               BIT(16)             /*!< endpoint data PID */
#define DEPCTL_EPACT              BIT(15)             /*!< endpoint active */
#define DEPCTL_MPL                BITS(0, 10)         /*!< maximum packet length */

/* device IN endpoint-x interrupt flag register bits definitions */
#define DIEPINTF_NAK              BIT(13)             /*!< NAK handshake sent by USBHS */
#define DIEPINTF_TXFE             BIT(7)              /*!< transmit FIFO empty */
#define DIEPINTF_IEPNE            BIT(6)              /*!< IN endpoint NAK effective */
#define DIEPINTF_EPTXFUD          BIT(4)              /*!< endpoint Tx FIFO underrun */
#define DIEPINTF_CITO             BIT(3)              /*!< control In Timeout interrupt */
#define DIEPINTF_EPDIS            BIT(1)              /*!< endpoint disabled */
#define DIEPINTF_TF               BIT(0)              /*!< transfer finished */

/* device OUT endpoint-x interrupt flag register bits definitions */
#define DOEPINTF_NYET             BIT(14)             /*!< NYET handshake is sent */
#define DOEPINTF_BTBSTP           BIT(6)              /*!< back-to-back SETUP packets */
#define DOEPINTF_EPRXFOVR         BIT(4)              /*!< endpoint Rx FIFO overrun */
#define DOEPINTF_STPF             BIT(3)              /*!< SETUP phase finished */
#define DOEPINTF_EPDIS            BIT(1)              /*!< endpoint disabled */
#define DOEPINTF_TF               BIT(0)              /*!< transfer finished */

/* device IN endpoint 0 transfer length register bits definitions */
#define DIEP0LEN_PCNT             BITS(19, 20)        /*!< packet count */
#define DIEP0LEN_TLEN             BITS(0, 6)          /*!< transfer length */

/* device OUT endpoint 0 transfer length register bits definitions */
#define DOEP0LEN_STPCNT           BITS(29, 30)        /*!< SETUP packet count */
#define DOEP0LEN_PCNT             BIT(19)             /*!< packet count */
#define DOEP0LEN_TLEN             BITS(0, 6)          /*!< transfer length */

/* device OUT endpoint-x transfer length register bits definitions */
#define DOEPLEN_RXDPID            BITS(29, 30)        /*!< received data PID */
#define DOEPLEN_STPCNT            BITS(29, 30)        /*!< SETUP packet count */
#define DIEPLEN_MCNT              BITS(29, 30)        /*!< multi count */
#define DEPLEN_PCNT               BITS(19, 28)        /*!< packet count */
#define DEPLEN_TLEN               BITS(0, 18)         /*!< transfer length */

/* device IN endpoint-x DMA address register bits definitions */
#define DIEPDMAADDR_DMAADDR       BITS(0, 31)         /*!< DMA address */

/* device OUT endpoint-x DMA address register bits definitions */
#define DOEPDMAADDR_DMAADDR       BITS(0, 31)         /*!< DMA address */

/* device IN endpoint-x transmit FIFO status register bits definitions */
#define DIEPTFSTAT_IEPTFS         BITS(0, 15)         /*!< IN endpoint Tx FIFO space remaining */

/* USB power and clock registers bits definition */
#define PWRCLKCTL_SHCLK           BIT(1)              /*!< stop HCLK */
#define PWRCLKCTL_SUCLK           BIT(0)              /*!< stop the USB clock */

#define RSTAT_GOUT_NAK                  1U    /* global OUT NAK (triggers an interrupt) */
#define RSTAT_DATA_UPDT                 2U    /* OUT data packet received */
#define RSTAT_XFER_COMP                 3U    /* OUT transfer completed (triggers an interrupt) */
#define RSTAT_SETUP_COMP                4U    /* SETUP transaction completed (triggers an interrupt) */
#define RSTAT_SETUP_UPDT                6U    /* SETUP data packet received */

#define DSTAT_EM_HS_PHY_30MHZ_60MHZ     0U    /* USB enumerate speed use high-speed PHY clock in 30MHz or 60MHz */
#define DSTAT_EM_FS_PHY_30MHZ_60MHZ     1U    /* USB enumerate speed use full-speed PHY clock in 30MHz or 60MHz */
#define DSTAT_EM_LS_PHY_6MHZ            2U    /* USB enumerate speed use low-speed PHY clock in 6MHz */
#define DSTAT_EM_FS_PHY_48MHZ           3U    /* USB enumerate speed use full-speed PHY clock in 48MHz */

#define DPID_DATA0                      0U    /* device endpoint data PID is DATA0 */
#define DPID_DATA1                      2U    /* device endpoint data PID is DATA1 */
#define DPID_DATA2                      1U    /* device endpoint data PID is DATA2 */
#define DPID_MDATA                      3U    /* device endpoint data PID is MDATA */

#define GAHBCS_DMAINCR(regval)    (GAHBCS_BURST & ((regval) << 1)) /*!< AHB burst type used by DMA*/

#define DMA_INCR0                 GAHBCS_DMAINCR(0U)                /*!< single burst type used by DMA*/
#define DMA_INCR1                 GAHBCS_DMAINCR(1U)                /*!< 4-beat incrementing burst type used by DMA*/
#define DMA_INCR4                 GAHBCS_DMAINCR(3U)                /*!< 8-beat incrementing burst type used by DMA*/
#define DMA_INCR8                 GAHBCS_DMAINCR(5U)                /*!< 16-beat incrementing burst type used by DMA*/
#define DMA_INCR16                GAHBCS_DMAINCR(7U)                /*!< 32-beat incrementing burst type used by DMA*/

#define DCFG_PFRI(regval)         (DCFG_EOPFT & ((regval) << 11))  /*!< end of periodic frame time configuration */

#define FRAME_INTERVAL_80         DCFG_PFRI(0U)                     /*!< 80% of the frame time */
#define FRAME_INTERVAL_85         DCFG_PFRI(1U)                     /*!< 85% of the frame time */
#define FRAME_INTERVAL_90         DCFG_PFRI(2U)                     /*!< 90% of the frame time */
#define FRAME_INTERVAL_95         DCFG_PFRI(3U)                     /*!< 95% of the frame time */

#define DCFG_DEVSPEED(regval)     (DCFG_DS & ((regval) << 0))      /*!< device speed configuration */

#define USB_SPEED_EXP_HIGH        DCFG_DEVSPEED(0U)                 /*!< device external PHY high speed */
#define USB_SPEED_EXP_FULL        DCFG_DEVSPEED(1U)                 /*!< device external PHY full speed */
#define USB_SPEED_INP_FULL        DCFG_DEVSPEED(3U)                 /*!< device internal PHY full speed */

#define DEP0_MPL(regval)          (DEP0CTL_MPL & ((regval) << 0))  /*!< maximum packet length configuration */

#define EP0MPL_64                 DEP0_MPL(0U)                      /*!< maximum packet length 64 bytes */
#define EP0MPL_32                 DEP0_MPL(1U)                      /*!< maximum packet length 32 bytes */
#define EP0MPL_16                 DEP0_MPL(2U)                      /*!< maximum packet length 16 bytes */
#define EP0MPL_8                  DEP0_MPL(3U)                      /*!< maximum packet length 8 bytes */

#define DOEP0_TLEN(regval)        (DOEP0LEN_TLEN & ((regval) << 0))     /*!< transfer length */
#define DOEP0_PCNT(regval)        (DOEP0LEN_PCNT & ((regval) << 19))    /*!< packet count */
#define DOEP0_STPCNT(regval)      (DOEP0LEN_STPCNT & ((regval) << 29))  /*!< SETUP packet count */

#define USB_ULPI_PHY                            1U                      /*!< ULPI interface external PHY */
#define USB_EMBEDDED_PHY                        2U                      /*!< embedded PHY */

#define GRXSTS_PKTSTS_IN                        2U
#define GRXSTS_PKTSTS_IN_XFER_COMP              3U
#define GRXSTS_PKTSTS_DATA_TOGGLE_ERR           5U
#define GRXSTS_PKTSTS_CH_HALTED                 7U

#define HCTL_30_60MHZ                           0U                      /*!< USB clock 30-60MHZ */
#define HCTL_48MHZ                              1U                      /*!< USB clock 48MHZ */
#define HCTL_6MHZ                               2U                      /*!< USB clock 6MHZ */

#define EP0_OUT                   ((uint8_t)0x00)                       /*!< endpoint out 0 */
#define EP0_IN                    ((uint8_t)0x80)                       /*!< endpoint in 0 */
#define EP1_OUT                   ((uint8_t)0x01)                       /*!< endpoint out 1 */
#define EP1_IN                    ((uint8_t)0x81)                       /*!< endpoint in 1 */
#define EP2_OUT                   ((uint8_t)0x02)                       /*!< endpoint out 2 */
#define EP2_IN                    ((uint8_t)0x82)                       /*!< endpoint in 2 */
#define EP3_OUT                   ((uint8_t)0x03)                       /*!< endpoint out 3 */
#define EP3_IN                    ((uint8_t)0x83)                       /*!< endpoint in 3 */

#endif /* __DRV_USB_REGS_H */
