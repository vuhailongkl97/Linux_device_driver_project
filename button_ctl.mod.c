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
	{ 0x103411bf, __VMLINUX_SYMBOL_STR(misc_deregister) },
	{ 0xfe990052, __VMLINUX_SYMBOL_STR(gpio_free) },
	{ 0x705c6b35, __VMLINUX_SYMBOL_STR(gpiod_unexport) },
	{ 0xc1514a3b, __VMLINUX_SYMBOL_STR(free_irq) },
	{ 0x88c9a5bc, __VMLINUX_SYMBOL_STR(misc_register) },
	{ 0xd6b8e852, __VMLINUX_SYMBOL_STR(request_threaded_irq) },
	{ 0xa90e6db8, __VMLINUX_SYMBOL_STR(gpiod_to_irq) },
	{ 0xce82b560, __VMLINUX_SYMBOL_STR(gpiod_get_raw_value) },
	{ 0x5b7f9c5f, __VMLINUX_SYMBOL_STR(gpiod_export) },
	{ 0x57dc7c6c, __VMLINUX_SYMBOL_STR(gpiod_set_debounce) },
	{ 0x87395c49, __VMLINUX_SYMBOL_STR(gpiod_direction_input) },
	{ 0x97b603c8, __VMLINUX_SYMBOL_STR(gpio_to_desc) },
	{ 0x47229b5c, __VMLINUX_SYMBOL_STR(gpio_request) },
	{ 0xdb7305a1, __VMLINUX_SYMBOL_STR(__stack_chk_fail) },
	{ 0x79ad7163, __VMLINUX_SYMBOL_STR(send_sig_info) },
	{ 0x8f678b07, __VMLINUX_SYMBOL_STR(__stack_chk_guard) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xecb3ce48, __VMLINUX_SYMBOL_STR(pid_task) },
	{ 0x70ee31e0, __VMLINUX_SYMBOL_STR(get_task_pid) },
	{ 0x2e5810c6, __VMLINUX_SYMBOL_STR(__aeabi_unwind_cpp_pr1) },
	{ 0xb1ad28e0, __VMLINUX_SYMBOL_STR(__gnu_mcount_nc) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "4AC26B0847306A4B7F1B57F");
