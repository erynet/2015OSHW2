#include <linux/module.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

MODULE_INFO(vermagic, VERMAGIC_STRING);

__visible struct module __this_module
__attribute__((section(".gnu.linkonce.this_module"))) = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

static const struct modversion_info ____versions[]
__used
__attribute__((section("__versions"))) = {
	{ 0x71637d48, __VMLINUX_SYMBOL_STR(module_layout) },
	{ 0xdcb0349b, __VMLINUX_SYMBOL_STR(sys_close) },
	{ 0x27e1a049, __VMLINUX_SYMBOL_STR(printk) },
	{ 0xe4689576, __VMLINUX_SYMBOL_STR(ktime_get_with_offset) },
	{ 0xdc825571, __VMLINUX_SYMBOL_STR(up_read) },
	{ 0x3587adac, __VMLINUX_SYMBOL_STR(d_path) },
	{ 0xf4ea5844, __VMLINUX_SYMBOL_STR(kmem_cache_alloc_trace) },
	{ 0xc0ef160f, __VMLINUX_SYMBOL_STR(kmalloc_caches) },
	{ 0x5add0a88, __VMLINUX_SYMBOL_STR(down_read) },
	{ 0xabfc46e7, __VMLINUX_SYMBOL_STR(pid_task) },
	{ 0x21c4447f, __VMLINUX_SYMBOL_STR(find_vpid) },
	{ 0xbdfb6dbb, __VMLINUX_SYMBOL_STR(__fentry__) },
};

static const char __module_depends[]
__used
__attribute__((section(".modinfo"))) =
"depends=";


MODULE_INFO(srcversion, "7F65DE9C82C0683896836D0");
