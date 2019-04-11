#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x977e40b4, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xf281db68, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0xf9a482f9, __VMLINUX_SYMBOL_STR(msleep) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0x97b603c8, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
	{ 0x28cc25db, __VMLINUX_SYMBOL_STR(arm_copy_from_user) },
	{ 0x28118cb6, __VMLINUX_SYMBOL_STR(__get_user_1) },
	{ 0x88c9a5bc, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0xfa2a45e, __VMLINUX_SYMBOL_STR(__memzero) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xa440881e, __VMLINUX_SYMBOL_STR(gpiod_direction_output_raw) },
	{ 0x8e865d3c, __VMLINUX_SYMBOL_STR(arm_delay_ops) },
	{ 0x67cdb1f7, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0x812e42e1, __VMLINUX_SYMBOL_STR(gpiod_set_raw_value) },
	{ 0x103411bf, __VMLINUX_SYMBOL_STR(misc_deregister) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";

