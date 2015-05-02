#include <linux/kernel.h> 	
#include <linux/module.h> 	
#include <linux/syscalls.h> 	 
#include <linux/types.h>	
#include <linux/sched.h>	
#include <linux/jiffies.h>
#include <linux/pid.h>
#include <linux/slab.h>
#include <asm/div64.h>

#define __NR_custom_syscall 322
#define Power_Coefficient 19

// variable to reference sys_call_table
unsigned long **sys_call_table;

// target syscall
asmlinkage int (*ref_sys_custom_syscall)(pid_t pid);

asmlinkage int sys_custom_syscall(pid_t pid)
{
	unsigned int i, j,  index_slash, index_eof, energy;
	unsigned long long created_time_in_ms, elapsed_time_in_ms, utime, stime, chdutime, chdstime, thdutime, thdstime, U, T;
	char *pathname = NULL, *p = NULL;
	
	struct mm_struct *mm;
	struct task_struct *curr;
	curr = pid_task(find_vpid(pid), PIDTYPE_PID);
	mm = curr->mm;

	if(mm)
	{
		down_read(&(mm->mmap_sem));
		if(mm->exe_file)
		{
			pathname = (char*)kmalloc(PATH_MAX, GFP_ATOMIC);
			if(pathname)
			{
				p = d_path(&(mm->exe_file->f_path), pathname, PATH_MAX);
			}
		}
		up_read(&(mm->mmap_sem));
	}

	index_slash = 0;
	index_eof = 0;
	for(i = 0; i < PATH_MAX; i++)
	{
		if(p[i] == '\0')
		{
			index_eof = i;
			break;
		}
		else if(p[i] == '/')
		{
			index_slash = i;
			continue;
		}
	}
	j = 0;
	for(i = (index_slash + 1); i <= index_eof; i++)
	{
		pathname[j] = p[i];
		j++;
	}

	created_time_in_ms = (unsigned long long)(curr->real_start_time / (1000 * 1000));
	elapsed_time_in_ms = (unsigned long long)((ktime_get_boot_ns() - curr->real_start_time) / (1000 * 1000));
	utime = cputime_to_jiffies(curr->utime);
	stime = cputime_to_jiffies(curr->stime);
	chdutime = curr->signal->cutime < curr->utime ? 0 : cputime_to_jiffies(curr->signal->cutime - curr->utime);
	chdstime = curr->signal->cstime < curr->stime ? 0 : cputime_to_jiffies(curr->signal->cstime - curr->stime);
	thdutime = cputime_to_jiffies(curr->signal->utime);
	thdstime = cputime_to_jiffies(curr->signal->stime);
	
	if((utime + chdutime + thdutime + stime + chdstime + thdstime) == 0)
	{
		U = ((utime + chdutime + thdutime) * 1000);
	}
	else
	{
		U = ((utime + chdutime + thdutime) * 1000) / (utime + chdutime + thdutime + stime + chdstime + thdstime);
	}

	T = elapsed_time_in_ms / 1000;	
	T = Power_Coefficient * T;

	energy = (int)((U * T) / 1000);

	printk(KERN_DEBUG "==================== Student Information ====================\n");
	printk(KERN_DEBUG "Student id: 0640353\n");
	printk(KERN_DEBUG "      Name: Lee DongHoe\n");
	printk(KERN_DEBUG "-------------------------------------------------------------\n");
	printk(KERN_DEBUG "   Process: \t%s(%6d)\n", pathname, (int)pid);
	printk(KERN_DEBUG "----------------------Time Information-----------------------\n");
	printk(KERN_DEBUG "   Created: \t%llu ms\n", created_time_in_ms);//curr->real_start_time / (1000 * 1000));
	printk(KERN_DEBUG "   Elapsed: \t%llu ms\n", elapsed_time_in_ms);//(ktime_get_boot_ns() - curr->real_start_time) / (1000 * 1000));
	printk(KERN_DEBUG " Frequency: \t%d Hz\n", HZ);
	printk(KERN_DEBUG "     utime: \t%llu ticks\n", utime);//cputime_to_jiffies(curr->utime));
	printk(KERN_DEBUG "     stime: \t%llu ticks\n", stime);//cputime_to_jiffies(curr->stime));
	printk(KERN_DEBUG " Chd.utime: \t%llu ticks\n", chdutime);//curr->signal->cutime < curr->utime ? 0 : cputime_to_jiffies(curr->signal->cutime - curr->utime));
	printk(KERN_DEBUG " Chd.stime: \t%llu ticks\n", chdstime);//curr->signal->cstime < curr->stime ? 0 : cputime_to_jiffies(curr->signal->cstime - curr->stime));
	printk(KERN_DEBUG " Thd.utime: \t%llu ticks\n", thdutime);//cputime_to_jiffies(curr->signal->utime));
	printk(KERN_DEBUG " Thd.stime: \t%llu ticks\n", thdstime);//cputime_to_jiffies(curr->signal->stime));
	printk(KERN_DEBUG "----------------------Energy Information---------------------\n");
	printk(KERN_DEBUG "    Energy: \t%d J\n", energy);
	printk(KERN_DEBUG "-------------------------------------------------------------\n");

	return energy;
}

// Do not edit bellow

static unsigned long **acquire_sys_call_table(void) 
{
	unsigned long int offset = PAGE_OFFSET;
	unsigned long **sct;
	
	while (offset < ULLONG_MAX) {
		sct = (unsigned long **)offset;

		if (sct[__NR_close] == (unsigned long *) sys_close)
			return sct; // end of sys_call_table

		offset += sizeof(void *);
	}

	return NULL;
}

static void disable_page_protection(void)
{
	unsigned long value;
	asm volatile("mov %%cr0, %0" : "=r" (value));

	if (!(value & 0x00010000))
		return;

	asm volatile("mov %0, %%cr0" : : "r" (value & ~0x00010000));
}

static void enable_page_protection(void)
{
	unsigned long value;
	asm volatile("mov %%cr0, %0" : "=r" (value));

	if ((value & 0x00010000))
		return;

	asm volatile("mov %0, %%cr0" : : "r" (value | 0x00010000));
}


// initialize function
int __init init_syscall_wrapper(void){	

	if (!(sys_call_table = acquire_sys_call_table()))
		return -1;

	disable_page_protection();
	ref_sys_custom_syscall = (void *)sys_call_table[__NR_custom_syscall];
	sys_call_table[__NR_custom_syscall] = (unsigned long *)sys_custom_syscall;
	enable_page_protection();
	printk(KERN_DEBUG "Custom Syscall Module Load\n");
	return 0;
}

// cleanup function
void __exit exit_syscall_wrapper(void){
	if (!sys_call_table)
		return;

	disable_page_protection();
	sys_call_table[__NR_custom_syscall] = (unsigned long *)ref_sys_custom_syscall;
	enable_page_protection();
	printk(KERN_DEBUG "Custom Syscall Module UnLoad\n");
}

module_init(init_syscall_wrapper);
module_exit(exit_syscall_wrapper);
MODULE_LICENSE("GPL");
