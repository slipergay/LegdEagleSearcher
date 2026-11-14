# 🦅 LegdEagleSearcher

**Legd 系列第二作 · The Second Work of the Legd Series**

快速、精准、高效的本地文件分类与搜索工具。
A fast, precise, and efficient local file classification and search tool.

[English](#-english) | [中文](#-中文)

---

<div align="center">

**Qt 6 · C++17 · MIT License**

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](./LICENSE)
[![Qt](https://img.shields.io/badge/Qt-6.x-41cd52?logo=qt)](https://www.qt.io/)
[![Platform](https://img.shields.io/badge/Platform-Windows%20%7C%20Linux%20%7C%20macOS-lightgrey)](https://github.com/slipergay/LegdEagleSearcher)
[![Release](https://img.shields.io/badge/Release-v1.0.0-blue)](https://github.com/slipergay/LegdEagleSearcher/releases)

</div>

---

## 🇨🇳 中文

### ✨ 功能特性

- 📂 **递归扫描** — 深度遍历目录，自动缓存相对路径，避免重复扫描
- 🗂️ **智能分类** — 文件夹 / 文档 / 图片 / 音频 / 视频 / 程序 / 压缩包 / 其他
- 💾 **持久化存储** — 分类结果实时写入 `nion.json`，下次打开自动加载
- ➕ **自定义分类** — 从全量文件列表中勾选，一键创建新分类
- 🖱️ **快捷操作** — 双击用系统默认程序打开，右键菜单：打开 / 重命名 / 删除 / 打开所在目录
- 🌙 **鎏金暗色主题** — 内置 `dark_gold.qss`，护眼且精致

### 📥 下载

| 文件 | 说明 |
|------|------|
| `LegdEagleSearcher-v1.0.0-windows-x64.zip` | Windows 便携版（MinGW 64-bit），解压即用，无需安装 Qt |

> 从 [Releases](https://github.com/slipergay/LegdEagleSearcher/releases) 获取最新版本。

### 🚀 使用流程

1. 点击 **"选择目录"**，指定要扫描的文件夹
2. 程序自动递归扫描并生成 `nion.json` 分类索引
3. 通过下拉框切换分类，双击条目打开文件
4. 右键条目可重命名、删除等
5. 点击 **"新建分类"**，勾选文件并输入名称即可保存

### 🛠️ 从源码构建

**环境要求**：Qt 6.x · MSVC / MinGW · qmake（推荐）或 CMake

```bash
qmake
make release
# 或在 Qt Creator 中打开项目直接运行
```

> ⚠️ 确保 `dark_gold.qss` 与可执行文件同目录以启用暗色主题。

### ⚠️ 注意事项

- 删除操作**永久删除**磁盘文件，不可恢复，请谨慎使用
- 重命名仅改文件名，分类索引不会自动同步
- 操作重要数据前建议先备份

### 📄 开源协议

基于 [MIT License](./LICENSE) 开源 · Copyright (c) 2026 slipergay

---

## 🇬🇧 English

### ✨ Features

- 📂 **Recursive Scan** — Deep directory traversal with relative path caching to avoid repeated scans
- 🗂️ **Smart Classification** — Directories / Documents / Pictures / Audio / Video / Programs / Archives / Others
- 💾 **Persistent Storage** — Results written to `nion.json` in real-time and auto-loaded on next launch
- ➕ **Custom Categories** — Select files from the full list and create new categories with one click
- 🖱️ **Quick Actions** — Double-click to open with the system default app; right-click menu: Open / Rename / Delete / Open containing folder
- 🌙 **Dark Gold Theme** — Built-in `dark_gold.qss` for an elegant, eye-friendly experience

### 📥 Download

| File | Description |
|------|-------------|
| `LegdEagleSearcher-v1.0.0-windows-x64.zip` | Windows portable build (MinGW 64-bit) — extract & run, no Qt required |

> Get the latest version from [Releases](https://github.com/slipergay/LegdEagleSearcher/releases).

### 🚀 Usage

1. Click **"Select Directory"** to choose the folder to scan
2. The program recursively scans and generates the `nion.json` index
3. Switch categories via the combo box; double-click an item to open it
4. Right-click an item to rename, delete, etc.
5. Click **"New Category"**, select files, enter a name, and save

### 🛠️ Build from Source

**Requirements**: Qt 6.x · MSVC / MinGW · qmake (recommended) or CMake

```bash
qmake
make release
# Or open the project in Qt Creator and click Run
```

> ⚠️ Keep `dark_gold.qss` in the same directory as the executable to enable the dark theme.

### ⚠️ Notes

- Delete operations **permanently remove** files from disk — not recoverable, use with caution
- Rename only changes the file name; the index is not auto-synced
- Back up important data before performing operations

### 📄 License

Licensed under the [MIT License](./LICENSE) · Copyright (c) 2026 slipergay
