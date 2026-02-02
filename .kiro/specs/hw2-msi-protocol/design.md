# 设计文档

## 概述

本设计文档描述了基于Murphi形式化验证工具的MSI和MESI缓存一致性协议的实现方案。设计采用基于侦听的共享总线架构，支持3个缓存和1个内存块的简化模型，用于验证协议的正确性。

## 架构

### 系统架构图

```
┌─────────────┐    ┌─────────────┐    ┌─────────────┐
│  Cache 0    │    │  Cache 1    │    │  Cache 2    │
│  (Snooper)  │    │  (Snooper)  │    │  (Snooper)  │
└──────┬──────┘    └──────┬──────┘    └──────┬──────┘
       │                  │                  │
       └──────────────────┼──────────────────┘
                          │
                    ┌─────▼─────┐
                    │ Shared Bus│
                    │ (Broadcast)│
                    └─────┬─────┘
                          │
                    ┌─────▼─────┐
                    │  Memory   │
                    │ Controller│
                    └───────────┘
```

### 组件说明

1. **缓存控制器（Cache Controller）**：每个处理器拥有一个缓存，包含一个缓存行，管理状态转换和数据操作
2. **共享总线（Shared Bus）**：连接所有缓存和内存，支持广播机制，传输请求类型、数据和控制信号
3. **内存控制器（Memory Controller）**：管理主存数据，在没有缓存响应时提供数据
4. **侦听器（Snooper）**：每个缓存的组件，监听总线事务并做出响应

## 组件和接口

### 数据结构设计

#### 缓存状态类型（MSI）
```murphi
StateType : enum {M, S, I}
```
- **M (Modified)**：缓存拥有唯一有效副本，数据可能与内存不一致
- **S (Shared)**：缓存拥有共享副本，数据与内存一致
- **I (Invalid)**：缓存行无效

#### 缓存状态类型（MESI）
```murphi
StateType : enum {M, E, S, I}
```
- **M (Modified)**：缓存拥有唯一有效副本，数据已修改
- **E (Exclusive)**：缓存拥有唯一有效副本，数据与内存一致
- **S (Shared)**：缓存拥有共享副本，数据与内存一致
- **I (Invalid)**：缓存行无效

#### 总线请求类型
```murphi
BusReqType : enum {BusNone, BusRd, BusRdX, BusUpgr}
```
- **BusNone**：总线空闲
- **BusRd**：读请求，请求共享数据副本
- **BusRdX**：独占读请求，请求独占数据副本
- **BusUpgr**：升级请求（仅MESI），从S状态升级到M状态

#### 总线控制类型
```murphi
BusCtrlType : enum {CtrlNone, CtrlFlush, CtrlFlushOpt}
```
- **CtrlNone**：无控制信号
- **CtrlFlush**：需要刷新数据到总线
- **CtrlFlushOpt**：可选刷新（MESI优化）

#### 总线结构
```murphi
Bus : record
  req     : BusReqType;    -- 请求类型
  source  : CacheId;       -- 请求源缓存ID
  data    : -1..15;        -- 数据（-1表示无数据）
  ctrl    : BusCtrlType;   -- 控制信号
end;
```

### 接口设计

#### 处理器接口
- **PrRd（处理器读）**：处理器发起读操作
- **PrWr（处理器写）**：处理器发起写操作

#### 总线接口
- **BusRd**：在总线上广播读请求
- **BusRdX**：在总线上广播独占读请求
- **BusUpgr**：在总线上广播升级请求（MESI）

#### 侦听接口
- **Snoop(CacheId)**：侦听总线事务并响应

## MSI协议设计

### 状态转换表

#### 表1：CPU操作触发的状态转换（MSI）

| 当前状态 | CPU操作 | 触发的总线事务 | 转移后状态 | 说明 |
|---------|---------|---------------|-----------|------|
| M       | PrRd    | 无            | M         | 命中，直接读取 |
| M       | PrWr    | 无            | M         | 命中，直接写入 |
| S       | PrRd    | 无            | S         | 命中，直接读取 |
| S       | PrWr    | BusRdX        | M         | 需要独占权限 |
| I       | PrRd    | BusRd         | S         | 缺失，请求共享副本 |
| I       | PrWr    | BusRdX        | M         | 缺失，请求独占副本 |

#### 表2：侦听总线事务触发的状态转换（MSI）

| 当前状态 | 侦听到的总线事务 | Cache执行的操作 | 转移后状态 | 说明 |
|---------|-----------------|----------------|-----------|------|
| M       | BusRd           | 写回数据到总线  | S         | 提供数据，降级为共享 |
| M       | BusRdX          | 写回数据到总线  | I         | 提供数据，失效 |
| S       | BusRd           | 无             | S         | 保持共享状态 |
| S       | BusRdX          | 无             | I         | 失效缓存行 |
| I       | BusRd           | 无             | I         | 保持无效状态 |
| I       | BusRdX          | 无             | I         | 保持无效状态 |

