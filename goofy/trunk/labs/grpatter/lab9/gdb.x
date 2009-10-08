define run
monitor reset
monitor soft_reset_halt
set $pc = 0
end
target remote localhost:3333
monitor arm7_9 force_hw_bkpts enable
b main
set
