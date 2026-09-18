\# 考勤管理系统 (AttendanceSystem)



\## 简介

轻量级命令行考勤管理系统，支持考勤记录的添加/修改/删除、按多条件查询、统计与报表、CSV 导入导出、数据备份与恢复。项目源码位于 `src/`，头文件位于 `include/`。



\## 主要功能

\- 添加 / 修改 / 删除考勤记录

\- 按员工ID、姓名、日期、状态、时间段查询

\- 员工考勤统计与月度报表生成

\- 异常（迟到/早退/缺勤）分析

\- 保存/加载数据文件、CSV 导入导出、备份/恢复

\- 简单的输入验证（日期/时间格式）



\## 项目结构（重要项）

\- `AttendanceSystem.sln` / `AttendanceSystem.vcxproj` — Visual Studio 工程文件

\- `src/` — 源代码（`main.c`, `menu\_system.c`, `data\_manager.c`, `utils.c`, `file\_operations.c` 等）

\- `include/` — 头文件（`structures.h`, `utils.h`, `menu\_system.h`, `data\_manager.h`, `file\_operations.h` 等）

\- `tests/` — 测试代码与运行脚本（`test\_suite.c`, `run\_tests.bat`）

\- `README.md` — 本说明

\- `convert\_to\_utf8.ps1`, `run\_utf8.bat` — 编码/运行辅助脚本

\- `Debug/`, `x64/` — 构建产物（提交时建议排除）



\## 环境与依赖

\- Windows 推荐使用 Visual Studio（支持 C++14）

\- 也可在 Linux/Windows 使用 g++，需支持 C++14

\- 编译器需能找到 `include/` 目录



\## 编译与运行



\- 使用 Visual Studio

&nbsp; 1. 打开 `AttendanceSystem.sln`

&nbsp; 2. 选择 Release/Debug 平台并构建

&nbsp; 3. 在生成目录运行 `AttendanceSystem.exe`



\- 使用命令行（g++）示例（若源码以 C++ 编译）：

&nbsp; - 在项目根目录运行：

&nbsp;   - g++（将所有源强制当作 C++ 编译）：

&nbsp;     `g++ -std=c++14 -x c++ -Iinclude src/\*.c -o AttendanceSystem.exe`

&nbsp;   - 或将 `.c` 重命名为 `.cpp` 后正常编译：

&nbsp;     `g++ -std=c++14 -Iinclude src/\*.cpp -o AttendanceSystem.exe`

&nbsp; - 运行：

&nbsp;   `./AttendanceSystem.exe`



\## 运行示例

\- 启动程序后按屏幕提示选择主菜单项（1-5 / 0 退出）。

\- 添加记录时输入格式：

&nbsp; - 日期：`YYYY-MM-DD`

&nbsp; - 时间：`HH:MM`（24 小时）



\## 测试

\- Windows：运行 `tests\\run\_tests.bat`（视项目内测试实现）

\- 手动检查：使用内置查询/导出功能验证数据一致性





\## 已知问题与注意

\- 输入处理混用 `scanf` 与 `fgets` 可能导致残留换行问题，已提供 `flush\_input\_buffer()` 等辅助函数，请在调试时关注输入流程。

\- 确保 `safe\_input\_\*` 系列函数使用正确的缓冲区大小（使用 `sizeof(buffer)`）。