### MSI协议实现逻辑

#### 处理器读请求（PrRd）

**状态M：**
```murphi
rule "PrRd, Cache State M"
  (caches[i].line.state = M) & (bus.req = BusNone)
==>
begin
  -- 命中，直接读取，无需操作
  -- 状态保持M
end;
```

**状态S：**
```murphi
rule "PrRd, Cache State S"
  (caches[i].line.state = S) & (bus.req = BusNone)
==>
begin
  -- 命中，直接读取，无需操作
  -- 状态保持S
end;
```

**状态I：**
```murphi
rule "PrRd, Cache State I"
  (caches[i].line.state = I) & (bus.req = BusNone)
==>
begin
  -- 缺失，发起BusRd请求
  bus.req := BusRd;
  bus.source := i;
  -- 等待总线处理后转换到S状态
end;
```

#### 处理器写请求（PrWr）

**状态M：**
```murphi
rule "PrWr, Cache State M"
  (caches[i].line.state = M) & (bus.req = BusNone)
==>
begin
  -- 命中，直接写入
  caches[i].line.data := (caches[i].line.data + 1) % 16;
  -- 状态保持M
end;
```

**状态S：**
```murphi
rule "PrWr, Cache State S"
  (caches[i].line.state = S) & (bus.req = BusNone)
==>
begin
  -- 需要独占权限，发起BusRdX请求
  bus.req := BusRdX;
  bus.source := i;
  -- 等待总线处理后转换到M状态并写入
end;
```

**状态I：**
```murphi
rule "PrWr, Cache State I"
  (caches[i].line.state = I) & (bus.req = BusNone)
==>
begin
  -- 缺失，发起BusRdX请求
  bus.req := BusRdX;
  bus.source := i;
  -- 等待总线处理后转换到M状态并写入
end;
```

#### 侦听逻辑（Snoop）

```murphi
procedure Snoop(i: CacheId);
begin
  if bus.req = BusRd then
    switch caches[i].line.state
      case M:
        -- 提供数据，降级为S
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlush;
        caches[i].line.state := S;
      
      case S:
        -- 保持S状态，无需操作
        
      case I:
        -- 保持I状态，无需操作
    endswitch;
    
  elsif bus.req = BusRdX then
    switch caches[i].line.state
      case M:
        -- 提供数据，失效
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlush;
        caches[i].line.state := I;
        undefine caches[i].line.data;
      
      case S:
        -- 失效缓存行
        caches[i].line.state := I;
        undefine caches[i].line.data;
      
      case I:
        -- 保持I状态，无需操作
    endswitch;
  endif;
end;
```

#### 总线事务处理

```murphi
rule "Process bus transaction"
  bus.req != BusNone
==>
begin
  -- 1. 所有缓存侦听并响应
  for i : CacheId do
    if i != bus.source then
      Snoop(i);
    endif;
  endfor;
  
  -- 2. 如果没有缓存提供数据，内存响应
  if bus.data = -1 then
    bus.data := mem.data;
  endif;
  
  -- 3. 更新请求源缓存的状态和数据
  if bus.req = BusRd then
    caches[bus.source].line.state := S;
    caches[bus.source].line.data := bus.data;
  elsif bus.req = BusRdX then
    caches[bus.source].line.state := M;
    caches[bus.source].line.data := bus.data;
    -- 如果是从S或I状态写，执行写操作
    caches[bus.source].line.data := (caches[bus.source].line.data + 1) % 16;
  endif;
  
  -- 4. 如果有数据写回，更新内存
  if bus.ctrl = CtrlFlush then
    mem.data := bus.data;
  endif;
  
  -- 5. 清除总线状态
  bus.req := BusNone;
  undefine bus.source;
  bus.data := -1;
  bus.ctrl := CtrlNone;
end;
```

## MESI协议设计

### 状态转换表

#### 表3：CPU操作触发的状态转换（MESI）

| 当前状态 | CPU操作 | 触发的总线事务 | 转移后状态 | 说明 |
|---------|---------|---------------|-----------|------|
| M       | PrRd    | 无            | M         | 命中，直接读取 |
| M       | PrWr    | 无            | M         | 命中，直接写入 |
| E       | PrRd    | 无            | E         | 命中，直接读取 |
| E       | PrWr    | 无            | M         | 命中，直接写入，升级到M |
| S       | PrRd    | 无            | S         | 命中，直接读取 |
| S       | PrWr    | BusUpgr       | M         | 需要失效其他副本 |
| I       | PrRd    | BusRd         | E或S      | 缺失，根据是否有其他副本决定 |
| I       | PrWr    | BusRdX        | M         | 缺失，请求独占副本 |

