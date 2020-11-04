/* SPDX-License-Identifier: GPL-2.0+ */
/*
 * Copyright (C) 2018 Simone CIANNI <simone.cianni@bticino.it>
 * Copyright (C) 2018 Raffaele RECALCATI <raffaele.recalcati@bticino.it>
 * Copyright (C) 2018 Jagan Teki <jagan@amarulasolutions.com>
 *
 * Configuration settings for the BTicion i.MX6DL Mamoj board.
 */

#ifndef __MX6DSYSD_CONFIG_H
#define __MX6DSYSD_CONFIG_H

#ifdef CONFIG_SPL
#include "imx6_spl.h"
#endif

#define CONFIG_MACH_TYPE	3980
#define CONFIG_MXC_UART_BASE	UART1_BASE
#define CONSOLE_DEV		"ttymxc0"

#include "mx6_common.h"

#define CONFIG_IMX_THERMAL

/* Size of malloc() pool */
#define CONFIG_SYS_MALLOC_LEN		(35 * SZ_1M)

#define CONFIG_MXC_UART

/* MMC Configs */
#define CONFIG_SYS_FSL_ESDHC_ADDR      0

#define CONFIG_FEC_MXC
#define IMX_FEC_BASE			ENET_BASE_ADDR
#define CONFIG_FEC_XCV_TYPE		RMII
#ifdef CONFIG_DM_ETH
#define CONFIG_ETHPRIME			"eth0"
#else
#define CONFIG_ETHPRIME			"FEC"
#endif
#define CONFIG_FEC_MXC_PHYADDR		1


#ifdef CONFIG_SUPPORT_EMMC_BOOT
#define EMMC_ENV \
	"emmcdev=2\0" \
	"update_emmc_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if ${get_cmd} ${update_sd_firmware_filename}; then " \
			"if mmc dev ${emmcdev} 1; then "	\
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0"
#else
#define EMMC_ENV ""
#endif

#define VIDEO_ARGS        "${video_args}"
#define VIDEO_ARGS_SCRIPT "run video_args_script; "

#define CONFIG_PREBOOT \
	"if hdmidet; then " \
		"setenv video_interfaces hdmi lvds; " \
	"else " \
		"setenv video_interfaces lvds hdmi; " \
	"fi;"

