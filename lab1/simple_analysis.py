#!/usr/bin/env python3
"""
gem5 O3 CPU 仿真结果简单分析脚本
仅使用Python标准库进行数据分析
"""

import csv
import sys

def load_data(csv_path):
    """加载仿真结果数据"""
    data = []
    with open(csv_path, 'r') as f:
        reader = csv.DictReader(f)
        for row in reader:
            try:
                row['regs'] = int(row['regs'])
                row['iq'] = int(row['iq'])
                row['rob'] = int(row['rob'])
                row['numCycles'] = int(row['numCycles']) if row['numCycles'] != 'NA' else None
                row['ROBFull'] = int(row['ROBFull']) if row['ROBFull'] != 'NA' else 0
                row['IQFull'] = int(row['IQFull']) if row['IQFull'] != 'NA' else 0
                row['FullRegs'] = int(row['FullRegs']) if row['FullRegs'] != 'NA' else 0
                data.append(row)
            except ValueError:
                continue
    return data

def analyze_iq_impact(data):
    """分析 IQ 条目数对性能的影响"""
    print("=== IQ 条目数对性能的影响分析 ===")
    
    # 按物理寄存器和ROB分组分析
    groups = {}
    for row in data:
        if row['numCycles'] is None:
            continue
        key = (row['regs'], row['rob'])
        if key not in groups:
            groups[key] = []
        groups[key].append(row)
    
    for (regs, rob), group in sorted(groups.items()):
        if len(group) >= 3:  # 至少有3个不同的IQ值
            group.sort(key=lambda x: x['iq'])
            print(f"\n物理寄存器={regs}, ROB={rob}:")
            prev_cycles = None
            for row in group:
                cycles = row['numCycles']
                change = ""
                if prev_cycles:
                    ratio = prev_cycles / cycles
                    if ratio > 1.1:
                        change = f" (提升 {ratio:.2f}x)"
                    elif ratio < 0.9:
                        change = f" (下降 {1/ratio:.2f}x)"
                print(f"  IQ={row['iq']:3d}: {cycles:,} cycles{change}")
                prev_cycles = cycles

def analyze_rob_impact(data):
    """分析 ROB 条目数对性能的影响"""
    print("\n=== ROB 条目数对性能的影响分析 ===")
    
    # 按物理寄存器和IQ分组分析
    groups = {}
    for row in data:
        if row['numCycles'] is None:
            continue
        key = (row['regs'], row['iq'])
        if key not in groups:
            groups[key] = []
        groups[key].append(row)
    
    for (regs, iq), group in sorted(groups.items()):
        if len(group) >= 3:  # 至少有3个不同的ROB值
            group.sort(key=lambda x: x['rob'])
            print(f"\n物理寄存器={regs}, IQ={iq}:")
            prev_cycles = None
            for row in group:
                cycles = row['numCycles']
                change = ""
                if prev_cycles:
                    ratio = prev_cycles / cycles
                    if ratio > 1.1:
                        change = f" (提升 {ratio:.2f}x)"
                    elif ratio < 0.9:
                        change = f" (下降 {1/ratio:.2f}x)"
                print(f"  ROB={row['rob']:3d}: {cycles:,} cycles{change}")
                prev_cycles = cycles

def analyze_regs_impact(data):
    """分析物理寄存器数对性能的影响"""
    print("\n=== 物理寄存器数对性能的影响分析 ===")
    
    # 按IQ和ROB分组分析
    groups = {}
    for row in data:
        if row['numCycles'] is None:
            continue
        key = (row['iq'], row['rob'])
        if key not in groups:
            groups[key] = []
        groups[key].append(row)
    
    for (iq, rob), group in sorted(groups.items()):
        if len(group) >= 2:  # 至少有2个不同的寄存器数
            group.sort(key=lambda x: x['regs'])
            print(f"\nIQ={iq}, ROB={rob}:")
            prev_cycles = None
            for row in group:
                cycles = row['numCycles']
                change = ""
                if prev_cycles:
                    ratio = prev_cycles / cycles
                    if ratio > 1.1:
                        change = f" (提升 {ratio:.2f}x)"
                    elif ratio < 0.9:
                        change = f" (下降 {1/ratio:.2f}x)"
                print(f"  物理寄存器={row['regs']:4d}: {cycles:,} cycles{change}")
                prev_cycles = cycles