#### 表4：侦听总线事务触发的状态转换（MESI）

| 当前状态 | 侦听到的总线事务 | Cache执行的操作 | 转移后状态 | 说明 |
|---------|-----------------|----------------|-----------|------|
| M       | BusRd           | 写回数据到总线  | S         | 提供数据，降级为共享 |
| M       | BusRdX          | 写回数据到总线  | I         | 提供数据，失效 |
| E       | BusRd           | 写回数据到总线  | S         | 提供数据，降级为共享 |
| E       | BusRdX          | 写回数据到总线  | I         | 提供数据，失效 |
| S       | BusRd           | 无             | S         | 保持共享状态 |
| S       | BusRdX          | 无             | I         | 失效缓存行 |
| S       | BusUpgr         | 无             | I         | 失效缓存行 |
| I       | BusRd           | 无             | I         | 保持无效状态 |
| I       | BusRdX          | 无             | I         | 保持无效状态 |
| I       | BusUpgr         | 无             | I         | 保持无效状态 |

### MESI协议关键改进

#### E状态的引入

E（Exclusive）状态是MESI相对于MSI的关键改进：

1. **减少总线流量**：当缓存独占数据时，从E状态写入可以直接升级到M状态，无需总线事务
2. **优化写操作**：避免了MSI协议中S状态写入时必须发起BusRdX的开销
3. **判断逻辑**：通过侦听BusRd时是否有其他缓存响应来判断进入E还是S状态

#### BusUpgr事务

BusUpgr是MESI协议的优化：

1. **用途**：从S状态升级到M状态时使用
2. **优势**：相比BusRdX，BusUpgr不需要传输数据，只需失效其他缓存
3. **效率**：减少了总线数据传输，提高了性能

### MESI协议实现逻辑

#### 处理器读请求（PrRd）

**状态E：**
```murphi
rule "PrRd, Cache State E"
  (caches[i].line.state = E) & (bus.req = BusNone)
==>
begin
  -- 命中，直接读取，无需操作
  -- 状态保持E
end;
```

**状态I（关键逻辑）：**
```murphi
rule "PrRd, Cache State I"
  (caches[i].line.state = I) & (bus.req = BusNone)
==>
begin
  -- 缺失，发起BusRd请求
  bus.req := BusRd;
  bus.source := i;
  -- 等待总线处理
  -- 如果有其他缓存响应（bus.ctrl = CtrlFlush），转换到S
  -- 如果没有其他缓存响应，转换到E
end;
```

#### 处理器写请求（PrWr）

**状态E：**
```murphi
rule "PrWr, Cache State E"
  (caches[i].line.state = E) & (bus.req = BusNone)
==>
begin
  -- 命中，直接写入并升级到M
  caches[i].line.data := (caches[i].line.data + 1) % 16;
  caches[i].line.state := M;
  -- 无需总线事务
end;
```

**状态S（使用BusUpgr）：**
```murphi
rule "PrWr, Cache State S"
  (caches[i].line.state = S) & (bus.req = BusNone)
==>
begin
  -- 需要失效其他副本，发起BusUpgr请求
  bus.req := BusUpgr;
  bus.source := i;
  -- 等待总线处理后转换到M状态并写入
end;
```

#### 侦听逻辑（Snoop）

```murphi
procedure Snoop(i: CacheId);
begin
  if bus.req = BusRd then
    switch caches[i].line.state
      case M:
        -- 提供数据，降级为S
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlush;
        caches[i].line.state := S;
      
      case E:
        -- 提供数据，降级为S
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlushOpt;
        caches[i].line.state := S;
      
      case S:
        -- 标记有共享副本
        bus.ctrl := CtrlFlushOpt;
      
      case I:
        -- 保持I状态
    endswitch;
    
  elsif bus.req = BusRdX then
    switch caches[i].line.state
      case M:
        -- 提供数据，失效
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlush;
        caches[i].line.state := I;
        undefine caches[i].line.data;
      
      case E:
        -- 提供数据，失效
        bus.data := caches[i].line.data;
        bus.ctrl := CtrlFlush;
        caches[i].line.state := I;
        undefine caches[i].line.data;
      
      case S:
        -- 失效缓存行
        caches[i].line.state := I;
        undefine caches[i].line.data;
      
      case I:
        -- 保持I状态
    endswitch;
    
  elsif bus.req = BusUpgr then
    switch caches[i].line.state
      case S:
        -- 失效缓存行
        caches[i].line.state := I;
        undefine caches[i].line.data;
      
      case I:
        -- 保持I状态
    endswitch;
  endif;
end;
```

#### 总线事务处理（关键改进）

