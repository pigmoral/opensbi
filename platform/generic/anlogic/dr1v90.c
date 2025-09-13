/*
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Copyright (c) 2025 Junhui Liu <junhui.liu@pigmoral.tech>
 */

#include <platform_override.h>
#include <sbi/riscv_io.h>
#include <sbi/sbi_console.h>

#define CSR_MNOCB		0x7F5
#define CSR_MNOCM		0x7F6
#define CSR_MCACHE_CTL		0x7CA
#define CSR_CCM_MCOMMAND	0x7CC
#define CSR_CCM_SUEN		0x7CE
#define CSR_MMISC_CTL		0x7D0

#define CCM_SUEN_ENABLE		0x03030303
#define CSR_CACHE_ENABLE	0x100C1
#define CCM_DC_INVAL_ALL	0x17
#define CCM_IC_INVAL_ALL	0xd

#define DR1V90_TIMER_ADDR	0x68030000
#define DR1V90_TIMER_SIZE	0x10000
#define DR1V90_TIMER_ALIGN	0x1000
#define DR1V90_TIMER_MTIMECTL	0xFF8

static int dr1v90_early_init(bool cold_boot)
{
	int rc;

	rc = generic_early_init(cold_boot);
	if (rc)
		return rc;

	/*
	 * There are only 8 PMPs in DR1V90. Merge the whole
	 * timer region to save PMPs.
	 */
	if (cold_boot)
		return sbi_domain_root_add_memrange(
					(ulong)DR1V90_TIMER_ADDR,
					DR1V90_TIMER_SIZE,
					DR1V90_TIMER_ALIGN,
					(SBI_DOMAIN_MEMREGION_MMIO |
					 SBI_DOMAIN_MEMREGION_M_READABLE |
					 SBI_DOMAIN_MEMREGION_M_WRITABLE));

	return 0;
}

static int dr1v90_final_init(bool cold_boot)
{
	void *mtimectl = (void *)(DR1V90_TIMER_ADDR + DR1V90_TIMER_MTIMECTL);
	u32 value;

	if (cold_boot) {
		/* select clk src */
		value = readl(mtimectl);
		writel(value | 0x4, mtimectl);

		/* Enable cache */
		csr_write(CSR_CCM_MCOMMAND, CCM_DC_INVAL_ALL);
		csr_write(CSR_CCM_MCOMMAND, CCM_IC_INVAL_ALL);
		csr_write(CSR_MCACHE_CTL, CSR_CACHE_ENABLE);

		/* Enable S/U mode CCM operation */
		csr_write(CSR_CCM_SUEN, CCM_SUEN_ENABLE);
	}

	return generic_final_init(cold_boot);
}

static int dr1v90_platform_init(const void *fdt, int nodeoff,
				const struct fdt_match *match)
{
	generic_platform_ops.early_init = dr1v90_early_init;
	generic_platform_ops.final_init = dr1v90_final_init;

	return 0;
}

static const struct fdt_match anlogic_dr1v90_match[] = {
	{ .compatible = "anlogic,dr1v90" },
	{ },
};

const struct fdt_driver anlogic_dr1v90 = {
	.match_table	= anlogic_dr1v90_match,
	.init		= dr1v90_platform_init,
};
