#!/usr/bin/env python3
"""
生成实验报告用的图表
使用 matplotlib 创建可视化图表
"""

import csv
import matplotlib.pyplot as plt
import numpy as np

# 设置中文字体
plt.rcParams['font.family'] = ['Arial Unicode MS', 'SimHei', 'DejaVu Sans']
plt.rcParams['axes.unicode_minus'] = False

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
    return [row for row in data if row['numCycles'] is not None]

def create_iq_impact_chart(data):
    """创建IQ影响分析图表"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    # 图1: IQ对性能的影响（固定regs=256, rob=64）
    subset = [row for row in data if row['regs'] == 256 and row['rob'] == 64]
    subset.sort(key=lambda x: x['iq'])
    
    iq_values = [row['iq'] for row in subset]
    cycles = [row['numCycles'] for row in subset]
    
    ax1.plot(iq_values, cycles, 'o-', linewidth=2, markersize=8, color='blue')
    ax1.set_xlabel('IQ 条目数')
    ax1.set_ylabel('CPU Cycles')
    ax1.set_title('IQ 条目数对性能的影响\n(物理寄存器=256, ROB=64)')
    ax1.grid(True, alpha=0.3)
    ax1.set_yscale('log')
    
    # 添加数值标签
    for i, (x, y) in enumerate(zip(iq_values, cycles)):
        ax1.annotate(f'{y:,}', (x, y), textcoords="offset points", xytext=(0,10), ha='center')
    
    # 图2: 不同配置下的IQ影响
    colors = ['blue', 'red', 'green']
    rob_values = [64, 256]
    
    for i, rob in enumerate(rob_values):
        subset = [row for row in data if row['regs'] == 256 and row['rob'] == rob]
        subset.sort(key=lambda x: x['iq'])
        
        iq_vals = [row['iq'] for row in subset]
        cyc_vals = [row['numCycles'] for row in subset]
        
        ax2.plot(iq_vals, cyc_vals, 'o-', linewidth=2, markersize=6, 
                label=f'ROB={rob}', color=colors[i])
    
    ax2.set_xlabel('IQ 条目数')
    ax2.set_ylabel('CPU Cycles')
    ax2.set_title('不同ROB配置下IQ的影响\n(物理寄存器=256)')
    ax2.grid(True, alpha=0.3)
    ax2.legend()
    ax2.set_yscale('log')
    
    plt.tight_layout()
    plt.savefig('/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/iq_impact.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

def create_rob_impact_chart(data):
    """创建ROB影响分析图表"""
    fig, (ax1, ax2) = plt.subplots(1, 2, figsize=(15, 6))
    
    # 图1: ROB对性能的影响（固定regs=256, iq=64）
    subset = [row for row in data if row['regs'] == 256 and row['iq'] == 64]
    subset.sort(key=lambda x: x['rob'])
    
    rob_values = [row['rob'] for row in subset]
    cycles = [row['numCycles'] for row in subset]
    
    ax1.plot(rob_values, cycles, 's-', linewidth=2, markersize=8, color='red')
    ax1.set_xlabel('ROB 条目数')
    ax1.set_ylabel('CPU Cycles')
    ax1.set_title('ROB 条目数对性能的影响\n(物理寄存器=256, IQ=64)')
    ax1.grid(True, alpha=0.3)
    ax1.set_yscale('log')
    
    # 添加数值标签
    for i, (x, y) in enumerate(zip(rob_values, cycles)):
        ax1.annotate(f'{y:,}', (x, y), textcoords="offset points", xytext=(0,10), ha='center')
    
    # 图2: 不同IQ配置下的ROB影响
    colors = ['red', 'orange', 'purple']
    iq_values = [16, 64, 256]
    
    for i, iq in enumerate(iq_values):
        subset = [row for row in data if row['regs'] == 256 and row['iq'] == iq]
        subset.sort(key=lambda x: x['rob'])
        
        rob_vals = [row['rob'] for row in subset]
        cyc_vals = [row['numCycles'] for row in subset]
        
        ax2.plot(rob_vals, cyc_vals, 's-', linewidth=2, markersize=6, 
                label=f'IQ={iq}', color=colors[i])
    
    ax2.set_xlabel('ROB 条目数')
    ax2.set_ylabel('CPU Cycles')
    ax2.set_title('不同IQ配置下ROB的影响\n(物理寄存器=256)')
    ax2.grid(True, alpha=0.3)
    ax2.legend()
    ax2.set_yscale('log')
    
    plt.tight_layout()
    plt.savefig('/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/rob_impact.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

def create_regs_impact_chart(data):
    """创建物理寄存器影响分析图表"""
    fig, ax = plt.subplots(1, 1, figsize=(10, 6))
    
    # 不同配置下的物理寄存器影响
    configs = [(64, 256), (256, 256)]
    colors = ['green', 'darkgreen']
    
    for i, (iq, rob) in enumerate(configs):
        subset = [row for row in data if row['iq'] == iq and row['rob'] == rob]
        subset.sort(key=lambda x: x['regs'])
        
        regs_vals = [row['regs'] for row in subset]
        cyc_vals = [row['numCycles'] for row in subset]
        
        ax.plot(regs_vals, cyc_vals, '^-', linewidth=2, markersize=8, 
               label=f'IQ={iq}, ROB={rob}', color=colors[i])
        
        # 添加数值标签
        for x, y in zip(regs_vals, cyc_vals):
            ax.annotate(f'{y:,}', (x, y), textcoords="offset points", 
                       xytext=(0,10), ha='center', fontsize=9)
    
    ax.set_xlabel('物理寄存器数')
    ax.set_ylabel('CPU Cycles')
    ax.set_title('物理寄存器数对性能的影响')
    ax.grid(True, alpha=0.3)
    ax.legend()
    ax.set_yscale('log')
    
    plt.tight_layout()
    plt.savefig('/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/regs_impact.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

def create_performance_overview(data):
    """创建性能概览图表"""
    fig, ((ax1, ax2), (ax3, ax4)) = plt.subplots(2, 2, figsize=(16, 12))
    
    # 图1: 最佳vs最差配置对比
    sorted_data = sorted(data, key=lambda x: x['numCycles'])
    best_10 = sorted_data[:10]
    worst_10 = sorted_data[-10:]
    
    best_cycles = [row['numCycles'] for row in best_10]
    worst_cycles = [row['numCycles'] for row in worst_10]
    
    ax1.bar(range(10), best_cycles, color='green', alpha=0.7, label='最佳10个配置')
    ax1.bar(range(10, 20), worst_cycles, color='red', alpha=0.7, label='最差10个配置')
    ax1.set_xlabel('配置排名')
    ax1.set_ylabel('CPU Cycles')
    ax1.set_title('最佳与最差配置性能对比')
    ax1.legend()
    ax1.set_yscale('log')
    
    # 图2: 性能分布直方图
    all_cycles = [row['numCycles'] for row in data]
    ax2.hist(all_cycles, bins=20, color='skyblue', alpha=0.7, edgecolor='black')
    ax2.set_xlabel('CPU Cycles')
    ax2.set_ylabel('配置数量')
    ax2.set_title('性能分布直方图')
    ax2.grid(True, alpha=0.3)
    
    # 图3: 阻塞事件分析
    rob_full = [row['ROBFull'] for row in data]
    iq_full = [row['IQFull'] for row in data]
    regs_full = [row['FullRegs'] for row in data]
    
    categories = ['ROB阻塞', 'IQ阻塞', '寄存器阻塞']
    avg_blocks = [np.mean(rob_full), np.mean(iq_full), np.mean(regs_full)]
    
    bars = ax3.bar(categories, avg_blocks, color=['red', 'orange', 'purple'], alpha=0.7)
    ax3.set_ylabel('平均阻塞次数')
    ax3.set_title('各种阻塞事件统计')
    ax3.grid(True, alpha=0.3)
    
    # 添加数值标签
    for bar, val in zip(bars, avg_blocks):
        ax3.annotate(f'{val:,.0f}', (bar.get_x() + bar.get_width()/2, bar.get_height()),
                    ha='center', va='bottom')
    
    # 图4: 参数相关性热力图（简化版）
    # 计算不同参数组合的平均性能
    param_impact = {}
    for row in data:
        key = f"R{row['regs']}_I{row['iq']}_B{row['rob']}"
        param_impact[key] = row['numCycles']
    
    # 选择几个代表性配置
    selected_configs = [
        "R64_I4_B4", "R64_I64_B64", "R256_I64_B64", 
        "R256_I256_B256", "R1024_I256_B256"
    ]
    
    config_names = ["最小配置", "中等配置1", "中等配置2", "大配置", "最大配置"]
    config_cycles = [param_impact.get(cfg, 0) for cfg in selected_configs]
    
    bars = ax4.bar(config_names, config_cycles, color='lightblue', alpha=0.7, edgecolor='navy')
    ax4.set_ylabel('CPU Cycles')
    ax4.set_title('典型配置性能对比')
    ax4.tick_params(axis='x', rotation=45)
    ax4.set_yscale('log')
    
    # 添加数值标签
    for bar, val in zip(bars, config_cycles):
        if val > 0:
            ax4.annotate(f'{val:,}', (bar.get_x() + bar.get_width()/2, bar.get_height()),
                        ha='center', va='bottom', rotation=0)
    
    plt.tight_layout()
    plt.savefig('/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/performance_overview.png', 
                dpi=300, bbox_inches='tight')
    plt.close()

def main():
    """主函数"""
    try:
        import matplotlib.pyplot as plt
    except ImportError:
        print("错误: 需要安装 matplotlib")
        print("请运行: pip3 install matplotlib")
        return
    
    csv_path = '/Users/lianglihang/Downloads/ComputerArchitecture/lab1/out/summary.csv'
    data = load_data(csv_path)
    
    if not data:
        print("无法加载数据")
        return
    
    print(f"加载了 {len(data)} 个有效仿真结果")
    print("正在生成图表...")
    
    create_iq_impact_chart(data)
    print("✅ IQ影响分析图表已生成: out/iq_impact.png")
    
    create_rob_impact_chart(data)
    print("✅ ROB影响分析图表已生成: out/rob_impact.png")
    
    create_regs_impact_chart(data)
    print("✅ 物理寄存器影响分析图表已生成: out/regs_impact.png")
    
    create_performance_overview(data)
    print("✅ 性能概览图表已生成: out/performance_overview.png")
    
    print("\n所有图表已生成完成！")

if __name__ == "__main__":
    main()

