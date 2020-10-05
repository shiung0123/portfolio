onerror {resume}
quietly WaveActivateNextPane {} 0

#PC
add wave -noupdate /tb_PipeCPU/*
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/PCSrc}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/Jump_EX}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/pc}
#add wave -noupdate /tb_PipeCPU/CPU/InstrMem/mem_array
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/opcode_IF}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/funct_IF}

#IF/ID
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/CTL/opcode}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/CTL/*}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/ALUCTL/*}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/RN1}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/RN2}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/RD1}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/RD2}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/shamt}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/immed}


#ID/EX
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/CTL/func}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/ALU/dataOut}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/SHIFTER/dataOut}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/MULTIPLIER/counter}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/HILO/HiOut}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/HILO/LoOut}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/MUX/dataOut}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/b_offset}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/jump_addr}

#EX/MEM
#add wave -noupdate {/tb_PipeCPU/CPU/DatMem/mem_array[8]}
#add wave -noupdate {/tb_PipeCPU/CPU/DatMem/mem_array[9]}
#add wave -noupdate {/tb_PipeCPU/CPU/DatMem/mem_array[10]}
#add wave -noupdate {/tb_PipeCPU/CPU/DatMem/mem_array[11]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/DatMem/addr}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/DatMem/wd}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/DatMem/rd}

#MEM/WB
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array}
#add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/RegWrite}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/WN}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/WD}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[8]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[9]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[16]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[17]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[18]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[19]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[20]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[21]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[22]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[23]}
add wave -noupdate -unsigned {/tb_PipeCPU/CPU/RegFile/file_array[24]}

configure wave -timeline 0
configure wave -timelineunits ns
update

