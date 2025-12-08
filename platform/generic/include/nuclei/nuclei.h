/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (C) 2026 Junhui Liu <junhui.liu@pigmoral.tech>
 */

#ifndef __RISCV_NUCLEI_H__
#define __RISCV_NUCLEI_H__

/* Cache Control and Maintenance CSRs */
#define CSR_MCACHE_CTL		0x7ca
#define CSR_CCM_MBEGINADDR	0x7cb
#define CSR_CCM_MCOMMAND	0x7cc
#define CSR_CCM_MDATA		0x7cd
#define CSR_CCM_SUEN		0x7ce

/* CSR_MCACHE_CTL */
#define MCACHE_CTL_IC_EN	BIT(0)
#define MCACHE_CTL_IC_PF_EN	BIT(6)
#define MCACHE_CTL_IC_CANCEL_EN	BIT(7)
#define MCACHE_CTL_DC_EN	BIT(16)

/* CSR_CCM_SUEN */
#define CCM_SUEN_UEN		BIT(0)
#define CCM_SUEN_SEN		BIT(1)
#define CCM_SUEN_UINVEN		BIT(8)
#define CCM_SUEN_SINVEN		BIT(9)
#define CCM_SUEN_UINVLEN	BIT(16)
#define CCM_SUEN_SINVLEN	BIT(17)
#define CCM_SUEN_UWBALLEN	BIT(24)
#define CCM_SUEN_SWBALLEN	BIT(25)

#endif /* __RISCV_NUCLEI_H__ */
