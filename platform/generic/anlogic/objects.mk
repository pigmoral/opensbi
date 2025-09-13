#
# SPDX-License-Identifier: BSD-2-Clause
#
# Copyright (C) 2026 Junhui Liu <junhui.liu@pigmoral.tech>
#

carray-platform_override_modules-$(CONFIG_PLATFORM_ANLOGIC_DR1V90) += anlogic_dr1v90
platform-objs-$(CONFIG_PLATFORM_ANLOGIC_DR1V90) += anlogic/dr1v90.o