#define CONFIG_EXTRA_ENV_SETTINGS \
	"script=boot.scr\0" \
	"image=zImage\0" \
	"fdt_file=undefined\0" \
	"fdt_addr=0x18000000\0" \
	"boot_fdt=try\0" \
	"ip_dyn=yes\0" \
	"console=" CONSOLE_DEV "\0" \
	"dfuspi=dfu 0 sf 0:0:10000000:0\0" \
	"dfu_alt_info_spl=spl raw 0x400\0" \
	"dfu_alt_info_img=u-boot raw 0x10000\0" \
	"dfu_alt_info=spl raw 0x400\0" \
	"fdt_high=0xffffffff\0"	  \
	"initrd_high=0xffffffff\0" \
	"mmcdev=" __stringify(CONFIG_SYS_MMC_ENV_DEV) "\0" \
	"mmcpart=1\0" \
	"finduuid=part uuid mmc ${mmcdev}:2 uuid\0" \
	"update_sd_firmware=" \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"if mmc dev ${mmcdev}; then "	\
			"if ${get_cmd} ${update_sd_firmware_filename}; then " \
				"setexpr fw_sz ${filesize} / 0x200; " \
				"setexpr fw_sz ${fw_sz} + 1; "	\
				"mmc write ${loadaddr} 0x2 ${fw_sz}; " \
			"fi; "	\
		"fi\0" \
	EMMC_ENV	  \
	"video_args_hdmi=setenv video_args $video_args " \
		"video=mxcfb${fb}:dev=hdmi,1280x720M@60,if=RGB24\0" \
	"video_args_lvds=setenv video_args $video_args " \
		"video=mxcfb${fb}:dev=ldb,LDB-XGA,if=RGB666\0" \
	"video_args_lcd=setenv video_args $video_args " \
		"video=mxcfb${fb}:dev=lcd,CLAA-WVGA,if=RGB666\0" \
	"fb=0\0" \
	"video_args_script=" \
		"for v in ${video_interfaces}; do " \
			"run video_args_${v}; " \
			"setexpr fb $fb + 1; " \
		"done\0" \
	"mmcargs=setenv bootargs console=${console},${baudrate} " \
		"root=PARTUUID=${uuid} rootwait rw " \
		VIDEO_ARGS "\0" \
	"loadbootscript=" \
		"fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${script};\0" \
	"bootscript=echo Running bootscript from mmc ...; " \
		"source\0" \
	"loadimage=fatload mmc ${mmcdev}:${mmcpart} ${loadaddr} ${image}\0" \
	"loadfdt=fatload mmc ${mmcdev}:${mmcpart} ${fdt_addr} ${fdt_file}\0" \
	"mmcboot=echo Booting from mmc ...; " \
		"run finduuid; " \
		VIDEO_ARGS_SCRIPT \
		"run mmcargs; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if run loadfdt; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
	"netargs=setenv bootargs console=${console},${baudrate} " \
		"root=/dev/nfs " \
		"ip=dhcp nfsroot=${serverip}:${nfsroot},v3,tcp\0" \
	"netboot=echo Booting from net ...; " \
		"run netargs; " \
		"if test ${ip_dyn} = yes; then " \
			"setenv get_cmd dhcp; " \
		"else " \
			"setenv get_cmd tftp; " \
		"fi; " \
		"${get_cmd} ${image}; " \
		"if test ${boot_fdt} = yes || test ${boot_fdt} = try; then " \
			"if ${get_cmd} ${fdt_addr} ${fdt_file}; then " \
				"bootz ${loadaddr} - ${fdt_addr}; " \
			"else " \
				"if test ${boot_fdt} = try; then " \
					"bootz; " \
				"else " \
					"echo WARN: Cannot load the DT; " \
				"fi; " \
			"fi; " \
		"else " \
			"bootz; " \
		"fi;\0" \
		"findfdt="\
			"if test $fdt_file = undefined; then " \
				"if test $board_name = SABREAUTO && test $board_rev = MX6QP; then " \
					"setenv fdt_file imx6qp-sabreauto.dtb; fi; " \
				"if test $board_name = SABREAUTO && test $board_rev = MX6Q; then " \
					"setenv fdt_file imx6q-sabreauto.dtb; fi; " \
				"if test $board_name = SABREAUTO && test $board_rev = MX6DL; then " \
					"setenv fdt_file imx6dl-sabreauto.dtb; fi; " \
				"if test $board_name = SABRESD && test $board_rev = MX6QP; then " \
					"setenv fdt_file imx6qp-sabresd.dtb; fi; " \
				"if test $board_name = SABRESD && test $board_rev = MX6Q; then " \
					"setenv fdt_file imx6q-sabresd-ldo.dtb; fi; " \
				"if test $board_name = SABRESD && test $board_rev = MX6DL; then " \
					"setenv fdt_file imx6dl-sabresd-ldo.dtb; fi; " \
				"if test $board_name = MX6DSYSD && test $board_rev = MX6DL; then " \
					"setenv fdt_file mx6dsysd.dtb; fi; " \
				"if test $fdt_file = undefined; then " \
					"echo WARNING: Could not determine dtb to use; fi; " \
			"fi;\0" \

#define CONFIG_BOOTCOMMAND \
	"run findfdt;" \
	"mmc dev ${mmcdev};" \
	"if mmc rescan; then " \
		"if run loadbootscript; then " \
		"run bootscript; " \
		"else " \
			"if run loadimage; then " \
				"run mmcboot; " \
			"else run netboot; " \
			"fi; " \
		"fi; " \
	"else run netboot; fi"

#define CONFIG_ARP_TIMEOUT     200UL

#define CONFIG_SYS_MEMTEST_START       0x10000000
#define CONFIG_SYS_MEMTEST_END         0x10010000
#define CONFIG_SYS_MEMTEST_SCRATCH     0x10800000

/* Physical Memory Map */
#define PHYS_SDRAM                     MMDC0_ARB_BASE_ADDR

#define CONFIG_SYS_SDRAM_BASE          PHYS_SDRAM
#define CONFIG_SYS_INIT_RAM_ADDR       IRAM_BASE_ADDR
#define CONFIG_SYS_INIT_RAM_SIZE       IRAM_SIZE

