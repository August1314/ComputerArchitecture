#!/usr/bin/env bash

set -euo pipefail

# 配置项（可根据需要覆盖）
GEM5_BIN=${GEM5_BIN:-/opt/gem5/build/RISCV/gem5.opt}
O3CONF=${O3CONF:-/lab1/O3CPU.py}
CMD_BIN=${CMD_BIN:-/lab1/daxpy.riscv}
OUT_BASE=${OUT_BASE:-/lab1/out}

mkdir -p "${OUT_BASE}"

declare -a REGS=(64 256 1024)
declare -a IQS=(4 16 64 256)
declare -a ROBS=(4 16 64 256)

summary_tmp=$(mktemp)
echo "regs,iq,rob,numCycles,ROBFull,IQFull,FullRegs" > "${OUT_BASE}/summary.csv"

for regs in "${REGS[@]}"; do
  for iq in "${IQS[@]}"; do
    for rob in "${ROBS[@]}"; do
      odir="${OUT_BASE}/regs${regs}-iq${iq}-rob${rob}"
      stats="${odir}/stats.txt"
      
      # 跳过已完成的组合
      if [ -f "${stats}" ]; then
        echo "[SKIP] regs=${regs} iq=${iq} rob=${rob} -> already exists" >&2
        numCycles=$(grep '^system.cpu.numCycles' "${stats}" | awk '{print $2}' || echo "NA")
        robFull=$(grep '^system.cpu.rename.ROBFullEvents' "${stats}" | awk '{print $2}' || echo "NA")
        iqFull=$(grep '^system.cpu.rename.IQFullEvents' "${stats}" | awk '{print $2}' || echo "NA")
        fullRegs=$(grep '^system.cpu.rename.fullRegistersEvents' "${stats}" | awk '{print $2}' || echo "NA")
        echo "${regs},${iq},${rob},${numCycles:-NA},${robFull:-NA},${iqFull:-NA},${fullRegs:-NA}" >> "${OUT_BASE}/summary.csv"
        continue
      fi
      
      mkdir -p "${odir}"
      echo "[RUN] regs=${regs} iq=${iq} rob=${rob} -> ${odir}" >&2
      "${GEM5_BIN}" -d "${odir}" \
        "${O3CONF}" \
        --cmd="${CMD_BIN}" \
        --num-phys-int-regs="${regs}" \
        --num-iq-entries="${iq}" \
        --num-rob-entries="${rob}"

      numCycles=$(grep '^system.cpu.numCycles' "${stats}" | awk '{print $2}' || echo "NA")
      robFull=$(grep '^system.cpu.rename.ROBFullEvents' "${stats}" | awk '{print $2}' || echo "NA")
      iqFull=$(grep '^system.cpu.rename.IQFullEvents' "${stats}" | awk '{print $2}' || echo "NA")
      fullRegs=$(grep '^system.cpu.rename.fullRegistersEvents' "${stats}" | awk '{print $2}' || echo "NA")
      echo "${regs},${iq},${rob},${numCycles:-NA},${robFull:-NA},${iqFull:-NA},${fullRegs:-NA}" >> "${OUT_BASE}/summary.csv"
    done
  done
done

echo "Done. Summary at: ${OUT_BASE}/summary.csv" >&2



