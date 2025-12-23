# 
# Usage: To re-create this platform project launch xsct with below options.
# xsct C:\Users\asus\Documents\Vivado\DMA1\DMA\platform.tcl
# 
# OR launch xsct and run below command.
# source C:\Users\asus\Documents\Vivado\DMA1\DMA\platform.tcl
# 
# To create the platform in a different location, modify the -out option of "platform create" command.
# -out option specifies the output directory of the platform project.

platform create -name {DMA}\
-hw {C:\Users\asus\Documents\Vivado\DMA1\design_1_wrapper.xsa}\
-proc {ps7_cortexa9_0} -os {standalone} -fsbl-target {psu_cortexa53_0} -out {C:/Users/asus/Documents/Vivado/DMA1}

platform write
platform generate -domains 
platform active {DMA}
platform generate
bsp reload
bsp setlib -name xilffs -ver 4.4
bsp write
bsp reload
catch {bsp regenerate}
platform generate -domains standalone_domain 
platform config -updatehw {C:/Users/asus/Documents/Vivado/DMA1/design_1_wrapper.xsa}
platform generate -domains 
platform active {DMA}
platform config -updatehw {C:/Users/asus/Documents/Projects/Vivado/DMA1/design_1_wrapper.xsa}
platform generate -domains 
