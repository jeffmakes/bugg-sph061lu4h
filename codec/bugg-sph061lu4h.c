// SPDX-License-Identifier: GPL-2.0-only
/*
 * Attempt to use the PCM peripheral to drive a Analog Devices AD400x ADC 
 * by configuring it to behave like a SPI bus master.
 * 
 * For the Bugg project: https://bugg.xyz
 *
 * - Adapted from the ics43432.c codec driver.
 * - Non configurable.
 * - I2S interface, 64 BCLs per frame, 32 bits per channel, 24 bit data
 *
 * Copyright (c) 2015 Axis Communications AB
 */

#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <sound/core.h>
#include <sound/pcm.h>
#include <sound/pcm_params.h>
#include <sound/soc.h>
#include <sound/initval.h>
#include <sound/tlv.h>

#define SPH061LU4H_RATE_MIN 7190 /* Hz, from data sheet */
//#define SPH061LU4H_RATE_MAX 52800  /* Hz, from data sheet */
#define SPH061LU4H_RATE_MAX 1000000  /* Hz, from data sheet */

//#define SPH061LU4H_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S32)
//#define SPH061LU4H_FORMATS (SNDRV_PCM_FMTBIT_S16_BE | SNDRV_PCM_FMTBIT_S16_LE) 
#define SPH061LU4H_FORMATS (SNDRV_PCM_FMTBIT_S16_LE | SNDRV_PCM_FMTBIT_S16_BE |\
SNDRV_PCM_FMTBIT_U16_LE | SNDRV_PCM_FMTBIT_U16_BE |\
SNDRV_PCM_FMTBIT_S24_LE | SNDRV_PCM_FMTBIT_S24_BE |\
SNDRV_PCM_FMTBIT_U24_LE | SNDRV_PCM_FMTBIT_U24_BE |\
SNDRV_PCM_FMTBIT_S32_LE | SNDRV_PCM_FMTBIT_S32_BE |\
SNDRV_PCM_FMTBIT_U32_LE | SNDRV_PCM_FMTBIT_U32_BE)


static struct snd_soc_dai_driver sph061lu4h_dai = {
	.name = "sph061lu4h-hifi",
	.capture = {
		.stream_name = "Capture",
		.channels_min = 2,
		.channels_max = 2,
		.rate_min = SPH061LU4H_RATE_MIN,
		.rate_max = SPH061LU4H_RATE_MAX,
		.rates = SNDRV_PCM_RATE_CONTINUOUS,
		.formats = SPH061LU4H_FORMATS,
	},
};

static const struct snd_soc_component_driver sph061lu4h_component_driver = {
	.idle_bias_on		= 1,
	.use_pmdown_time	= 1,
	.endianness		= 1,
	.non_legacy_dai_naming	= 1,
};

static int sph061lu4h_probe(struct platform_device *pdev)
{
	return devm_snd_soc_register_component(&pdev->dev,
			&sph061lu4h_component_driver,
			&sph061lu4h_dai, 1);
}

#ifdef CONFIG_OF
static const struct of_device_id sph061lu4h_ids[] = {
	{ .compatible = "bugg,sph061lu4h", },
	{ }
};
MODULE_DEVICE_TABLE(of, sph061lu4h_ids);
#endif

static struct platform_driver sph061lu4h_driver = {
	.driver = {
		.name = "sph061lu4h",
		.of_match_table = of_match_ptr(sph061lu4h_ids),
	},
	.probe = sph061lu4h_probe,
};

module_platform_driver(sph061lu4h_driver);

MODULE_DESCRIPTION("ASoC SPH061LU4H driver");
MODULE_AUTHOR("Jeff Gough <j@monad.is>");
MODULE_LICENSE("GPL v2");
