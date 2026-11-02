<a id="top"></a>

<p><a href="#english">English</a> | <a href="#chinese">中文</a></p>

<a id="english"></a>

# CE1CHS

CE1CHS is a GB2312 Chinese text reader and rendering project for Windows CE 1.0. It contains a native application and the reusable `CE1CHScore` display-core DLL. The code targets the Visual C++ 5.0 C89 subset, uses no STL, and supports the CE 1.0 1-bpp/2-bpp grayscale display model.

## Repository layout

```text
CE1CHS/
  CE1CHS/          Native application, resources, and CE1CHS.dsw
  CE1CHScore/      Display-core DLL, exports, and CE1CHScore.dsw
  assets/          HZK16, resource manifest, and GB2312 test documents
```

The two project directories must remain siblings. The application project references the core import library through a relative path.

## References and third-party dependencies

| Item                                                        | Purpose                                                                                                              |
| ----------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- |
| `CE1CHScore`                                                | Local DLL project providing text measurement, drawing, font access, and hibernation support. Its source is included. |
| [aguegu/BitmapFont](https://github.com/aguegu/BitmapFont)   | Source of `assets/HZK16`, the 16x16 GB2312 bitmap font.                                                              |
| [lovyan03/LovyanGFX](https://github.com/lovyan03/LovyanGFX) | Source of the ASCII 8x16 glyph data compiled into the core DLL.                                                      |
| Microsoft Windows CE Programmer's Guide and Reference       | API compatibility baseline for Windows CE 1.0.                                                                       |

`commctrl.lib` and `coredll.lib` are supplied by the Windows CE SDK. No additional third-party runtime library, STL, or modern C++ runtime is required. See `assets/MANIFEST.md` for asset provenance and checksums.

## Recommended build environment

- Microsoft Visual C++ 5.0 with the Windows CE Toolkit for Visual C++ 5.0.
- Windows CE 1.0 SDK and the SH3 compiler tools, including `shcl.exe`, `link.exe`, and `rc.exe`.
- A period-compatible Windows 95 or Windows NT 4.0 build environment. Windows NT 4.0 is recommended when using the official emulator, remote debugging, or remote tools.
- Active configuration: `Win32 (WCE SH) Release` for both projects.

A modern Visual Studio installation is not a drop-in replacement for this toolchain.

## Build order: DLL and import library first

### 1. Build CE1CHScore

1. Open `CE1CHScore/CE1CHScore.dsw` in Visual C++ 5.0.
2. Select **Build > Set Active Configuration**, then select `CE1CHScore - Win32 (WCE MIPS/SH) Release`.
3. Under **Project > Settings > Link**, verify that `CE1CHSCore.def` is used for exports and that import-library generation is not disabled. The supplied project already contains `/def:"CE1CHSCore.def"` and `Ignore_Export_Lib 0`.
4. Select **Build > Rebuild All**. Do not use **Compile**, because compiling a source file does not link the DLL or generate its import library.
5. Confirm that both outputs exist before building the application:

```text
CE1CHScore/WCESHRel/CE1CHScore.dll
CE1CHScore/WCESHRel/CE1CHScore.lib
```

The `.def` file exports the `CE1Core*` functions. The DLL is required on the device, while the generated `.lib` is the import library used by the EXE linker and is also included in the validated deployment layout below.

### 2. Link and build CE1CHS

1. Open `CE1CHS/CE1CHS.dsw` in a separate Visual C++ 5.0 workspace.
2. Select `CE1CHS - Win32 (WCE MIPS/SH) Release` as the active configuration.
3. Under **Project > Settings > Link > General > Object/library modules**, verify this entry in addition to `commctrl.lib` and `coredll.lib`:

```text
..\CE1CHScore\WCESHRel\CE1CHScore.lib
```

1. Keep the core DLL, import library, and application on the same target architecture and configuration. Do not mix SH3 with MIPS or x86em outputs.
2. Select **Build > Rebuild All**. The application output is:

```text
CE1CHS/WCESHRel/CE1CHS.exe
```

The supplied `.dsp` files already contain the required release settings. Normally, verification is sufficient; do not add a second copy of the import library.

### 3. Resource inputs

Keep the following files beside `CE1CHS.rc`:

```text
app.ico
bg.2bp
logo1.2bp
logo2.2bp
```

They are compiled into the EXE. The bitmap files do not need to be added as separate project files, but `CE1CHS.rc` must remain in the project. Use only CE-compatible 1-bpp or 2-bpp grayscale bitmap resources.

## Build troubleshooting

| Symptom                                                                        | Cause and correction                                                                                                                                                                       |
| ------------------------------------------------------------------------------ | ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------ |
| `CE1CHScore.lib` is not generated                                              | Select the SH3 Release configuration, retain `CE1CHSCore.def` and its exports, enable import-library generation, and use **Rebuild All** on the DLL project. Build the DLL before the EXE. |
| `LNK2001` for `CE1Core*` or `LNK1181` for the import library                   | The DLL was not built first, the relative library path is wrong, or configurations were mixed. Restore the exact `..\CE1CHScore\WCESHRel\CE1CHScore.lib` link entry.                       |
| Resource compiler cannot open `bg.2bp`, `logo1.2bp`, `logo2.2bp`, or `app.ico` | Place the files beside `CE1CHS.rc`, keep the `.rc` file in the project, and run **Rebuild All**. Do not rename only the extension to change a bitmap's format.                             |
| `C2059` or `C2061` near `CE1LANGUAGE`                                          | Older enum names collided with platform macros. Keep the current `CE1_LANG_CHINESE`, `CE1_LANG_ENGLISH`, and `CE1_LANG_COUNT` identifiers.                                                 |
| `C1004: unexpected end of file found`                                          | Check for truncated files, unmatched braces, and incomplete edits. Re-copy complete ASCII source files and fix earlier diagnostics first; old compilers often emit cascading errors.       |
| `GetModuleFileName` is undefined or unresolved                                 | This API is unavailable in the target CE 1.0 SDK. The current application uses embedded bitmap resources instead of executable-directory discovery; do not reintroduce that call.          |
| `LoadImage` with `LR_LOADFROMFILE` fails to build or load                      | CE 1.0 does not support that file-loading mode. Use a 1-bpp/2-bpp `.2bp` resource and `LoadBitmap`, as in the current source.                                                              |
| `ShowScrollBar`, `GetScrollPos`, or `SetScrollPos` is unavailable              | These APIs are outside the CE 1.0 subset. Retain the current `WS_VSCROLL`, `SetScrollInfo`, and `GetScrollInfo` implementation.                                                            |
| Non-ASCII source or modern-language errors                                     | Keep source, comments, and identifiers in ASCII and within the VC++ 5.0 C89 subset. Chinese display strings are GB2312 byte data, not modern Unicode source literals.                      |

## Required device deployment

**The following four files and their exact directories are mandatory. The application will not start with an incomplete or rearranged deployment.**

| File             | Required device path      |
| ---------------- | ------------------------- |
| `CE1CHS.exe`     | `\CE1CHS\CE1CHS.exe`      |
| `HZK16`          | `\CE1CHS\HZK16`           |
| `CE1CHScore.dll` | `\Windows\CE1CHScore.dll` |
| `CE1CHScore.lib` | `\Windows\CE1CHScore.lib` |

1. Create a new `CE1CHS` folder at the root of the device or emulator filesystem.
2. Copy the EXE and `assets/HZK16` into `\CE1CHS`.
3. Copy both the DLL and LIB files into `\Windows`.
4. Do not omit the LIB file or substitute `\Program Files` or another directory layout.

## Runtime tests

Copy the complete `assets/test` directory to `\CE1CHS\test` on the device or emulator. This step is recommended for functional testing.

Launch `\CE1CHS\CE1CHS.exe` and open the test documents through the native Open button. The test files contain GB2312 data, not UTF-8.

- `test_ascii.txt`: ASCII text.
- `test_cjk.txt`: Chinese glyph coverage.
- `test_mixed.txt`: Mixed Chinese and ASCII text.
- `test_control.txt`: Tabs and line endings.
- `test_edge.txt`: Deliberately malformed and truncated GB2312 boundary data.

An optional direct launch command is:

```text
\CE1CHS\CE1CHS.exe -f \CE1CHS\HZK16 \CE1CHS\test\test_cjk.txt
```

***

<a id="chinese"></a>

# CE1CHS 中文说明

CE1CHS 是面向 Windows CE 1.0 的 GB2312 中文文本阅读与渲染项目，包含原生应用程序和可复用的 `CE1CHScore` 显示核心 DLL。代码以 Visual C++ 5.0 的 C89 子集为基线，不使用 STL，并遵循 CE 1.0 的 1bpp/2bpp 灰阶显示模型。

## 目录结构

```text
CE1CHS/
  CE1CHS/          原生应用、资源文件及 CE1CHS.dsw
  CE1CHScore/      显示核心 DLL、导出定义及 CE1CHScore.dsw
  assets/          HZK16、资源清单及 GB2312 测试文档
```

两个工程目录必须保持同级关系，因为主程序通过相对路径引用核心项目的导入库。

## 参考项目与第三方依赖

| 项目                                                          | 用途                                       |
| ----------------------------------------------------------- | ---------------------------------------- |
| `CE1CHScore`                                                | 仓库内的本地 DLL 工程，提供文本测量、绘制、字库访问及休眠支持，源码已包含。 |
| [aguegu/BitmapFont](https://github.com/aguegu/BitmapFont)   | `assets/HZK16` 的来源，即 16x16 GB2312 点阵字库。  |
| [lovyan03/LovyanGFX](https://github.com/lovyan03/LovyanGFX) | 编译进核心 DLL 的 ASCII 8x16 字模数据来源。           |
| Microsoft Windows CE Programmer's Guide and Reference       | Windows CE 1.0 API 兼容性基线文档。              |

`commctrl.lib` 和 `coredll.lib` 由 Windows CE SDK 提供。项目不需要其他第三方运行库、STL 或现代 C++ 运行时。资源来源和校验值见 `assets/MANIFEST.md`。

## 建议编译环境

- Microsoft Visual C++ 5.0，以及 Windows CE Toolkit for Visual C++ 5.0。
- Windows CE 1.0 SDK 和 SH3 编译工具，包括 `shcl.exe`、`link.exe`、`rc.exe`。
- 与工具链同时代的 Windows 95 或 Windows NT 4.0 构建环境；如需官方模拟器、远程调试或远程工具，建议使用 Windows NT 4.0。
- 两个工程均选择 `Win32 (WCE MIPS/SH) Release` 配置。

现代 Visual Studio 不能直接替代这套目标工具链。

## 编译顺序：先生成 DLL 和导入库

### 1. 编译 CE1CHScore

1. 使用 Visual C++ 5.0 打开 `CE1CHScore/CE1CHScore.dsw`。
2. 选择 **Build > Set Active Configuration**，然后选择 `CE1CHScore - Win32 (WCE SH) Release`。
3. 在 **Project > Settings > Link** 中确认使用 `CE1CHSCore.def` 导出符号，且未禁用导入库生成。仓库工程已包含 `/def:"CE1CHSCore.def"` 和 `Ignore_Export_Lib 0`。
4. 选择 **Build > Rebuild All**。不要只执行 **Compile**，因为单文件编译不会链接 DLL，也不会生成导入库。
5. 编译主程序前，确认以下两个文件均已生成：

```text
CE1CHScore/WCESHRel/CE1CHScore.dll
CE1CHScore/WCESHRel/CE1CHScore.lib
```

`.def` 文件负责导出 `CE1Core*` 函数。DLL 是设备端运行组件；生成的 `.lib` 是 EXE 链接时使用的导入库，同时也属于下文已经验证的部署文件集合。

### 2. 链接并编译 CE1CHS

1. 在另一个 Visual C++ 5.0 工作区中打开 `CE1CHS/CE1CHS.dsw`。
2. 将活动配置设为 `CE1CHS - Win32 (WCE MIPS/SH) Release`。
3. 在 **Project > Settings > Link > General > Object/library modules** 中，确认除 `commctrl.lib`、`coredll.lib` 外还包含：

```text
..\CE1CHScore\WCESHRel\CE1CHScore.lib
```

1. 核心 DLL、导入库和应用程序必须使用相同目标架构及配置，不要混用 SH3、MIPS、x86em 输出。
2. 选择 **Build > Rebuild All**。主程序输出为：

```text
CE1CHS/WCESHRel/CE1CHS.exe
```

仓库中的 `.dsp` 已包含所需 Release 设置。正常情况下只需核对，不要重复添加同一个导入库。

### 3. 资源输入文件

以下文件必须与 `CE1CHS.rc` 位于同一目录：

```text
app.ico
bg.2bp
logo1.2bp
logo2.2bp
```

这些文件会编译进 EXE。位图不需要逐个 Add to Project，但 `CE1CHS.rc` 必须保留在工程中。位图资源只能使用 CE 兼容的 1bpp 或 2bpp 灰阶格式。

## 编译疑难问题

| 现象                                                     | 原因与处理方法                                                                                           |
| ------------------------------------------------------ | ------------------------------------------------------------------------------------------------- |
| 未生成 `CE1CHScore.lib`                                   | 选择 SH3 Release 配置，保留 `CE1CHSCore.def` 及其导出，启用导入库生成，并对 DLL 工程执行 **Rebuild All**。必须先编译 DLL，再编译 EXE。 |
| `CE1Core*` 出现 `LNK2001`，或导入库出现 `LNK1181`               | DLL 未先构建、相对库路径错误或配置混用。恢复精确的 `..\CE1CHScore\WCESHRel\CE1CHScore.lib` 链接项。                          |
| 资源编译器无法打开 `bg.2bp`、`logo1.2bp`、`logo2.2bp` 或 `app.ico` | 将文件放在 `CE1CHS.rc` 同目录，确认 `.rc` 已加入工程，再执行 **Rebuild All**。仅修改扩展名不会改变位图格式。                          |
| `CE1LANGUAGE` 附近出现 `C2059`、`C2061`                     | 旧枚举名称与平台宏冲突。保留当前的 `CE1_LANG_CHINESE`、`CE1_LANG_ENGLISH`、`CE1_LANG_COUNT` 标识符。                     |
| `C1004: unexpected end of file found`                  | 检查文件截断、花括号不匹配及不完整编辑，重新复制完整的 ASCII 源文件，并优先处理更早的错误；旧编译器常产生级联报错。                                     |
| `GetModuleFileName` 未定义或链接无法解析                         | 目标 CE 1.0 SDK 不提供此 API。当前程序使用内嵌位图资源，不再查找 EXE 目录，不应重新加入该调用。                                        |
| `LoadImage` 配合 `LR_LOADFROMFILE` 无法编译或加载               | CE 1.0 不支持该文件加载模式。按当前代码使用 1bpp/2bpp `.2bp` 资源和 `LoadBitmap`。                                      |
| `ShowScrollBar`、`GetScrollPos`、`SetScrollPos` 不可用      | 这些 API 不在 CE 1.0 子集内。保留当前的 `WS_VSCROLL`、`SetScrollInfo`、`GetScrollInfo` 实现。                       |
| 出现非 ASCII 源码或现代语言特性错误                                  | 源码、注释和标识符保持 ASCII，并限制在 VC++ 5.0 C89 子集内。中文显示字符串使用 GB2312 字节数据，而不是现代 Unicode 源码字面量。                |

## 实机或虚拟机必要部署

**以下四个文件及其指定目录均为强制要求。文件缺失或目录布局改变会导致软件无法启动。**

| 文件               | 设备端必须放置的路径                |
| ---------------- | ------------------------- |
| `CE1CHS.exe`     | `\CE1CHS\CE1CHS.exe`      |
| `HZK16`          | `\CE1CHS\HZK16`           |
| `CE1CHScore.dll` | `\Windows\CE1CHScore.dll` |
| `CE1CHScore.lib` | `\Windows\CE1CHScore.lib` |

1. 在实机或虚拟机整机根目录新建 `CE1CHS` 文件夹。
2. 将 EXE 和 `assets/HZK16` 复制到 `\CE1CHS`。
3. 将 DLL 和 LIB 两个文件复制到 `\Windows`。
4. 不要省略 LIB 文件，也不要改用 `\Program Files` 或其他目录布局。

## 实际运行测试

建议将完整的 `assets/test` 目录复制到设备的 `\CE1CHS\test`，用于实际运行测试。

启动 `\CE1CHS\CE1CHS.exe`，通过原生打开按钮选择测试文档。测试文件使用 GB2312 数据，不是 UTF-8。

- `test_ascii.txt`：ASCII 文本。
- `test_cjk.txt`：中文字模覆盖。
- `test_mixed.txt`：中文与 ASCII 混合文本。
- `test_control.txt`：制表符和换行。
- `test_edge.txt`：故意构造的非法及截断 GB2312 边界数据。

也可使用以下命令直接启动测试：

```text
\CE1CHS\CE1CHS.exe -f \CE1CHS\HZK16 \CE1CHS\test\test_cjk.txt
```

