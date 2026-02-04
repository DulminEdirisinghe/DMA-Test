# Usage with Vitis IDE:
# In Vitis IDE create a Single Application Debug launch configuration,
# change the debug type to 'Attach to running target' and provide this 
# tcl script in 'Execute Script' option.
# Path of this script: C:\Users\asus\Documents\Projects\Vivado\dma_transfer\DMA1\APP2_system\_ide\scripts\debugger_app2-default.tcl
# 
# 
# Usage with xsct:
# To debug using xsct, launch xsct and run below command
# source C:\Users\asus\Documents\Projects\Vivado\dma_transfer\DMA1\APP2_system\_ide\scripts\debugger_app2-default.tcl
# 
connect -url tcp:127.0.0.1:3121
targets -set -nocase -filter {name =~"APU*"}
rst -system
after 3000
targets -set -filter {jtag_cable_name =~ "Digilent Zybo 210279545290A" && level==0 && jtag_device_ctx=="jsn-Zybo-210279545290A-13722093-0"}
fpga -file C:/Users/asus/Documents/Projects/Vivado/dma_transfer/DMA1/APP2/_ide/bitstream/design_1_wrapper.bit
targets -set -nocase -filter {name =~"APU*"}
loadhw -hw C:/Users/asus/Documents/Vivado/DMA1/DMA/export/DMA/hw/design_1_wrapper.xsa -mem-ranges [list {0x40000000 0xbfffffff}] -regs
configparams force-mem-access 1
targets -set -nocase -filter {name =~"APU*"}
source C:/Users/asus/Documents/Projects/Vivado/dma_transfer/DMA1/APP2/_ide/psinit/ps7_init.tcl
ps7_init
ps7_post_config
targets -set -nocase -filter {name =~ "*A9*#0"}
dow C:/Users/asus/Documents/Projects/Vivado/dma_transfer/DMA1/APP2/Debug/APP2.elf
configparams force-mem-access 0
targets -set -nocase -filter {name =~ "*A9*#0"}
con