#define CONFIG_SYS_INIT_SP_OFFSET \
	(CONFIG_SYS_INIT_RAM_SIZE - GENERATED_GBL_DATA_SIZE)
#define CONFIG_SYS_INIT_SP_ADDR \
	(CONFIG_SYS_INIT_RAM_ADDR + CONFIG_SYS_INIT_SP_OFFSET)

/* Environment organization */
#define CONFIG_ENV_SIZE			(8 * 1024)

#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_ENV_OFFSET		(768 * 1024)
#endif

/* Framebuffer */
#define CONFIG_VIDEO_BMP_RLE8
#define CONFIG_SPLASH_SCREEN
#define CONFIG_SPLASH_SCREEN_ALIGN
#define CONFIG_BMP_16BPP
#define CONFIG_VIDEO_LOGO
#define CONFIG_VIDEO_BMP_LOGO
#define CONFIG_IMX_HDMI
#define CONFIG_IMX_VIDEO_SKIP

/* Client */
#define CONFIG_USBD_HS


/* Falcon Mode */
#define CONFIG_SPL_FS_LOAD_ARGS_NAME	"args"
#define CONFIG_SPL_FS_LOAD_KERNEL_NAME	"uImage"
#define CONFIG_SYS_SPL_ARGS_ADDR       0x18000000

/* Falcon Mode - MMC support: args@1MB kernel@2MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTOR  0x800   /* 1MB */
#define CONFIG_SYS_MMCSD_RAW_MODE_ARGS_SECTORS (CONFIG_CMD_SPL_WRITE_SIZE / 512)
#define CONFIG_SYS_MMCSD_RAW_MODE_KERNEL_SECTOR        0x1000  /* 2MB */

#define CONFIG_SYS_FSL_USDHC_NUM	3
#if defined(CONFIG_ENV_IS_IN_MMC)
#define CONFIG_SYS_MMC_ENV_DEV		2	/* SDHC4 */
#endif
#if 1
#ifdef CONFIG_CMD_PCI
#define CONFIG_PCI_SCAN_SHOW
#define CONFIG_PCIE_IMX
#define CONFIG_PCIE_IMX_PERST_GPIO	IMX_GPIO_NR(7, 12)
#define CONFIG_PCIE_IMX_POWER_GPIO	IMX_GPIO_NR(3, 19)
#endif
#endif
/* I2C Configs */
#define CONFIG_SYS_I2C
#define CONFIG_SYS_I2C_MXC
#define CONFIG_SYS_I2C_MXC_I2C1		/* enable I2C bus 1 */
#define CONFIG_SYS_I2C_MXC_I2C2		/* enable I2C bus 2 */
#define CONFIG_SYS_I2C_MXC_I2C3		/* enable I2C bus 3 */
#define CONFIG_SYS_I2C_SPEED		  100000

#if 0
/* PMIC */
#define CONFIG_POWER
#define CONFIG_POWER_I2C
#define CONFIG_POWER_PFUZE100
#define CONFIG_POWER_PFUZE100_I2C_ADDR	0x08
#endif
#undef CONFIG_LDO_BYPASS_CHECK   /*PJ*/

/* USB Configs */
#ifdef CONFIG_CMD_USB
#define CONFIG_EHCI_HCD_INIT_AFTER_RESET
#define CONFIG_MXC_USB_PORTSC		(PORT_PTS_UTMI | PORT_PTS_PTW)
#define CONFIG_MXC_USB_FLAGS		0
#define CONFIG_USB_MAX_CONTROLLER_COUNT	2 /* Enabled USB controller number */
#endif

#if 0
/* RTC PCF8563 Configs */
#define CONFIG_RTC_PCF8563
#define CONFIG_SYS_I2C_RTC_ADDR 0x51 
#define CONFIG_CMD_DATE
#define CONFIG_SYS_RTC_BUS_NUM 1
#endif

#undef CONFIG_IPADDR
#define CONFIG_IPADDR			192.168.11.2
#define CONFIG_NETMASK			255.255.255.0
#undef CONFIG_SERVERIP
#define CONFIG_SERVERIP			192.168.11.1

#define CONFIG_CMD_GPT


#endif  /* __MX6SABRESD_CONFIG_H */
