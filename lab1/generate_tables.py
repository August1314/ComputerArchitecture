#!/usr/bin/env python3
"""
生成实验报告用的表格
"""

import csv

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

def generate_complete_table(data):
    """生成完整的参数组合表格"""
    print("完整实验结果表格（前20个最佳配置）")
    print("=" * 80)
    print(f"{'物理寄存器':>8} {'IQ':>4} {'ROB':>4} {'CPU Cycles':>12} {'ROB阻塞':>10} {'IQ阻塞':>10} {'寄存器阻塞':>10}")
    print("-" * 80)
    
    valid_data = [row for row in data if row['numCycles'] is not None]
    sorted_data = sorted(valid_data, key=lambda x: x['numCycles'])
    
    for i, row in enumerate(sorted_data[:20]):
        print(f"{row['regs']:>8} {row['iq']:>4} {row['rob']:>4} "
              f"{row['numCycles']:>12,} {row['ROBFull']:>10,} "
              f"{row['IQFull']:>10,} {row['FullRegs']:>10,}")

def generate_iq_analysis_table(data):
    """生成IQ影响分析表格"""
    print("\n\nIQ条目数对性能的影响（固定物理寄存器=256, ROB=64）")
    print("=" * 50)
    print(f"{'IQ条目数':>8} {'CPU Cycles':>12} {'性能提升':>10}")
    print("-" * 50)
    
    subset = [row for row in data if row['regs'] == 256 and row['rob'] == 64 and row['numCycles']]
    subset.sort(key=lambda x: x['iq'])
    
    baseline = None
    for row in subset:
        if baseline is None:
            baseline = row['numCycles']
            improvement = "基线"
        else:
            improvement = f"{baseline / row['numCycles']:.2f}x"
        print(f"{row['iq']:>8} {row['numCycles']:>12,} {improvement:>10}")

def generate_rob_analysis_table(data):
    """生成ROB影响分析表格"""
    print("\n\nROB条目数对性能的影响（固定物理寄存器=256, IQ=64）")
    print("=" * 50)
    print(f"{'ROB条目数':>8} {'CPU Cycles':>12} {'性能提升':>10}")
    print("-" * 50)
    
    subset = [row for row in data if row['regs'] == 256 and row['iq'] == 64 and row['numCycles']]
    subset.sort(key=lambda x: x['rob'])
    
    baseline = None
    for row in subset:
        if baseline is None:
            baseline = row['numCycles']
            improvement = "基线"
        else:
            improvement = f"{baseline / row['numCycles']:.2f}x"
        print(f"{row['rob']:>8} {row['numCycles']:>12,} {improvement:>10}")

def generate_regs_analysis_table(data):
    """生成物理寄存器影响分析表格"""
    print("\n\n物理寄存器数对性能的影响（固定IQ=64, ROB=256）")
    print("=" * 50)
    print(f"{'物理寄存器数':>10} {'CPU Cycles':>12} {'性能提升':>10}")
    print("-" * 50)
    
    subset = [row for row in data if row['iq'] == 64 and row['rob'] == 256 and row['numCycles']]
    subset.sort(key=lambda x: x['regs'])
    
    baseline = None
    for row in subset:
        if baseline is None:
            baseline = row['numCycles']
            improvement = "基线"
        else:
            improvement = f"{baseline / row['numCycles']:.2f}x"
        print(f"{row['regs']:>10} {row['numCycles']:>12,} {improvement:>10}")

def generate_bottleneck_table(data):
    """生成瓶颈分析表格"""
    print("\n\n性能瓶颈分析")
    print("=" * 80)
    
    # 最差的几个配置
    valid_data = [row for row in data if row['numCycles'] is not None]
    worst_configs = sorted(valid_data, key=lambda x: x['numCycles'], reverse=True)[:5]
    
    print("最差性能配置:")
    print(f"{'物理寄存器':>8} {'IQ':>4} {'ROB':>4} {'CPU Cycles':>12} {'主要瓶颈':>15}")
    print("-" * 60)
    
    for row in worst_configs:
        bottleneck = "未知"
        if row['ROBFull'] > 10000000:
            bottleneck = "ROB阻塞"
        elif row['IQFull'] > 10000000:
            bottleneck = "IQ阻塞"
        elif row['FullRegs'] > 1000000:
            bottleneck = "寄存器阻塞"
        
        print(f"{row['regs']:>8} {row['iq']:>4} {row['rob']:>4} "
              f"{row['numCycles']:>12,} {bottleneck:>15}")

def main():
    """主函数"""
    csv_path = '/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/summary.csv'
    data = load_data(csv_path)
    
    if not data:
        print("无法加载数据")
        return
    
    generate_complete_table(data)
    generate_iq_analysis_table(data)
    generate_rob_analysis_table(data)
    generate_regs_analysis_table(data)
    generate_bottleneck_table(data)

if __name__ == "__main__":
    main()

