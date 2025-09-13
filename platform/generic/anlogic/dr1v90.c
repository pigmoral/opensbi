/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2023-2026 Anlogic Corporation or its affiliates.
 * Copyright (c) 2026 Junhui Liu <junhui.liu@pigmoral.tech>
 */

#include <nuclei/nuclei.h>
#include <platform_override.h>
#include <sbi/riscv_io.h>
#include <sbi/sbi_console.h>

#define CCM_SUEN_ENABLE		(CCM_SUEN_UEN | CCM_SUEN_SEN |			\
				 CCM_SUEN_UINVEN | CCM_SUEN_SINVEN |		\
				 CCM_SUEN_UINVLEN | CCM_SUEN_SINVLEN |		\
				 CCM_SUEN_UWBALLEN | CCM_SUEN_SWBALLEN)
#define CSR_CACHE_ENABLE	(MCACHE_CTL_IC_EN | MCACHE_CTL_IC_PF_EN |	\
				 MCACHE_CTL_IC_CANCEL_EN | MCACHE_CTL_DC_EN)

/* Timer unit */
#define TIMER_ADDR		0x68030000

#define TIMER_MTIMECTL_OFFSET	0xff8
#define TIMER_MTIMECTL_CLKSRC	BIT(2)

#define TIMER_MMODE_SIZE	0xd000
#define TIMER_MMODE_ALIGN	0x1000

static int dr1v90_early_init(bool cold_boot)
{
	int rc;

	rc = generic_early_init(cold_boot);
	if (rc)
		return rc;

	if (cold_boot) {
		/* For the Timer m-mode region */
		rc = sbi_domain_root_add_memrange(TIMER_ADDR, TIMER_MMODE_SIZE, TIMER_MMODE_ALIGN,
						  (SBI_DOMAIN_MEMREGION_MMIO |
						   SBI_DOMAIN_MEMREGION_M_READABLE |
						   SBI_DOMAIN_MEMREGION_M_WRITABLE));
		if (rc)
			return rc;
	}

	return 0;
}

static int dr1v90_final_init(bool cold_boot)
{
	void *mtimectl = (void *)(TIMER_ADDR + TIMER_MTIMECTL_OFFSET);
	u32 val;

	if (cold_boot) {
		/* Select core_aon_clk as clock source */
		val = readl(mtimectl);
		writel(val | TIMER_MTIMECTL_CLKSRC, mtimectl);

		/* Enable cache */
		csr_write(CSR_MCACHE_CTL, CSR_CACHE_ENABLE);

		/* Enable CCM operations in S/U mode */
		csr_write(CSR_CCM_SUEN,	CCM_SUEN_ENABLE);
	}

	return generic_final_init(cold_boot);
}

static bool dr1v90_single_fw_region(void)
{
	return true;
}

static int dr1v90_platform_init(const void *fdt, int nodeoff,
				const struct fdt_match *match)
{
	generic_platform_ops.early_init = dr1v90_early_init;
	generic_platform_ops.final_init = dr1v90_final_init;
	generic_platform_ops.single_fw_region = dr1v90_single_fw_region;

	return 0;
}

static const struct fdt_match anlogic_dr1v90_match[] = {
	{ .compatible = "anlogic,dr1v90" },
	{ /* sentinel */},
};

const struct fdt_driver anlogic_dr1v90 = {
	.match_table	= anlogic_dr1v90_match,
	.init		= dr1v90_platform_init,
};
