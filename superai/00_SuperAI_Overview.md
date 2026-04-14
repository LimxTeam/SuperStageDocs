# SuperAI 模块概述

## 简介

SuperAI 是 SuperStage 插件内置的 AI 智能助手模块。它的核心定位是 **SuperStage 资产的使用教程**——用户无需翻阅文档或观看教学视频，在编辑器内直接用中文提问，AI 即可给出操作指引并直接帮助完成场景搭建。

SuperAI 运行在 Unreal Engine 编辑器中，通过自然语言对话驱动 **35+ 个专用工具**，覆盖场景搭建、属性配置、材质编辑、蓝图操作、资产管理、视口控制等完整工作流。

## 系统架构

```
┌──────────────────────────────────────────────┐
│                  聊天面板 UI                    │
│             SSuperAIChat (Slate)               │
│  消息气泡 · 流式输出 · 工具调用可视化 · 会话管理   │
└──────────────────┬───────────────────────────┘
                   │ 用户消息
                   ▼
┌──────────────────────────────────────────────┐
│             后端通信客户端                       │
│          FSuperAIBackendClient                 │
│    流式 SSE 解析 · 消息组装 · 多轮对话上下文       │
└──────────────────┬───────────────────────────┘
                   │ LLM 返回工具调用
                   ▼
┌──────────────────────────────────────────────┐
│              工具注册表                         │
│          FSuperAIToolRegistry                  │
│   35+ 工具 · 自描述 Schema · 参数校验            │
└──────────────────┬───────────────────────────┘
                   │ 分发到具体工具
                   ▼
┌──────────────────────────────────────────────┐
│            工具基类 + 具体工具                    │
│     FSuperAIToolBase → FSuperAITool_Xxx        │
│   UE 反射 · Actor 操作 · 资产管理 · 材质图编辑    │
└──────────────────────────────────────────────┘
                   │ MCP 协议
                   ▼
┌──────────────────────────────────────────────┐
│            MCP HTTP 服务器                      │
│          FSuperAIMCPServer                     │
│  向外部 MCP 客户端暴露全部工具 · 手动启停控制       │
└──────────────────────────────────────────────┘
```

### 核心组件

| 组件 | 类名 | 职责 |
|------|------|------|
| 模块入口 | `FSuperAIModule` | 生命周期管理、单例持有、面板注册 |
| 聊天面板 | `SSuperAIChat` | 消息显示、用户输入、流式输出、工具调用可视化 |
| 后端客户端 | `FSuperAIBackendClient` | LLM API 通信、流式 SSE 解析、多轮对话 |
| 工具注册表 | `FSuperAIToolRegistry` | 工具注册与发现、Schema 生成 |
| 工具基类 | `FSuperAIToolBase` | 参数提取、Actor 查找、属性反射导航 |
| 会话管理器 | `FSuperAIChatSessionManager` | 多会话保存/切换/删除 |
| MCP 服务器 | `FSuperAIMCPServer` | HTTP 服务器，向外部工具暴露 AI 能力 |
| 配置管理器 | `FSuperAIConfigManager` | API Key 安全存储、模型参数配置 |

### 文件结构

```
Source/SuperAI/
├── Public/
│   ├── SuperAIModule.h              — 模块入口声明
│   ├── SuperAITypes.h               — 类型定义（消息、工具、事件）
│   ├── SuperAIConfig.h              — 配置管理器声明
│   ├── ISuperAITool.h               — 工具接口
│   └── UI/
│       └── SSuperAIChat.h           — 聊天面板 Widget
├── Private/
│   ├── SuperAIModule.cpp            — 模块启动/关闭
│   ├── SuperAIConfig.cpp            — 配置序列化/反序列化
│   ├── SuperAIBackendClient.cpp     — LLM 通信
│   ├── SuperAIChatSessionManager.cpp — 会话管理
│   ├── SuperAIMCPServer.cpp         — MCP HTTP 服务器
│   ├── SuperAIToolRegistry.cpp      — 工具注册表
│   ├── UI/
│   │   └── SSuperAIChat.cpp         — 聊天面板实现
│   └── Tools/                       — 35+ 个工具目录
│       ├── SuperAIToolBase.h/cpp    — 工具基类
│       ├── ListSuperAssets/         — SuperStage 资产列表
│       ├── SpawnActor/              — 生成 Actor
│       ├── SetProperty/             — 设置属性
│       ├── MaterialEditor/          — 材质图编辑
│       └── ...                      — 更多工具
```

## 通信协议

### 聊天流程

1. 用户在聊天面板输入消息
2. `FSuperAIBackendClient` 将消息发送至 LLM API（支持流式 SSE）
3. LLM 返回文本回复或工具调用请求
4. 工具调用由 `FSuperAIToolRegistry` 分发到具体工具执行
5. 执行结果回传 LLM，LLM 基于结果继续回复
6. 流式文本逐字显示在聊天面板中

### MCP 协议

SuperAI 内置 MCP（Model Context Protocol）HTTP 服务器，允许外部 AI 客户端（如 Claude Desktop、Cursor 等）通过标准 MCP 协议调用全部 35+ 个工具。

- **启动方式**：在聊天面板状态栏点击 MCP 开关按钮
- **通信方式**：HTTP SSE（Server-Sent Events）
- **安全控制**：手动启停，不使用时不占用端口和资源

## 配置管理

配置文件存储在 `Saved/SuperAI/Config.sav`，使用二进制格式：

| 配置项 | 说明 |
|--------|------|
| API Key | LLM 服务的 API 密钥（XOR 混淆存储） |
| API 端点 | LLM 服务地址 |
| 模型名称 | 使用的 LLM 模型 |
| 系统提示 | 自定义系统提示词 |

在聊天面板点击设置按钮即可修改配置。
