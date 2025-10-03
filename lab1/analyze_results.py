#!/usr/bin/env python3
"""
gem5 O3 CPU 仿真结果分析脚本
分析 IQ/ROB/物理寄存器数对性能的影响
"""

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np

# 设置中文字体
plt.rcParams['font.sans-serif'] = ['SimHei', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

def load_data(csv_path):
    """加载仿真结果数据"""
    df = pd.read_csv(csv_path)
    # 转换数据类型
    df['numCycles'] = pd.to_numeric(df['numCycles'], errors='coerce')
    df['ROBFull'] = pd.to_numeric(df['ROBFull'], errors='coerce').fillna(0)
    df['IQFull'] = pd.to_numeric(df['IQFull'], errors='coerce').fillna(0)
    df['FullRegs'] = pd.to_numeric(df['FullRegs'], errors='coerce').fillna(0)
    return df

def analyze_iq_impact(df):
    """分析 IQ 条目数对性能的影响"""
    print("=== IQ 条目数对性能的影响分析 ===")
    
    # 固定其他参数，分析 IQ 的影响
    for regs in [64, 256, 1024]:
        for rob in [64, 256]:
            subset = df[(df['regs'] == regs) & (df['rob'] == rob)].copy()
            if len(subset) > 0:
                subset = subset.sort_values('iq')
                print(f"\n物理寄存器={regs}, ROB={rob}:")
                for _, row in subset.iterrows():
                    print(f"  IQ={row['iq']:3d}: {row['numCycles']:,} cycles")

def analyze_rob_impact(df):
    """分析 ROB 条目数对性能的影响"""
    print("\n=== ROB 条目数对性能的影响分析 ===")
    
    # 固定其他参数，分析 ROB 的影响
    for regs in [64, 256, 1024]:
        for iq in [16, 64]:
            subset = df[(df['regs'] == regs) & (df['iq'] == iq)].copy()
            if len(subset) > 0:
                subset = subset.sort_values('rob')
                print(f"\n物理寄存器={regs}, IQ={iq}:")
                for _, row in subset.iterrows():
                    print(f"  ROB={row['rob']:3d}: {row['numCycles']:,} cycles")

def analyze_regs_impact(df):
    """分析物理寄存器数对性能的影响"""
    print("\n=== 物理寄存器数对性能的影响分析 ===")
    
    # 固定其他参数，分析物理寄存器的影响
    for iq in [16, 64]:
        for rob in [64, 256]:
            subset = df[(df['iq'] == iq) & (df['rob'] == rob)].copy()
            if len(subset) > 0:
                subset = subset.sort_values('regs')
                print(f"\nIQ={iq}, ROB={rob}:")
                for _, row in subset.iterrows():
                    print(f"  物理寄存器={row['regs']:4d}: {row['numCycles']:,} cycles")

def create_heatmaps(df):
    """创建热力图"""
    fig, axes = plt.subplots(1, 3, figsize=(18, 6))
    
    for i, regs in enumerate([64, 256, 1024]):
        subset = df[df['regs'] == regs]
        pivot = subset.pivot(index='rob', columns='iq', values='numCycles')
        
        sns.heatmap(pivot, annot=True, fmt='.0f', cmap='YlOrRd', 
                   ax=axes[i], cbar_kws={'label': 'CPU Cycles'})
        axes[i].set_title(f'物理寄存器数 = {regs}')
        axes[i].set_xlabel('IQ 条目数')
        axes[i].set_ylabel('ROB 条目数')
    
    plt.tight_layout()
    plt.savefig('/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/heatmaps.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

def create_line_plots(df):
    """创建折线图分析趋势"""
    fig, axes = plt.subplots(2, 2, figsize=(15, 12))
    
    # IQ 影响 (固定 regs=256, rob=64)
    subset = df[(df['regs'] == 256) & (df['rob'] == 64)].sort_values('iq')
    axes[0,0].plot(subset['iq'], subset['numCycles'], 'o-', linewidth=2, markersize=8)
    axes[0,0].set_xlabel('IQ 条目数')
    axes[0,0].set_ylabel('CPU Cycles')
    axes[0,0].set_title('IQ 条目数对性能的影响\n(物理寄存器=256, ROB=64)')
    axes[0,0].grid(True, alpha=0.3)
    
    # ROB 影响 (固定 regs=256, iq=64)
    subset = df[(df['regs'] == 256) & (df['iq'] == 64)].sort_values('rob')
    axes[0,1].plot(subset['rob'], subset['numCycles'], 's-', linewidth=2, markersize=8, color='orange')
    axes[0,1].set_xlabel('ROB 条目数')
    axes[0,1].set_ylabel('CPU Cycles')
    axes[0,1].set_title('ROB 条目数对性能的影响\n(物理寄存器=256, IQ=64)')
    axes[0,1].grid(True, alpha=0.3)
    
    # 物理寄存器影响 (固定 iq=64, rob=64)
    subset = df[(df['iq'] == 64) & (df['rob'] == 64)].sort_values('regs')
    axes[1,0].plot(subset['regs'], subset['numCycles'], '^-', linewidth=2, markersize=8, color='green')
    axes[1,0].set_xlabel('物理寄存器数')
    axes[1,0].set_ylabel('CPU Cycles')
    axes[1,0].set_title('物理寄存器数对性能的影响\n(IQ=64, ROB=64)')
    axes[1,0].grid(True, alpha=0.3)
    
    # 阻塞事件分析
    subset = df[(df['regs'] == 64)].groupby('iq')[['ROBFull', 'IQFull', 'FullRegs']].mean()
    x = subset.index
    axes[1,1].bar(x - 0.2, subset['ROBFull'], 0.2, label='ROB Full', alpha=0.7)
    axes[1,1].bar(x, subset['IQFull'], 0.2, label='IQ Full', alpha=0.7)
    axes[1,1].bar(x + 0.2, subset['FullRegs'], 0.2, label='Regs Full', alpha=0.7)
    axes[1,1].set_xlabel('IQ 条目数')
    axes[1,1].set_ylabel('平均阻塞事件数')
    axes[1,1].set_title('资源阻塞事件分析\n(物理寄存器=64)')
    axes[1,1].legend()
    axes[1,1].grid(True, alpha=0.3)
    
    plt.tight_layout()
    plt.savefig('/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/line_plots.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

def generate_summary_table(df):
    """生成汇总表格"""
    # 找出最佳和最差配置
    best_config = df.loc[df['numCycles'].idxmin()]
    worst_config = df.loc[df['numCycles'].idxmax()]
    
    print("\n=== 性能汇总 ===")
    print(f"最佳配置: 物理寄存器={best_config['regs']}, IQ={best_config['iq']}, ROB={best_config['rob']}")
    print(f"         CPU Cycles: {best_config['numCycles']:,}")
    print(f"最差配置: 物理寄存器={worst_config['regs']}, IQ={worst_config['iq']}, ROB={worst_config['rob']}")
    print(f"         CPU Cycles: {worst_config['numCycles']:,}")
    print(f"性能差距: {worst_config['numCycles'] / best_config['numCycles']:.2f}x")

def main():
    """主函数"""
    # 加载数据
    df = load_data('/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/summary.csv')
    
    print(f"加载了 {len(df)} 个仿真结果")
    print(f"参数组合: 物理寄存器 {sorted(df['regs'].unique())}")
    print(f"         IQ {sorted(df['iq'].unique())}")
    print(f"         ROB {sorted(df['rob'].unique())}")
    
    # 数据分析
    analyze_iq_impact(df)
    analyze_rob_impact(df)
    analyze_regs_impact(df)
    generate_summary_table(df)
    
    # 创建可视化
    print("\n正在生成可视化图表...")
    create_heatmaps(df)
    create_line_plots(df)
    print("图表已保存到 out/ 目录")

if __name__ == "__main__":
    main()