def analyze_bottlenecks(data):
    """分析性能瓶颈"""
    print("\n=== 性能瓶颈分析 ===")
    
    # 找出阻塞事件最多的配置
    high_rob_full = [row for row in data if row['ROBFull'] > 10000000]
    high_iq_full = [row for row in data if row['IQFull'] > 10000000]
    high_regs_full = [row for row in data if row['FullRegs'] > 1000000]
    
    if high_rob_full:
        print("\nROB 阻塞严重的配置:")
        for row in sorted(high_rob_full, key=lambda x: x['ROBFull'], reverse=True)[:3]:
            print(f"  regs={row['regs']}, iq={row['iq']}, rob={row['rob']}: "
                  f"ROB阻塞={row['ROBFull']:,}, cycles={row['numCycles']:,}")
    
    if high_iq_full:
        print("\nIQ 阻塞严重的配置:")
        for row in sorted(high_iq_full, key=lambda x: x['IQFull'], reverse=True)[:3]:
            print(f"  regs={row['regs']}, iq={row['iq']}, rob={row['rob']}: "
                  f"IQ阻塞={row['IQFull']:,}, cycles={row['numCycles']:,}")
    
    if high_regs_full:
        print("\n物理寄存器阻塞严重的配置:")
        for row in sorted(high_regs_full, key=lambda x: x['FullRegs'], reverse=True)[:3]:
            print(f"  regs={row['regs']}, iq={row['iq']}, rob={row['rob']}: "
                  f"寄存器阻塞={row['FullRegs']:,}, cycles={row['numCycles']:,}")

def generate_summary_table(data):
    """生成汇总表格"""
    valid_data = [row for row in data if row['numCycles'] is not None]
    
    if not valid_data:
        print("没有有效的性能数据")
        return
    
    # 找出最佳和最差配置
    best_config = min(valid_data, key=lambda x: x['numCycles'])
    worst_config = max(valid_data, key=lambda x: x['numCycles'])
    
    print("\n=== 性能汇总 ===")
    print(f"总共完成 {len(valid_data)} 个配置的仿真")
    print(f"\n最佳配置: 物理寄存器={best_config['regs']}, IQ={best_config['iq']}, ROB={best_config['rob']}")
    print(f"         CPU Cycles: {best_config['numCycles']:,}")
    print(f"最差配置: 物理寄存器={worst_config['regs']}, IQ={worst_config['iq']}, ROB={worst_config['rob']}")
    print(f"         CPU Cycles: {worst_config['numCycles']:,}")
    print(f"性能差距: {worst_config['numCycles'] / best_config['numCycles']:.2f}x")
    
    # 生成简单的表格
    print(f"\n{'regs':>4} {'iq':>3} {'rob':>3} {'cycles':>12} {'ROBFull':>10} {'IQFull':>10} {'RegsF':>8}")
    print("-" * 60)
    for row in sorted(valid_data, key=lambda x: x['numCycles'])[:10]:
        print(f"{row['regs']:>4} {row['iq']:>3} {row['rob']:>3} "
              f"{row['numCycles']:>12,} {row['ROBFull']:>10,} "
              f"{row['IQFull']:>10,} {row['FullRegs']:>8,}")

def main():
    """主函数"""
    csv_path = '/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/summary.csv'
    
    # 加载数据
    data = load_data(csv_path)
    
    if not data:
        print("无法加载数据")
        return
    
    print(f"加载了 {len(data)} 个仿真结果")
    
    # 数据分析
    analyze_iq_impact(data)
    analyze_rob_impact(data)
    analyze_regs_impact(data)
    analyze_bottlenecks(data)
    generate_summary_table(data)

if __name__ == "__main__":
    main()

