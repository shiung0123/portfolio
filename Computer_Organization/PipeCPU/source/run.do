quit -sim
vlib Final

vlog -work Final ./tb_PipeCPU.v
vlog -work Final ./src/*

vsim -novopt Final.tb_PipeCPU

do ./wave.do

run -all

