#!/usr/bin/env python3
import csv
import sys
from pathlib import Path

"""
解析递归目录下所有 stats.txt，输出 CSV：
cols: regs, iq, rob, numCycles, ROBFull, IQFull, FullRegs, outdir
目录命名约定：.../regs{R}-iq{I}-rob{B}/stats.txt
"""

def parse_triplet_from_outdir(outdir: Path):
    name = outdir.name
    parts = name.split('-')
    d = {}
    for p in parts:
        if p.startswith('regs'):
            d['regs'] = p[len('regs'):]
        elif p.startswith('iq'):
            d['iq'] = p[len('iq'):]
        elif p.startswith('rob'):
            d['rob'] = p[len('rob'):]
    return d.get('regs'), d.get('iq'), d.get('rob')

def extract_metric(lines, key):
    for ln in lines:
        if ln.startswith(key + '\t') or ln.startswith(key + ' '):
            parts = ln.split()
            if len(parts) >= 2:
                return parts[1]
    return 'NA'

def main():
    base = Path(sys.argv[1]) if len(sys.argv) > 1 else Path('out')
    rows = []
    for stats in base.rglob('stats.txt'):
        outdir = stats.parent
        regs, iq, rob = parse_triplet_from_outdir(outdir)
        lines = stats.read_text(encoding='utf-8', errors='ignore').splitlines()
        row = {
            'regs': regs or 'NA',
            'iq': iq or 'NA',
            'rob': rob or 'NA',
            'numCycles': extract_metric(lines, 'system.cpu.numCycles'),
            'ROBFull': extract_metric(lines, 'system.cpu.rename.ROBFullEvents'),
            'IQFull': extract_metric(lines, 'system.cpu.rename.IQFullEvents'),
            'FullRegs': extract_metric(lines, 'system.cpu.rename.fullRegistersEvents'),
            'outdir': str(outdir),
        }
        rows.append(row)

    writer = csv.DictWriter(sys.stdout, fieldnames=list(rows[0].keys()) if rows else [
        'regs','iq','rob','numCycles','ROBFull','IQFull','FullRegs','outdir'])
    writer.writeheader()
    for r in rows:
        writer.writerow(r)

if __name__ == '__main__':
    main()