```murphi
rule "Process bus request"
  bus.req != BusNone
==>
begin
  -- 1. 所有缓存侦听并响应
  for i : CacheId do
    if i != bus.source then
      Snoop(i);
    endif;
  endfor;
  
  -- 2. 如果没有缓存提供数据，内存响应
  if bus.data = -1 then
    bus.data := mem.data;
  endif;
  
  -- 3. 更新请求源缓存的状态和数据
  if bus.req = BusRd then
    -- 根据是否有其他缓存响应决定E或S
    if bus.ctrl = CtrlNone then
      caches[bus.source].line.state := E;  -- 独占
    else
      caches[bus.source].line.state := S;  -- 共享
    endif;
    caches[bus.source].line.data := bus.data;
    
  elsif bus.req = BusRdX then
    caches[bus.source].line.state := M;
    caches[bus.source].line.data := bus.data;
    caches[bus.source].line.data := (caches[bus.source].line.data + 1) % 16;
    
  elsif bus.req = BusUpgr then
    -- 从S升级到M，已有数据，只需改状态和写入
    caches[bus.source].line.state := M;
    caches[bus.source].line.data := (caches[bus.source].line.data + 1) % 16;
  endif;
  
  -- 4. 如果有数据写回，更新内存
  if bus.ctrl = CtrlFlush then
    mem.data := bus.data;
  endif;
  
  -- 5. 清除总线状态
  bus.req := BusNone;
  undefine bus.source;
  bus.data := -1;
  bus.ctrl := CtrlNone;
end;
```

## 错误处理

### 不变式验证

#### MSI协议不变式

1. **唯一修改者**：最多只有一个缓存处于M状态
2. **共享一致性**：所有S状态缓存具有相同数据
3. **互斥性**：M状态存在时不存在S状态
4. **内存一致性**：无M状态时，S状态缓存数据与内存一致

#### MESI协议不变式

1. **唯一独占者**：最多只有一个缓存处于M或E状态
2. **共享一致性**：所有S状态缓存具有相同数据
3. **互斥性**：M或E状态存在时不存在S状态
4. **内存一致性**：无M状态时，S和E状态缓存数据与内存一致

### 调试策略

1. **使用-tv选项**：生成违规轨迹，快速定位问题
2. **简化场景**：将NUM_CACHE从3减少到2，简化调试
3. **添加临时不变式**：辅助定位具体问题点
4. **分析状态序列**：理解状态转换的因果关系

## 测试策略

### 验证目标

1. **正确性验证**：确保所有不变式在所有可达状态中成立
2. **完整性验证**：确保所有状态转换都被正确实现
3. **性能评估**：比较MSI和MESI的状态空间大小

### 测试步骤

1. **编译Murphi模型**：`./mu msi.m` 和 `./mu mesi.m`
2. **构建验证器**：`make msi` 和 `make mesi`
3. **运行验证**：`./msi` 和 `./mesi`
4. **检查输出**：确认"No error found"和状态数量
5. **调试错误**：使用`./msi -tv`查看错误轨迹

### 预期结果

- **MSI协议**：约22000-25000个状态，45000-50000条规则
- **MESI协议**：状态数量可能略有不同，但应无错误
- **运行时间**：应在1秒内完成验证

## 设计决策和理由

### 简化假设

1. **单缓存行**：每个缓存只有一个缓存行，简化状态空间
2. **单内存块**：只有一个内存单元，简化验证
3. **3个缓存**：足以验证协议正确性，同时保持状态空间可管理
4. **原子操作**：所有规则执行都是原子的，简化并发模型

### 关键设计选择

1. **侦听优先**：缓存侦听器优先于内存控制器响应
2. **数据范围**：数据值限制在0-15，减少状态空间
3. **总线仲裁**：通过BusNone状态确保总线互斥访问
4. **状态转换原子性**：每个规则完整执行一次状态转换

### MESI优化分析

1. **E状态优势**：减少独占数据写入时的总线事务
2. **BusUpgr优势**：减少S到M转换时的数据传输
3. **性能提升**：在写密集型工作负载中显著减少总线流量
4. **复杂度代价**：增加了一个状态和一个总线事务类型

## 协议问题和改进方向

### MSI协议问题

1. **写操作开销**：S状态写入需要BusRdX，即使已有数据
2. **总线流量**：频繁的数据传输增加总线负载
3. **伪共享**：不同数据在同一缓存行导致不必要的失效

### MESI协议问题

1. **状态复杂度**：增加了E状态，增加了实现复杂度
2. **判断开销**：需要判断是否有其他缓存持有数据
3. **仍存在伪共享**：E状态不能完全解决伪共享问题

### 可能的改进方向

1. **MOESI协议**：添加O（Owned）状态，允许脏数据共享
2. **目录协议**：使用目录替代广播，提高可扩展性
3. **分层协议**：多级缓存一致性协议
4. **点对点协议**：如CHI协议，避免总线瓶颈
5. **细粒度一致性**：字级或块级一致性，减少伪共享
